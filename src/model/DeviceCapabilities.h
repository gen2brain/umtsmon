/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2007  Klaas van Gend
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

#ifndef DEVICECAPABILITIES_INTERFACE
#define DEVICECAPABILITIES_INTERFACE

#include <qstring.h>


/// Interface for higher-level classes to retrieve info of the Device
class DeviceCapabilities
{
public:
	virtual ~DeviceCapabilities() {};

	/** the number of ports a device has.
	 *  @returns 0 (in case of a NoDevice) up to 4 (max we've encountered)
	 */
	virtual int  getNumberOfPorts(void) const = 0;
	
	virtual const QString&	getPPPSerialPortName(void) const = 0;	
	virtual const QString&	getATSerialPortName(void) const = 0;
	
	/** @returns the name of the C++ class of the Device
	 *  this probably only has value to programmers and support
	 */
	virtual QString getDeviceClassName(void) = 0;
	
	/** returns true if the SIM_PIN2 workaround is needed
	 *  (Original Vodafone3G and Solomon SCM250i only)
	 * @return true if workaround is needed
	 */
	virtual bool needsPIN2workaround(void) = 0;
	
	/** enum defining all known card types
	 *  note: IDs are in a derivative of leetspeek (see 'leet' in Wikipedia)
	 *  to fit "recognizable IDs" into 8 hex nibbles
	 * 
	 *  - this need to be recognizable, fixed IDs 
	 *    because they will appear in the config files
	 */
	enum CardType
	{
		UNKNOWN = 0,
		NULLDEVICE = 1,                    // EMPTY DEVICE
		
		GENERIC_USER_SERIAL = 0x5e21a700,  // SERIAL00
		MODEM_ACM           = 0x1770D0AC,  // MOD_AC 
		MODEM_ACM_ERICSSON  = 0x1770D0E2,  // MOD_ER
		MODEM_RFCOMM        = 0x1770D02F,  // MOD_RF
		GENERIC_CS_SERIAL   = 0x5e21a7c5,  // SERIALCS
		OPTION_4PORT        = 0xcaa1c004,  // QAALCO_4 (qualcomm chipset)
		OPTION_3PORT = OPTION_4PORT,
		OPTION_VODAFONE_3G  = 0xcaa1c001,  // QAALCO_1 (qualcomm chipset)
		OPTION_NOZOMI       = 0x17020177,  // NOZOM
		OPTION_HSO          = 0x06710450,  // OPTI_HS0
		HUAWEI_MULTI        = 0x600A1000,  // GOOAI
		BROADCOM_GPRS_EDGE  = 0xB20AD079,  // BROAD_79 (79 as in 'SE GC79')
		BROADCOM_57K6       = 0xB20AD057,  // BROAD_57 (57k6 port speed limitation)  
		SIERRA_2PORT	    = 0x51322402,  // SIERRA_2
		NOVATEL_2PORT = GENERIC_USER_SERIAL,
		SONYERICSSON_GC79 = BROADCOM_GPRS_EDGE,
	};
	
	/// returns the type of the Device
	virtual CardType getDeviceType(void) const = 0;

	bool isSinglePort(void)
	{
		return getPPPSerialPortName() == getATSerialPortName();
	}; 
};

/// Interface for higher-level classes to retrieve info of the Device on the radio topic
class DeviceRadioCapabilities
{
public:
	virtual ~DeviceRadioCapabilities() {};

	/** answers if the modem has a method to set radio preferences (UMTS only, GPRS only, etc)
	 *  @returns  true if the Device class has an implementation for this
	 */
	virtual bool hasRadioPreferences(void) const = 0;
	
	/// returns a list of all Radio Preferences the card supports, default setting is first 
	virtual const QStringList getRadioPreferences(void) =0;
	
	/// returns true if setting the Radio Preference was OK
	virtual bool setRadioPreferences(int aPref) = 0; 

	/// returns the default radio preference for the card
	virtual int getDefaultRadioPreference(void) = 0; 
};

#endif // DEVICECAPABILITIES_INTERFACE
