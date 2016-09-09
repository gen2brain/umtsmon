/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2007,2008  Klaas van Gend, Phil Gorsuch
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
#include "AutoDetect.h"
#include "FileStuff.h"
#include "SerialPort.h"
#include "Query.h"
#include "Popup.h"
#include "Runner.h"

#include <assert.h>
#include <usb.h>

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
Device*
AutoDetectBase::createDevice(void)
{
	DEBUG3(" AutoDetectBase::createDevice() for CardType 0x%08X\n", theDeviceID);
	assert(theDeviceID);
	if (theDeviceID==0)
		return NULL;

	Device* myDevicePtr = Device::createDeviceInstance(theDeviceID);
	if (myDevicePtr == NULL)
		return NULL;

	if (myDevicePtr->setupPorts(thePortsList) == true)
	{
		sendLog(GOOD, "Device created successfully");
	}
	else
	{
		sendLog(BAD, "Device couldn't be created");
		DEBUG1(" AutoDetectBase::createDevice() failed :-(\n");
		delete myDevicePtr;
		myDevicePtr = NULL;
	}
	return myDevicePtr;
}

//////////////////////////////////////////////////////////////////////////////
AutoDetectBase::AutoDetectResults
AutoDetectBase::go(void)
{
	DEBUG3("AutoDetectBase::go() for '%s'\n", getName().ascii());
	AutoDetectResults myResult = traverseTrees();
	if (myResult == DEVICE_DETECTED)
	{
		if (!findPortNames())
			myResult = DEVICE_ERROR;
	}
	return myResult;
}

//////////////////////////////////////////////////////////////////////////////
void
AutoDetectBase::sendLog(LogQuality isGood, const QString& aMessage)
{
	// FIXME: for now, this is just a print to stdout.
	// in the future, this should be calling back a registered function that
	// either dumps to stdout, a log file or to a text view window
	QString myQuality;
	switch (isGood)
	{
	case GOOD:
		myQuality="GOOD";
		DEBUG2("GOOD: '%s'\n", aMessage.ascii());
		break;
	case BAD:
		myQuality="BAD";
		DEBUG2("BAD: '%s'\n", aMessage.ascii());
		break;
	case KILLING:
		myQuality="KILLING";
		DEBUG1("KILLING: '%s'\n", aMessage.ascii());
		break;
	case INFO:
		myQuality="INFO";
		DEBUG3("INFO: '%s'\n", aMessage.ascii());
		break;
	}
}



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
AutoDetectBase::AutoDetectResults
AutoDetect_USB::traverseTrees(void)
{
	DEBUG3(" AutoDetect_USB::traverseTrees()\n");
	usb_init();
	usb_find_busses();
	usb_find_devices();

	struct usb_bus* bus = NULL;

	for (bus = usb_get_busses(); bus; bus = bus->next)
	{
		struct usb_device* dev = NULL;
		for (dev = bus->devices; dev; dev = dev->next)
		{
			DEBUG4("  Checking USB device on %s:%s\n", bus->dirname, dev->filename);
			switch(matchIDs(dev->descriptor.idVendor, dev->descriptor.idProduct))
			{
			case NOTHING_FOUND:
				break;
			case DEVICE_DETECTED:
				theUSBVendorID = dev->descriptor.idVendor;
				theUSBDeviceID = dev->descriptor.idProduct;
				return DEVICE_DETECTED;
			case DEVICE_ERROR:
				return DEVICE_ERROR;
			}
		}
	}
	// FIXME: if we got here, we couldn't find a device by ID
	// let's check if we can find any usbserial or usbmodems (ttyACM) this way

	sendLog(BAD, "No USB-based device found");
	return NOTHING_FOUND;
}

