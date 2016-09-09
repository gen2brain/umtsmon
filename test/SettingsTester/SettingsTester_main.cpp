/* umtsmon - a program to control/monitor your UMTS card
 * Copyright (C) 2006  Klaas van Gend
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

// use the local header files!!!
#include "umtsmon_global.h"
#include "TheSettingsSingleton.h"

#include <qstring.h>
#include <qstringlist.h>
#include <qdir.h>
#include <assert.h>
#include <errno.h>

/* This is an automated test program to test various functionality of
 * TheSettingsSingleton
 * 
 * It should be run at least once before every release - any issues should be fixed.
 *
 * NOTE: THIS CODE HAS KNOWLEDGE ABOUT THE INTERNALS OF TheSettingsSingleton
 *       DON'T COPY CODE FROM THIS TESTER INTO YOUR CODE
 *       DON'T ASSUME THAT ANY CODE IN HERE STAYS THE SAME 
 */


#include "../TestFrame.cpp"

// conveniently set theVerbosity to a number of your liking
int  theVerbosity = 5;



int main( void )
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
	
	//////////////////////////////////////////////////////////////////
	// add a settings and save settings
	chapter("Reading/Writing of settings and persistancy checks\n");
	QString myTestString = "was here";
	TheSettingsSingleton::getQSRef().writeEntry(APPNAME, myTestString);
	check (myTestString == TheSettingsSingleton::getQSRef().readEntry(APPNAME, "nope"),
		"setting is present\n");
	TheSettingsSingleton::makeChangesPersistent();
	
	// this should result in a file... let's check...
	check(mySettingsDir.exists(), "settings directory " + mySettingsLocation 
				+ " now exists (again)\n", true);
	
	// check for real persistancy - the setting should still be present
	check (myTestString == TheSettingsSingleton::getQSRef().readEntry(APPNAME, "nope"),
		"setting is apparently persistent\n", true);

	// check for changeing and persistance as well
	myTestString = "was here, too";
	TheSettingsSingleton::getQSRef().writeEntry(APPNAME, myTestString);
	TheSettingsSingleton::makeChangesPersistent();
	check (myTestString == TheSettingsSingleton::getQSRef().readEntry(APPNAME, "nope"),
		"change is apparently present\n");
		
	// check for changeing to bool
	//TheSettingsSingleton::getQSRef().writeEntry(APPNAME, true);
	//TheSettingsSingleton::makeChangesPersistent();
	//check (myTestString == TheSettingsSingleton::getQSRef().readEntry(APPNAME, false),
		//"change string setting to bool is allowed\n");
	

	//////////////////////////////////////////////////////////////////
	// tests for removeSubTree()
	chapter("reading/writing/traversing/deleting setting trees\n");
	
	QString myBase = "/profile";
	TheSettingsSingleton::getQSRef().writeEntry(myBase + "/1", "1");
	TheSettingsSingleton::getQSRef().writeEntry(myBase + "/2", "2");
	TheSettingsSingleton::getQSRef().writeEntry(myBase + "/2/a", "a");
	TheSettingsSingleton::getQSRef().writeEntry(myBase + "/2/b", "b");
	TheSettingsSingleton::getQSRef().writeEntry(myBase + "/2/z/c", "c");
	TheSettingsSingleton::getQSRef().writeEntry(myBase + "/2/z/d", "d");
	TheSettingsSingleton::getQSRef().writeEntry(myBase + "/2/y/e", "e");
	TheSettingsSingleton::getQSRef().writeEntry(myBase + "/2/x/f", "f");
	TheSettingsSingleton::getQSRef().writeEntry(myBase + "/3", "3");
	check ("3" == TheSettingsSingleton::getQSRef().readEntry(myBase + "/3", "1"),
		"tree setting present\n", true);
	
	// check if the entryList and subkeyList return expected info
    check( TheSettingsSingleton::getQSRef().entryList(myBase).count() == 3,
    		"there are 3 direct entries in "+ myBase + "\n");
    check( TheSettingsSingleton::getQSRef().subkeyList(myBase).count() == 1,
    		"there is 1 3 sub key in "+ myBase + "\n");
    check( TheSettingsSingleton::getQSRef().entryList(myBase+"/2/z").count() == 2,
    		"there are 2 direct entries in " + myBase + "/2/z\n");
    check( TheSettingsSingleton::getQSRef().subkeyList(myBase+"/2").count() == 3,
    		"there are 3 sub keys in " + myBase + "/2\n");

	// let's delete the SubTree    		
	check(TheSettingsSingleton::removeSubTree(myBase), "removeSubTree returned ok\n", true);

	// let's check the results afterwards:
    check( TheSettingsSingleton::getQSRef().entryList(myBase).count() == 0,
    		"there are 0 direct entries in "+ myBase + "\n");
    check( TheSettingsSingleton::getQSRef().subkeyList(myBase).count() == 0,
    		"there are 0 sub keys in "+ myBase + "\n");
	
	
	// keep this last for summary:
	finish();
}

