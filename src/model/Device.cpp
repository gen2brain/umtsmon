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

#include "umtsmon_global.h"
#include "Device.h"
#include "Query.h"
#include "SerialPort.h"
#include "Popup.h"


Device::~Device()
{
	closeATSerial();
}


void
Device::closeATSerial(void)
{
	if (theATSerialPortPtr)
	{
		delete theATSerialPortPtr;
		theATSerialPortPtr = NULL;
		Query::setSerial(NULL);
	}
}

bool 
Device::resetATSerial()
{
	assert(theATSerialPortPtr != NULL);
	if (theATSerialPortPtr == NULL)
		return false;
	return theATSerialPortPtr->resetDev();
}

Device* 
Device::createDeviceInstance(CardType aCardType)
{
	Device* myDevicePtr = NULL;
	
	DEBUG3("Device creation for type 0x%08x requested\n", aCardType);
	switch (aCardType)
	{
	case UNKNOWN:
		// boo, bad, don't get here!!!
		// - will be handled outside the switch()
		DEBUG1("Attempt on creating device UNKNOWN - not good\n");
		break;
	case OPTION_3PORT: // is also for OPTION_4PORT
	case OPTION_NOZOMI:
		myDevicePtr = new Device_Option;
		break;
	case OPTION_HSO:
		myDevicePtr = new Device_OptionHSO;
		break;
	case OPTION_VODAFONE_3G:
		myDevicePtr = new Device_Vodafone3G;
		break;
	case BROADCOM_GPRS_EDGE:
		myDevicePtr = new Device_BroadCom_GPRS_EDGE;
		break;
	case BROADCOM_57K6:
		myDevicePtr = new Device_BroadCom57k6;
		break;
	case HUAWEI_MULTI:
		myDevicePtr = new Device_Huawei;
		break;
	case NULLDEVICE:
		myDevicePtr = new Device_NULL;
		break;
	case SIERRA_2PORT:
		myDevicePtr = new Device_Sierra;
		break;
	case GENERIC_USER_SERIAL:
		myDevicePtr = new Device_User_Serial;
		break;
	case GENERIC_CS_SERIAL:
		myDevicePtr = new Device_CS_Serial;
		break;
	case MODEM_ACM:
		myDevicePtr = new Device_ACM;
		break;
	case MODEM_RFCOMM:
		myDevicePtr = new Device_RFComm;
		break;
	case MODEM_ACM_ERICSSON:
		myDevicePtr = new Device_ACM_Ericsson;
		break;
	}

	if (myDevicePtr != NULL)
		myDevicePtr->theDeviceType = aCardType;
	else
		Popup::Critical("Critical error in device setup\nPlease rerun with -v5\ne-mail results to author");
	return myDevicePtr;
}




bool
Device::probePortForAT( const QString& aPortName ) const
{
	DEBUG3("Device: probing port '%s' for AT commands\n", aPortName.ascii() );
	Query::ReturnCode myResult = Query::ERROR;


	TempSerialPort mySerialPort;

	// the SerialPort class will automatically setup the Query class
	if (mySerialPort.openDev(aPortName) == false)
	{
		goto not_good;
	}

	
	for (int i=0; i<3; i++)
	{
		Query myQueryAT("AT");
		myResult = myQueryAT.run();
 		if (myResult == Query::OK)
 		{
 			DEBUG3("Got Query::OK from port\n");
 			break;
 		}
 		if (myResult != Query::ERROR)
 		{
			DEBUG2("Device port '%s': no response to \"AT\", return code %d\n", 
					aPortName.ascii(), myResult);
			goto not_good;
 		}
 		// in case of ERROR: retry...
		DEBUG2("Device on port '%s' responded with ERROR, sleep & retry\n", 
			aPortName.ascii());
		sleep(1);
	}

not_good:
	// and exit with returncode true or false
	DEBUG3("Device: probing port '%s' %s\n", aPortName.ascii(),
		(myResult == Query::OK)?"successful":"failed");
	return (myResult == Query::OK);
}

bool 
Device::setupPorts(const QStringList& aListOfPorts, int aPortSpeed)
{
	if (theATSerialPortName.isEmpty())
	{
		QString myATName = aListOfPorts.last();
		if (myATName.contains("/dev") > 0)
			theATSerialPortName = myATName;
		else
			theATSerialPortName = "/dev/" + myATName;
	}
	if (thePPPSerialPortName.isEmpty())
	{
		QString myName = aListOfPorts.first();
		if (myName.contains("/dev") > 0)
			thePPPSerialPortName = myName;
		else
			thePPPSerialPortName = "/dev/" + myName;
	}

	// check if the ports respond and adjust if necessary
	if (probePortForAT(thePPPSerialPortName) == false)
		thePPPSerialPortName = theATSerialPortName;
	if (probePortForAT(theATSerialPortName) == false)
	{
		if (theATSerialPortName != thePPPSerialPortName)
		{
			// if the ports are different, we know the PPP port works
			theATSerialPortName = thePPPSerialPortName;
		}
		else
			return false;
	}
		
	DEBUG3("set AT  serial to '%s'\n", theATSerialPortName.ascii()); 
	DEBUG3("set PPP serial to '%s'\n", thePPPSerialPortName.ascii()); 

	theATSerialPortPtr = new LongSerialPort;

	// the SerialPort class will automatically setup the Query class
	if (theATSerialPortPtr->openDev(theATSerialPortName, aPortSpeed, getPortSleepTime()) == false)
	{
		closeATSerial();
		return false;
	}
	
	return true;
}

