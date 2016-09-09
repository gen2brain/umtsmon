/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2006,2007,2008,2009  Klaas van Gend
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

#ifndef DEVICE_CLASS
#define DEVICE_CLASS


// forward declarations:
class SerialPort;

#include <assert.h>
#include <qstring.h>
#include <qstringlist.h>

#include <termios.h>
#include <vector>

#include "ConnectionInfo.h"
#include "DeviceCapabilities.h"


/// Device is an abstract base class that contains everything specific to a device
class Device : public DeviceCapabilities, public DeviceRadioCapabilities
{
public:
	// NOTICE THE MISSING CONSTRUCTOR HERE - IT'S IN THE PROTECTED SECTION

	/// destructor is public: anyone can delete this class (as opposed to constructing one)
	virtual ~Device();

	/** Factory method (see Design Patterns book) to create the Device class instance
	 *  The only way to create a Device instance is to use this method
	 *  @param	aCardType  - enum defines what Device class to construct
	 */
	static Device* createDeviceInstance(enum CardType aCardType);


	/// closes and deletes theATSerialPortName
	void closeATSerial(void);

	/** resets (i.e. close+open) the Device's AT Serial Port.
	 *  one better ensures that no Queries will be run after this...
	 *  typically only called by TheDeviceManagerSingleton
	 *  @returns true if successful
	 */
	bool resetATSerial();

	/** setupPorts will initialise the Device class' serial ports.
	 *  Most Device types will take the first entry to be the PPP port
	 *  and the last entry to be the AT port, but implementation details
	 *  exist - the Sierra Wireless devices will probe to tell which port
	 *  should be AT and which should be PPP. In the case of only one port,
	 *  AT and PPP will go through this one
	 *  @param aListOfPorts list with PARTIAL device names (e.g. ttyUSB1 or ttyS7)
	 *                      Must contain at least one entry.
	 *  @param aPortSpeed   baud rate of the port to use. 
	 *                      Note that for many devices, this number is irrelevant.
	 *  @returns true if all setup went ok
	 */ 
	virtual bool setupPorts(const QStringList& aListOfPorts,int aPortSpeed = B115200);
	
	
	/// @returns PPP port device name
	const QString&	getPPPSerialPortName(void) const	
		{ return thePPPSerialPortName; };
	
	/// @returns AT port device name
	const QString&	getATSerialPortName(void) const	
		{ return theATSerialPortName; };


	/// @returns pointer to the device-specific ConnectionInfo class, do not cache
	ConnectionInfo*  getConnectionInfoPtr(void)	const
		{return theConnectionInfoPtr;};

	/** answers if the modem has a method to set radio preferences (UMTS only, GPRS only, etc)
	 *  @returns  true if the Device class has an implementation for this
	 */
	virtual bool hasRadioPreferences(void) const  {return false;};
	
	/// returns a list of all Radio Preferences the card supports, default setting is first 
	virtual const QStringList getRadioPreferences(void)    {return theRadioPreferences;};
	
	/// returns true if setting the Radio Preference was OK
	virtual bool setRadioPreferences( __attribute__ ((unused)) int aPref)   {return false;}; 

	/// returns the default radio preference for the card
	virtual int getDefaultRadioPreference(void)   {return 0;}; 
	
	/// probes a port for answer with APP1 on ATI command 
	virtual bool checkATI_for_APP1(__attribute__ ((unused)) const QString& aSerialPort) {return false;};

	/// returns a string describing the Device C++ class - not the device itself
	virtual QString getDeviceClassName(void) = 0;

	/// returns the type of the Device
	virtual DeviceCapabilities::CardType getDeviceType(void) const
		{ return theDeviceType; };

	/// "normal" Devices do not need this workaround
	virtual bool needsPIN2workaround(void) {return false;}

protected:
	/** test if a port name is actually hosting a responding modem
	 *  @param aPortName	the name of the port to probe
	 *  @returns true if responsive
	 */
	virtual bool probePortForAT( const QString& aPortName ) const;

	QString		theATSerialPortName;
	QString		thePPPSerialPortName;

    QStringList theRadioPreferences;

