/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2006,2007,2008  Klaas van Gend
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation
 * applicable version is GPL version 2 only.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */


#include "Query.h"
#include "SerialPort.h"
#include "umtsmon_global.h"

#include <assert.h>


// make sure that upon initialisation, theSerialPortPtr is NULL
static SerialPort* theSerialPortPtr = NULL;
static QMutex	   theQueryMutex;


SerialPort* Query::setSerial(SerialPort* aSerialPortPtr)
{
	DEBUG5("%p = Query::setSerial(%p)\n", theSerialPortPtr, aSerialPortPtr);
	SerialPort* myTempPtr = theSerialPortPtr; 
    theSerialPortPtr = aSerialPortPtr;
    return myTempPtr;
}

bool Query::hasSerial(void)
{
	return theSerialPortPtr != NULL;
}


QString escapeString(const QString& anIString)
{
	QString myOString;
	for (int i=0; i< anIString.length();i++)
	{
		QChar myChar = anIString[i];
		if (isprint(myChar.toAscii()))
			myOString += myChar;
		else
			myOString += "\\x" + QString::number(myChar.upper().latin1(), 16);
	}
	return myOString;
}




Query::~Query()
{
	if (hasLock)
	{
		DEBUG5("Query instance %p abandoned MUTEX\n", this);
		theQueryMutex.unlock();
		hasLock=false;
	}
}




Query::ReturnCode Query::receiveMore()
{
	// as we know the Query has already run, let's clean out the string list first
	theQueryString="";
	isFirstRun = false;
	theAnswer.clear();
	return realRun();
}


Query::ReturnCode Query::runUntilDone()
{
	ReturnCode myResult = run();
	theQueryString="";
	isFirstRun = false;
	int i=0;
	while (myResult == Query::UNKNOWN_ERROR)
	{
		DEBUG5("Query::runUntilDone needs more time...\n");
		// sleep for 100 ms
		struct timespec sleep_time;
		sleep_time.tv_sec=0;
		sleep_time.tv_nsec=100*1000*1000; 
		nanosleep(&sleep_time,NULL);
		// and retry
		myResult = realRun();
		// and check for larger timeouts
		if (i++ > 100)
		{
			DEBUG1("Query::runUntilDone needed more than 10 seconds. Bailing out.\n");
			break;
		}
	}
	
	return myResult;
}

Query::ReturnCode Query::run()
{
	// safety net
	assert(theSerialPortPtr != NULL);
	if (theSerialPortPtr == NULL)
	{
		return NO_DEVICE;
	}

	// more safety net: lock the mutex
	assert(hasLock == false);
	if (theQueryMutex.tryLock()==FALSE)
	{
		hasLock = false;
		DEBUG5("Query instance %p trylock FAILED\n", this);
		return NO_LOCK;
	}
	hasLock=true;
	DEBUG5("Query instance %p acquired MUTEX\n", this);

	// ***** first make sure there's no residual info on the line
	QString myExcess = receiveStringOnce();
	if (!myExcess.isEmpty())
	{
		DEBUG2("QUERY: excess bytes received: '%s'\n", escapeString(myExcess).ascii());
	}

	return realRun();
}

Query::ReturnCode Query::realRun()
{
	// ***** send our message
	if (isFirstRun == true)
	{
		DEBUG4("Query sends the following mesage: '%s'\n", theQueryString.ascii());
		if (theSerialPortPtr->writeDev("\r"+theQueryString+"\r") 
			== false)
		{
			return NO_DEVICE;
		}
	}

	// ***** receive the response and split it up
	int i=0;
	theAnswer = QStringList::split("\r\n", receiveStringRetry());
	for ( QStringList::Iterator myIt = theAnswer.begin(); 
               myIt != theAnswer.end(); ++myIt ) 
	{
		i=i+1;
		if ((*myIt).stripWhiteSpace().isEmpty())
		{
			DEBUG4("answer %d removed - empty\n", i);
			QStringList::Iterator myDeleted = (myIt--);
			theAnswer.erase(myDeleted);
			continue;
		}
        DEBUG4("answer %d:'%s'\n", i, (*myIt).stripWhiteSpace().ascii());
	}

	// ***** interpret the response

	if (isFirstRun == true)
	{	
		// remove echos from answers
		checkQuery();
	}

	// check to see if any OK/ERROR in output
	return interpretResult();
}