// ###########################################################################
// ###########################################################################

Device_Option::Device_Option()
{
	theRadioPreferences = QStringList::split("#",
		"Only connect to GSM networks#"
		"Only connect to UMTS networks#"
		"If you have a choice - GPRS first#"
		"If you have a choice - UMTS first#"	
		"Stick to the network the card connects to#"
		"Automatic - let the card decide", false);
	// This list is taken from Paul Hardwick's "Option Fusion Howto" page.

	theConnectionInfoPtr = new ConnectionInfo; 
}


Device_Option::~Device_Option ()
{
	DEBUG5("Device_Option::~Device_Option\n");
	if (theConnectionInfoPtr)
	{
		delete theConnectionInfoPtr;
		theConnectionInfoPtr = NULL;
	}
}


bool
Device_Option::setRadioPreferences(int aPref)
{
	if (aPref<0 || aPref>5)
		return false;

	// FIXME: I have no clue what the second ,2 means...
	Query myQuery( "AT_OPSYS=" + QString::number(aPref) + ",2" );
	
 	Query::ReturnCode myResult = myQuery.runUntilDone();
 	if (myResult == Query::OK)
 		return true;
 	if (myResult == Query::NO_LOCK)
 		return false;
	return false;
} 


// ###########################################################################
// ###########################################################################

bool 
Device_ACM::setupPorts(const QStringList& aListOfPorts)
{
	if (Device::setupPorts(aListOfPorts, getPortSpeed()) == false)
		return false;
	
	// use local scoping to ensure that this Query dies in time
	{
		// we need echo on, but for Moto phones, it's off by default.
		Query myQuery("ATE1");
		// don't use runUntilDone because the answer might not be complete... Sigh!!!
		DEBUG2("ACM: Result from ATE1 query: %d\n", myQuery.run());
	}
	
	return true;
}


// ###########################################################################
// ###########################################################################

bool 
Device_BroadCom_GPRS_EDGE::setupPorts(const QStringList& aListOfPorts)
{
	if (Device_Single_Serial::setupPorts(aListOfPorts) == false)
		return false;
	return doCFUN();
}

bool 
Device_BroadCom_GPRS_EDGE::doCFUN(void)
{
	Query myQueryAT("AT+CFUN=1,1");
	Query::ReturnCode myResult = myQueryAT.runUntilDone();
 	if (myResult != Query::OK)
		return false;
	return true;
}


// ###########################################################################
// ###########################################################################

Device_Huawei::Device_Huawei() : Device_Option() 
{
	theRadioPrefsList.push_back( HuaweiRadioPrefs( 2,1, "GPRS/EDGE Preferred") );
	theRadioPrefsList.push_back( HuaweiRadioPrefs( 2,2, "3G Preferred") );
	theRadioPrefsList.push_back( HuaweiRadioPrefs(13,1, "GPRS/EDGE Only") );
	theRadioPrefsList.push_back( HuaweiRadioPrefs(14,2, "3G only") );

	// the Option constructor fills theRadioPreferences list wrongly.
	// let's make it right.
	theRadioPreferences.clear();
	RadioPrefsList::iterator myI = theRadioPrefsList.begin();
	while (myI != theRadioPrefsList.end())
	{
		theRadioPreferences.push_back( (*myI).theDescription);
		myI++;
	}

	// Device_Option::Device_Option already created a CI instance 
	delete theConnectionInfoPtr;
	// in an attempt to make the Huawei E226 work, we use the
	// ACM version here - no CGREG/CREG queries for Huawei
	theConnectionInfoPtr = new ConnectionInfo_ACM; 
}


bool
Device_Huawei::setupPorts(const QStringList& aListOfPorts)
{
	// duplicate list because we cannot change the original one
	QStringList myList = aListOfPorts;
	
	// Many people reported that the last port of a Huawei device is not usable
	// so, let's remove it from the list...
	while (myList.count() > 2)
	{
		DEBUG3("Huawei workaround in place: drop last port\n");
		myList.pop_back();
	}
	return Device::setupPorts(myList);
}



