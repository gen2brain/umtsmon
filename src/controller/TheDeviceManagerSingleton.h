/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2007, 2009  Klaas van Gend
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

#ifndef THEDEVICEMANAGERSINGLETON_CLASS
#define THEDEVICEMANAGERSINGLETON_CLASS

#include "AutoDetect.h"
#include "DeviceCapabilities.h"

#include <qvaluelist.h>

//////////////////////////////////////////////////////////////////////////////

/** TheDeviceManagerSingleton abstracts, checks and protects the active instance
 *  of the Device class
 */
class TheDeviceManagerSingleton
{
private:
	/** private constructor as demanded by the Singleton Design Pattern
	 *  will be called by the get###() method if needed
	 */
	TheDeviceManagerSingleton(void);
	/** private destructor as demanded by the Singleton Design Pattern
	 */
	~TheDeviceManagerSingleton(void);	

	// installed into atexit to destruct singeton on exit.
	static void destroy(void);
	
	/// private, unimplemented copy constructor - don't use
	TheDeviceManagerSingleton(const TheDeviceManagerSingleton& aPtr);
	/// private, unimplemented assign operator - don't use
	TheDeviceManagerSingleton& operator=(TheDeviceManagerSingleton& aPtr);
	
	/** the STATIC base for all get...Ref members
	 *  if not Device exists yet, it will try to create one
	 */
	static Device& getDeviceRef(void);

public:

	/// always use this member to get access to the DeviceCapabilities - never cache
	static DeviceCapabilities& getDeviceCapsRef(void)
		{	return dynamic_cast<DeviceCapabilities&>(getDeviceRef()); };
	/// always use this member to get access to the DeviceRadioCapabilities - never cache
	static DeviceRadioCapabilities& getDeviceRadioCapsRef(void)
		{	return dynamic_cast<DeviceRadioCapabilities&>(getDeviceRef()); };
	/// always use this member to get access to the ConnectionInfo - never cache
	static ConnectionInfo* getConnectionInfoPtr(void)
		{ 	return getDeviceRef().getConnectionInfoPtr(); };
	
	/** resets (i.e. close+reopen) the Device's AT Serial Port.
	 *  one better ensures that no Queries will be during this...
	 *  @returns true if successful
	 */
	static bool resetDevice()
	{	
		ConnectionInfoInhibitor myStartingInhibitor;
		return getDeviceRef().resetATSerial(); 
	};
	
	/// the main entry member to the Singleton
	static TheDeviceManagerSingleton& me(void);
	
	/// used by main() to set a command-line specified AT port name
	void setSuggestedAT(const QString& anATPortName);
	/// used by main() to set a command-line specified PPP port name
	void setSuggestedPPP(const QString& aPPPPortName);
	/// used by main() to set the device type	
	void setSuggestedDeviceType(DeviceCapabilities::CardType  aDeviceType);
	/// used by main() to force autodetection, instead of relying on config file
	void setForceAutodetection(void);

	/// actually try to detect a device
	bool tryDeviceDetection(void);
	
private:
	bool setupDevice(void);
	void readFromConfigFile(void);
	void writeToConfigFile(void);
	bool doDeviceCreation(AutoDetectBase& aSuggestedAutoDetector);

	/// empties theAutoDetectList and deletes all autodetectors in it
	void emptyAutoDetectList(void);
	
	/// initialises theAutoDetectList with all autodetectors
	void setupAutoDetectList(void);
	
	// typedef to simplify usage of the AutoDetect list
	typedef QValueList<AutoDetectBase*> AutoDetectList;
	
	/// list of all various AutoDetect subclass instances
	AutoDetectList theAutoDetectList;
	
	/** specialised autodetect pointer, to the autodetect class that can 
	 *  actually build a Device from supplied parameters.
	 *  it's also the first entry of the AutoDetectList.
	 */
	NonAutoDetect theNonAutoDetectClass;
};



#endif // THEDEVICEMANAGERSINGLETON_CLASS
