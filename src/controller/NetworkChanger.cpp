/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2006, 2007, 2008  Klaas van Gend
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

#include "umtsmon_global.h"
#include "NetworkChanger.h"
#include "Query.h"
#include "OperatorList.h"

#include <assert.h>
#include <unistd.h>

NetworkChanger::~NetworkChanger() 
{
	// to make sure that no Query exists anymore 
	// otherwise will have locking issues elsewhere
	if (theQueryPtr != NULL)
	{
		delete theQueryPtr;
		theQueryPtr = NULL;
	}
};

bool NetworkChanger::setCardToAutomatic(void) const
{
	// set new registration mode,
	// see "3GPP 27007-700 chapter 7.3, PLMN selection" 
	Query myQuery("AT+COPS=0");
	if (myQuery.runUntilDone() != Query::OK)
		{
			// no big deal, it happens, fail silently
			DEBUG4("Query AT+COPS=0 failed...\n");
			return false;
		}
	return true;
}


NetworkChanger::RegistrationMode 
NetworkChanger::getRegistrationState(void) const
{
	// see "3GPP 27007-700 chapter 7.3, PLMN selection" 
	Query myQuery("AT+COPS?");
	if (myQuery.runUntilDone() != Query::OK)
			return MODE_ERROR;
	if (myQuery.hasAnswer()==false)
		return MODE_ERROR;

	// The usual answer should look like: '+COPS: 4,0,"vodafone NL",2'
	QString myAnswer = myQuery.getAnswer().first().section(":",1,1);
	QString myFirstChar = myAnswer.stripWhiteSpace().left(1);
	int myDecimal = myFirstChar.toShort();
	assert(myDecimal <= 4);
	return static_cast<RegistrationMode>(myDecimal);
}


bool 
NetworkChanger::startSearching(void)
{
	assert(theQueryPtr == NULL);
	// see "3GPP 27007-700 chapter 7.3, PLMN selection" 
	theQueryPtr = new Query("AT+COPS=?");

	Query::ReturnCode myResult = theQueryPtr->run();
	if (myResult == Query::ERROR)
	{
		stopSearch();
		return false;
	}
	return true;
}


bool 
NetworkChanger::isSearchCompleted(void)
{
	assert(theQueryPtr != NULL);
	Query::ReturnCode myCode = theQueryPtr->receiveMore();
	if (myCode != Query::OK)
	{
		return false;
	}

	// if there is more than one answer, we will have to iterate through them...
	if (theQueryPtr->hasAnswer())
		for (QStringList::Iterator myIterator = theQueryPtr->getAnswer().begin();
			myIterator !=  theQueryPtr->getAnswer().end(); myIterator++)
		{
			QString myAnswer = *myIterator;
			theOperatorList.fill(myAnswer);
		}

	stopSearch();
	return true;
}


void 
NetworkChanger::stopSearch(void)
{
	if (theQueryPtr)
	{
		delete theQueryPtr;
		theQueryPtr = NULL;
	}
}


/**************************************************************************** 
 * BASICALLY THERE IS A HARD LINE BETWEEN THE ABOVE AND THE BELOW 
 *     - Above is ask for a list of networks
 *     - Below is switching to one of the network IDs
 */


NetworkChanger::SwitchResults
NetworkChanger::startSwitchToNetworkID(unsigned long aNetworkID)
{
DEBUG5("NetworkChanger::startSwitchToNetworkID\n");

	Operator* myID = theOperatorList.findNetworkID(aNetworkID);
	if(myID == NULL)
	{
		// something's really wrong here
		assert(false);
		return ERROR;
	}

	switch(myID->getIconType())
	{
	case Operator::NOT_VALID:
	case Operator::FORBIDDEN:
		return ERROR;
	case Operator::CURRENT:
		return IS_CURRENT;
	case Operator::AVAILABLE:	
		{
		QString myAT = "AT+COPS=1,2,\"" + QString::number(aNetworkID) + "\"";
	
		assert(theQueryPtr == NULL);
		theQueryPtr = new Query(myAT);
		// because the user has already waited 40 seconds to get this far,
		// I want to try very hard to make this proceed - so, if there's something
		// blocking the Query, we retry...
		for (int i=0; i<3; i++)
		{
			Query::ReturnCode myResult = theQueryPtr->run();
			if (myResult == Query::ERROR)
				return ERROR;
			if (myResult == Query::OK)
				return OK;
			if (myResult != Query::NO_LOCK)
				return NOT_READY_YET;
			// if we get here, we couldn't get a lock
			// so sleep for a second and retry
			sleep(1);
		} // end-of-while
		// if we get here, starting the Query timed out 3 times...
		break;
		} // end-of-case
	} // end-of-switch

	stopSearch();
	return ERROR;
}


NetworkChanger::SwitchResults 
NetworkChanger::isSwitchCompleted(void)
{
	assert(theQueryPtr != NULL);
	SwitchResults myResult = NOT_READY_YET;
	switch (theQueryPtr->receiveMore())
	{
	case Query::OK:
		// we're done!!!
		myResult = OK;
		break;
	case Query::ERROR:
	case Query::NO_CARRIER:
	case Query::NO_DEVICE:
	case Query::NO_LOCK:
		// I/O troubles or real errors
		// note that stopSearch() is not called, the QueryPtr remains
		return ERROR;
	case Query::UNKNOWN_ERROR:
		// NOT FINISHED YET
		return NOT_READY_YET;	
	}
	
	stopSearch();
	return myResult;
}

QString 
NetworkChanger::getErrorMessage(void)
{
	DEBUG5("NetworkChanger::getErrorMessage(void)\n");
	// some basic sanity tests
	if (theQueryPtr == NULL)
		return "";
	if (theQueryPtr->hasAnswer()==false)
		return "";
	
	DEBUG5("NetworkChanger::getErrorMessage(void) iterate\n");
	// let's hunt for ERROR and return that line
	for (QStringList::Iterator myIterator = theQueryPtr->getAnswer().begin();
		myIterator !=  theQueryPtr->getAnswer().end(); myIterator++)
	{
		QString myAnswer = *myIterator;
		// search without case sensitivity
printf("myAnswer ='%s'\n",myAnswer.ascii());		
		if (myAnswer.contains("error", false))
			return myAnswer;
	}
	return "";
}
