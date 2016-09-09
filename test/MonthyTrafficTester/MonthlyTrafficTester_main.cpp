/* umtsmon - a program to control/monitor your UMTS card
 * Copyright (C) 2007  Klaas van Gend
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

#include <assert.h>
#include <errno.h>

#include "MonthlyTraffic.cpp"

#include <qdir.h>
#include "../TestFrame.cpp"

// conveniently set theVerbosity to a number of your liking
int  theVerbosity = 5;

/** this class is a friend of MonthlyTraffic and therefore
 *  can access the innards of MonthlyTraffic
 */
class MonthlyTrafficTester
{
public:
	MonthlyTrafficTester(MonthlyTraffic& anMTRef)
		: theMTRef(anMTRef)
		{};

	/** use to manipulate the date of a MonthlyTraffic instance
	 *  note that we use 'whitebox' knowledge here
	 */
	void setDate(int aYear, int aMonth)
		{
			theMTRef.theDate.setYMD(aYear, aMonth, theMTRef.theDate.day());
		}; 

	bool getIsRoaming(void)
		{ return theMTRef.isRoaming;};
	unsigned long getCounterBytesSent(void)
		{ return theMTRef.theCounterBytesSent;};
	unsigned long getCounterBytesReceived(void)
		{ return theMTRef.theCounterBytesReceived;};
	unsigned long getHomeTraffic(void)
		{ return theMTRef.theHomeTraffic;};
	unsigned long getRoamingTraffic(void)
		{ return theMTRef.theRoamingTraffic;};	

	void setHome(unsigned long aNumber)
		{ theMTRef.theHomeTraffic = aNumber;};
	void setRoaming(unsigned long aNumber)
		{ theMTRef.theRoamingTraffic = aNumber;};
	void setReceivedBytes(unsigned long aNumber)
		{ theMTRef.theCounterBytesReceived = aNumber;};
	void setSentBytes(unsigned long aNumber)
		{ theMTRef.theCounterBytesSent = aNumber;};

	void save(void)
		{ theMTRef.save();};

	MonthlyTraffic& theMTRef;
};


void removeSettings(void)
{
	// a config file of this tester should be called $HOME/.umtsmontester/umtsmontesterrc
	// Let's check if such directory exists, and kill it
	QString mySettingsLocation = QDir::homeDirPath() + "/." + APPNAME;
	QString mySettingsFile = mySettingsLocation + "/" + APPNAME + "rc";
	QString mySettingsLockFile = mySettingsLocation + "/." + APPNAME + "rc.lock";
	QDir mySettingsDir(mySettingsLocation);

	if (mySettingsDir.exists())
	{
		testmsg("** removing settingsdir %s\n", mySettingsLocation.ascii());
		check (mySettingsDir.remove(mySettingsFile), "removal of exisiting file '" 
			+ mySettingsFile + "'\n");
		check (mySettingsDir.remove(mySettingsLockFile), "removal of exisiting file '" 
			+ mySettingsLockFile + "'\n");
		check (mySettingsDir.rmdir(mySettingsLocation), "removal of exisiting dir '" 
			+ mySettingsLocation + "'\n", true);
	}
	check(!mySettingsDir.exists(), "settings directory " + mySettingsLocation 
				+ " does not exist (anymore)\n");
}


