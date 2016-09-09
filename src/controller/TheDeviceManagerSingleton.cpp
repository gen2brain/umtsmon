/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2007, 2009  Klaas van Gend
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
#include "TheDeviceManagerSingleton.h"
#include "Device.h"
#include "TheSettingsSingleton.h"
#include "Popup.h"
#include "Runner.h"
#include "FileStuff.h"
#include "HelperList.h"

#include <typeinfo>
#include <assert.h>

static TheDeviceManagerSingleton* theDeviceMgrSingletonInstancePtr = NULL;
static Device* theStaticDevicePtr = NULL;


static const char* thePPPKeyStr = "/device/PPPPortName";
static const char* theATKeyStr = "/device/ATPortName";
static const char* theDeviceTypeKeyStr = "/device/DeviceType";


TheDeviceManagerSingleton::TheDeviceManagerSingleton()
{
	DEBUG5("TheDeviceManagerSingleton::TheDeviceManagerSingleton()\n");

	// before doing anything else, let's make sure there's not a pppd running already
	Runner myRunner;
	if (myRunner.doesPPPDexist()!=-1)
	{
		// FIXME: remove this Popup...
		Popup::Critical(QObject::tr("Apparently there is already a pppd process running.\n"
			"This prevents umtsmon from detecting your card,\numtsmon will now close."), NULL);
	}

	theStaticDevicePtr = Device::createDeviceInstance(DeviceCapabilities::NULLDEVICE); 	
	
	atexit(destroy);
}

void TheDeviceManagerSingleton::destroy(void)
{
	// this delete will trigger the destructor
	delete theDeviceMgrSingletonInstancePtr;
	theDeviceMgrSingletonInstancePtr = NULL;
}

TheDeviceManagerSingleton::~TheDeviceManagerSingleton(void)
{
	DEBUG5("DESTRUCTOR TheDeviceManagerSingleton::~TheDeviceManagerSingleton()\n");
	emptyAutoDetectList();
	if (theStaticDevicePtr)
		delete theStaticDevicePtr;
}


// dear programmers, keep in mind that this member is static,
// i.e. you cannot use any of the other members in the class, 
// nor any of the variables 
TheDeviceManagerSingleton& TheDeviceManagerSingleton::me(void)
{
	// do we exist? if not: create us
	if (theDeviceMgrSingletonInstancePtr == NULL)
		theDeviceMgrSingletonInstancePtr = new TheDeviceManagerSingleton();
	return *theDeviceMgrSingletonInstancePtr;
}

// dear programmers, keep in mind that this member is static,
// i.e. you cannot use any of the other members in the class, 
// nor any of the variables 
Device& TheDeviceManagerSingleton::getDeviceRef(void)
{
	// do we exist? if not: create us
	if (theStaticDevicePtr == NULL)
	{
		me();
	}

	// at this moment, we are very sure that a DevicePtr exists,
	// even though it might be a null device...	
	assert(theStaticDevicePtr != NULL);
	return *theStaticDevicePtr;
}

bool
TheDeviceManagerSingleton::setupDevice(void)
{
	DEBUG3("TheDeviceManagerSingleton::setupDevice(void)\n");
	// did the user config on the command line?
	// did he force us not to look at the config file?
	if (theNonAutoDetectClass.theSuggestedATPortName.isEmpty() 
	    && theNonAutoDetectClass.theForceAutodetection==false)
	{
		readFromConfigFile();
	}

	// no? then let's run the autodetectors
	// note that the first autodetector is the NonAutoDetect,
	// that - if we have specified settings on command line or in 
	// the config file - will instantiate the Device immediately
	// without going through the autodetectors
	DEBUG3("Start iterating through all AutoDetectors\n\n");
	DEBUG4("Kernel: '%s'\n", FileStuff::getFileFirstLine("/proc/version").ascii());
	AutoDetectList::iterator myDetector = theAutoDetectList.begin();
	while (myDetector != theAutoDetectList.end())
	{
		try
		{
			if ((*myDetector)->go() == AutoDetectBase::DEVICE_DETECTED)
			{
				if (doDeviceCreation(**myDetector))
				{
					DEBUG3("Everything done - device created\n\n");
					
					// at this point, we know where to expect the serial port(s)
					// let's do some checks on read/write access now - for the ports
					// and also suid checks on the helper applications we have
					HelperList myList1;
					if (myList1.testAndRun() == HelperList::QUIT)
						exit(0);
					
					return true;
				}
			}
		}
		catch (StopAutodetectionCycle)
		{
			// break this while - apparently it makes no sense to continue
			DEBUG3("Autodetection cycle is halted for now...\n");
			break;
		}
		++myDetector;
		DEBUG3(" Let's continue with the next AutoDetector\n\n");
	}
	DEBUG3(" No device detected...\n");
	return false;
}


bool
TheDeviceManagerSingleton::doDeviceCreation(AutoDetectBase& aSuggestedAutoDetector)
{
	Device* myDevicePtr = aSuggestedAutoDetector.createDevice();
	if (myDevicePtr==NULL)
		return false;
	theStaticDevicePtr = myDevicePtr;
	
	// OK, so we got a real Device now. Let's store its settings for future use
	// but only if we have not used the data from the config file...
	if (typeid(aSuggestedAutoDetector) != typeid(NonAutoDetect))
	{
		DEBUG1("Found hardware configuration stored to disk\n");
		writeToConfigFile();
	}
	else
	{
		DEBUG2("Using existing hardware config, not written to disk\n");
	}

	return true;	
}

