/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2008  Klaas van Gend
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

#ifndef MONTHLYTRAFFIC_H_
#define MONTHLYTRAFFIC_H_

#include <qstring.h>
#include <qdatetime.h>

/** thrown by MonthlyTraffic::update in case the date rolls over into a new month
 *  e.g.  user starts connection at Jan 31, 2008 at 23:50.
 *  this should be catched by anyone calling update() and able to stop pppd
 */ 
class MonthRolloverException
{
};

/** thrown by MonthlyTraffic::warnIfNeeded and/or MonthlyTraffic::update if
 *  the data or time limit is reached
 */ 
class OverThresholdException
{
public:
	enum ThresholdType { TIME, TRAFFIC };
	OverThresholdException( ThresholdType aReachedThresh) :
			theReachedThreshold(aReachedThresh) { };
	ThresholdType getThreshReached(void) { return theReachedThreshold; };
private:
	OverThresholdException(void);
	ThresholdType theReachedThreshold;
};

/** this class keeps the traffic (data/time) limits for a certain month
 */
class MonthlyTraffic
{
public:
	/** constructor, will load relevant settings into this class, based on current profile
	 *  and current date (if no specific date set)
	 *  @param aYear      specify nonzero (e.g. 2008) if not current year
	 *  @param aMonth     specify nonzero (e.g. 1 for Jan) if not current month
	 */
	MonthlyTraffic(unsigned int aYear=0, unsigned int aMonth=0);
	
	~MonthlyTraffic();

	/** updates the transmit/receive/total statistics of this Month 
	 *  FIXME: what to do if switching from roaming to home and vice-versa???
	 *  @param aCounterBytesSent      total amount of bytes as reported by ppp, 
	 *                                should be monotonously increasing
	 *  @param aCounterBytesReceived  total amount of bytes as reported by ppp
	 *                                should be monotonously increasing
	 *  @throws MonthRolloverException if the date rolls over to a new month 
	 *  @throws OverThresholdException if the data/time limit is reached 
	 *  @returns the total amount of traffic for this month for the current network type
	 */
	unsigned long update(unsigned long aCounterBytesSent, unsigned long aCounterBytesReceived);

	/// @returns the number of seconds connected
	unsigned long getSecondsConnected(void) const;
	
	/// @returns the total number of seconds connected this month for this roaming and profile
	unsigned long getTotalSecondsConnected(void) const;
	
	enum ThresholdStatus
	{
		BELOW,			// total for month/network/profile is below threshold percentage
		BETWEEN,		// traffic inbetween threshold percentage and 100% of limit
		OVER_LIMIT,		// traffic exceeded 100% of data limit
		LIMITS_DISABLED // user didn't specify any limits
	};

	/** @returns the status of the current amount of time 
	 *  below, inbetween or over the limits
	 */
	ThresholdStatus getTimeThresholdStatus(void) const;

	/** @returns the status of the current amount of traffic 
	 *  below, inbetween or over the limits
	 */
	ThresholdStatus getTrafficThresholdStatus(void) const;
	
	/// @returns the total amount of traffic for this month for the current network type
	unsigned long getTotalTraffic(void) const;

	/** resets everything (again) and starts counting the traffic
	 *    (everything is: roaming state, profile data, counters)
	 *  @param anIsRoaming true if currently roaming
	 */
	void start(bool anIsRoaming);
	
private:
	/// the date for which the instance is initialised
	QDate theDate;

	/// start date/time of the connection
	QDateTime theConnectionStartTime;
	
	/// true if roaming, false if home network
	bool isRoaming;

	/// the number of bytes sent in the current PPP Session
	unsigned long theCounterBytesSent;
	/// the number of bytes received in the current PPP Session
	unsigned long theCounterBytesReceived;
	
	/// the total amount of traffic (RX+TX) *in bytes* for Home Network THIS MONTH
	unsigned long theHomeTraffic;
	/// the total amount of traffic (RX+TX) *in bytes* for Roaming THIS MONTH
	unsigned long theRoamingTraffic;	

	/// the total amount of traffic (RX+TX) *in bytes* for Roaming Network THIS MONTH
	unsigned long theRoamingSeconds;
	/// the total amount of traffic (RX+TX) *in bytes* for Home Network THIS MONTH
	unsigned long theHomeSeconds;

	// *** the following are copies from the data in Profile ***

	/** warn if a monthly data limit threshold should be reached
	 *  (upon reaching that threshold, ONE popup is emitted and then this
	 *  member is set to false to prevent duplicate warnings)
	 */
	bool isWarnOnDataThreshold;
	/// the percentage above which to warn if traffic exceeds
	unsigned int theThresholdPercentage;
	/// the data limit (RX+TX) *in bytes* for Home Network
	unsigned long theHomeDataLimit;
	/// the data limit (RX+TX) *in bytes* for Roaming 
	unsigned long theRoamingDataLimit;

	/// the time limit *in seconds* for Home Network
	unsigned long theHomeTimeLimit;
	/// the time limit *in seconds* for Roaming 
	unsigned long theRoamingTimeLimit;

	/// the name of the profile we're currently using
	QString theProfileName;

	/// true if an exception has been thrown for month rollover in update()
	bool isMonthRollOverDetected;
	
private:	
	MonthlyTraffic::ThresholdStatus
	percentageToStatus(unsigned long aPercentage) const;

	/** returns the QSettings PathName+Key for a certain key
	 *  i.e. will return "Traffic/2008/01/HomeLimit"   if aKey is "HomeLimit" 
	 *  and theMonth/theYear are set to 01 and 2008 respectively
	 *  @param aKey key to create the full pathname for
	 *  @returns QString with full path name
	 */
	QString createPathName(const QString& aKey) const;

	/// writes the current statistics to disk (might be an expensive operation!)
	void save(void);
	
	/** throw a warning if needed
	 *  @throws OverThresholdException if the data limit is reached
	 */ 
	void warnIfNeeded();

	/// updates the profile name and profile-related data
	void refreshProfileData();	
	
	
	// for automated testing purposes
	friend class MonthlyTrafficTester;
};

#endif // MONTHLYTRAFFIC_H_