//////////////////////////////////////////////////////////////////////////////
AutoDetectBase::AutoDetectResults
AutoDetect_USB::matchIDs(unsigned long aVendorID, unsigned long aProductID)
{
	DEBUG3(" AutoDetect_USB::matchIDs(0x%04lx, 0x%04lx)\n", aVendorID, aProductID);

struct s_args
{
	unsigned long theUSBVendorID;
	unsigned long theUSBProductID;
	QString       theGOODString;
	Device::CardType theDeviceID;
};


static struct s_args theArgsTable[] =
{
	//VENDOR, PRODUCT,     DESCRIPTION,           DEVICE TYPE
	//-------------------------------------------------------------------

	//////////////////// HUAWEI
	{ 0x12d1,  0x1001,     "Huawei E6xx",         Device::HUAWEI_MULTI, },
	{ 0x12d1,  0x1003,     "Huawei E220 / E800",  Device::HUAWEI_MULTI, },
	{ 0x12d1,  0x1004,     "Huawei E220bis",      Device::HUAWEI_MULTI, },
	// catch-all Huawei, keep last:
	{ 0x12d1,  0,          "Unknown Huawei card", Device::HUAWEI_MULTI, },

	//////////////////// NOVATEL
	// many of the below product IDs taken from the 2.6.27.7 kernel
	{ 0x1410, 0x1400, "Novatel U730",                Device::NOVATEL_2PORT, },
	{ 0x1410, 0x1410, "Novatel U740",                Device::NOVATEL_2PORT, },
	{ 0x1410, 0x1420, "Novatel EU870",               Device::NOVATEL_2PORT, },
	{ 0x1410, 0x1430, "Novatel XU870 HSDPA",         Device::NOVATEL_2PORT, },
	{ 0x1410, 0x1450, "Novatel X950D",               Device::NOVATEL_2PORT, },
	{ 0x1410, 0x2410, "Novatel EU740",               Device::NOVATEL_2PORT, },
	{ 0x1410, 0x2420, "Novatel EU870D",              Device::NOVATEL_2PORT, },
	{ 0x1410, 0x4400, "Novatel MC950",               Device::NOVATEL_2PORT, },
	{ 0x1410, 0x7000, "Novatel \"Future HSPA 1\"",   Device::NOVATEL_2PORT, },
	{ 0x1410, 0x7001, "Novatel Ovation MC990D",      Device::NOVATEL_2PORT, },

	// Unsupported Novatel Wireless Devices (EVDO)
	{ 0x1410, 0x1100, "Novatel Merlin XS620/S640",   Device::UNKNOWN, },
	{ 0x1410, 0x1110, "Novatel Merlin S620",         Device::UNKNOWN, },
	{ 0x1410, 0x1120, "Novatel Merlin EX720",        Device::UNKNOWN, },
	{ 0x1410, 0x1130, "Novatel Merlin S720",         Device::UNKNOWN, },
	{ 0x1410, 0x2110, "Novatel Merlin ES620 / Merlin ES720 / Ovation U720", Device::UNKNOWN, },
	{ 0x1410, 0x2100, "Novatel EV620 CDMA/EV-DO",    Device::UNKNOWN, },
	{ 0x1410, 0x2110, "Novatel ES720 CDMA/EV-DO",    Device::UNKNOWN, },
	{ 0x1410, 0x2120, "Novatel ES725 CDMA/EV-DO",    Device::UNKNOWN, },
	{ 0x1410, 0x2130, "Novatel Merlin ES620 SM Bus", Device::UNKNOWN, },
	{ 0x1410, 0x4100, "Novatel U727",                Device::UNKNOWN, },
	{ 0x1410, 0x6000, "Novatel \"Future EV-DO 1\"",    Device::UNKNOWN, },
	{ 0x1410, 0x6001, "Novatel \"Future EV-DO 2\"",    Device::UNKNOWN, },
	// catch-all Novatel, keep last:
	{ 0x1410, 0,      "Unknown Novatel Wireless",    Device::NOVATEL_2PORT, },

	// Dell (OEM based on Novatel)
	{ 0x413c, 0x8115, "Dell Wireless 5500 Mobile Broadband HSDPA Mini-Card",     Device::NOVATEL_2PORT, },
	{ 0x413c, 0x8116, "Dell Wireless 5505 Mobile Broadband HSDPA Mini-Card",     Device::NOVATEL_2PORT, },
	{ 0x413c, 0x8118, "Dell Wireless 5510 Mobile Broadband HSDPA ExpressCard",   Device::NOVATEL_2PORT, },
	{ 0x413c, 0x8136, "Dell Wireless HSDPA 5520",                                Device::NOVATEL_2PORT, },
	{ 0x413c, 0x8137, "Dell Wireless HSDPA 5520",                                Device::NOVATEL_2PORT, },
	{ 0x413c, 0x8138, "Dell Wireless 6620 Voda I Mobile Broadband 3G HSDPA",     Device::NOVATEL_2PORT, },
	// Unsupported DELL (EVDO)
	{ 0x413c, 0x8114, "Dell Wireless 5700 Mobile Broadband CDMA/EVDO Mini-Card", Device::UNKNOWN, },
	{ 0x413c, 0x8117, "Dell Wireless 5700 Mobile Broadband CDMA/EVDO ExpressCard", Device::UNKNOWN, },
	{ 0x413c, 0x8128, "Dell Wireless 5700 Mobile Broadband CDMA/EVDO Mini-Card", Device::UNKNOWN, },
	{ 0x413c, 0x8129, "Dell Wireless 5700 Mobile Broadband CDMA/EVDO Mini-Card", Device::UNKNOWN, },
	{ 0x413c, 0x8133, "Dell Wireless 5720",                                      Device::UNKNOWN, },

	//////////////////// SIERRA WIRELESS (& alike)
	{ 0x1199, 0x6804, "Sierra Wireless '6804'", Device::NOVATEL_2PORT, },
	{ 0x1199, 0x6820, "Sierra Wireless Aircard 875", Device::NOVATEL_2PORT, },
	{ 0x1199, 0x6812, "Sierra Wireless MC8775 or AC875U",  Device::SIERRA_2PORT, },
	{ 0x1199, 0x6813, "Sierra Wireless Thinkpad MC8775", Device::SIERRA_2PORT, },
	{ 0x1199, 0x6815, "Sierra Wireless MC8775", Device::SIERRA_2PORT, },
	{ 0x1199, 0x6821, "Sierra Wireless AC875U", Device::SIERRA_2PORT, },
	{ 0x1199, 0x6832, "Sierra Wireless MC8780", Device::SIERRA_2PORT, },
	{ 0x1199, 0x6833, "Sierra Wireless MC8781", Device::SIERRA_2PORT, },
	{ 0x1199, 0x683A, "Sierra Wireless MC8785", Device::SIERRA_2PORT, },
	{ 0x1199, 0x683B, "Sierra Wireless MC8785 (Composite)", Device::SIERRA_2PORT, },
	{ 0x1199, 0x683C, "Sierra Wireless MC8790", Device::SIERRA_2PORT, },
	{ 0x1199, 0x683D, "Sierra Wireless MC8790", Device::SIERRA_2PORT, },
	{ 0x1199, 0x683E, "Sierra Wireless MC8790", Device::SIERRA_2PORT, },
	{ 0x1199, 0x6850, "Sierra Wireless AC880",  Device::SIERRA_2PORT, },
	{ 0x1199, 0x6851, "Sierra Wireless AC881",  Device::SIERRA_2PORT, },
	{ 0x1199, 0x6855, "Sierra Wireless AC880U", Device::SIERRA_2PORT, },
	{ 0x1199, 0x6856, "Sierra Wireless AC881U", Device::SIERRA_2PORT, },
	{ 0x1199, 0x6852, "Sierra Wireless AirCard 880 E", Device::SIERRA_2PORT, },
	{ 0x1199, 0x6853, "Sierra Wireless AirCard 881 E", Device::SIERRA_2PORT, },
	{ 0x1199, 0x6859, "Sierra Wireless AirCard 885 E", Device::SIERRA_2PORT, },
	{ 0x1199, 0x685A, "Sierra Wireless AirCard 885 E", Device::SIERRA_2PORT, },
	{ 0x1199, 0x6880, "Sierra Wireless, type unknown", Device::SIERRA_2PORT, },
	{ 0x1199, 0x6890, "Sierra Wireless, type unknown", Device::SIERRA_2PORT, },
	{ 0x1199, 0x6891, "Sierra Wireless, type unknown", Device::SIERRA_2PORT, },
	{ 0x1199, 0x6892, "Sierra Wireless, type unknown", Device::SIERRA_2PORT, },
	{ 0x03f0, 0x1e1d, "HP HS2300 / MC8775 or AC875U", Device::SIERRA_2PORT, },
	// Sierra Wireless device in TRU-Install mode
	//   - probably device was not recognized by installed sierra driver
	{ 0x1199, 0x0fff, "Sierra Wireless device in TRU-Install mode", Device::UNKNOWN, },
	// Unsupported Sierra Wireless Devices (EVDO & alike)
	{ 0x1199, 0x0017, "Sierra Wireless EM5625 - EVDO", Device::UNKNOWN, },
	{ 0x1199, 0x0018, "Sierra Wireless MC5720 - EVDO", Device::UNKNOWN, },
	{ 0x1199, 0x0019, "Sierra Wireless AirCard 595 - EVDO", Device::UNKNOWN, },
	{ 0x1199, 0x0020, "Sierra Wireless MC5725 - EVDO", Device::UNKNOWN, },
	{ 0x1199, 0x0021, "Sierra Wireless AirCard 597E - EVDO", Device::UNKNOWN, },
	{ 0x1199, 0x0023, "Sierra Wireless AirCard - EVDO", Device::UNKNOWN, },
	{ 0x1199, 0x0023, "Sierra Wireless USB Dongle C597 - EVDO", Device::UNKNOWN, },
	{ 0x1199, 0x0120, "Sierra Wireless USB Dongle 595U - EVDO", Device::UNKNOWN, },
	{ 0x1199, 0x0218, "Sierra Wireless MC5720 - EVDO", Device::UNKNOWN, },
	{ 0x1199, 0x0220, "Sierra Wireless MC5725 - EVDO", Device::UNKNOWN, },
	{ 0x1199, 0x6468, "Sierra Wireless MP3G - EVDO", Device::UNKNOWN, },
	{ 0x0f30, 0x1b1d, "Sierra Wireless MC5720 - EVDO", Device::UNKNOWN, },
	// catch-all Sierra Wireless, keep last:
	{ 0x1199,      0, "Unknown Sierra Wireless", Device::GENERIC_USER_SERIAL, },

	//////////////////// Toshiba
	{ 0x0930, 0x0d45, "Toshiba G450 HSDPA modem/phone", Device::OPTION_3PORT, },

	//////////////////// Qualcomm ODMs
	{ 0x05C6, 0x6613, "ONDA N501HS / H600",          Device::OPTION_3PORT, },
	{ 0x19d2, 0x0031, "ZTE MF636",                   Device::OPTION_3PORT, },
	{ 0x05c6, 0x19d2, "Acer Aspire one 150X 3G (Qualcomm HS-USB)", Device::OPTION_3PORT, },
	//////////////////// OPTION
	{ 0x1955, 0x1000, "Option Icon or similar", Device::OPTION_3PORT, },
	{ 0x05c6, 0x1000, "Option Icon in USB Mass Storage Mode - please switch", Device::UNKNOWN, },
	// keep the specialised one (0x5000 for V3G) first, the catch-all (0 for generic Options) last
	{ 0x0af0, 0x5000, "orig Vodafone 3G",      Device::OPTION_VODAFONE_3G, },
	// the following is info from: http://www.teltonika.lt/uploads/docs/3g_linux.pdf
	{ 0x0af0, 0x6501, "Teltonika ModemUSB",    Device::OPTION_3PORT, },
	{ 0x0af0, 0x6600, "Option Icon as modem",  Device::OPTION_3PORT, },
	{ 0x0af0, 0x6601, "Option Icon as modem",  Device::OPTION_3PORT, },
	{ 0x0af0, 0x6901, "Option Icon as modem",  Device::OPTION_3PORT, },
	/////////////////// OPTION HSO (all USB IDs derived from hso v1.2 driver)
	{ 0x0af0, 0x6711, "Option HSO-capable",  Device::OPTION_HSO, },
	{ 0x0af0, 0x6731, "Option HSO-capable",  Device::OPTION_HSO, },
	{ 0x0af0, 0x6751, "Option HSO-capable",  Device::OPTION_HSO, },
	{ 0x0af0, 0x6771, "Option HSO-capable",  Device::OPTION_HSO, },
	{ 0x0af0, 0x6791, "Option HSO-capable",  Device::OPTION_HSO, },
	{ 0x0af0, 0x6811, "Option HSO-capable",  Device::OPTION_HSO, },
	{ 0x0af0, 0x6911, "Option HSO-capable",  Device::OPTION_HSO, },
	{ 0x0af0, 0x6951, "Option HSO-capable",  Device::OPTION_HSO, },
	{ 0x0af0, 0x6971, "Option Icon 225 (HSO-capable)", Device::OPTION_HSO, },
	{ 0x0af0, 0x7011, "Option GE 0301 (HSO-capable)", Device::OPTION_HSO, },
	{ 0x0af0, 0x7031, "Option HSO-capable",  Device::OPTION_HSO, },
	{ 0x0af0, 0x7051, "Option HSO-capable",  Device::OPTION_HSO, },
	{ 0x0af0, 0x7071, "Option HSO-capable",  Device::OPTION_HSO, },
	{ 0x0af0, 0x7111, "Option HSO-capable",  Device::OPTION_HSO, },
	{ 0x0af0, 0x7211, "Option HSO-capable",  Device::OPTION_HSO, },
	{ 0x0af0, 0x7251, "Option HSO-capable",  Device::OPTION_HSO, },
	{ 0x0af0, 0x7271, "Option HSO-capable",  Device::OPTION_HSO, },
	{ 0x0af0, 0x7301, "Option GE40x (HSO-capable)",    Device::OPTION_HSO, },
	{ 0x0af0, 0x7311, "Option HSO-capable",  Device::OPTION_HSO, },
	{ 0x0af0, 0x7361, "Option GE40x (HSO-capable)",    Device::OPTION_HSO, },
	{ 0x0af0, 0x7401, "Option GI 0401 (HSO-capable)",  Device::OPTION_HSO, },
	{ 0x0af0, 0x7501, "Option GTM 382 (HSO-capable)",  Device::OPTION_HSO, },
	{ 0x0af0, 0x7601, "Option GE40x (HSO-capable)",    Device::OPTION_HSO, },
	{ 0x0af0, 0xc031, "Option Icon EDGE",    Device::OPTION_HSO, },
	{ 0x0af0, 0xd013, "Option HSxPA module", Device::OPTION_HSO, },
	{ 0x0af0, 0xd031, "Option Icon 321",     Device::OPTION_HSO, },
	{ 0x0af0, 0xd033, "Option Icon 322",     Device::OPTION_HSO, },

	/// the catch-all Option
	{ 0x0af0,      0, "Option GT family",    Device::OPTION_3PORT, },

	//////////////////// 4G SYSTEMS
	{ 0x1955, 0x4e44, "4GSystems USB Box",      Device::UNKNOWN, },
	{ 0x11f5,      0, "4GSystems USB Box",      Device::GENERIC_USER_SERIAL, },

	//////////////////// PANTECH
	// also known as Verizon PC5750, doesn't work with umtsmon
	{ 0x106c, 0x3702, "PANTECH/Verizon PC5750", Device::UNKNOWN, },

	//////////////////// ERICSSON - found in e.g. Lenovo
	{ 0x0bdb, 0x1900, "Ericsson Mobile Broadband Module F3507g", Device::MODEM_ACM_ERICSSON, },
	{ 0x413c, 0x8147, "Dell Wireless 5530 Mobile Broadband (3G HSPA) Mini-Card", Device::MODEM_ACM_ERICSSON, }, // is identical to F3507g
	
	//////////////////// Various
	// from the 2.6.24 Linux /drivers/usb/serial/airprime.c:
	{ 0x0c88, 0x17da, "Kyocera Wireless KPC650/Passport", Device::GENERIC_USER_SERIAL, },
	// from the 2.6.24 Linux /drivers/usb/serial/option.c:
	{ 0x16d5, 0x6501, "Anydata ADU E100A",      Device::OPTION_3PORT, },
	{ 0x16d5, 0x6502, "Anydata ADU E100A",      Device::OPTION_3PORT, },
	{ 0x1a8d, 0x1002, "Bandrich C100_1",        Device::OPTION_3PORT, },
	{ 0x1a8d, 0x1003, "Bandrich C100_1",        Device::OPTION_3PORT, },

	// keep this one last
	{ 0, 0, "", Device::UNKNOWN },
};

	int i;
	for (i=0; theArgsTable[i].theUSBVendorID; i++)
	{
		if (aVendorID == theArgsTable[i].theUSBVendorID)
		{
			// also need to match product ID?
			if (theArgsTable[i].theUSBProductID != 0)
			{
				if (aProductID != theArgsTable[i].theUSBProductID)
				{
					continue;
				}
			}
			// if we get here, we're cool
			theDeviceID = theArgsTable[i].theDeviceID;
			sendLog(GOOD, "A '" + theArgsTable[i].theGOODString + "' modem detected");

			if (theDeviceID == Device::UNKNOWN)
			{
				sendLog(KILLING, "However, this device is not supported");
				return NOTHING_FOUND;
			}
			return DEVICE_DETECTED;
		}
	}

	return NOTHING_FOUND;
}

