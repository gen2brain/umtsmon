/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2006,2007,2008  Christofer Wesseling, Klaas van Gend
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

#ifndef PROFILE_H_
#define PROFILE_H_

#include "TheSettingsSingleton.h" 

#include <qstring.h>
#include <qstringlist.h>

/** The Profile class manages a user profile
 *  * any Profile has a name
 *  * only one Profile at a time can be the "default profile"
 *  Note that the Profile class doesn't store settings itself, it immediately
 *  sends them through to TheSettingsSingleton.
 */
class Profile
{

public:

	/** constructor
	 *  @param aProfileName   the profile to load settings for. If left empty, load the current active profile
	 */
	Profile(QString aProfileName = "");
	
	/// destructor
	virtual ~Profile(void) {};

	//////////////////////////////////////////////// 
 	// a list of getters & setters
 
	QString getAPN(void) const;
	bool setAPN(QString anAPN);
	
	QString getUsername(void) const;
	bool setUsername(QString anUsername);

	QString getPassword(void) const;
	bool setPassword(QString aPassword);

	bool getNoIpDefault(void) const;
	bool setNoIpDefault(bool aNoIpDefault);

	// true: System uses the connection of this profile as default route
	bool getIsDefaultRoute(void) const;
	bool setIsDefaultRoute(bool aDefaultroute);
	
	// true: if there is an existins default route, replace it!
	bool getReplaceDefaultRoute(void) const;
	bool setReplaceDefaultRoute(bool aReplaceDefaultRoute);

	// true: use peer dns...
	// if true, PPP will ask for ###
	bool getUsePeerDns(void) const;
	bool setUsePeerDns(bool anUserPeerDns);
	
	// true: doesn't apply VJ compression to the packets of the ppp-connection
	bool getNoCompression(void) const;
	bool setNoCompression(bool aNoCompression);
	
	/** config for "no CCP" (Control Compression Protocol)
	 *  note that this should only be enabled if the peer (i.e. the operator)
	 *  has buggy systems
	 */
	bool getNoCCP(void) const;
	bool setNoCCP(bool aNoCCP);

	/// true: enable the 'noauth' option
	bool getNoAuth(void) const;
	bool setNoAuth(bool aNoAuth);

	/// true: warn if a monthly data limit threshold is reached
	bool getWarnOnDataThreshold(void) const;
	bool setWarnOnDataThreshold(bool aWarning);
	
	/// the percentage above which to warn if traffic exceeds
	unsigned int getThresholdPercentage(void) const;
	bool setThresholdPercentage(unsigned int aPercentage);

	/** the data limit in MiB (i.e. 2^20 bytes) for home networks
	 *  use 0 for "unlimited"
	 */
	unsigned long getHomeDataLimit(void) const;
	bool setHomeDataLimit(unsigned long aLimit);
	
	/** the data limit in MiB (i.e. 2^20 bytes) for roaming
	 *  use 0 for "unlimited"
	 */
	unsigned long getRoamingDataLimit(void) const;
	bool setRoamingDataLimit(unsigned long aLimit);

	/** the time limit in seconds for home networks
	 *  use 0 for "unlimited"
	 */
	unsigned long getHomeTimeLimit(void) const;
	bool setHomeTimeLimit(unsigned long aLimit);
	
	/** the time limit in seconds for roaming
	 *  use 0 for "unlimited"
	 */
	unsigned long getRoamingTimeLimit(void) const;
	bool setRoamingTimeLimit(unsigned long aLimit);
	
	/** returns the CID number - if modified
	 *  this option is not in the Profile GUI
	 *  
	 *  (see man/advanced_umtsmon_tricks.txt)
	 *
	 *  @returns CID number, default 1
	 */
	unsigned int getCID(void) const;
	// bool setCID(unsigned int aCID);
	
	/** if you want to use a custom CID Prefix, set it here
	 *  not setting it, will imply a default CID prefix of *99***
	 *  
	 *  (see man/advanced_umtsmon_tricks.txt)
	 *
	 *  @return  CID Prefix - or empty string if none set (which means use default)
	 */
	QString getCIDPrefix(void) const;
	//bool setCIDPrefix(const QString& aCIDPrefix);

	// true: this profile has seen successful connections
	bool getWasConnectionSuccess(void) const;
	bool setWasConnectionSuccess(bool aWasConnectionSuccess);
	
	/** in order to allow priviledged options to PPP or just 
	 *  other options that we don't provide in the UI, use "call namme"
	 *  and create the /etc/ppp/peers/name file with the right parameters
	 *  
	 *  (see man/advanced_umtsmon_tricks.txt)
	 *
	 * @return the name to call - if any. The string will be empty if there is none
	 */
	QString getCallName(void) const;
	//bool   setCallName(const QString& aCallName);
	
	
    /* ***************************************************************** */

	/** forces saving of this Profile (and probably all other settings as well)
	 *  note that saving the profile doesn't make it active - you need to do that separately
	 */
	void save(void) const;
	
	/** removes the profile from the settings and resets the Active profile if needed 
	 *  FIXME: make this one static like the below members
	 *  (note that this Profile *instance* remains)
	 */
	bool remove(void) const;
	
	/** get the name of the profile currently marked as active, 
	 *  @returns an empty string if none
	 */
	static QString getActiveProfileName(void);

	/** set a certain profile to be the active one
	 *  @param aProfileName  the profile to make active. if name doesn't exists, return false
	 *  @returns true if profile name was set to active
	 */ 
	static bool setActiveProfile(const QString& aProfileName);
	
	/// Returns a list of all known Profiles
	static QStringList getProfileList(void); 

	/// returns true if a name is valid - or false if not
	static bool checkForValidProfileName(const QString& aName);

	/// returns the current profile name of this Profile instance
	const QString& getProfileName(void) const
		{ return theProfileName; };

private:
	QString theProfileName;
	
	/// returns the full path for keys of the current Profile
	const QString getKeyPathName(const QString aKey) const;


};

#endif /*PROFILE_H_*/
