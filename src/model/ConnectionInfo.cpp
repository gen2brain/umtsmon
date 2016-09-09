/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2006,2007,2008,2009  Klaas van Gend
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
#include "ConnectionInfo.h"
#include "Query.h"
#include "OperatorList.h"

#include <qfile.h>
#include <qregexp.h>
#include <qtranslator.h>

#include <assert.h>

int ConnectionInfo::theInhibitionCount = 0;

bool ConnectionInfo::isInhibited(void) const
{
	if (theInhibitionCount>0)
	{
		DEBUG3("ConnectionInfo probe inhibited\n");
		return true;
	}
	return false;
}



ConnectionInfo::~ConnectionInfo()
{
	if (theOperatorPtr!=NULL)
	{
		delete theOperatorPtr;
		theOperatorPtr = NULL;
	}
}


void ConnectionInfo::refresh(void)
{
	refreshRegistration();
	
	refreshSignalQuality();

	if (isDeviceRegistered())
	{
		refreshOperator();
	}
	else
	{
		if (isSearching())
			theOperatorName = QObject::tr("Searching...");
		else	
			theOperatorName = QObject::tr("n/a");
	}
	
	// keep this one last - it might throw some exceptions
	refreshTrafficStats();
}


void ConnectionInfo::refreshRegistration(void)
{
	if (!isInhibited())
	{
		theRegistrationValue = refreshCGREG();
		// if not successful, retry with AT+CREG
		if (theRegistrationValue == REGISTRATION_UNKNOWN)
			theRegistrationValue = refreshCREG();
	}
}

void ConnectionInfo::refreshSignalQuality(void)
{
	DEBUG4("ConnectionInfo::refreshSignalQuality(void)\n");
	// only send Query if allowed
	if (isInhibited())
	{
		theSignalQuality=99;
		return;
	}
	
	// only do this when registered 
	//  - data won't make sense otherwise	
	if (!isDeviceRegistered() && theRegistrationValue!=TRYING_TO_REGISTER)
		theSignalQuality=99;
	else
	{
		Query myQuery("AT+CSQ");
		if (myQuery.runUntilDone() != Query::OK)
		{
			// no big deal, it happens, fail silently
			DEBUG3("Query.run() failed\n");
			return;
		}
		
		// result: '+CQS: 19,0'  -> '19,0'
		QString myResult = myQuery.getAnswer().first().section(" ",1,1);
		// result: '19,0' -> int 19 and display it...
		theSignalQuality = myResult.section(",",0,0).toInt();
	}
}


void ConnectionInfo::refreshOperator(void)
{
	// only send Query if allowed
	if (isInhibited())
		return;
	DEBUG4("ConnectionInfo::refreshOperator(void)\n");

	// query for current operator
	Query myQuery("AT+COPS?");
	if (myQuery.runUntilDone() != Query::OK)
		{
			// no big deal, it happens, fail silently
			DEBUG4("Query.run() failed...\n");
			return;
		}
	QString myAnswer = myQuery.getAnswer().first();
	QString myNewOperatorName; 
	if (myAnswer == "+COPS: 0")
	{
		myNewOperatorName = QObject::tr("n/a");
		theRadioType  = -1;
	}
	else
	{
		// result: '+COPS: 0,0,"vodafone SE",0'  -> 'vodafone SE'
		myNewOperatorName = myAnswer.section("\"",1,1);
		// filter after the second quote+comma -> ,2
		theRadioType = myAnswer.section("\",",1,1).toInt();
	}
	
	// some operator names start with a number (e.g. '3' in the UK).
	// all network ID's we've seen, are above 30000.
	// so we can hopefully safely assume that any number above 1000
	// is a network ID
	// we leave it to resolveNetworkIDtoName to cache the lookup
	if (theOperatorName != myNewOperatorName)
	{
		if (myNewOperatorName.toULong()>1000)
			theOperatorName = resolveNetworkIDtoName(myNewOperatorName);
		else
			theOperatorName = myNewOperatorName;
	}
}

