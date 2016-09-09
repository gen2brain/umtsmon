/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2007, 2008  Klaas van Gend
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

#ifndef AUTODETECT_CLASS
#define AUTODETECT_CLASS

#include "Device.h"

/// thrown by AutoDetect_USB_ZeroCD if device was switched
class StopAutodetectionCycle
{
};

//////////////////////////////////////////////////////////////////////////////

/** abstract base class of the autodetection code
 */
class AutoDetectBase
{
public:
	AutoDetectBase(void) : 	theDeviceID(DeviceCapabilities::UNKNOWN) {};
	virtual ~AutoDetectBase() {};
	
	
	enum AutoDetectResults
	{
		NOTHING_FOUND,     // nothing was detected
		DEVICE_DETECTED,   // device was detected OK
		DEVICE_ERROR       // device was detected, but not usable
	};
	
	/// perform the autodetection and find corresponding ports 
	virtual AutoDetectResults go(void);
	
	/** pure virtual function
	 *  @returns the name of the Autodetection class
	 */
	virtual QString getName(void) = 0;

	/** creates the device that was autodetected
	 *  @returns pointer to the Device or NULL if not possible/error
	 */ 
	virtual Device* createDevice(void);

protected:

	/** pure virtual function
	 *  traverse the internal trees (PCI lists, USB lists, etc) to find
	 *  possible devices, then calls matchIDs to check if they match.
	 *  @returns something from the enum AutoDetectResults 
	 */
	virtual AutoDetectResults traverseTrees(void) = 0;
	
	/** pure virtual function
	 *  test to see if this vendor / (product/device) ID matches a known device
	 *  @param aVendorID
	 *  @param aProductID
	 *  @returns true if match found, theDeviceID will be 
	 *           updated to match the detected combination (might be different 
	 *           values, though), false if no match.
	 */ 
	virtual AutoDetectResults matchIDs(unsigned long aVendorID, unsigned long aProductID) = 0; 
	
	/// pure virtual function - discover the AT/PPP port names
	virtual bool findPortNames(void) = 0;
	
	enum LogQuality
	{
		GOOD,		// use GOOD if this is a positive ID of a certain card
		BAD,		// use BAD if an optional piece of detection failed, detection might not succeed but can continue
		KILLING,	// use KILLING if a REQUIRED piece of detection cannot be done
		INFO		// use INFO to state that something's present that can help detection
	};	

	/// logs a message about autodetection
	void sendLog(LogQuality isGood, const QString& aMessage);


protected:
	DeviceCapabilities::CardType         theDeviceID;
	
	QStringList 	thePortsList;
};


//////////////////////////////////////////////////////////////////////////////

/** "not really" autodetection - attempts to build device from user or 
 *  config file specified parameters.
 */
class NonAutoDetect : public AutoDetectBase
{
public:
	NonAutoDetect()
		: theSuggestedDeviceType(DeviceCapabilities::UNKNOWN),
	      theForceAutodetection(false) {};
	
	/// augment go by checking for forced autodetection 
	virtual AutoDetectResults go(void);

	//// empty implementation 
	virtual AutoDetectResults traverseTrees(void) 
		{return theForceAutodetection?NOTHING_FOUND:DEVICE_DETECTED;};
	
	/// empty implementation
	virtual AutoDetectResults matchIDs(unsigned long aVendorID, unsigned long aProductID);

	/// empty
	virtual bool findPortNames(void) {return true;};

	/// @returns the name of the Autodetection class
	virtual QString getName(void)
		{return "Device creation from supplied parameters";};
	
	QString theSuggestedATPortName;
	QString theSuggestedPPPPortName;
	DeviceCapabilities::CardType theSuggestedDeviceType;
	bool    theForceAutodetection;
};

//////////////////////////////////////////////////////////////////////////////

/** USB autodetection
 */
class AutoDetect_USB : public AutoDetectBase
{
public:
	//// implementation - opens libusb and uses that to find usb devices
	virtual AutoDetectResults traverseTrees(void);
	
	/// implementation - match vendor/product with known IDs for USB
	virtual AutoDetectResults matchIDs(unsigned long aVendorID, unsigned long aProductID); 

	/// implementation - discover the AT/PPP port names
	virtual bool findPortNames(void);

	/// @returns the name of the Autodetection class
	virtual QString getName(void)
		{return "USB Autodetection";}
protected:
	/** suggests the user to insmod the usbserial module with parameters and
	 *  checks if it helps
	 *  @returns false if not
	 */
	bool suggestModprobe(void);
	/** HSO devices have an entry in either /proc or /sys that tells the type 
	 *  of port it is. This member reads and parses that entry - the results will
	 *  be printed and (if ok) the port will be added to the port list on the correct place.
	 * @param myHSx the port name
	 * @param myHSOTypeFileName the file containing the entry
	 */
	void detectHSOType(const QString& myHSx, const QString& myHSOTypeFileName);
	