	ConnectionInfo* theConnectionInfoPtr;
	SerialPort*	theATSerialPortPtr;
	
	CardType	theDeviceType;
	
	/// for any Device type except the Bluetooth one, this is 20000
	virtual int getPortSleepTime(void)
		{ return 20000; };

protected:
	/** Protected constructor: this class cannot be constructed by 3rdParties.
	 *  call the *static* createDeviceInstance method for that
	 */
	Device() : theConnectionInfoPtr(NULL),
		   theATSerialPortPtr(NULL) {};

private:
	/// private, unimplemented copy constructor - don't use
	Device(const Device&);
	/// private, unimplemented assign operator - don't use
	Device& operator=(Device& aPtr);
};


//############################################################################
//############################################################################
//############################################################################

/** empty implementation of the Device class - not usable for anything real
 *  this class is used when there is no device:
 *   * device detection hasn't run yet 
 *   * device detection has not yielded results
 *   * device was removed 
 */
class Device_NULL : public Device
{
public:
	Device_NULL() { theConnectionInfoPtr = NULL; };
	virtual ~Device_NULL () {};

	virtual int  getNumberOfPorts(void) const {return 0;};

	virtual QString getDeviceClassName(void) {return "No Device present\n"; };
};


//############################################################################
//############################################################################
//############################################################################

/// implementation of Device class for Option cards (both GT and Fusion+ cards)
class Device_Option : public Device
{
public:
	Device_Option();
	virtual ~Device_Option ();

	/// The Option cards have AT_OPSYS to set radio preferences
	virtual bool hasRadioPreferences(void) const  {return true;};
	
	/// returns a list of all Radio Preferences the card supports, default setting is first 
	virtual const QStringList getRadioPreferences(void) {return theRadioPreferences;};

	/// returns true if setting the Radio Preference was OK
	virtual bool setRadioPreferences(int  aPref); 

	/// the default radio preference for Option Cards is "prefer UMTS"
	virtual int getDefaultRadioPreference(void)   {return 3;}; 

	virtual int  getNumberOfPorts(void) const {return 3;};

	virtual QString getDeviceClassName(void) {return "Qualcomm-based chipset\n"; };

protected:
	QStringList theRadioPreferences;
};


//############################################################################
//############################################################################
//############################################################################

/** implementation of Device class for Huawei cards with multiple ports
 *  (as far as Klaas knows, all cards by Huawei qualify - at least:
 *       E169/E170/E220/E270/E612/E620/E660
 *   still unknown: E510)
 */
class Device_Huawei : public Device_Option
{
public:
	Device_Huawei(); 
	virtual ~Device_Huawei () {};

	virtual bool hasRadioPreferences(void) const  {return true;};
	virtual bool setRadioPreferences(int  aPref); 
	virtual int getDefaultRadioPreference(void)   {return 1;}; /// 3G preferred 

	virtual int  getNumberOfPorts(void) const {return 2;};
	virtual QString getDeviceClassName(void) {return "Generic Huawei card\n"; };

	/// overridden because we might need to drop the last port for Huawei devices
	virtual bool setupPorts(const QStringList& aListOfPorts);
	
	
private:
	struct HuaweiRadioPrefs
	{
		int     theFirst;
		int     theSecond;
		QString theDescription;
		
		HuaweiRadioPrefs(int a, int b, const QString& c) 
			: theFirst(a), theSecond(b), theDescription(c) {};
	};

	typedef std::vector<HuaweiRadioPrefs> RadioPrefsList;
	RadioPrefsList theRadioPrefsList;
	
	/// returns the port speed (as a termios number, e.g. B115200) of the single serial port
	int  getPortSpeed(void) {return (B115200);};
};

//############################################################################
//############################################################################
//############################################################################

/** implementation of Device class for the original Vodafone 3G card
 *  different in usb hub IDs, has a known bug in PIN handling
 */
class Device_Vodafone3G : public Device_Option
{
public:
	Device_Vodafone3G() { theConnectionInfoPtr = new ConnectionInfo; };
	virtual ~Device_Vodafone3G () {};