bool
Device_Huawei::setRadioPreferences(int aPref)
{
	if (aPref<0 || aPref>3)
		return false;

	QString myTempString = "AT^SYSCFG=" 
	         + QString::number(theRadioPrefsList[aPref].theFirst)
	         + "," 
	         + QString::number(theRadioPrefsList[aPref].theSecond)
	         + ",3FFFFFFF,1,2";

	Query myQuery( myTempString);
 	Query::ReturnCode myResult = myQuery.runUntilDone();
 	return (myResult == Query::OK);
} 

// ###########################################################################
// ###########################################################################

Device_Sierra::Device_Sierra()
{
	theRadioPreferences = QStringList::split("#",
		"Automatic - let the card decide#"
		"Connect to UMTS 3G networks only#"
		"Connect to GSM 2G networks only#"
		"If you have a choice - UMTS 3G preferred#"
		"If you have a choice - GSM 2G preferred#"	
		, false);

	theConnectionInfoPtr = new ConnectionInfo; 
}

bool 
Device_Sierra::setupPorts(const QStringList& aListOfPorts)
{
	QStringList myList = aListOfPorts;
	
	assert (thePPPSerialPortName.isEmpty());
	
	// let's assume we have a list of 10 ports ;-)
	// find the first one that has APP1
	QStringList::Iterator myI;
	for (myI = myList.begin(); myI != myList.end(); myI++)
	{
		if (checkATI_for_APP1(*myI))
		{
			thePPPSerialPortName = "/dev/" + *myI;
			myList.erase(myI);
			break;
		}
	}

	// so we now assigned the PPP port and removed it from the list
	// (or we didn't do either if none of the ports replies with APP1)

	// the rest is standard:
	return Device::setupPorts(myList);
}


bool
Device_Sierra::checkATI_for_APP1(const QString& aSerialPort) 
{
	if (probePortForAT(aSerialPort)==false)
		return false;

	DEBUG3("Device: probing port '%s' for APP1 on ATI command\n", aSerialPort.latin1());
	Query::ReturnCode myResult = Query::ERROR;

	TempSerialPort mySerialPort;

	// the SerialPort class will automatically setup the Query class
	if (mySerialPort.openDev(aSerialPort) == false)
	{
		return false;
	}

	Query myQueryATI("ATI");
	myResult = myQueryATI.run();
 	if (myResult == Query::OK)
 	{
 		DEBUG3("  Got Query::OK from port\n");
        if (myQueryATI.hasAnswer()) 
        {
 			for (QStringList::Iterator myIterator = myQueryATI.getAnswer().begin();
			     myIterator !=  myQueryATI.getAnswer().end(); myIterator++)
			{
				QString myAnswer = *myIterator;
				if (myAnswer.contains("APP1\n")) 
				{
					DEBUG3("  Port answered with APP1 on ATI command\n");
					return true;
				}
			}	
 		}
	} 
 	else 
 	{
		DEBUG3("  Got not Query::OK from port\n");
	}

	return false;
}

bool
Device_Sierra::setRadioPreferences(int aPref)
{
	if (aPref<0 || aPref>4)
		return false;

	QString myTempString;
	Query myQuery( "AT!SELRAT=" + myTempString.setNum(aPref));
	
 	Query::ReturnCode myResult = myQuery.runUntilDone();
 	if (myResult == Query::OK)
 		return true;
 	if (myResult == Query::ERROR)
 		return false;
 		
	return false;
} 

int
Device_Sierra::getDefaultRadioPreference()
{
	
	Query myQuery("AT!SELRAT?");
	/* should return for example:
	 *      at!selrat?
	 *	!SELRAT: 03, UMTS 3G Preferred
	 *
	 *	OK
	 *
	 * Possible answers:
	 *  !SELRAT: 00, Automatic
	 *  !SELRAT: 01, UMTS 3G Only
	 *  !SELRAT: 02, GSM 2G Only
	 *  !SELRAT: 03, UMTS 3G Preferred
	 *  !SELRAT: 04, GSM 2G Preferred  
	 */
	Query::ReturnCode myResult = myQuery.runUntilDone();

	if(myResult == Query::OK)
	{
		QString myAnswer = myQuery.getAnswer().first();
		if(myAnswer.contains("00"))
			return 0;
		if(myAnswer.contains("01")) 
			return 1;
		if(myAnswer.contains("02"))
			return 2;
		if(myAnswer.contains("03")) 
			return 3;
		if(myAnswer.contains("04"))
			return 4;	
	}
	
	//if we get here, log error return the default answer
	DEBUG2("Could not getDefaultRadioPreference() from Device\n");
	return 0;
}


bool 
Device_ACM_Ericsson::setupPorts(const QStringList& aListOfPorts, int aPortSpeed)
{
	if (Device::setupPorts(aListOfPorts, aPortSpeed) == false)
		return false;
	Query myQueryAT("AT+CFUN=1,0");
	Query::ReturnCode myResult = myQueryAT.runUntilDone();
	return myResult == Query::OK;
}