//////////////////////////////////////////////////////////////////////////////
void 
AutoDetect_USB::detectHSOType(const QString& myHSx, const QString& myHSOTypeFileName)
{
	if (!FileStuff::doesFileExist(myHSOTypeFileName))
		return;
	QString myHSOType = FileStuff::getFileFirstLine(myHSOTypeFileName);
	if (myHSOType == "Application")
	{
		DEBUG4("Found HSO Application Port on %s\n", myHSx.ascii());
		// make sure that good application/modem ports are first in the list
		thePortsList.push_front(myHSx);
		return;
	}
	if (myHSOType == "Modem")
	{
		DEBUG4("Found HSO Modem Port on %s\n", myHSx.ascii());
		// make sure that good application/modem ports are first in the list
		thePortsList.push_front(myHSx);
		return;
	}
	if (myHSOType == "Control")
	{
		DEBUG4("Found HSO Control Port on %s\n", myHSx.ascii());
		// make sure that good control ports are last in the list
		thePortsList.push_back(myHSx);
		return;
	}
	if (myHSOType == "Diagnostic")
	{
		DEBUG4("Found HSO Diagnostic Port on %s - ignoring\n", myHSx.ascii());
		return;
	}
	DEBUG1("Found unknown HSO port type: %s - ignoring", myHSOType.ascii());
}