int main()
{
	// I'm not entirely sure if this is allowed and/or works
	errno = 0;
	
	// welcome to user
	printf("Test app is called '%s' version '%s'\n", APPNAME, APPRELEASE);
	// and check if the APPNAME is correct
	check(QString(APPNAME) != QString("umtsmon"), "Tester is not called umtsmon\n", true);

	removeSettings();

	QDate theCurrentDate = QDate::currentDate();
	unsigned long theTotalHomeTraffic = 0;
	unsigned long theTotalRoamingTraffic = 0;


	//
	// FIXME: no attempt is made to check for the warning upon reaching the threshold
	//

chapter("simple persistancy and update tests\n");
	// let's create a MonthlyTester and update and save and kill it
	{
    MonthlyTraffic myMT(false);
    MonthlyTrafficTester myMTT(myMT);
    myMT.update(123, 256);
    check(myMTT.getIsRoaming()==false, "not roaming - good\n");
    check(myMTT.getCounterBytesSent()==123, "bytes sent is correct\n");
    check(myMTT.getCounterBytesReceived()==256, "bytes received is correct\n");
    theTotalHomeTraffic = 684+511;
    check( myMT.update(684, 511) == theTotalHomeTraffic, "update returns the right numbers\n");
    check(myMTT.getCounterBytesSent()==684, "bytes sent is correct\n");
    check(myMTT.getCounterBytesReceived()==511, "bytes received is correct\n");
    check(myMTT.getHomeTraffic()==0, "no home traffic yet - nothing saved\n");
    check(myMTT.getRoamingTraffic()==0, "no roaming traffic yet\n");
	}
	// check for persistence of the above home RX+TX *and* 
	// check if roaming doesn't influence that 
	{
    MonthlyTraffic myMT(true);
    MonthlyTrafficTester myMTT(myMT);
    check(myMTT.getIsRoaming()==true, "roaming - good\n");
    check(myMTT.getCounterBytesSent()==0, "bytes sent is correct\n");
    check(myMTT.getCounterBytesReceived()==0, "bytes received is correct\n");
    check(myMTT.getHomeTraffic()==theTotalHomeTraffic, "home traffic is correct\n");
    //
    theTotalRoamingTraffic = 345+678;
    check(myMT.update(345, 678) == theTotalRoamingTraffic, "update returns the right numbers\n");
    check(myMTT.getCounterBytesSent()==345, "bytes sent is correct\n");
    check(myMTT.getCounterBytesReceived()==678, "bytes received is correct\n");
    check(myMTT.getHomeTraffic()==theTotalHomeTraffic, "home traffic is still correct\n");
    check(myMTT.getRoamingTraffic()==0, "roaming traffic is still zero\n");
	}
	// check for persistence of the above home *and* roaming traffic numbers
	{ 
    MonthlyTraffic myMT(false);
    MonthlyTrafficTester myMTT(myMT);
    check(myMTT.getHomeTraffic()==684+511, "home traffic is correct\n");
    check(myMTT.getRoamingTraffic()==345+678, "roaming traffic is correct\n");
	}
	// check for larger numbers
	{
    MonthlyTraffic myMT(false);
    MonthlyTrafficTester myMTT(myMT);
    theTotalHomeTraffic += 1234684L+123456789L;
    check( myMT.update(1234684L, 123456789L) == theTotalHomeTraffic, "update works for larger numbers\n");
	}	
printf("theTotalHomeTraffic : %lu\n", theTotalHomeTraffic);
	
chapter("thresholds and limits");
	// Within home network, let's test for 3 cases, that means adapting a Profile 3x	
	{
	Profile myProfile;
	myProfile.setHomeDataLimit(5);
	myProfile.setThresholdPercentage(50);
	myProfile.save();
    MonthlyTraffic myMT(false);
    MonthlyTrafficTester myMTT(myMT);
    check( myMT.getThresholdStatus()==MonthlyTraffic::OVER_LIMIT, "over limit is correctly detected\n");
	}	
	{
	Profile myProfile;
	// keep in mind that the Profile's data limits are in megabytes, not in bytes
	myProfile.setHomeDataLimit(static_cast<unsigned long>((theTotalHomeTraffic>>20)*1.5));
	myProfile.save();
	MonthlyTraffic myMT(false);
	MonthlyTrafficTester myMTT(myMT);
	check( myMT.getThresholdStatus()==MonthlyTraffic::BETWEEN, 
	       "inbetween threshold and limit is correctly detected\n");
	}	
	{
	Profile myProfile;
	// keep in mind that the Profile's data limits are in megabytes, not in bytes
	myProfile.setHomeDataLimit(theTotalHomeTraffic);
	myProfile.save();
	MonthlyTraffic myMT(false);
	MonthlyTrafficTester myMTT(myMT);
	check( myMT.getThresholdStatus()==MonthlyTraffic::BELOW, 
	       "below threshold is correctly detected\n");
	}	
	{
	Profile myProfile;
	// keep in mind that the Profile's data limits are in megabytes, not in bytes
	myProfile.setHomeDataLimit(0);
	myProfile.save();
	MonthlyTraffic myMT(false);
	MonthlyTrafficTester myMTT(myMT);
	check( myMT.getThresholdStatus()==MonthlyTraffic::BELOW, 
	       "no limit set, should result in BELOW\n");
	}	
	// and do two tests for Roaming to cover all lines there
	{
	Profile myProfile;
	// keep in mind that the Profile's data limits are in megabytes, not in bytes
	myProfile.setRoamingDataLimit(theTotalRoamingTraffic);
	myProfile.save();
	MonthlyTraffic myMT(true);
	MonthlyTrafficTester myMTT(myMT);
	check( myMT.getThresholdStatus()==MonthlyTraffic::BELOW, 
	       "below threshold for roaming is correctly detected\n");
	}	
	{
	Profile myProfile;
	// keep in mind that the Profile's data limits are in megabytes, not in bytes
	myProfile.setRoamingDataLimit(0);
	myProfile.save();
	MonthlyTraffic myMT(true);
	MonthlyTrafficTester myMTT(myMT);
	check( myMT.getThresholdStatus()==MonthlyTraffic::BELOW, 
	       "no limit set for roaming, should result in BELOW\n");
	}	
	
	
chapter("more complex cases part 1: restart of ppp\n");	
	// ppp returns zero traffic for both - that should trigger a save
	{ 
    MonthlyTraffic myMT(false);
    MonthlyTrafficTester myMTT(myMT);
    check(myMTT.getHomeTraffic()==theTotalHomeTraffic, "home traffic is correct\n");
    theTotalHomeTraffic += 100+102;
    check(myMT.update(100,102)==theTotalHomeTraffic, "update 1 returns correct number\n");
    check(myMT.update(0,0)==theTotalHomeTraffic, "update 2 returns correct number\n");
	}

	// ppp returns a lower TX - that should trigger a save
	{ 
    MonthlyTraffic myMT(true);
    MonthlyTrafficTester myMTT(myMT);
    check(myMTT.getRoamingTraffic()==theTotalRoamingTraffic, "roaming traffic is correct\n");
    theTotalRoamingTraffic += 23221+36789;
    check(myMT.update(23221,36789)==theTotalRoamingTraffic, "update 1 returns correct number\n");
    theTotalRoamingTraffic += (23456-23221) + 12345;
    check(myMT.update(23456,12345)==theTotalRoamingTraffic, "update 2 returns correct number\n");
	}
	
chapter("more complex cases part 2: rollover of date\n\n");
	{
	QDate myPreviousMonth = theCurrentDate.addMonths(-1);
	MonthlyTraffic myMT(true, myPreviousMonth.year(), myPreviousMonth.month());
    MonthlyTrafficTester myMTT(myMT);
    // setup the old month 
    myMTT.setHome   (3456789);
    myMTT.setRoaming(2345678);
    myMTT.save();
	}
	// check that a current month doesn't know
	{
    MonthlyTraffic myMT(true);
    MonthlyTrafficTester myMTT(myMT);
    check(myMTT.getRoamingTraffic()==theTotalRoamingTraffic, "roaming traffic for this month is correct\n");
    check(myMTT.getHomeTraffic()==theTotalHomeTraffic, "home traffic for this month is correct\n");
	}
	// and check that a previous month does
	{
	QDate myPreviousMonth = theCurrentDate.addMonths(-1);
	MonthlyTraffic myMT(true, myPreviousMonth.year(), myPreviousMonth.month());
    MonthlyTrafficTester myMTT(myMT);
    check(myMTT.getRoamingTraffic()==2345678, "roaming traffic for last month is correct\n");
    check(myMTT.getHomeTraffic()==3456789, "home traffic for last month is correct\n");
    myMTT.setReceivedBytes(2345);
    myMTT.setSentBytes    (1234);
    try
    {
    	myMT.update(1244,2368);
    } 
    catch (MonthRolloverException myException)
    {
    	check(true, "update detected a new month and threw an exception\n");
    };
    // note that after the exception, the MonthlyTraffic still exists...
    // data is not saved yet
    check(myMTT.getHomeTraffic()==3456789, "home traffic for last month is untouched\n");
	// and because the instance still exists, new updates will still work 
    check(myMT.update(1254,2368)==2345678+1254+2368, "new update()s still work as expected\n");
	}
    
	// keep this last for summary:
	finish();
	return 0;
}