QString 
ConnectionInfo::getRadiotypeString(int aRadioType)
{
	QString theType;
	switch (aRadioType)
	{
	case -1:
		theType = "n/a";
		break;
	case 0:
		// officially: GSM
		theType = "GPRS";
		break;
	case 1:
		// officially: GSM compact
		theType = "GSM compact";
		break;
	case 2:
		// officially: UTRAN
		theType = "UMTS";
		break;
	default:
		theType = "???";
		break;
	}

	return theType;
}


QString ConnectionInfo::getRadiotype(void) const
{ 
	if ( theRegistrationValue==REGISTERED_HOME ||
	     theRegistrationValue==REGISTERED_ROAMING)
		return ConnectionInfo::getRadiotypeString(theRadioType);
	else
	{
		if (theRegistrationValue==TRYING_TO_REGISTER)
			return QObject::tr("Searching...");
		else
			return QObject::tr("n/a");
	} 
}

// this member contains the implementation of the official specification
// 3GPP TS 27.007 V7.0.0 (2006-03) - chapter 10.1.19
// unfortunately, this command is only 'Optional'.
ConnectionInfo::Registration ConnectionInfo::refreshCGREG(void)
{
	if (isInhibited())
	{
		// we shouldn't really get here, but better safe then sorry
		return REGISTRATION_UNKNOWN;
	}
	DEBUG4("ConnectionInfo::refreshCGREG(void)\n");

	Query myQuery( "AT+CGREG?" );
 	Query::ReturnCode myResult = myQuery.runUntilDone();
 	
 	if (myResult != Query::OK)
 	{
 		DEBUG2("unfortunately REGISTRATION_UNKNOWN as answer to AT+CGREG?\n");
 		return REGISTRATION_UNKNOWN;
 	}
	
	// answer should be something like: +CGREG: <n>,<stat>[,<lac>,<ci>]
	// where we don't care about <n>
	// and <stat> can be 0,1,2,3,4,5 
	// For now, we don't care about <lac> or <ci> either
	//
	// in some cases, we will just receive "OK" - thanks Sierra!
	if (!myQuery.hasAnswer())
		return REGISTRATION_UNKNOWN;

	QString myAnswer = myQuery.getAnswer().first();
	if (!myAnswer.contains("CGREG"))
	{
		DEBUG2("unfortunately unparsible answer '%s' to AT+CGREG.\n", myAnswer.ascii());
		return REGISTRATION_UNKNOWN;
	}
	
	bool isOK;
	int myCGREG = myAnswer.section(",",1,1).toInt(&isOK);
	
	if (isOK)
		return static_cast<Registration>(myCGREG);
	else
		return REGISTRATION_UNKNOWN;
}

// this member contains the implementation of the official specification
// 3GPP TS 27.007 V7.0.0 (2006-03) - chapter 7.2
// unfortunately, this command is only 'Optional'.
ConnectionInfo::Registration ConnectionInfo::refreshCREG(void)
{
	if (isInhibited())
	{
		// we shouldn't really get here, but better safe then sorry
		return REGISTRATION_UNKNOWN;
	}
	DEBUG4("ConnectionInfo::refreshCREG(void)\n");
	
	Query myQuery( "AT+CREG?" );
 	Query::ReturnCode myResult = myQuery.runUntilDone();
 	
 	if (myResult != Query::OK)
 	{
 		DEBUG2("unfortunately REGISTRATION_UNKNOWN as answer to AT+CREG?\n");
 		return REGISTRATION_UNKNOWN;
 	}
	
	// answer should be something like: +CREG: <n>,<stat>[,<lac>,<ci>]
	// where we don't care about <n>
	// and <stat> can be 0,1,2,3,4,5 
	// For now, we don't care about <lac> or <ci> either
 	if (!myQuery.hasAnswer())
 	{
 		DEBUG2("Unfortunately only replied \"OK\" to CREG\n");
 		return REGISTRATION_UNKNOWN;
 	}
 		
 	QString myAnswer = myQuery.getAnswer().first();
	if (!myAnswer.contains("CREG"))
 	{
 		DEBUG2("unfortunately unparsible answer '%s' to AT+CREG.\n", myAnswer.ascii());
 		return REGISTRATION_UNKNOWN;
 	}
	
	bool isOK;
	int myCGREG = myAnswer.section(",",1,1).toInt(&isOK);
	
	if (isOK)
		return static_cast<Registration>(myCGREG);
	else
		return REGISTRATION_UNKNOWN;
}