//////////////////////////////////////////////////////////////////////////////
bool AutoDetect_USB::findPortNames(void)
{
	// Unfortunately, this info is not available from libusb :-(
	DEBUG3(" AutoDetect_USB::findPortNames()\n");

	// let's check for /dev/ttyHS* entries - if appropriate
	if (theDeviceID==Device::OPTION_HSO)
	{
		DEBUG4("looking for ttyHS* ports\n");
		// according to the README of the HSO 1.2 drivers, we should start by
		// checking /proc/hso/devices/ttyHS*
		// we want the entry that reads "Control" for AT
		// and the entry that reads "Application" for PPP
		int myHSNumber;
		
		// we have to check for a lot of devices - due to insert/remove/insert/remove issues
		// the original Option version of the driver stores this in /proc
		for (myHSNumber=0; myHSNumber<=4; myHSNumber++)
		{
			QString myHSx = "ttyHS" + QString::number(myHSNumber);
			QString myProc = "/proc/hso/devices/" + myHSx;
			detectHSOType(myHSx, myProc);
		}
		for (myHSNumber=0; myHSNumber<=4; myHSNumber++)
		{
			QString myHSx = "ttyHS" + QString::number(myHSNumber);
			QString mySys = "/sys/class/tty/" + myHSx + "/hsotype";
			detectHSOType(myHSx, mySys);
		}
		if (thePortsList.empty())
			DEBUG1("HSO device, but no /dev/ttyHS* devices found...\n");
	}

	// if we're an HSO device and we didn't find any /dev/ttyHS* dev entries
	// OR we are a regular USB serial device, let's probe ttyUSB* dev entries
	if (thePortsList.empty())
	{
		DEBUG4("looking for ttyUSB ports\n");
		int myUSBSerialPort;
		// we have to check for a lot of devices - due to insert/remove/insert/remove issues
		for (myUSBSerialPort=0; myUSBSerialPort<32; myUSBSerialPort++)
		{
			QString myttyUSBx = "ttyUSB" + QString::number(myUSBSerialPort);

			if (!FileStuff::doesFileExist("/sys/class/tty/" + myttyUSBx))
				continue;

			// a USB serial port was found!
			thePortsList.push_back(myttyUSBx);
			DEBUG4("  Found serial port number %zd with name '%s'\n", thePortsList.count(), myttyUSBx.ascii());
		}
	}

	// if nothing found, let's also check for real USB Modems
	if (thePortsList.empty())
	{
		DEBUG4("looking for ttyACM ports\n");
		if (FileStuff::doesFileExist("/dev/ttyACM0"))
			thePortsList.push_back("ttyACM0");
		if (FileStuff::doesFileExist("/dev/ttyACM1"))
			thePortsList.push_back("ttyACM1");
	}

	sendLog(INFO, "There are " + QString::number(thePortsList.count()) + " ports available");

	// probing of this port will be done later...
	return true;
}

