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

#include "umtsmon_global.h"

#include "MonthlyTraffic.h"
#include "Profile.h"

#include <assert.h>

static const char* theMonthlyTrafficStr = "/MonthlyTraffic";
static const char* theHomeTrafficStr = "HomeTrafficInBytes";
static const char* theRoamingTrafficStr = "RoamingTrafficInBytes";
static const char* theHomeSecondsStr = "HomeConnectedSeconds";
static const char* theRoamingSecondsStr = "RoamingConnectedSeconds";

/* constructors / destructors / operators first */

MonthlyTraffic::MonthlyTraffic(
		unsigned int aYear, 
		unsigned int aMonth)
			:	isRoaming(TheSettingsSingleton::getQSRef().readBoolEntry(theWasRoamingSettingName, false)),
				theCounterBytesSent(0),
				theCounterBytesReceived(0),
				isMonthRollOverDetected(false)
{
	// find out which date to create the instance for
	if (aMonth==0 || aYear==0)
		theDate = QDate::currentDate();
	else
		theDate = QDate(aYear, aMonth, 1);

	refreshProfileData();
}

MonthlyTraffic::~MonthlyTraffic()
{
	// let's assume the worst - nothing saved yet
	save();
	TheSettingsSingleton::getQSRef().writeEntry(theWasRoamingSettingName, isRoaming);
}

/////////////////////////////////////////////////////////////////////////////
/////// implementation of the rest of the members in alphabetic order ///////
/////////////////////////////////////////////////////////////////////////////

QString 
MonthlyTraffic::createPathName(const QString& aKey) const
{
	return theMonthlyTrafficStr + QString("/") 
			+ QString::number(theDate.year()) + QString("/") 
			+ QString::number(theDate.month()) + QString("/")
			+ Profile::getActiveProfileName() + QString("/")
			+ aKey;
}


unsigned long 
MonthlyTraffic::getSecondsConnected(void) const
{
	if (theConnectionStartTime.isValid())
		return theConnectionStartTime.secsTo(QDateTime::currentDateTime());
	else
		return 0;
}


MonthlyTraffic::ThresholdStatus 
MonthlyTraffic::getTimeThresholdStatus(void) const
{
	unsigned long myPercentage = 0;
	if (isRoaming && theRoamingTimeLimit!=0)
	{
		myPercentage = theRoamingSeconds / theRoamingTimeLimit;
	}
	if (!isRoaming && theHomeTimeLimit!=0)
	{
		myPercentage = theHomeSeconds / theHomeTimeLimit;
	}
	return percentageToStatus(myPercentage);
}
	
MonthlyTraffic::ThresholdStatus 
MonthlyTraffic::getTrafficThresholdStatus(void) const
{
	// I'm not entirely sure if it's cheaper to do 64-bit integer calculations
	// or floating point calculations... I'm assuming 64-bit for now...
	unsigned long long myTraffic = theCounterBytesReceived + theCounterBytesSent;
	unsigned long long myPercentage = 0;
	if (isRoaming)
	{
		myTraffic += theRoamingTraffic;
		if (theRoamingDataLimit == 0)
			myPercentage = 0;
		else
			myPercentage = (100*myTraffic) / theRoamingDataLimit;
	}
	else
	{
		myTraffic += theHomeTraffic;
		if (theHomeDataLimit == 0)
			myPercentage = 0;
		else
			myPercentage = (100*myTraffic) / theHomeDataLimit;
	}
	return percentageToStatus(myPercentage);
}
	
MonthlyTraffic::ThresholdStatus
MonthlyTraffic::percentageToStatus(unsigned long aPercentage) const
{
	if (aPercentage == 0)
		return LIMITS_DISABLED;
	if (aPercentage < theThresholdPercentage)
		return BELOW;
	if (aPercentage >= 100)
		return OVER_LIMIT;
	return BETWEEN;
}


unsigned long 
MonthlyTraffic::getTotalSecondsConnected(void) const
{
	if (isRoaming)
		return theRoamingSeconds+getSecondsConnected();
	else	
		return theHomeSeconds+getSecondsConnected();
}


unsigned long 
MonthlyTraffic::getTotalTraffic(void) const
{
	// and return the total traffic for this month so far
	if (isRoaming)
		return theRoamingTraffic+theCounterBytesReceived+theCounterBytesSent;
	else	
		return theHomeTraffic+theCounterBytesReceived+theCounterBytesSent;
}


void 
MonthlyTraffic::refreshProfileData(void)
{	
	// and retrieve the traffic data for that specific month
	theHomeTraffic = TheSettingsSingleton::getQSRef().readEntry(
		createPathName(theHomeTrafficStr), 0).toULong();
	theRoamingTraffic = TheSettingsSingleton::getQSRef().readEntry(
		createPathName(theRoamingTrafficStr), 0).toULong();	
	theHomeSeconds = TheSettingsSingleton::getQSRef().readEntry(
		createPathName(theHomeSecondsStr), 0).toULong();
	theRoamingSeconds = TheSettingsSingleton::getQSRef().readEntry(
		createPathName(theRoamingSecondsStr), 0).toULong();	

	// retrieve data from the active profile
	Profile myCurProfile;
	isWarnOnDataThreshold = myCurProfile.getWarnOnDataThreshold();
	theThresholdPercentage = myCurProfile.getThresholdPercentage();
	theHomeDataLimit = myCurProfile.getHomeDataLimit()*1024*1024;
	theRoamingDataLimit = myCurProfile.getRoamingDataLimit()*1024*1024;
	theHomeTimeLimit = myCurProfile.getHomeTimeLimit();
	theRoamingTimeLimit = myCurProfile.getRoamingTimeLimit();
	theProfileName = myCurProfile.getProfileName();
}


