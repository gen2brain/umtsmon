/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2006,2007,2008  Klaas van Gend
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

#ifndef SERIALPORT_CLASS
#define SERIALPORT_CLASS

// required for port speeds
#include <termio.h>

#include <qstring.h>
#include <list>

/** The SerialPort class sets up, maintains and reads/writes the serial port
 *  connection. Code derived in part from Paul Hardwick's 'comgt' 
 *  application.
 * 
 *  NOTE: This SerialPort class cannot multiplex with PPP data streams
 *  It will thus work for any single serial port modems.
 *  The modem.cpp file in kppp details how to work around that.
 * 
 *  in general, the SerialPort class will be created and kept by Device
 *  and Device will use the static member of Query to install the SerialPort 
 *  to Query.
 */
class SerialPort
{
public:
	SerialPort()  : theComFD(-1) {};
	virtual ~SerialPort() {};

	/** open a serial device for communication. 
	 *  If opening successful, it will also register itself for the Queryclass 
	 *  @param  aDevName    Device name to open, e.g. /dev/tts/0
	 *  @param  aPortSpeed  Speed to operate the serial port (def=115200 baud)
	 *  @returns true if successful, false if fails recoverably
	 */
	virtual bool openDev(const QString& aDevName, int aPortSpeed = B115200, long aSleepTime = 20000);
	
	/** closes the serial port device
	 *  and unregisters itself in the Query class
	 */
	virtual bool closeDev();

	/** resets the serial port device
	 *  needed for single port serial devices after
	 *  pppd has closed its connection
	 */
	virtual bool resetDev(void);

	/** Write a string to the device
	 *  @param   aString  string to send to device, nothing will be added.
	 *  @returns true if successful
	 */
	virtual bool writeDev(const QString& aString);

	/** Gets a single byte from comm. device.  
	* Return -1 if none avail. 
	* Return 0 if should be available but there is none
	*/
	virtual int getOneByte(void);

private:
	/// Communication file descriptor.
	int theComFD; 

	/// perform a small sleep, in microseconds
	void doSmallSleep(unsigned long aNumberOfMicroSecs);

	/// time to wait before deciding no more characters are forthcoming
	long theSleepTime;
	
	int thePortSpeed;

protected:
	/// contains the device name - once openDev() has been called
	QString theDevName;
	
private:
	virtual void aDummy(void) = 0;
};


/** the SimulatedSerialPort class can be used to simulate the behavior of the
 *  regular SerialPort class - use it to test Queries and such
 * 
 *  you'll have to feed it first, though - through the ### member.
 */
class SimulatedSerialPort : public SerialPort
{
public:
	SimulatedSerialPort() : theItemIndex(-1), isAllSatisfied(true) {};
	virtual ~SimulatedSerialPort() {};
	
	virtual bool openDev(const QString& aDevName, int aPortSpeed = B115200, long aSleepTime = 20000);
	virtual bool closeDev();
	virtual bool writeDev(const QString& aString);
	virtual int getOneByte(void);

	// specific for this class
	void addSerialStatement(const QString& aQueryString, const QString& aResponse);
	bool isAllUsed(void) const  {return theQueryList.empty();};
	bool wasAllSatisfied(void) const {return isAllSatisfied;};
	
private:
	struct keydata {QString key; QString data;};
	typedef std::list<keydata*>  QueryList;
	QueryList  theQueryList;

	QueryList::iterator theCurrentItem;
	int theItemIndex;
	
	bool isAllSatisfied;
	
	virtual void aDummy(void) {};
};
 
/** This class is the regular long-term use of SerialPort.
 */
class LongSerialPort : public SerialPort
{
public:
	LongSerialPort() {}; 
	virtual ~LongSerialPort();
	virtual bool openDev(const QString& aDevName, int aPortSpeed = B115200, long aSleepTime = 20000);
	
private:
	SerialPort* theOldPtr;
	virtual void aDummy(void) {};
};

/** This class is for use by PPPConnection. This TempSerialPort will open another
 *  SerialPort class, store the current port set in Query and return that state upon
 *  destruction. 
 *  Note that this modification of Query already is done on creation - not on opening the device!
 */
class TempSerialPort : public SerialPort
{
public:
	TempSerialPort(); 
	virtual ~TempSerialPort(); 
	
private:
	SerialPort* theOldPtr;
	virtual void aDummy(void) {};
};
 

#endif // defined SERIALPORT_CLASS