//////////////////////////////////////////////////////////////////////////////
bool AutoDetect_USB::suggestModprobe(void)
{
	// if we get here, we detected something looking like a usb2serial device
	// yet we didn't see any ttyUSB ports.
	// darn - let's see if the usbserial driver is loaded and suggest
	// to load it otherwise
	DEBUG4("AutoDetect_USB::suggestModprobe(void)\n");
	if (FileStuff::doesFileContainString("/proc/modules", QString("usbserial")))
	{
		DEBUG1("no /dev/ttyUSB* entries, but the usbserial module is loaded???\n");
		return false;
	}

	QString myMessage = QObject::tr("Your device was recognised as a "
		"usbserial device,\nyet the corresponding kernel module is not "
		"running on your system.\n\nDo you want to load that module now?");
	if (Popup::YesNoQuestion(myMessage))
	{
		Runner myRunner;
		char myCommandString[256];
		snprintf(myCommandString, 255, "/sbin/modprobe usbserial vendor=0x%lx product=0x%lx", theUSBVendorID, theUSBDeviceID);
		DEBUG1("%s\n", myCommandString);
		myRunner.runCommand(Runner::SOMESU, myCommandString);

		// we now have to wait and see...
		sleep(1);
		if (findPortNames())
			return true;
	}
	return false;
}



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

AutoDetectBase::AutoDetectResults
AutoDetect_USB_ZeroCD::matchIDs(unsigned long aVendorID, unsigned long aProductID)
{
	DEBUG3(" AutoDetect_USB::matchIDs(0x%04lx, 0x%04lx)\n", aVendorID, aProductID);

struct s_args
{
	unsigned long theUSBVendorID;
	unsigned long theUSBProductID;
	QString       theGOODString;
	Device::CardType theDeviceID;
};


static struct s_args theArgsTable[] =
{
	//VENDOR, PRODUCT,     DESCRIPTION,           DEVICE TYPE
	//-------------------------------------------------------------------

	// note: most of the below list comes from the usb_modeswitch.conf file
	// dated March 16, 2008

	//////////////////// HUAWEI
	{ 0x12d1,  0x1001,     "Huawei E169G",        Device::UNKNOWN, },
	{ 0x12d1,  0x1003,     "Huawei E220 / E800",  Device::UNKNOWN, },

	//////////////////// NOVATEL
	{ 0x1410, 0x5010, "Novatel Wireless Ovation MC950D HSUPA and XU950D", Device::UNKNOWN, },

	//////////////////// ONDA
	{ 0x19d2, 0x2000, "ONDA/ZTE modems", Device::UNKNOWN, },
	
	//////////////////// Toshiba
	{ 0x0930, 0x0d46, "Toshiba G450 HSDPA modem/phone", Device::UNKNOWN, },
	
	//////////////////// OPTION
	{ 0x05c6, 0x1000, "Option Globsurfer Icon", Device::UNKNOWN, },
	{ 0x0AF0, 0x6911, "Option Icon 7.2 HSO firmware", Device::OPTION_HSO, },
	{ 0x0AF0, 0x6971, "Option Icon 225 HSO firmware", Device::OPTION_HSO, },
	{ 0x0AF0, 0x7011, "Option Globetrotter HSUPA", Device::OPTION_HSO, },

	// keep this one last
	{ 0, 0, "", Device::UNKNOWN },
};

	int i;
	for (i=0; theArgsTable[i].theUSBVendorID; i++)
	{
		if (aVendorID == theArgsTable[i].theUSBVendorID)
		{
			// also need to match product ID?
			if (theArgsTable[i].theUSBProductID != 0)
			{
				if (aProductID != theArgsTable[i].theUSBProductID)
				{
					continue;
				}
			}
			theDeviceID = theArgsTable[i].theDeviceID;
			DEBUG3("This is a known ZeroCD Device: \"%s\"\n",
					theArgsTable[i].theGOODString.ascii());
			return DEVICE_DETECTED;
		}
	}

	return NOTHING_FOUND;
}

//////////////////////////////////////////////////////////////////////////////
bool AutoDetect_USB_ZeroCD::findPortNames(void)
{
	DEBUG3(" AutoDetect_USB_ZeroCD::findPortNames()\n");

	// the parent's findPortNames() will do just fine
	// we will happily take that list and decide based on that...
	AutoDetect_USB::findPortNames();

	// all ZeroCD Devices we know of have at least two ports - most have 3...
	if (thePortsList.count() < 2)
	{
		switchDevice();

		// and of course - we didn't detect the device
		// there's no real need to continue - the switch will take a few more
		// seconds so let's quit the autodetection completely.
		throw StopAutodetectionCycle();
	}
	
	return false;
}