	virtual int  getNumberOfPorts(void) const {return 3;};
	virtual QString getDeviceClassName(void) {return "Original Vodafone 3G card\n"; };
	
	virtual bool needsPIN2workaround(void) {return true;};
};


//############################################################################
//############################################################################
//############################################################################

/** implementation of Device class for the Option HSO cards
 *  These Option devices use a different driver /dev/HSO*** (or /dev/tty/HS*) 
 *  It's also possible to use them as a networking interface, but that is 
 *  not supported by umtsmon.
 */
class Device_OptionHSO : public Device_Option
{
public:
	Device_OptionHSO() { theConnectionInfoPtr = new ConnectionInfo_HSO; }
	virtual ~Device_OptionHSO () {};

	virtual int  getNumberOfPorts(void) const {return 2;};
	virtual QString getDeviceClassName(void) {return "Option HSO-type device\n"; };

protected:
	/** on HSO devices, if they show up in sysfs, they're bound to work :-P
	 *  actually, probing the ports may not work...
	 *  @returns always true
	 */
	virtual bool probePortForAT( __attribute__ ((unused)) const QString& aPortName ) const
	{ return true; }
};


//############################################################################
//############################################################################
//############################################################################

/** the Single Serial Device abstracts the notion of everything that provides
 *  only one Serial Port - this in contrast to e.g. the Open 4 port usb2serial and the Nozomi
 *  devices which are 3 or 4 ports.
 * 
 *  A Single Serial device has concurrency issues - whilst a PPP connection is open,
 *  it is not possible (for now) to do e.g. signal quality requests
 */
class Device_Single_Serial : public Device
{
public:
	Device_Single_Serial() { theConnectionInfoPtr = new ConnectionInfo; };
	virtual ~Device_Single_Serial()	{};

	virtual int  getNumberOfPorts(void) const {return 1;};

	virtual bool setupPorts(const QStringList& aListOfPorts)
	{ return Device::setupPorts(aListOfPorts, getPortSpeed()); };

	
protected:
	/// returns the port speed (as a termios number, e.g. B115200) of the single serial port
	virtual int  getPortSpeed(void) = 0;
};


//############################################################################
//############################################################################
//############################################################################

/// implemention of Device class for Broadcom 4306-based cards (Sony Ericsson GC79, Option GT EDGE and GT Combo EDGE)
class Device_BroadCom_GPRS_EDGE : public Device_Single_Serial
{
public:
	virtual ~Device_BroadCom_GPRS_EDGE() {};

	virtual QString getDeviceClassName(void) {return "Broadcom based 2G card\n"; };

	// overloaded member, also need to do AT+CFUN
	virtual bool setupPorts(const QStringList& aListOfPorts);
	
	
protected:
	/** this function needs to be called on start of the device to power on
	 *  the radio.
	 */
	bool doCFUN(void);

	/// returns the port speed (as a termios number, e.g. B115200) of the single serial port
	virtual int  getPortSpeed(void) {return (B115200);};
};

//############################################################################
class Device_BroadCom57k6 : public Device_BroadCom_GPRS_EDGE 
{
public:
	virtual QString getDeviceClassName(void) {return "Broadcom based 2.5G card\n"; };

protected:
	/// returns the port speed (as a termios number, e.g. B115200) of the single serial port
	virtual int  getPortSpeed(void) {return (B57600);};
};


//############################################################################
//############################################################################
//############################################################################

/** implemention of Device class that handles a user-configured serial port.
 *  don't expect this Device class to contain much functionality
 */ 
class Device_User_Serial : public Device_Single_Serial
{
public:
	virtual ~Device_User_Serial() {};

	virtual QString getDeviceClassName(void) {return "User provided serial port\n"; };

protected:
	/// returns the port speed (as a termios number, e.g. B115200) of the single serial port
	int  getPortSpeed(void) {return (B115200);};
};


//############################################################################
//############################################################################
//############################################################################

/** implemention of Device class that handles a PCNCIA cs_serial devices
 *  this cs_serial device does not show up in the pcmcia devices list.
 */ 
class Device_CS_Serial : public Device_Single_Serial
{
public:
	virtual ~Device_CS_Serial() {};

