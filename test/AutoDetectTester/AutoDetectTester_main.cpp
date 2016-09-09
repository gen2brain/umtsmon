/* umtsmon - a program to control/monitor your UMTS card
 * This file (C) 2007, 2008  Klaas van Gend
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
#include "AutoDetect.h"

#include <qstring.h>

#include <assert.h>
#include <errno.h>

#include "../TestFrame.cpp"

// conveniently set theVerbosity to a number of your liking
int  theVerbosity = 5;


// we subclass the AutoDetect_PCMCIA to get access to some of 
// the protected variables
class AutoDetectTesterP : public AutoDetect_PCMCIA
{
public:
	Device::CardType         getDeviceID(void) {return theDeviceID;};

	void setDeviceID(Device::CardType         myID) {theDeviceID = myID;};
	
	const QString& getAT (void) {return thePortsList.last();};
	const QString& getPPP(void) {return thePortsList.first();};

	void setAT (QString myAT) {thePortsList.push_front(myAT);};
	void setPPP (QString myPPP) {thePortsList.push_back(myPPP);};
};

class AutoDetectTesterU : public AutoDetect_USB
{
public:
	Device::CardType         getDeviceID(void) {return theDeviceID;};

	void setDeviceID(Device::CardType         myID) {theDeviceID = myID;};
	
	const QString& getAT (void) {return thePortsList.last();};
	const QString& getPPP(void) {return thePortsList.first();};

	void setAT (QString myAT) {thePortsList.push_front(myAT);};
	void setPPP (QString myPPP) {thePortsList.push_back(myPPP);};
};

class AutoDetectTesterR : public AutoDetect_Rawport
{
public:
	Device::CardType         getDeviceID(void) {return theDeviceID;};

	void setDeviceID(Device::CardType         myID) {theDeviceID = myID;};
	
	const QString& getAT (void) {return thePortsList.last();};
	const QString& getPPP(void) {return thePortsList.first();};

	void setAT (QString myAT) {thePortsList.push_front(myAT);};
	void setPPP (QString myPPP) {thePortsList.push_back(myPPP);};
	
	virtual QString getName() {return "tester";};
};



int main()
{
	// I'm not entirely sure if this is allowed and/or works
	errno = 0;
	
	// welcome to user
	printf("Test app is called '%s' version '%s'\n", APPNAME, APPRELEASE);


#if 0
	AutoDetectTesterP myPCMCIA;
	check(!myPCMCIA.getName().isEmpty(), "PCMCIA class does have a name\n");
	check( myPCMCIA.getVendorID() == 0, "VendorID is 0\n");
	check( myPCMCIA.getDeviceID() == 0, "DeviceID is 0\n");

	// let's do some matching
	check( myPCMCIA.matchIDs(0,0) == AutoDetectBase::NOTHING_FOUND, "no known vendor 0\n"); 
	check( myPCMCIA.matchIDs(CardDetection::NEC,0) == AutoDetectBase::DEVICE_ERROR, 
			"should not detect NEC based cards here\n"); 
	check( myPCMCIA.matchIDs(CardDetection::OPTION,0) == AutoDetectBase::DEVICE_DETECTED, 
			"supplying vendor OPTION already gives Nozomi\n"); 
	check( myPCMCIA.getDeviceID() == Device::OPTION_NOZOMI, "DeviceID is now NOZOMI\n");
	check( myPCMCIA.findPortNames() == false, "no Nozomi card inserted - ports are not available\n");
	myPCMCIA.setVendorID(CardDetection::UNKNOWN);
	myPCMCIA.setDeviceID(Device::UNKNOWN);
#endif

#if 0
	printf("\n*** Let's do some real tests:\n"
       "From here onwards, the test code expects a GC79 in the PCMCIA slot\n\n");
	// at this moment, Klaas has the SonyEricsson GC79 inserted.
	check( myPCMCIA.go() == AutoDetectBase::DEVICE_DETECTED, "Found the GC79 device\n");
	check( myPCMCIA.getDeviceID() == Device::BROADCOM_GPRS_EDGE, "DeviceID is now BROADCOM_GPRS_EDGE\n");

	check( myPCMCIA.getAT() == "/dev/ttyS1", "device name is now correct\n"); 
	check( myPCMCIA.getPPP()== "/dev/ttyS1", "AT and PPP names are the same\n"); 
#endif


#if 0
	AutoDetectTesterU myUSB;
	check(!myUSB.getName().isEmpty(), "USB class does have a name\n");
	printf("\n*** Let's do some real tests:\n"
       "From here onwards, the test code expects a Option GT in the PCMCIA slot\n\n");
	// at this moment, Klaas has the Option GT Max inserted.
	check( myUSB.go() == AutoDetectBase::DEVICE_DETECTED, "Found the GT device\n");
	check( myUSB.getDeviceID() == Device::OPTION_3PORT, "DeviceID is now OPTION_3PORT\n");

	check( myUSB.getAT() == "/dev/ttyUSB0", "device name is now correct\n"); 
	check( myUSB.getPPP()== "/dev/ttyUSB2", "AT and PPP names are different\n"); 
#endif

#if 1
	AutoDetectTesterR myRaw;
	check(!myRaw.getName().isEmpty(), "Raw class does have a name\n");
	printf("\n*** Let's do some real tests:\n"
       "From here onwards, the test code expects a Option GT in the PCMCIA slot\n\n");
	// at this moment, Klaas has the Option GT Max inserted.
	check( myRaw.go() == AutoDetectBase::DEVICE_DETECTED, "Found the GT device\n");
	check( myRaw.getDeviceID() == Device::OPTION_3PORT, "DeviceID is now OPTION_3PORT\n");

	check( myRaw.getAT() == "/dev/ttyUSB0", "device name is now correct\n"); 
	check( myRaw.getPPP()== "/dev/ttyUSB2", "AT and PPP names are different\n"); 
#endif

	// keep this last for summary:
	finish();
	return 0;
}



void printAbortMessage(__attribute__ ((unused)) int aSignalNumber)
{
	printf("\n\n  *** " APPNAME " version " APPRELEASE " closed due to an"
	           " unrecoverable program error.\n"
	       "  *** Please e-mail all of the above output to"
	           " umtsmon-develop@lists.sourceforge.net,\n"
	       "  *** Or go to the " APPNAME " website and file a bug.\n\n\n"); 
	// do not call exit here - or the signal will not be handled correctly
}

