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


#include "umtsmon_global.h"
#include "TheSettingsSingleton.h"

#include <assert.h>
#include <qdir.h>

// for atexit(3)
#include <cstdlib>

// the various strings related to general settings
// I'd prefer to keep them together - here
const char* theLanguageSetting               = "/General/Language";
const char* theGeneralAutoConnectSettingName = "/General/Autoconnect";
const char* theWasRoamingSettingName         = "/General/WasRoaming";


static TheSettingsSingleton* theSingletonInstancePtr = NULL;
static QSettings* theQSettingsInstancePtr = NULL;


TheSettingsSingleton::TheSettingsSingleton()
{
	DEBUG5("TheSettingsSingleton::TheSettingsSingleton()\n");
	assert(theQSettingsInstancePtr == NULL);
	theQSettingsInstancePtr = new QSettings();

	// I want to store the settings on the usual place: $HOME/.umtsmon/umtsmonrc
	QString mySettingsLocation = QDir::homeDirPath() + "/." + APPNAME;
	DEBUG2("Settings will be saved in : '%s'\n", mySettingsLocation.ascii());
	theQSettingsInstancePtr->insertSearchPath (QSettings::Unix, mySettingsLocation);
	theQSettingsInstancePtr->beginGroup(APPNAME);

	// register the end member to destruct properly at 
	// the end of normal application operation
	atexit(TheSettingsSingleton::end);
}


TheSettingsSingleton::~TheSettingsSingleton(void)
{
	DEBUG5("DESTRUCTOR TheSettingsSingleton::~TheSettingsSingleton()\n");
	if (theQSettingsInstancePtr)
	{
		delete theQSettingsInstancePtr;
		theQSettingsInstancePtr = NULL;
	}
}


void TheSettingsSingleton::end(void)
{
	delete theSingletonInstancePtr;
	theSingletonInstancePtr = NULL;
}


// dear programmers, keep in mind that this member is static,
// i.e. you cannot use any of the other members in the class, 
// nor any of the variables 
QSettings& TheSettingsSingleton::getQSRef(void)
{
	// do we exist? if not: create us
	if (theSingletonInstancePtr == NULL)
		theSingletonInstancePtr = new TheSettingsSingleton();
	
	// once that is done, theQSettingsInstancePtr should have a legal pointer
	assert (theQSettingsInstancePtr != NULL);
	return *theQSettingsInstancePtr;
}

void TheSettingsSingleton::makeChangesPersistent(void)
{
	if (theSingletonInstancePtr != NULL)
		theSingletonInstancePtr->end();
}



bool TheSettingsSingleton::removeSubTree(const QString& aSubTreeName)
{
	DEBUG3("deleteSubTree '%s' start\n", aSubTreeName.ascii());
	QStringList myList = getQSRef().subkeyList(aSubTreeName);
	QStringList::Iterator it = myList.begin();
	while( it != myList.end() ) 
	{
		// recursively scan through all "subdirectories" first
		if (removeSubTree (aSubTreeName + "/" + (*it)) == false)
		{
			DEBUG1("deleteSubTree returned false\n");
			return false;
		}
		++it;
	}
	// we've scanned through all "subdirectories" (and deleted their contents) 
	// now let's remove all regular entries in this directory
	myList =getQSRef().entryList(aSubTreeName);
	it = myList.begin();
	while( it != myList.end() ) 
	{
		DEBUG3("  remove Entry: %s\n", (*it).ascii());
		TheSettingsSingleton::getQSRef().removeEntry(aSubTreeName + "/" +(*it));
		++it;
	}
	// we're cool!
	DEBUG3("deleteSubTree '%s' end\n", aSubTreeName.ascii());
	return true;    
}


	