	/// (filled by traverseTrees) contains the USB vendor ID
	unsigned long theUSBVendorID;
	/// (filled by traverseTrees) contains the USB device ID
	unsigned long theUSBDeviceID;		
};

//////////////////////////////////////////////////////////////////////////////

/** USB autodetection - specific for the ZeroCD devices that need switching
 *  once it finds a device, it will prompt the user and then claim that
 *  no device was found - allowing the regular USB autodetector to find the switched device
 */
class AutoDetect_USB_ZeroCD : public AutoDetect_USB
{
public:
	
	/// implementation - match vendor/product with known IDs for USB
	virtual AutoDetectResults matchIDs(unsigned long aVendorID, unsigned long aProductID); 

	/// @returns the name of the Autodetection class
	virtual QString getName(void)
		{return "USB ZeroCD Autodetection";};

	/// implementation - discover the AT/PPP port names
	virtual bool findPortNames(void);

private:
	/** this member is specific to the ZeroCD autodetector and contains the code
	 *  to actually switch the device and verify that switch
	 *  @returns true if successful
	 */
	bool switchDevice(void);
	
};

//////////////////////////////////////////////////////////////////////////////

/** PCMCIA autodetection
 *  for now only works for BroadCom and Nozomi based chipsets, as they have a PCMCIA serial controller inside
 *  this class does not detect any of the pcmcia->usb2serial devices
 */
class AutoDetect_PCMCIA : public AutoDetectBase
{
public:
	//// implementation - traverses the /sys/class/pcmcia_socket tree for PCMCIA cards
	virtual AutoDetectResults traverseTrees(void);

	/// implementation - match vendor/product with known IDs for PCMCIA
	virtual AutoDetectResults matchIDs(unsigned long aVendorID, unsigned long aProductID); 

	/// implementation - discover the AT/PPP port names
	virtual bool findPortNames(void);

	/// implementation - returns the name of the Autodetection class
	virtual QString getName(void)
		{return "PCMCIA (PCCard) Autodetection";};
		
private:
	/// returns true if PCMCIA browsing in /sys is available
	bool isPCMCIABrowsingPossible(void);

	/// discover the AT/PPP port names for the Broadcom 4306 series
	bool find4306PortNames(void);

	/// discover the AT/PPP port names for the Nozomi-based series
	bool findNozomiPortNames(void);

};


//////////////////////////////////////////////////////////////////////////////

/** Raw port autodetection - this includes ttyS* ttyUSB* ttyACM* rfcomm*
 */
class AutoDetect_Rawport : public AutoDetectBase
{
public:
	//// implementation - creates a list of all serial ports on the system
	virtual AutoDetectResults traverseTrees(void);
	
	/// implementation - not used for Rawport
	virtual AutoDetectResults matchIDs(unsigned long aVendorID, unsigned long aProductID); 

	/** implementation - discover the AT/PPP port names 
	 *  (working from the first one detected by traverseTrees)
	 */
	virtual bool findPortNames(void);

	/// @returns the name of the Autodetection class
	virtual QString getName(void) = 0;

protected:
	QString theDevName;
		
private:
	QStringList theValidSerialPortsList;		
};


//////////////////////////////////////////////////////////////////////////////

/** Raw USB Serial port autodetection - ttyUSB*
 */
class AutoDetect_RawUSBports : public AutoDetect_Rawport
{
public:
	AutoDetect_RawUSBports() {theDevName = "ttyUSB*";};

	/// @returns the name of the Autodetection class
	virtual QString getName(void) 
		{return "Raw USB Serial Probing";};
};


/** CSSerial port autodetection - ttyS*, but only if CSSerial is detected anyway
 */
class AutoDetect_CSSerialPorts : public AutoDetect_Rawport
{
public:
	AutoDetect_CSSerialPorts() {theDevName = "ttyS*";};

	/// @returns the name of the Autodetection class
	virtual QString getName(void) 
		{return "PCMCIA Serial Modem Probing";};
};


/** Phone modems - test the ttyACM* and rfcomm* ports
 */
class AutoDetect_PhoneModems : public AutoDetect_Rawport
{
public:
	virtual AutoDetectResults go(void);

	/// @returns the name of the Autodetection class
	virtual QString getName(void) 
		{return "Phone Modem Probing";};
};


#endif // AUTODETECT_CLASS
