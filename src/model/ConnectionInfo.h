/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2006,2007,2008,2009 Klaas van Gend
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

#ifndef CONNECTIONINFO_CLASS
#define CONNECTIONINFO_CLASS

#include "umtsmon_global.h"
#include "MonthlyTraffic.h"

#include <qobject.h>
#include <qstring.h>

// forward declaration: real declaration after ConnectionInfo class
class ConnectionInfoInhibitor;

// forward declaration: real include is in CPP file 
class Operator;

/** The ConnectionInfo class creates and keeps metrics on the radio
 *  connection.
 */
class ConnectionInfo
{
public:
	ConnectionInfo() :  
		theRegistrationValue(REGISTRATION_UNKNOWN),
		theSignalQuality(0),
		theOperatorName(""),
		isProcNetDevStatisticPresent(false),
		theCounterBytesSent(0),
		theCounterBytesReceived(0),
		theOperatorPtr(NULL),
		thePreviousNetworkID(0)
			{};
	virtual ~ConnectionInfo();

	/// refreshes the connection information
	void refresh(void);

	/** returns the signal quality.
	 *  the value should (according to 3GPP TS 27.007 V7.0.0 (2006-03) - chapter 8.5)
	 *  be interpreted as follows:
	 *  0	 113 dBm or less
	 *  1	 111 dBm
	 *  2...30	 109...  53 dBm
	 *  31	 51 dBm or greater
	 *  99	not known or not detectable
	 */
	unsigned int getSignalQuality(void) const 
		{ return theSignalQuality; };
	
	/** returns the signal quality as a string - a number or "n/a" if not available
	 *  See getSignalQuality() for more details
	 */
	QString getSignalQualityString(void) const
		{ return (theSignalQuality==99)?QObject::tr("n/a"):QString::number(theSignalQuality);};

	const QString& getOperatorName(void) const 
		{ return theOperatorName; };

	/** static member: returns a string defining the Radiotype
	 *  follows 3GPP TS 27.007 V7.0.0 (2006-03) - chapter 7.3
	 */
	static QString getRadiotypeString(int aRadioType);

	/** this member returns the radio type, but also takes registration into account
	 *  i.e. if disconnected but searching, will return 'Searching...'.
	 */
	QString getRadiotype(void) const;

	/** info on whether the device is registered in *an* allowed operator network
	 *  (either roaming or home)
	 */
	bool isDeviceRegistered(void) const
		{ return (theRegistrationValue==REGISTERED_HOME || theRegistrationValue==REGISTERED_ROAMING); };

	/// @returns true if we are currently connected to a roaming network
	bool isRoaming(void) const
		{ return (theRegistrationValue==REGISTERED_ROAMING); };
	
	/// @returns true if the card is currently searching for a network
	bool isSearching(void) const
		{ return (theRegistrationValue==TRYING_TO_REGISTER); };

	/// @returns the number of bytes sent in the current ppp session
	unsigned long getCounterBytesSent(void) const
		{ return theCounterBytesSent; };
	/// @returns the number of bytes received in the current ppp session
	unsigned long getCounterBytesReceived(void) const
		{ return theCounterBytesReceived; };

	/// @returns the delta between current and last upload observation in bytes
	unsigned long getUploadDelta(void) const
		{ return theUpDelta; };
	/// @returns the delta between current and last download observation in bytes
	unsigned long getDownloadDelta(void) const
		{ return theDownDelta; };

	
	// the following 5 members just wrap MonthlyTraffic
	
	/// @returns the number of seconds connected
	unsigned long getSecondsConnected(void) const
		{ return theMonthlyTraffic.getSecondsConnected(); };

	/** @returns the status of the current amount of time connected 
	 *  below, inbetween or over the limits
	 */
	MonthlyTraffic::ThresholdStatus getTimeThresholdStatus(void) const
		{ return theMonthlyTraffic.getTimeThresholdStatus(); }; 

	/// @returns the total number of seconds connected this month for this roaming and profile
	unsigned long getTotalSecondsConnected(void) const
		{ return theMonthlyTraffic.getTotalSecondsConnected(); };

	/// @returns the total amount of traffic for this month for the current network type
	unsigned long getTotalTraffic(void) const
		{ return theMonthlyTraffic.getTotalTraffic(); }; 

	/** @returns the status of the current amount of traffic 
	 *  below, inbetween or over the limits
	 */
	MonthlyTraffic::ThresholdStatus getTrafficThresholdStatus(void) const
		{ return theMonthlyTraffic.getTrafficThresholdStatus(); }; 
		

	
protected:
	/// asks the signal quality from the device
	virtual void refreshSignalQuality(void);

	/// retrieves the Regstration state from the device
	virtual void refreshRegistration(void);
	
	/** this function checks if ppp0 exists and 
	 *  refreshes the bytes read / bytes sent  statics
	 */
	virtual void refreshTrafficStats(void);

	/** returns true if ConnectionInfo should be prevented from
	 *  from sending AT commands
	 */
	bool isInhibited(void) const;