//////////////////////////////////////////////////////////////////////////////
bool AutoDetect_USB_ZeroCD::switchDevice(void)
{
	DEBUG3("AutoDetect_USB_ZeroCD::switchDevice(void)\n");

	Runner myRunner;
	if (myRunner.getPath(Runner::USBMODESWITCH).isEmpty())
	{
		Popup::Warning(QObject::tr("We think we found your device, but it shows too little ports.\n"
				"This usually indicated that your device needs 'switching'.\n"
				"However, no switch application (usb_modeswitch) was found.\n"
				"we will continue, but this might cause trouble later on..."))  ;
		return false;
	}

	// let's ask the user nicely - if he has a switcher installed, that is...
	if (myRunner.getPath(Runner::USBMODESWITCH).isEmpty())
	{
		Popup::Warning(QObject::tr("umtsmon detected that you need to 'modeswitch' your device.\n"
				"However, you do not have any switch program installed.\n"
				"We suggest you install usb_modeswitch and edit its config file"));
		return false;
	}
	else
	{
		if (Popup::YesNoQuestionWithMemory("SwitchDevice", QObject::tr("umtsmon detected that you need to switch your device.\n"
				"Click Yes if you agree.\n Note: this does not work properly for all devices yet!")))
		{
			myRunner.runCommand(Runner::USBMODESWITCH, "");
		}
	}

	QStringList myStdOut = myRunner.getStdOutStringList();
	QStringList myStdErr = myRunner.getStdErrStringList();

	// if there is no output at all, usb_modeswitch was run through kdesu/etc
	// otherwise, myStdOut should contain a lot of info

	QStringList::Iterator it;
	DEBUG4("*****output of usb_modeswitch to stdout\n");
	for (it = myStdOut.begin(); it != myStdOut.end(); ++it )
	{
	    DEBUG4("* %s\n", (*it).ascii());
	}
	DEBUG4("*****end-of-output\n");
	DEBUG4("*****output of usb_modeswitch to stderr\n");
	for (it = myStdErr.begin(); it != myStdErr.end(); ++it )
	{
	    DEBUG4("* %s\n", (*it).ascii());
	}
	DEBUG4("*****end-of-output\n");

	return true;
}



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
AutoDetectBase::AutoDetectResults
AutoDetect_PCMCIA::traverseTrees(void)
{
	DEBUG3(" AutoDetect_PCMCIA::traverseTrees()\n");
	if (!isPCMCIABrowsingPossible())
		return NOTHING_FOUND;

	// work from thePCMCIASocketsDirectory here
	// it will contain at least one and maybe multiple pcmcia_socket dirs
    // each containing a link to a device
	QString mySockName;
	FileStuff::FindInDir myPCMCIASockDir = FileStuff::FindInDir("/sys/class/pcmcia_socket",
				"pcmcia_socket*", QDir::Dirs);
	while( !( (mySockName=myPCMCIASockDir.getNextEntry()) .isEmpty()) )
	{
		QString myPCMCIASocketDevice = mySockName + "/device";
		QFileInfo mySockDev(myPCMCIASocketDevice);
		if (!mySockDev.isSymLink())
			continue;

		QString myPCMCIASocketDeviceLink = mySockDev.readLink();

		// the directory contains several entries, usually starting with 0000.
		// So let's iterate over those entries and check for Vendor IDs
		FileStuff::FindInDir myPCMCIADir = FileStuff::FindInDir(
					myPCMCIASocketDevice, "0000:*", QDir::Dirs);
		QString my0000DirName;
		while( !( (my0000DirName=myPCMCIADir.getNextEntry()) .isEmpty()) )
		{
			QString myVendorID;
			QString myDeviceID;
			QString mySubVendorID;

			QString myTemp = my0000DirName + "/vendor";
			if (!FileStuff::doesFileExist(myTemp))
				sendLog(BAD, "Could not open file: " + myTemp);
			else
				myVendorID = FileStuff::getFileFirstLine(myTemp);

			myTemp = my0000DirName + "/device";
			if (!FileStuff::doesFileExist(myTemp))
				sendLog(BAD, "Could not open file: " + myTemp);
			else
				myDeviceID = FileStuff::getFileFirstLine(myTemp);

			switch (matchIDs(myVendorID.toULong(NULL,0), myDeviceID.toULong(NULL,0)))
			{
			case DEVICE_DETECTED:
				return DEVICE_DETECTED;
				break;
			case DEVICE_ERROR:
				// FIXME: This may be the wrong action here
				// as there might be another, good, device in this list
				return DEVICE_ERROR;
				break;
			case NOTHING_FOUND:
				DEBUG5("PCMCIA card found, but not detected: '%s'\n", my0000DirName.ascii());
				DEBUG5("VendorID:    0x%4s\n", myVendorID.ascii());
				DEBUG5("DeviceID:    0x%4s\n", myDeviceID.ascii());
				myTemp = my0000DirName + "/subsystem_vendor";
				if (FileStuff::doesFileExist(myTemp))
					mySubVendorID = FileStuff::getFileFirstLine(myTemp);
				else
					mySubVendorID = "<<not present>>";
				DEBUG5("SubVendorID: 0x%4s\n", mySubVendorID.ascii());
				break;
			} // end-of-switch
		} // end-of-while
	} // end-of-while
	sendLog(BAD, "no PCMCIA device found");
	return NOTHING_FOUND;
}