void
TheDeviceManagerSingleton::emptyAutoDetectList(void)
{
	// the list is a list of pointers
	// just delete the pointers and pop them should be enough
	
	// keep in mind that the first one should be the non-autodetect
	theAutoDetectList.pop_front();
	
	while (!theAutoDetectList.isEmpty())
	{
		delete theAutoDetectList.last();
		theAutoDetectList.pop_back();
	}
}

void TheDeviceManagerSingleton::readFromConfigFile(void)
{
	DEBUG3("TheDeviceManagerSingleton::readFromConfigFile()\n");
	QString myPPPString;
	QString myATString;
	QString myDeviceTypeString;
	unsigned long myDeviceTypeLong;
	
	// PPP
	myPPPString = TheSettingsSingleton::getQSRef().readEntry(thePPPKeyStr, "");
	DEBUG4("read PPP port from config: '%s'.\n", myPPPString.ascii());  
	if (myPPPString.isEmpty())
		goto nogood; 
	
	// AT
	myATString = TheSettingsSingleton::getQSRef().readEntry(theATKeyStr, "");
	DEBUG4("read AT port from config: '%s'.\n", myATString.ascii());  
	if (myATString.isEmpty())
		goto nogood; 

	// read hexadecimal type string
	myDeviceTypeString = TheSettingsSingleton::getQSRef().readEntry(theDeviceTypeKeyStr, 
		QString::number(DeviceCapabilities::UNKNOWN));
	myDeviceTypeLong = myDeviceTypeString.toULong(NULL, 0); 
	DEBUG4("read device type from config: 0x%08lX\n", myDeviceTypeLong);
	if  (myDeviceTypeLong == DeviceCapabilities::UNKNOWN)
		goto nogood; 
	
	// good - we read a full set of parameters
	theNonAutoDetectClass.theSuggestedDeviceType = static_cast<DeviceCapabilities::CardType>(myDeviceTypeLong); 
	theNonAutoDetectClass.theSuggestedPPPPortName = myPPPString;
	theNonAutoDetectClass.theSuggestedATPortName = myATString;

	DEBUG3("Complete config read from file - good :-)\n");
	return;

nogood:
	// bad - no full set 
	DEBUG1("Stored device configuration incomplete, reverting to autodetection\n");
    theNonAutoDetectClass.theForceAutodetection = true;
}

void TheDeviceManagerSingleton::writeToConfigFile(void)
{
	DEBUG3("TheDeviceManagerSingleton::writeToConfigFile()\n");
	assert(theStaticDevicePtr != NULL);
	assert(!theStaticDevicePtr->getPPPSerialPortName().isEmpty());
	TheSettingsSingleton::getQSRef().writeEntry(thePPPKeyStr, 
			theStaticDevicePtr->getPPPSerialPortName() );	
	assert(!theStaticDevicePtr->getATSerialPortName().isEmpty());
	TheSettingsSingleton::getQSRef().writeEntry(theATKeyStr, 
			theStaticDevicePtr->getATSerialPortName() );
	// write type string in hex	
	assert(theStaticDevicePtr->getDeviceType()!=DeviceCapabilities::UNKNOWN);
	TheSettingsSingleton::getQSRef().writeEntry(theDeviceTypeKeyStr, 
			"0x" + QString::number(theStaticDevicePtr->getDeviceType(),16) );	
}

void
TheDeviceManagerSingleton::setSuggestedAT(const QString& anATPortName)
{
	theNonAutoDetectClass.theSuggestedATPortName = anATPortName;
	DEBUG1("Set suggested AT port to '%s'\n", anATPortName.ascii()); 
}

void
TheDeviceManagerSingleton::setSuggestedPPP(const QString& aPPPPortName)
{
	theNonAutoDetectClass.theSuggestedPPPPortName = aPPPPortName;
	DEBUG1("Set suggested PPP port to '%s'\n", aPPPPortName.ascii()); 
}

void
TheDeviceManagerSingleton::setSuggestedDeviceType(const DeviceCapabilities::CardType aDeviceType)
{
	theNonAutoDetectClass.theSuggestedDeviceType = aDeviceType;
}

void
TheDeviceManagerSingleton::setupAutoDetectList(void)
{
	if (!theAutoDetectList.isEmpty())
	{
		emptyAutoDetectList();
	}
	
	// the non-autodetect is the first in the autodetect list
	// if anything gets supplied to this autodetector, it will attempt to
	// create the device
	// if that fails, the other autodetects will take over
	theAutoDetectList.push_back(&theNonAutoDetectClass);
	
	// warning: make sure these remain in this order, because otherwise
	// you might get errors (e.g. the PCMCIA will error if it finds a USB2serial-based card)
	// and one definitely wants to use the USB autodetect over the RawUSBports autodetect 
	theAutoDetectList.push_back(new AutoDetect_USB_ZeroCD());
	theAutoDetectList.push_back(new AutoDetect_USB());
	theAutoDetectList.push_back(new AutoDetect_PCMCIA());
	theAutoDetectList.push_back(new AutoDetect_CSSerialPorts());
	theAutoDetectList.push_back(new AutoDetect_PhoneModems());
	theAutoDetectList.push_back(new AutoDetect_RawUSBports());
	//	theAutoDetectList.push_back(new AutoDetect_RawSerialPorts());
}

void
TheDeviceManagerSingleton::setForceAutodetection(void)
{
	theNonAutoDetectClass.theForceAutodetection = true;
	DEBUG1("Set force autodetection to true\n"); 
}

bool
TheDeviceManagerSingleton::tryDeviceDetection(void)
{
	setupAutoDetectList();
	return setupDevice();
}