void ConnectionInfo::refreshTrafficStats(void)
{
	DEBUG3("ConnectionInfo::refreshPPPStats\n");
	unsigned long myCounterBytesSent = 0;
	unsigned long myCounterBytesReceived = 0;
	
	// update the rx and tx byte values
	bool myIsProcNetDevStatPresent = false;
	QFile myFile("/proc/net/dev");
	if (myFile.open(IO_ReadOnly) == true)
	{
		QString myLine;
		// FIXME: This is hardcoded for ppp0!!!
		QRegExp myPPP("ppp0");
		while(myFile.readLine(myLine, 255)> 0)
		{
			// perform substring searching
			if (myPPP.search(myLine,0) != -1)
			{
				myIsProcNetDevStatPresent = true;
				QStringList myList = QStringList::split(QRegExp("[ :]"), myLine);
				myCounterBytesSent = myList[9].toLong();
				myCounterBytesReceived = myList[1].toLong();
				break;	// breaks while loop
			}
		}
		myFile.close();
	}
	
	// or did we change state?
	if (myIsProcNetDevStatPresent != isProcNetDevStatisticPresent)
	{
		DEBUG2("PPP State changed to %s\n", myIsProcNetDevStatPresent?"present":"gone");
		isProcNetDevStatisticPresent = myIsProcNetDevStatPresent;
		// did we start the connection?
		if (myIsProcNetDevStatPresent == true)
		{
			theMonthlyTraffic.start(isRoaming());
		}
	}

	if (myCounterBytesSent > theCounterBytesSent)
		theUpDelta = myCounterBytesSent - theCounterBytesSent;
	else
		theUpDelta = 0;
	theCounterBytesSent = myCounterBytesSent;
	
	if (myCounterBytesReceived > theCounterBytesReceived)
		theDownDelta = myCounterBytesReceived - theCounterBytesReceived;
	else
		theDownDelta = 0;
	theCounterBytesReceived = myCounterBytesReceived;
	
	// whatever the outcome is, let's update the monthly traffic stats
	theMonthlyTraffic.update(theCounterBytesSent, theCounterBytesReceived);
}


const QString&  
ConnectionInfo::resolveNetworkIDtoName(const QString& aNetworkID)
{
	// aNetworkID should contain a string containing just a number, in the
	// range between 20,201 (Belgacom, Belgium) and 732,111 (Colombia Movil, Colombia)
	// (of course, these are observed numbers - numbers outside this range may exist)
	
	unsigned long myID = aNetworkID.toULong();
	assert(myID > 9999 && myID < 999999);
	
	if (thePreviousNetworkID==myID && theOperatorPtr!=NULL)
		return theOperatorPtr->getNetworkName();
	
	// if we get here, we definitely need to do a lookup
	thePreviousNetworkID = myID;
	if (theOperatorPtr != NULL)
	{
		delete theOperatorPtr;
		theOperatorPtr = NULL;
	}
	
	// the lookup will always return a valid class, 
	// if necessary containing bogus info
	theOperatorPtr = Operator::lookup(aNetworkID);
	assert(theOperatorPtr != NULL);
	return theOperatorPtr->getNetworkName();
}






void ConnectionInfo_ACM::refreshRegistration(void)
{
	DEBUG5("ConnectionInfo_ACM::refreshRegistration(void)\n");
	// if we're allowed to use CGREG/CREG, let's immediately do that
	if (isCGREGUsed==false)
	{
		if (theSignalQuality>0 && theSignalQuality<99)
		{
			// we have a signal quality - we must be registered!
			// unfortunately no way to figure out if roaming :-(
			theRegistrationValue = REGISTERED_HOME;
		}
		else
		{
			// oh. we don't have a means to figure out registration info.
			theRegistrationValue = TRYING_TO_REGISTER;
		}
	}
	else
	{
		refreshRegistration();
	}
}