	virtual QString getDeviceClassName(void) {return "Generic PCMCIA serial modem\n"; };

protected:
	/// returns the port speed (as a termios number, e.g. B115200) of the single serial port
	int  getPortSpeed(void) {return (B115200);};
};



//############################################################################
//############################################################################
//############################################################################

/// USB-connected mobile phone modem - e.g. connected over /dev/ttyACM0
class Device_ACM : public Device_Single_Serial
{
public:
	virtual ~Device_ACM() {};

	virtual QString getDeviceClassName(void) {return "USB-connected mobile phone modem\n"; };

	Device_ACM() : Device_Single_Serial() 
	{
		theConnectionInfoPtr = new ConnectionInfo_ACM(); 
	};
	
	/// overloaded member, also needs to do ATE1
	virtual bool setupPorts(const QStringList& aListOfPorts);

	/** FIXME: Let's just set it to true for all ACM devices
	 *  even if we only know that the Solomon SCMi250u needs this
	 */
	virtual bool needsPIN2workaround(void) {return true;};

protected:
	/// returns the port speed (as a termios number, e.g. B115200) of the single serial port
	int  getPortSpeed(void) {return (B115200);};
};


//############################################################################
//############################################################################
//############################################################################

/** Bluetooth connected mobile phone modem - e.g. connected over /dev/rfcomm0
 * 
 * This type of modem is special because it requires modifications for the 
 * roundtrip latencies in the serial connection.
 */ 
class Device_RFComm : public Device_Single_Serial
{
public:
	virtual ~Device_RFComm() {};

	virtual QString getDeviceClassName(void) {return "Bluetooth connected mobile phone modem\n"; };
	
protected:
	/// returns the port speed (as a termios number, e.g. B115200) of the single serial port
	virtual int  getPortSpeed(void) {return (B57600);};

	/// overriden for this Bluetooth device - longer sleeps are required
	int getPortSleepTime(void)
		{ return 200000;};
};

//############################################################################
//############################################################################
//############################################################################

/// implementation of Device class for Sierra Wireless cards 
class Device_Sierra : public Device
{
public:
	Device_Sierra();
	virtual ~Device_Sierra () {};

	virtual bool setupPorts(const QStringList& aListOfPorts);

	/// The Sierra cards have AT!SELRAT to set radio preferences
	virtual bool hasRadioPreferences(void) const  {return true;};
	
	/// returns a list of all Radio Preferences the card supports, default setting is first 
	virtual const QStringList getRadioPreferences(void) {return theRadioPreferences;};

	/// returns true if setting the Radio Preference was OK
	virtual bool setRadioPreferences(int  aPref); 

	// interogates the card for the current radio preference 
	virtual int getDefaultRadioPreference(void); 

	virtual int  getNumberOfPorts(void) const {return 2;};

	virtual QString getDeviceClassName(void) {return "Generic Sierra Wireless Card\n"; };


protected:
	QStringList theRadioPreferences;

	/** For Sierra Wireless cards, issuing ATI will result in several lines of answers
	 *  including a line stating "APP1" which indicates that this port is of PPP type.
	 *  Sierra Wireless devices are sensitive to this - don't mix the ports!
	 *  As we've observed devices which have their AT port as first and others (same brand,
	 *  same revision) which have their AT port as second :-(
	 *  @param   aSerialPort   device to run ATI Query on
	 *  @returns true if APP1 was amongst the answers 
	 */ 
	bool checkATI_for_APP1(const QString& aSerialPort); 
};

class Device_ACM_Ericsson : public Device_ACM 
{
public:
	virtual ~Device_ACM_Ericsson() {};

	virtual QString getDeviceClassName(void) {return "Ericsson umtsm device\n"; };
	Device_ACM_Ericsson()
	{
		ConnectionInfo_ACM_Ericsson *a = new ConnectionInfo_ACM_Ericsson();
		theConnectionInfoPtr = a;
	};
	// overloaded member, also need to do AT+CFUN
	virtual bool setupPorts(const QStringList& aListOfPorts,int aPortSpeed );
};	


#endif // defined DEVICE_CLASS