//////////////////////////////////////////////////////////////////////////////
AutoDetectBase::AutoDetectResults
AutoDetect_PCMCIA::matchIDs(unsigned long aVendorID, unsigned long aProductID)
{
	DEBUG3(" AutoDetect_PCMCIA::matchIDs(0x%04lx, 0x%04lx)\n", aVendorID, aProductID);
	switch(aVendorID)
	{
	case 0x1931:
		sendLog(GOOD, "Detected Option Nozomi based card \"Nozomi\" PCMCIA card");
		theDeviceID = Device::OPTION_NOZOMI;
		return DEVICE_DETECTED;
		break;
	case 0x14e4:
	{
		sendLog(INFO, "Detected BroadCom-based PCMCIA card");
		sendLog(INFO, "Broadcom device ID: 0x" + QString::number(aProductID,16));
		if (aProductID == 0x4322)
		{
			sendLog(GOOD, "Detected BroadCom-based PCMCIA card with 4306 UART, probably Sony Ericsson GC79");
			theDeviceID = Device::BROADCOM_GPRS_EDGE;
			return DEVICE_DETECTED;
		}
		if (aProductID == 0x4333)
		{
			sendLog(GOOD, "Detected BroadCom-based PCMCIA card Option GT EDGE");
			theDeviceID = Device::BROADCOM_57K6;
			return DEVICE_DETECTED;
		}
		if (aProductID == 0x4344)
		{
			sendLog(GOOD, "Detected BroadCom-based Sony Ericsson GC86");
			theDeviceID = Device::BROADCOM_57K6;
			return DEVICE_DETECTED;
		}
		sendLog(BAD, "Unknown BroadCom device detected - might be just wifi card");
		break;
	}
	case 0x1045: // OPTI
	case 0x1033: // NEC
		sendLog(KILLING, "Detected a USB-type PCMCIA card - should have been detected already");
		return DEVICE_ERROR;
		break;
	} // end-of-switch

	// nothing we know of :-(
	return NOTHING_FOUND;
}