void
MonthlyTraffic::save(void)
{
	DEBUG3("MonthlyTraffic::save()\n");
	// we only need to save one (for now)
	if (isRoaming)
	{
		theRoamingTraffic += theCounterBytesSent + theCounterBytesReceived;
		theRoamingSeconds += getSecondsConnected();
	}
	else
	{
		theHomeTraffic += theCounterBytesSent + theCounterBytesReceived;
		theHomeSeconds += getSecondsConnected();
	}
	theConnectionStartTime = QDateTime(); // reset to null (invalid)
	
	// because writeEntry doesn't exist in a unsigned long variety,
	// we convert to QString first
	TheSettingsSingleton::getQSRef().writeEntry(createPathName(theRoamingTrafficStr),
		QString::number(theRoamingTraffic)); 
	TheSettingsSingleton::getQSRef().writeEntry(createPathName(theHomeTrafficStr),
		QString::number(theHomeTraffic)); 
	TheSettingsSingleton::getQSRef().writeEntry(createPathName(theRoamingSecondsStr),
		QString::number(theRoamingSeconds)); 
	TheSettingsSingleton::getQSRef().writeEntry(createPathName(theHomeSecondsStr),
		QString::number(theHomeSeconds)); 
	DEBUG5("MonthlyTraffic::save: totals for %d/%d are: home=%luB/%lus, roaming=%luB/%lus\n",
			theDate.year(), theDate.month(), 
			theHomeTraffic, theHomeSeconds, 
			theRoamingTraffic, theRoamingSeconds);

	// as these are now persistant on disk, we need to reset the values
	theCounterBytesSent = 0;
	theCounterBytesReceived = 0;
	
}


void
MonthlyTraffic::start(bool anIsRoaming)
{
	DEBUG3("MonthlyTraffic::start(anIsRoaming=%d)\n", anIsRoaming);
	theConnectionStartTime = QDateTime::currentDateTime();	
	isRoaming = anIsRoaming;
	assert(theCounterBytesSent == 0);
	assert(theCounterBytesReceived == 0);
	refreshProfileData();
}


unsigned long
MonthlyTraffic::update(unsigned long aCounterBytesSent, unsigned long aCounterBytesReceived)
{
	DEBUG5("MonthlyTraffic::update(%lu, %lu)\n", aCounterBytesSent, aCounterBytesReceived);
	// if either counter is below the existing counter, that's an indication of trouble.
	// probably ppp has reset its counters. Let's save the current statistics and start
	// allover with accounting
	// note that we know fore sure that we lost track of some of the traffic :-(
	if ( aCounterBytesSent < theCounterBytesSent || 
	     aCounterBytesReceived < theCounterBytesReceived )
	{
		DEBUG2("MonthlyTraffic::update detected non-monotonous traffic statistics. Some traffic bytes not accounted for!\n");
		DEBUG3("previous Sent/Received numbers were: %lu, %lu\n", theCounterBytesSent, theCounterBytesReceived);
		// if it happened to only one, make sure to adjust the other one
		// because save() will set the theCounterBytes* to zero
		if (aCounterBytesSent > theCounterBytesSent)
			aCounterBytesSent -= theCounterBytesSent;
		if (aCounterBytesReceived > theCounterBytesReceived)
			aCounterBytesReceived -= theCounterBytesReceived;
		save();
	}

	// and check for / replace the counters
	assert(	aCounterBytesSent >= theCounterBytesSent );
	assert(	aCounterBytesReceived >= theCounterBytesReceived );
	theCounterBytesSent = aCounterBytesSent;
	theCounterBytesReceived = aCounterBytesReceived;
	
	// Check for date rollovers here
	if (!isMonthRollOverDetected && QDate::currentDate() > theDate)
	{
		DEBUG2("rollover to new date detected\n");
		
		// is it a rollover to a new month?
		QDate myDate(theDate.year(), theDate.month(), 1);
		if (QDate::currentDate() >= myDate.addMonths(1))
		{
			DEBUG1("MonthlyTraffic::update detected rollover to new month!\n");
			// the problem is that pppd doesn't care - it keeps counting
			// we can't fix that, but we can tell the user to close the
			// connection at his earliest convenience. Until then, we will
			// keep counting in the old month.
			isMonthRollOverDetected=true;
			throw MonthRolloverException();
			// note how we deliberately do NOT update the date here!!!
		}
		else
		{
			//  update the internal date of our instance
			theDate = QDate::currentDate();
		}
	}
	
	// is it time to send out a warning?
	warnIfNeeded();

	return getTotalTraffic();
}


void
MonthlyTraffic::warnIfNeeded()
{
	if (isWarnOnDataThreshold)
	{
		if ((getTimeThresholdStatus()==OVER_LIMIT) ||
				(getTimeThresholdStatus()==BETWEEN))
		{
			isWarnOnDataThreshold = false;
			throw OverThresholdException(OverThresholdException::TIME);
		}
		else
			if	((getTrafficThresholdStatus()==OVER_LIMIT) ||
					(getTrafficThresholdStatus()==BETWEEN))	
			{
				isWarnOnDataThreshold = false;
				throw OverThresholdException(OverThresholdException::TRAFFIC);
			}
	} 
}
 