void 
ConnectionInfo_HSO::refreshTrafficStats(void)
{
	DEBUG3("ConnectionInfo_HSO::refreshPPPStats\n");
	unsigned long myCounterBytesSent = 0;
	unsigned long myCounterBytesReceived = 0;
	
	// for HSO devices, if the hso0 line is present in /proc/net/dev, it 
	// will remain there for as long as the device is inserted.
	// that makes it easier compared to the PPP case:
	//    (no need to check if the line goes away)
	// And harder at the same time:
	//    (if umtsmon is quit and restarted, we need to adjust for the in hs0)
	
	// update the rx and tx byte values
	bool myIsProcNetDevPresent = false;
	QFile myFile("/proc/net/dev");
	if (myFile.open(IO_ReadOnly) == true)
	{
		QString myLine;
		// FIXME: This is hardcoded for hso0!!!
		QRegExp myHSORegExp("hso0");
		while(myFile.readLine(myLine, 255)> 0)
		{
			// perform substring searching
			if (myHSORegExp.search(myLine,0) != -1)
			{
				myIsProcNetDevPresent = true;
				QStringList myList = QStringList::split(QRegExp("[ :]"), myLine);
				myCounterBytesSent = myList[9].toLong();
				myCounterBytesReceived = myList[1].toLong();
				break;	// breaks while loop
			}
		}
		myFile.close();
	}
	
	// or did we change state?
	if (isProcNetDevStatisticPresent==false && myIsProcNetDevPresent==true)
	{
		DEBUG2("HSO State appeared in /proc/net/dev\n");
		isProcNetDevStatisticPresent = true;
		theBaseSent = myCounterBytesSent;
		theBaseReceived = myCounterBytesReceived;
		theMonthlyTraffic.start(isRoaming());
	}
	
	// adjust for the start value
	myCounterBytesSent -= theBaseSent;
	myCounterBytesReceived -= theBaseReceived;

	if (myCounterBytesSent > theCounterBytesSent)
		theUpDelta = myCounterBytesSent - theCounterBytesSent;
	else
		theUpDelta = 0;
	theCounterBytesSent = myCounterBytesSent;
	
	if (myCounterBytesReceived > theCounterBytesReceived)
		theDownDelta = myCounterBytesReceived - theCounterBytesReceived;
	else
		theDownDelta = 0;
	theCounterBytesReceived = myCounterBytesReceived;
	
	// whatever the outcome is, let's update the monthly traffic stats
	theMonthlyTraffic.update(theCounterBytesSent, theCounterBytesReceived);
}

void ConnectionInfo_ACM_Ericsson::refreshSignalQuality(void)
{
	DEBUG4("ConnectionInfo_ACM_Ericsson::refreshSignalQuality(void)\n");
	// only send Query if allowed
	if (isInhibited())
	{
		theSignalQuality=99;
		return;
	}
	
	// only do this when registered 
	//  - data won't make sense otherwise	
	if (!isDeviceRegistered())
		theSignalQuality=99;
	else
	{
		// CIND returns follwing values:
		// +CIND: ("battchg",(0-5)),("signal",(0-5)),("batterywarning",(0-1)),("chargerconnected",(0-1)),("service",(0-1)),("sounder",(0-1)),("message",(0-1)),("call",(0-1)),("roam",(0-1)),("smsfull",(0-1)),("callsetup",(0-3)),("callheld",(0-1))

		Query myQuery("AT+CIND?");
		if (myQuery.runUntilDone() != Query::OK)
		{
			// no big deal, it happens, fail silently
			DEBUG3("Query.run() failed\n");
			return;
		}
		// signalquality is second parameter ranging from 0 .. 5 
		// result: '+CIND: 3,5,0,0,...'  -> '3,5,0,0,...'
		QString myResult = myQuery.getAnswer().first().section(" ",1,1);
		// result: '3,5,0,0,...' -> int 5 and convert it it...
		switch (myResult.section(",",1,1).toInt()) {
		case 0: theSignalQuality = 0; break;
		case 1: theSignalQuality = 5; break;
		case 2: theSignalQuality = 10; break;
		case 3: theSignalQuality = 15; break;
		case 4: theSignalQuality = 20; break;
		case 5: theSignalQuality = 25; break;
		default: theSignalQuality = 99; break;
		}
	}
	DEBUG4("ConnectionInfo_ACM_Ericsson::theSignalQuality is %i\n",theSignalQuality);
}