//////////////////////////////////////////////////////////////////////////////
bool AutoDetect_PCMCIA::findPortNames(void)
{
	DEBUG3(" AutoDetect_PCMCIA::findPortNames()\n");
	assert(theDeviceID);

	switch(theDeviceID)
	{
	case Device::BROADCOM_GPRS_EDGE:
	case Device::BROADCOM_57K6:
		return find4306PortNames();
		break;
	case Device::OPTION_NOZOMI:
		return findNozomiPortNames();
		break;
	default:
		// should not happen!!!
		assert(false);
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////
bool AutoDetect_PCMCIA::findNozomiPortNames(void)
{
	DEBUG3(" AutoDetect_PCMCIA::findNozomiPortNames()\n");
	if (!FileStuff::doesFileContainString("/proc/modules", QString("nozomi")))
	{
		sendLog(KILLING, "No kernel module for nozomi-based card loaded");
		return false;
	}
	if (FileStuff::doesFileContainString("/proc/devices", QString("nozomi")))
	{
		thePortsList.push_front("/dev/nozomi0"); // first entry = PPP
		thePortsList.push_back("/dev/nozomi2"); // last  entry = AT
		sendLog(GOOD, "found /dev/nozomi* - the 'Holtzman' version of Nozomi drivers");
	}
	else
	{
		if (FileStuff::doesFileContainString("/proc/devices", QString("noz")))
		{
			thePortsList.push_front("/dev/noz0"); // first entry = PPP
			thePortsList.push_back("/dev/noz2"); // last  entry = AT
			sendLog(GOOD, "found /dev/noz* - the 'Hardwick' version of Nozomi drivers.");
		}
		else
		{
			sendLog(BAD, "No kernel device nozomi found");
			return false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////
bool AutoDetect_PCMCIA::find4306PortNames(void)
{
	DEBUG3(" AutoDetect_PCMCIA::find4306PortNames()\n");
	// apparently, (in higher kernels than 2.6.13-rc1???), there is a direct link to the serial port:
	// klaas@jip:/> find /sys/class/pcmcia_socket/pcmcia_socket0/device/ -name '*tty*'
	//   /sys/class/pcmcia_socket/pcmcia_socket0/device/0000:03:00.1/tty:ttyS2
	//   /sys/class/pcmcia_socket/pcmcia_socket0/device/0000:03:00.1/tty:ttyS1

	FileStuff::FindInDir myDevDir = FileStuff::FindInDir(
				"/sys/class/pcmcia_socket/pcmcia_socket0/device",
				"0000:*", QDir::Dirs);
	QString my0000DirName;
	QString myFileName;
	while( !( (my0000DirName=myDevDir.getNextEntry()) .isEmpty()) )
	{
		FileStuff::FindInDir myDir = FileStuff::FindInDir(my0000DirName,
					"tty:*", QDir::Dirs);
		while( !( (myFileName=myDir.getNextEntry()) .isEmpty()) )
		{
			QString myDevName = myFileName.section(':', -1);
			thePortsList.push_back(myDevName);
			sendLog(GOOD, "Detected Broadcom serial port on /dev/" +
						myDevName);
			return true;
		}
	}
	return false;
}



//////////////////////////////////////////////////////////////////////////////
bool
AutoDetect_PCMCIA::isPCMCIABrowsingPossible(void)
{
	// check for pcmcia_core in /proc/modules (=lsmod)
	if (FileStuff::doesFileContainString("/proc/modules", QString("pcmcia_core")))
	{
		sendLog(INFO, "pcmcia_core kernel support (yenta) found");
	}
	else
	{
		sendLog(BAD, "pcmcia_core kernel support (yenta) not found");
	}
	if (FileStuff::doesFileContainString("/proc/devices", QString("pcmcia")))
	{
		sendLog(INFO, "PCMCIA kernel module found - cardctl support possible");
	}
	else
	{
		sendLog(BAD, "PCMCIA kernel module not found - enabling/disabling pccards not possible");
	}

	// check for pcmcia in /sys/class
	if (FileStuff::doesFileExist("/sys/class/pcmcia_socket"))
	{
		sendLog(INFO, "pcmcia browsing available");
		return true;
	}
	else
	{
		sendLog(BAD, "PCMCIA browsing in SysFS not available - no autodetection");
		return false;
	}
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
AutoDetectBase::AutoDetectResults
AutoDetect_Rawport::traverseTrees(void)
{
	DEBUG3(" AutoDetect_Rawport::traverseTrees()\n");
	assert(!theDevName.isEmpty());
	// let's  iterate through all known ports defined above
	// and keep a list of everything that at least can be opened
	theValidSerialPortsList.clear();
	QString myTTYName;

	DEBUG3("  Checking for ports of type /dev/%s\n", theDevName.ascii());
	FileStuff::FindInDir myDevDir = FileStuff::FindInDir("/dev",
				theDevName,
				static_cast<QDir::FilterSpec>(QDir::All | QDir::System | QDir::Hidden));
	while( !( (myTTYName=myDevDir.getNextEntry()) .isEmpty()) )
	{
		DEBUG3("   Checking properties of port %s\n", myTTYName.ascii());
		TempSerialPort mySerial;
		if (mySerial.openDev(myTTYName))
		{
			QString myMessage = "Serial port " + myTTYName + " is operational";
			sendLog(GOOD, myMessage);
			theValidSerialPortsList.push_back(myTTYName);
		}
		else
		{
			DEBUG3("     - Nothing on port %s\n", myTTYName.ascii());
		}
		mySerial.closeDev();
	}

	DEBUG3("end of traverseTrees()\n");
	if (theValidSerialPortsList.isEmpty())
	{
		sendLog(BAD, "No usable device found");
		return NOTHING_FOUND;
	}
	else
	{
		// this is a little hopeful... we only know that
		// such serial ports exists for now...
		DEBUG3("There are %zd devices in my valid list\n",theValidSerialPortsList.count());
		return DEVICE_DETECTED;
	}
}


//////////////////////////////////////////////////////////////////////////////
AutoDetectBase::AutoDetectResults
AutoDetect_Rawport::matchIDs(__attribute__ ((unused)) unsigned long aVendorID,
                             __attribute__ ((unused)) unsigned long aProductID)
{

	// nothing we know - serial ports don't have vendor or product ids :-(
	return NOTHING_FOUND;
}


// this class is used in AutoDetect_Rawport::findPortNames(void)
class s_my_pair
{
public:
	s_my_pair(void) {DEBUG1("S_MY_PAIR VOID\n");};
	s_my_pair(const QString& pn, int bc) : thePortName(pn), theBaudCode(bc) {};
	QString thePortName;
	int     theBaudCode;
};


//////////////////////////////////////////////////////////////////////////////
bool AutoDetect_Rawport::findPortNames(void)
{
	int aBaudCode;
	DEBUG3(" AutoDetect_Rawport::findPortNames()\n");

struct s_args
{
	int           aBaudCode;
	unsigned long aBaudRate;
};

// list of baud rates to try
static struct s_args myBaudRates[] =
{
//	{B4000000, 4000000},
//	{B1152000, 1152000},
//	{B576000,   576000},
//	{B460800,   460800},
//	{B230400,   230400},
	{B115200,   115200},
	{B57600,     57600},
	{B38400,     38400},
	{B19200,     19200},
	{B9600,       9600},
	{0, 0},
};


//	list<s_my_pair>  myPortList;
	QValueList<s_my_pair>  myPortList;

	// we now have a list of valid serial devices in theValidSerialPortsList
	// we need to iterate through that list to see if there's a response to AT
	QStringList::Iterator myIt;
	for ( myIt = theValidSerialPortsList.begin(); myIt != theValidSerialPortsList.end(); ++myIt )
	{
		DEBUG5(" - probing for AT on device: %s\n", (*myIt).ascii());
		int i;
		for (i=0; myBaudRates[i].aBaudCode != 0; i++)
		{
			TempSerialPort mySerial;
			DEBUG5("    - testing baud rate %ld bps\n", myBaudRates[i].aBaudRate);
			if (mySerial.openDev(*myIt, myBaudRates[i].aBaudCode) == false)
			   continue;
			Query myQueryAT("AT");
			Query::ReturnCode myResult = myQueryAT.run();
			// unknown error probably means "i did get an answer, but no echo"...
	 		if (myResult == Query::OK) // || myResult == Query::UNKNOWN_ERROR)
	 		{
	 			DEBUG3("probe successful for baud rate %ld\n", myBaudRates[i].aBaudRate);
	 			myPortList.push_back(s_my_pair(*myIt,myBaudRates[i].aBaudRate));
	 			aBaudCode = myBaudRates[i].aBaudCode;
	 			break;
	 		}
		}
	}
	if (myPortList.empty()==false)
	{
		thePortsList.push_front(myPortList.first().thePortName); // PPP = first;
		thePortsList.push_back(myPortList.last().thePortName);  // AT  = last
		theDeviceID = DeviceCapabilities::GENERIC_USER_SERIAL;
	}

	DEBUG3(" end of AutoDetect_Rawport::findPortNames()\n");
	return (myPortList.empty()==false);
}




//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
AutoDetectBase::AutoDetectResults
AutoDetect_PhoneModems::go(void)
{
	DEBUG3("AutoDetect_PhoneModems::go()\n");
	theDevName = "ttyACM*";
	AutoDetectResults myResult = AutoDetectBase::go();
	if (myResult == DEVICE_DETECTED)
	{
		theDeviceID = DeviceCapabilities::MODEM_ACM;
	}
	else
	{
		theDevName = "rfcomm*";
		myResult = AutoDetectBase::go();
		if (myResult == DEVICE_DETECTED)
		{
			theDeviceID = DeviceCapabilities::MODEM_RFCOMM;
		}
	}
	return myResult;
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
AutoDetectBase::AutoDetectResults
NonAutoDetect::matchIDs(__attribute__ ((unused)) unsigned long aVendorID,
                             __attribute__ ((unused)) unsigned long aProductID)
{
	// nothing we know - serial ports don't have vendor or product ids :-(
	return NOTHING_FOUND;
}

//////////////////////////////////////////////////////////////////////////////
AutoDetectBase::AutoDetectResults
NonAutoDetect::go(void)
{
	if (theForceAutodetection)
	{
		DEBUG2("Force Autodetection enabled - skipping device instantiation from supplied values\n");
		return NOTHING_FOUND;
	}


	if (theSuggestedDeviceType == DeviceCapabilities::UNKNOWN)
	{
		// nothing in config
		DEBUG2("No valid configuration available in config file\n");
		return NOTHING_FOUND;
	}
	else
	{
		// let's try with the supplied parameters
		theDeviceID = theSuggestedDeviceType;
		thePortsList.push_back(theSuggestedATPortName);  // AT: last  in list
		thePortsList.push_front(theSuggestedPPPPortName); // PPP: first in list
		return AutoDetectBase::go();
	}
}