// read out the data from the serial port
QString Query::receiveStringRetry(void) const
{
	assert(theSerialPortPtr != NULL);

	QString myCompleteAnswer("");

	// some devices take a while before they answer,
	// or the answer "takes a while". That's why we retry and
	// wait for three subsequent "no more" before we think we have
	// the complete message
	int myRetries = 3;
	while (myRetries > 0)
	{
		QString myPartAnswer = receiveStringOnce();
		if (myPartAnswer.isEmpty()==false)
		{
			// we received a valid character - reset the counter...
			myRetries=3;
			myCompleteAnswer += myPartAnswer;
		}
		else
			myRetries--;
		DEBUG6("Query::receiveStringRetry %d\n", myRetries);
	}
	DEBUG5("receiveStringRetry: '%s'\n", escapeString(myCompleteAnswer).ascii());
	return myCompleteAnswer;
}

// read out the data from the serial port
QString Query::receiveStringOnce(void) const
{
	int ch;
	QString myCompleteAnswer("");
	while( (ch=theSerialPortPtr->getOneByte()) >0)
	{
		myCompleteAnswer += ch;
	}
	DEBUG6("Query::receiveStringOnce %s\n", escapeString(myCompleteAnswer).ascii());
	return myCompleteAnswer;
}


// attempt to interpret the last line (OK/ERROR/etc)
Query::ReturnCode Query::interpretResult(void)
{
	ReturnCode myReturnCode = UNKNOWN_ERROR;

	QStringList::Iterator myIterator = theAnswer.begin();
	for (myIterator = theAnswer.begin();myIterator != theAnswer.end(); ++myIterator)
	{
		QString myAnswer = *myIterator;
		// let's do the plain, simple answers first
		if (myAnswer == "OK")
			myReturnCode = OK;
		if (myAnswer == "ERROR")
			myReturnCode = ERROR;
		if (myAnswer == "NO CARRIER")
			myReturnCode = NO_CARRIER;
		
		// filter Huawei-specifics
		// FIXME: This needs work
		if (myAnswer.left(1) == "^")
		{
			DEBUG5("found Huawei state info: '%s'\n", myAnswer.ascii());
			myIterator = theAnswer.erase(myIterator);
			--myIterator;
		}
	}
	
	if (myReturnCode != UNKNOWN_ERROR)
	{
		DEBUG5("because known statement #%d was detected, removing obvious answer '%s'\n",
				myReturnCode, theAnswer.last().ascii());	
		theAnswer.pop_back();
	}

	// if the answer however contained a specific error
	// message like +CME ERROR: SIM PIN, let's return
	// error as well - but don't pop the error
	if (theAnswer.last().contains("ERROR",true))
	{
		DEBUG5("found error in message!\n");
		myReturnCode = ERROR;
	}

	return myReturnCode;
}

// check if first line equals the Query, and remove it if so.
bool Query::checkQuery(void)
{
	if ( theQueryString == (*(theAnswer.begin())).stripWhiteSpace() )
	{
		DEBUG5("checkQuery: removing echo up front\n");
		theAnswer.pop_front();
		return true;
	}
	// apparently this extra check is needed for Samsung Innov8 or i8510 mobile phone
	// see bug # 2558652
	if ( (theQueryString+"\xd"+theQueryString) == (*(theAnswer.begin())).stripWhiteSpace() )
	{
		DEBUG5("checkQuery: removing double echo up front\n");
		theAnswer.pop_front();
		return true;
	}
	return false;
}

bool Query::sendMore(const QString& aString)
{
	// safety net
	assert(theSerialPortPtr != NULL);
	if (theSerialPortPtr == NULL)
	{
		return false;
	}

	// ***** send our message
	return theSerialPortPtr->writeDev(aString+"\r");
}


QStringList& Query::getAnswer()
{
	// if this assert kicks in, it probably means you ran getAnswer() before run()
	// OR
	// the Query hasn't completed yet - you might need to run receiveMore() 
	// a few times until the answer gets in...
	DEBUG5("Query::getAnswer returns %d answer(s)\n", theAnswer.count() ); 
	assert (!theAnswer.isEmpty());
	return theAnswer; 
}
