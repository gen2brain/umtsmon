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

#ifndef THESETTINGSSINGLETON_H_
#define THESETTINGSSINGLETON_H_


#include <qsettings.h>
#include <qstring.h>

// the settings keys used in the [General] Section
extern const char* theLanguageSetting;
extern const char* theGeneralAutoConnectSettingName;
extern const char* theWasRoamingSettingName;



/** the Settings Singleton class is a singleton as meant in the 
 *  Design Pattern "Singleton" from the Erich Gamma book.
 * 
 *  It thus has global access - everyone can ask for a reference to the 
 *  QSettings class and it is always guaranteed to exist.
 *  Don't cache the pointer, always use it like this: 
 *     TheSettingsSingleton::getQSRef.something() 
 *
 *  Note that this Singleton is different from the official pattern in that it
 *  does not return a reference to itself, but to the QSettings instance. 
 * 
 *  The QSettings class is configured to store/retrieve its settings from
 *  the file:  $HOME/.umtsmon/umtsmonrc
 *  So, a beginGroup() is already set - don't use
 * 
 * 
 * Klaas chose to implement this as a Singleton for several reasons:
 *   - not to have to pass pointers around
 *   - not to have to create a global pointer 
 *   - not to have to care when exactly the QSettings class is to be created
 *   - not to have to care if the QSettings class is destroyed 
 *     (as only on destruction it will save its settings)
 */
class TheSettingsSingleton
{
public:
	/** the entry to the Singleton class
	 *  return is always a valid reference to the QSettings instance
	 * 
	 *  NOTE: do *not* call other members than readEntry() or writeEntry() of
	 *  the QSettings class!!!
	 *  
	 *  As a key, you can specify e.g. "profile_athome/APN" or "/profile_athome/APN"
	 *      (which are equivalent and will be placed in the [PPP] section)
	 *  or you can specify  e.g "updateStats" or "/updateStats"
	 *      (which will be placed in the [General] section)
	 */
	static QSettings& getQSRef(void);

	static TheSettingsSingleton& me(void);

	/** removes a tree of settings.
	 *  you should use this e.g. if you want to remove a profile with settings
	 *  @param aSubTreeName  contains the name to remove, e.g. /profile_athome
	 *  @return false if tree not found
	 */
	static bool removeSubTree(const QString& aSubTreeName);
	
	/** settings are normally only saved on system exit.
	 *  Calling this member will save the settings instantly.
	 */
	static void makeChangesPersistent(void);

private:
	/** private constructor as demanded by the Singleton Design Pattern
	 *  will be called by the getQSRef() method if needed
	 */
	TheSettingsSingleton(void);
	/** private destructor as demanded by the Singleton Design Pattern
	 *  will be called by the end() method if needed
	 */
	~TheSettingsSingleton(void);	
	
	/** It is very important that this Singleton is destroyed on exit or the settings
	 *  won't get written to disk. This static member will be setup by the constructor
	 *  to be called atexit().
	 */
	static void end(void);

	/// private, unimplemented copy constructor - don't use
	TheSettingsSingleton(const TheSettingsSingleton& aPtr);
	/// private, unimplemented assign operator - don't use
	TheSettingsSingleton& operator=(TheSettingsSingleton& aPtr);
};



#endif /*THESETTINGSSINGLETON_H_*/
