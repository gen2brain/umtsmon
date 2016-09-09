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
#include "Profile.h"

#include <assert.h>
#include <errno.h>

#include "../TestFrame.cpp"
#include "../../src/model/Profile.cpp"

#include <qdir.h>

// conveniently set theVerbosity to a number of your liking
int  theVerbosity = 5;





int main()
{
	// I'm not entirely sure if this is allowed and/or works
	errno = 0;
	
	// welcome to user
	printf("Test app is called '%s' version '%s'\n", APPNAME, APPRELEASE);
	// and check if the APPNAME is correct
	check(QString(APPNAME) != QString("umtsmon"), "Tester is not called umtsmon\n", true);

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

	// let's create a naive profile and save and kill it and see if it persists
	{
		Profile myFirst("First");
		myFirst.setAPN("FirstOne");
		myFirst.setUsername("Klaas");
		myFirst.setPassword("Secret");
		myFirst.save();
		check(Profile::getProfileList().contains("First"), "Profile First exists\n", true);
	}
	{
		Profile myFirst("First");
		Profile mySecond("Second");
		check(myFirst.getAPN() == "FirstOne", "APN was persistent - good\n", true);
		check(myFirst.getUsername() == "Klaas", "Username was persistent - good\n", true);
		check(myFirst.getPassword() == "Secret", "Password was persistent - good\n", true);
		check(mySecond.getAPN() == "", "APN did not smear out to other profiles - good\n", true);
		check(Profile::getProfileList().contains("First"), "Profile First exists\n", true);
		check(Profile::getProfileList().contains("Second")==0, "Profile Second does not exist - good\n", true);
	}		

	// let's test the active profile thingies
	check(Profile::setActiveProfile("Second") == false, "Cannot set Second active as it doesn't exist\n");
	check(Profile::setActiveProfile("First"), "set First active succeeds\n");
	check(Profile::getActiveProfileName() == "First", "First is now the active profile\n");
	check(Profile::setActiveProfile("Wrong")==false, "setting Wrong profile fails - good\n");
	check(Profile::getActiveProfileName() == "First", "First is still the active profile\n");
	{
		Profile mySecond("Second");
		mySecond.setAPN("SecondTwo");
		mySecond.setIsDefaultRoute(false); // set it to opposite of default
		mySecond.setNoIpDefault(true); // set it to opposite of default
		mySecond.setReplaceDefaultRoute(false); // set it to opposite of default
		mySecond.save();
	}
	check(Profile::getActiveProfileName() == "First", "First is again the active profile\n");
	check(Profile::setActiveProfile("Second"), "set Second active succeeds\n");
	check(Profile::getActiveProfileName() == "Second", "Second is now the active profile\n");
	check(Profile::setActiveProfile("Wrong")==false, "setting Wrong profile fails - good\n");
	check(Profile::getActiveProfileName() == "Second", "Second is still the active profile\n");
	{
		// method one to create a Default profile
		Profile myActive;
		check(myActive.getProfileName() == "Second", "An unspecified Profile will be of Second type\n");
		check(myActive.getIsDefaultRoute() == false, "IsDefaultRoute is false - good\n");
		check(myActive.getNoIpDefault() == true, "NoIpDefault is true - good\n");
		check(myActive.getReplaceDefaultRoute() == false, "ReplaceDefaultRoute is false - good\n");
	}
	{
		// method two to create a Default profile
		Profile myActive2("");
		check(myActive2.getProfileName() == "Second", "An unnamed Profile will be of Second type\n");
	}

	// Second is now active. If we remove it, First should be made active automatically
	{
		Profile mySecond("Second");
		check(mySecond.remove(), "Removal was OK\n");
	}
	check(Profile::getProfileList().contains("Second")==0, "Profile Second does not exist anymore - good\n", true);
	check(Profile::getActiveProfileName() == "First", "First is again the active profile\n", true);
	{
		// let's use method one to create a Default profile again
		Profile myActive;
		check(myActive.getProfileName() == "First", "An unspecified Profile will be of First type now\n");
		check(myActive.getIsDefaultRoute() == true, "IsDefaultRoute is true - good\n");
		check(myActive.getNoIpDefault() == false, "NoIpDefault is false - good\n");
		check(myActive.getReplaceDefaultRoute() == true, "ReplaceDefaultRoute is true - good\n");
	}
    
    
    
    
	// keep this last for summary:
	finish();
	return 0;
}