	/** the following are the return values of AT+CGREG
	 *  3GPP TS 27.007 V7.0.0 (2006-03) - chapter 10.1.19
	 */
	enum Registration
	{
	NOT_REGISTERED=0,       // not registered, MT is not currently searching an operator to register to
	REGISTERED_HOME=1,      // registered, home network
	TRYING_TO_REGISTER=2,   // not registered, but MT is currently trying to 
	                        // attach or searching an operator to register to
	                        // (also used by ConnectionInfo_ACM as internal state
	                        //  for "no CSQ yet")
	REGISTRATION_DENIED=3,  // registration denied
	REGISTRATION_UNKNOWN=4, // unknown
	REGISTERED_ROAMING=5    // registered, roaming
	};

	Registration theRegistrationValue;
	unsigned int theSignalQuality;
	int theRadioType;
	QString theOperatorName;

	/// true if we have a ppp0 entry in /proc/net/dev
	bool isProcNetDevStatisticPresent;
	/// the bytes sent in the current session
	unsigned long theCounterBytesSent;
	/// the bytes received in the current session
	unsigned long theCounterBytesReceived;

	/// the delta between current and last upload observation in bytes
	unsigned long theUpDelta;
	/// the delta between current and last download observation in bytes
	unsigned long theDownDelta;
	
	MonthlyTraffic theMonthlyTraffic;
	
	/** pointer to operator - used for resolving number-to-name issues
	 *  in case AT+COPS only returns the network ID, not the operator name
	 */
	const Operator* theOperatorPtr;
	unsigned long thePreviousNetworkID;

	
	/** if AT+COPS only returns a number, this member can attempt to resolve 
	 *  the number to a name, using the [KnownOperators] section in the config file
	 *  @param aNetworkID   network ID to look up
	 *  @returns            QString containing network name or "star + NetworkID" 
	 *                      if not found
	 */
	const QString& resolveNetworkIDtoName(const QString& aNetworkID);
	
private:
	void refreshOperator(void);
	Registration refreshCGREG(void);
	Registration refreshCREG(void);

	/** this variable acts like a counting semaphore,
	 *  increased and decreased by friend class ConnectionInfoInhibitor
	 */
	static int theInhibitionCount;
	
	/** the above variable 'theInhibitionCount' must be accessible by 
	 *  ConnectionInfoInhibitor. But we don't want to expose it to anyone
	 *  else. The solution is to declare the Inhibitor as a friend class
	 */
	friend class ConnectionInfoInhibitor;
};

//###########################################################################
/** specialised ConnectionInfo class for the HSO type of devices 
 */
class ConnectionInfo_HSO : public ConnectionInfo
{
public:
	ConnectionInfo_HSO() 
		: ConnectionInfo()
		{DEBUG5("ConnectionInfo_HSO::ConnectionInfo_HSO()")};
	virtual ~ConnectionInfo_HSO() {};

	/// overridden from base class - statistics for HSO device, no ppp
	virtual void refreshTrafficStats(void);
private:
	unsigned long theBaseSent;
	unsigned long theBaseReceived;
};


//###########################################################################
/** specialised ConnectionInfo class to work around AT limitations in most
 *  USB Modem implementations (mostly USB connected mobile phones)
 *  
 *  The limitations are:
 *    - no CGREG or CREG 
 *    - or issueing it will cause the phone to start reconnecting :-(
 *    - FIXME: wasn't there a AT+COPS limitation??? 
 */
class ConnectionInfo_ACM : public ConnectionInfo
{
public:
	ConnectionInfo_ACM() : ConnectionInfo(),isCGREGUsed(false) {};
	virtual ~ConnectionInfo_ACM() {};
	virtual void refreshRegistration();
	
	void setUseOfCGREG(bool aBool)
		{ isCGREGUsed = aBool; };
		
private:
	bool isCGREGUsed;
};

/** specialised ConnectionInfo class for ericcson F3507g,
    which uses AT+CIND for getting the signal quality
 */
class ConnectionInfo_ACM_Ericsson : public ConnectionInfo
{
public:
	ConnectionInfo_ACM_Ericsson() : ConnectionInfo() {};
	virtual ~ConnectionInfo_ACM_Ericsson() {};
protected:		
	void refreshSignalQuality(void);
};


//###########################################################################
/** This is a helper class to ConnectionInfo. 
 * Creating an instance of this class will prevent ConnectionInfo from
 * actually sending out AT commands to retrieve connection metrics.
 * just make this class a local variable - if it goes out of scope, 
 * the inhibition will automatically end.
 */
class ConnectionInfoInhibitor
{
public:
	ConnectionInfoInhibitor() {ConnectionInfo::theInhibitionCount++; DEBUG4("ConnectionInfo:InhibitionCount ++, now is %d\n",ConnectionInfo::theInhibitionCount);  };
	~ConnectionInfoInhibitor() {ConnectionInfo::theInhibitionCount--; DEBUG4("ConnectionInfo:InhibitionCount --, now is %d\n",ConnectionInfo::theInhibitionCount); };
};


#endif // defined CONNECTIONINFO_CLASS
