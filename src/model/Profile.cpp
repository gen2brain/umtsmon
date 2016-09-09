/* umtsmon - a program to control/monitor your UMTS card
 * Copyright (C) 2006,2007,2008  Christofer Wesseling, Klaas van Gend
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
#include <qregexp.h>

// to prevent keys from varying between setter and getter
// the below static const strings are defined.
static const char* theProfilesStr = "/ConnectionProfiles";
static const char* theActiveProfileStr = "ActiveProfile";
static const char* theDefaultNameStr = "Default";
static const char* theDefaultRouteStr = "DefaultRoute";
static const char* theApnStr = "apn";
static const char* theUsernameStr = "username";
static const char* thePasswordStr = "password";
static const char* theNoIpDefaultStr = "noIpDefault";
static const char* theReplaceDefaultRouteStr = "replaceDefaultRoute";
static const char* theUsePeerDnsStr = "usePeerDns";
static const char* theNoCompressionStr = "noCompression";
static const char* theNoCCPStr = "noCCP";
static const char* theNoAuthStr = "noAuth";
static const char* theWarnOnDataThresholdStr = "doWarnOnThreshold";
static const char* theThresholdPercentageStr = "thresholdPercentage";
static const char* theRoamingDataLimitStr = "roamingDataLimit";
static const char* theHomeDataLimitStr = "homeDataLimit";
static const char* theRoamingTimeLimitStr = "roamingTimeLimit";
static const char* theHomeTimeLimitStr = "homeTimeLimit";
static const char* theCIDPrefixStr = "CIDprefix";
static const char* theCIDStr = "CID";
static const char* theWasConnectionSuccess = "WasConnectionSuccess";
static const char* theCallName = "call";



const QString Profile::getKeyPathName(const QString aKey) const
{
	assert(Profile::checkForValidProfileName(aKey));
	QString myProfileName;
	if (Profile::checkForValidProfileName(theProfileName))
		myProfileName = theProfileName;
	else
		myProfileName = theDefaultNameStr;
	return theProfilesStr + QString("/") + myProfileName + "/prof/" + aKey; 
}	



Profile::Profile(QString aProfileName)
{
	DEBUG2("Profile::Profile('%s')\n", aProfileName.ascii());
	
	// if no profile name specified, let's load one from the settings
	// if no active profile mentioned in settings, start a profile
	// with the default name (i.e. 'Default'). That profile probably
	// only contains the default settings :-) 
	if (aProfileName.isEmpty())
	{
		aProfileName = getActiveProfileName();
	}
	assert(checkForValidProfileName(aProfileName));

	theProfileName = aProfileName;
	DEBUG2("  finally created profile for '%s'\n", theProfileName.ascii());
	assert(!theProfileName.isEmpty());
}


////////////////////////////////////////////////////////////
// GETTERS: return the .... setting of the current profile

 
QString
Profile::getAPN(void) const
{	
	return TheSettingsSingleton::getQSRef().readEntry(getKeyPathName(theApnStr), "");
}

QString
Profile::getUsername(void) const
{
	return TheSettingsSingleton::getQSRef().readEntry(getKeyPathName(theUsernameStr), "");
}

QString 
Profile::getPassword(void) const
{
	return TheSettingsSingleton::getQSRef().readEntry(getKeyPathName(thePasswordStr), "");
}

bool
Profile::getNoIpDefault(void) const
{
	return TheSettingsSingleton::getQSRef().readBoolEntry(getKeyPathName(theNoIpDefaultStr), false);
}

bool 
Profile::getIsDefaultRoute(void) const
{
	return TheSettingsSingleton::getQSRef().readBoolEntry(getKeyPathName(theDefaultRouteStr), true);
}

bool 
Profile::getReplaceDefaultRoute(void) const
{
	return TheSettingsSingleton::getQSRef().readBoolEntry(getKeyPathName(theReplaceDefaultRouteStr), false);
}

bool 
Profile::getUsePeerDns(void) const
{
	return TheSettingsSingleton::getQSRef().readBoolEntry(getKeyPathName(theUsePeerDnsStr), true);
}

bool 
Profile::getNoCompression(void) const
{
	return TheSettingsSingleton::getQSRef().readBoolEntry(getKeyPathName(theNoCompressionStr), true);
}

bool 
Profile::getNoCCP(void) const
{
	return TheSettingsSingleton::getQSRef().readBoolEntry(getKeyPathName(theNoCCPStr), false);
}

bool 
Profile::getNoAuth(void) const
{
	return TheSettingsSingleton::getQSRef().readBoolEntry(getKeyPathName(theNoAuthStr), false);
}

bool 
Profile::getWarnOnDataThreshold(void) const
{
	return TheSettingsSingleton::getQSRef().readBoolEntry(getKeyPathName(theWarnOnDataThresholdStr), false);
}

unsigned int 
Profile::getThresholdPercentage(void) const
{
	return TheSettingsSingleton::getQSRef().readNumEntry(getKeyPathName(theThresholdPercentageStr), false);
}

unsigned long 
Profile::getHomeDataLimit(void) const
{
	return TheSettingsSingleton::getQSRef().readNumEntry(getKeyPathName(theHomeDataLimitStr), 0);
}
	
unsigned long 
Profile::getRoamingDataLimit(void) const
{
	return TheSettingsSingleton::getQSRef().readNumEntry(getKeyPathName(theRoamingDataLimitStr), 0);
}

unsigned long 
Profile::getHomeTimeLimit(void) const
{
	return TheSettingsSingleton::getQSRef().readNumEntry(getKeyPathName(theHomeTimeLimitStr), 0);
}
	
unsigned long 
Profile::getRoamingTimeLimit(void) const
{
	return TheSettingsSingleton::getQSRef().readNumEntry(getKeyPathName(theRoamingTimeLimitStr), 0);
}

QString
Profile::getCIDPrefix(void) const
{
	return TheSettingsSingleton::getQSRef().readEntry(getKeyPathName(theCIDPrefixStr), "");
}

unsigned int 
Profile::getCID(void) const
{
	return TheSettingsSingleton::getQSRef().readNumEntry(getKeyPathName(theCIDStr), 1);
}

bool 
Profile::getWasConnectionSuccess(void) const
{
	return TheSettingsSingleton::getQSRef().readBoolEntry(
			getKeyPathName(theWasConnectionSuccess), false);
}

QString
Profile::getCallName(void) const
{
	return TheSettingsSingleton::getQSRef().readEntry(getKeyPathName(theCallName), "");
}

////////////////////////////////////////////////////////////
// SETTERS: change the .... setting of the current profile

bool 
Profile::setAPN(QString anApn)
{
	assert(!theProfileName.isEmpty());
	return TheSettingsSingleton::getQSRef().writeEntry(getKeyPathName(theApnStr), anApn );
}

bool 
Profile::setUsername(QString anUsername)
{
	assert(!theProfileName.isEmpty());
	return TheSettingsSingleton::getQSRef().writeEntry(getKeyPathName(theUsernameStr), anUsername );	
}

bool Profile::setPassword(QString aPassword)
{
	assert(!theProfileName.isEmpty());
	return TheSettingsSingleton::getQSRef().writeEntry(getKeyPathName(thePasswordStr), aPassword );
}

bool 
Profile::setNoIpDefault(bool aNoIpDefault)
{
	assert(!theProfileName.isEmpty());
	return TheSettingsSingleton::getQSRef().writeEntry(getKeyPathName(theNoIpDefaultStr), aNoIpDefault);
}

bool 
Profile::setIsDefaultRoute(bool aDefaultRoute)
{
	assert(!theProfileName.isEmpty());
	return TheSettingsSingleton::getQSRef().writeEntry(getKeyPathName(theDefaultRouteStr), aDefaultRoute );
}

bool 
Profile::setReplaceDefaultRoute(bool aReplaceDefaultRoute)
{
	assert(!theProfileName.isEmpty());
	return TheSettingsSingleton::getQSRef().writeEntry(getKeyPathName(theReplaceDefaultRouteStr), aReplaceDefaultRoute );
}

bool 
Profile::setUsePeerDns(bool anUsePeerDns)
{
	assert(!theProfileName.isEmpty());
	return TheSettingsSingleton::getQSRef().writeEntry(getKeyPathName(theUsePeerDnsStr), anUsePeerDns );
}

bool 
Profile::setNoCompression(bool aNoCompression)
{
	assert(!theProfileName.isEmpty());
	return TheSettingsSingleton::getQSRef().writeEntry(getKeyPathName(theNoCompressionStr), aNoCompression );
}

bool 
Profile::setNoCCP(bool aNoCCP)
{
	assert(!theProfileName.isEmpty());
	return TheSettingsSingleton::getQSRef().writeEntry(getKeyPathName(theNoCCPStr), aNoCCP );
}

bool 
Profile::setNoAuth(bool aNoAuth)
{
	assert(!theProfileName.isEmpty());
	return TheSettingsSingleton::getQSRef().writeEntry(getKeyPathName(theNoAuthStr), aNoAuth );
}

bool 
Profile::setWarnOnDataThreshold(bool aWarning)
{
	assert(!theProfileName.isEmpty());
	return TheSettingsSingleton::getQSRef().writeEntry(getKeyPathName(theWarnOnDataThresholdStr), aWarning );
}

bool 
Profile::setThresholdPercentage(unsigned int aPercentage)
{
	assert(!theProfileName.isEmpty());
	return TheSettingsSingleton::getQSRef().writeEntry(getKeyPathName(theThresholdPercentageStr), static_cast<int>(aPercentage) );
}

bool 
Profile::setHomeDataLimit(unsigned long aLimit)
{
	assert(!theProfileName.isEmpty());
	return TheSettingsSingleton::getQSRef().writeEntry(getKeyPathName(theHomeDataLimitStr), static_cast<int>(aLimit) );
}

bool 
Profile::setRoamingDataLimit(unsigned long aLimit)
{
	assert(!theProfileName.isEmpty());
	return TheSettingsSingleton::getQSRef().writeEntry(getKeyPathName(theRoamingDataLimitStr), static_cast<int>(aLimit) );
}

bool 
Profile::setHomeTimeLimit(unsigned long aLimit)
{
	assert(!theProfileName.isEmpty());
	return TheSettingsSingleton::getQSRef().writeEntry(getKeyPathName(theHomeTimeLimitStr), static_cast<int>(aLimit) );
}

bool 
Profile::setRoamingTimeLimit(unsigned long aLimit)
{
	assert(!theProfileName.isEmpty());
	return TheSettingsSingleton::getQSRef().writeEntry(getKeyPathName(theRoamingTimeLimitStr), static_cast<int>(aLimit) );
}

bool 
Profile::setWasConnectionSuccess(bool aWasConnectionSuccess)
{
	assert(!theProfileName.isEmpty());
	return TheSettingsSingleton::getQSRef().writeEntry(getKeyPathName(
			theWasConnectionSuccess), aWasConnectionSuccess );
}

////////////////////////////////////////////////////////////
// save the profile to the HDD - New APN Name = New Profile
void 
Profile::save(void) const
{
	TheSettingsSingleton::makeChangesPersistent();
}

////////////////////////////////////////////////////////////
bool 
Profile::remove(void) const
{
	DEBUG1("Request to remove profile '%s'\n", theProfileName.ascii());
	assert(!theProfileName.isEmpty());
	
	// if this was the active profile, we need to set another one current
	if (Profile::getActiveProfileName() == theProfileName)
		Profile::setActiveProfile(Profile::getProfileList().first());
	
	return TheSettingsSingleton::removeSubTree(theProfilesStr+QString("/")+ theProfileName);
}

////////////////////////////////////////////////////////////
bool 
Profile::setActiveProfile(const QString& aProfileName)
{
	if (Profile::getProfileList().contains(aProfileName) > 0)
		return TheSettingsSingleton::getQSRef().writeEntry(theProfilesStr+QString("/")+theActiveProfileStr, aProfileName);
	return false;
}

QStringList
Profile::getProfileList(void) 
{ 
	return TheSettingsSingleton::getQSRef().subkeyList(theProfilesStr); 
}

QString 
Profile::getActiveProfileName(void)
{
	QString myName = TheSettingsSingleton::getQSRef().readEntry(
			theProfilesStr+QString("/")+theActiveProfileStr, "");
	if (myName == "")
	{
		// nothing found?
		// do we actually have one or more entries in the list?
		if (getProfileList().count() > 0)
		{
			myName = getProfileList().first();
			DEBUG1("No active profile found, selecting the first profile: '%s'\n", myName.ascii()); 
			setActiveProfile(myName);
		}
	}
	if (checkForValidProfileName(myName))
		return myName;
	else
		return theDefaultNameStr;
}

bool 
Profile::checkForValidProfileName(const QString& aName)
{
	// empty is not allowed
	if (aName.isEmpty())
		return false;
	// no whitespace? 
	QRegExp myRegExp("^\\w+$");
	if (myRegExp.search(aName) < 0)
		return false;
	// dashes are not allowed
	if (aName.contains("-"))
		return false;
	
	return true;
}
