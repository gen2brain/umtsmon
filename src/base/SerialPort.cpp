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


#include "umtsmon_global.h"
#include "SerialPort.h"
#include "Query.h"
#include "Popup.h"

#include <stdio.h>
#include <termio.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>

bool SerialPort::closeDev()
{
    DEBUG3("SerialPort::closeDev() for FD %d\n", theComFD);
	if (theComFD != -1)
		close(theComFD);
	theComFD=-1;
	return true;
}

void SerialPort::doSmallSleep(unsigned long microsecs)
{
	// the below statement was taken from comgt:
	//   I use select() 'cause CX/UX 6.2 doesn't have usleep().
	//   On Linux, usleep() uses select() anyway.
	struct timeval timeout;
	timeout.tv_sec=microsecs/1000000L;
	timeout.tv_usec=microsecs-(timeout.tv_sec*1000000L);
	// select() is notorious for returning early. That's why I specifically check&retry
	while (timeout.tv_sec + timeout.tv_usec != 0)
	{
	  	DEBUG6("dosmallsleep of %ld s, %ld us\n", timeout.tv_sec, timeout.tv_usec);
	  	select(1,0,0,0,&timeout);
	}  
}

bool SerialPort::resetDev(void)
{
	if (closeDev()==false)
		return false;
	sleep(1);
	if (openDev(theDevName, thePortSpeed, theSleepTime)==false)
		return false;
	int ch=1;
	while (ch > 0)
	{
		ch = getOneByte();
		DEBUG6("read after reset: '%c' %d\n", ch, ch);
	}
	return true;
}

bool SerialPort::openDev(const QString& aDeviceName, int aSpeed, long aSleepTime)
{
  if ((theComFD = open(aDeviceName.ascii(), 
                       O_RDWR|O_EXCL|O_NONBLOCK|O_NOCTTY)) < 0) 
  {
	QString myErrorMsg = QString("Serial::openDev-open(\"%1\") failed").arg(aDeviceName); 
	perror(myErrorMsg.ascii());
	theComFD = -1;
	return false;
  }

  theSleepTime = aSleepTime;
  thePortSpeed = aSpeed;
  theDevName = aDeviceName;

  // for security:
  // let's check first if this is actually a character device node.
  struct stat myStatBuf;
  if (fstat(theComFD, &myStatBuf) != 0)
  {
	QString myErrorMsg = QString("Serial::openDev-fstat(\"%1\") failed").arg(aDeviceName); 
	perror(myErrorMsg.ascii());
	theComFD = -1;
	return false;
  }
  if (!S_ISCHR(myStatBuf.st_mode))
  {
	Popup::Critical(QObject::tr("SECURITY ERROR: You are trying to open something that is not a character device\n" APPNAME " will close now."));
	return false;
  }

  struct termio myTermIo;
  if (ioctl (theComFD, TCGETA, &myTermIo) < 0) 
  {
	QString myErrorMsg = QString("Serial::openDev-ioctl(\"%1\") get termio buf failed").arg(aDeviceName); 
	perror(myErrorMsg.ascii());
	theComFD = -1;
	return false;
  }
  myTermIo.c_iflag &= ~(IGNCR | ICRNL | IGNBRK | IUCLC | INPCK | IXON | IXANY | IGNPAR );
  myTermIo.c_oflag &= ~(OPOST | OLCUC | OCRNL | ONLCR | ONLRET);
  myTermIo.c_lflag &= ~(ICANON | XCASE | ECHO | ECHOE | ECHONL);
  myTermIo.c_lflag |= ISIG | IEXTEN | ECHOK | ECHOKE | ECHOCTL;
  myTermIo.c_cc[VMIN] = 1;
  myTermIo.c_cc[VTIME] = 5;
  myTermIo.c_cc[VEOF] = 1;

  myTermIo.c_cflag &= ~(CBAUD | CSIZE | CSTOPB | CLOCAL | PARENB);
  myTermIo.c_cflag |= CLOCAL | HUPCL;
  myTermIo.c_cflag |= (aSpeed | CS8 | CREAD );
  if (ioctl(theComFD, TCSETA, &myTermIo) < 0) 
  {
		QString myErrorMsg = QString("Serial::openDev-ioctl(\"%1\") set termio failed").arg(aDeviceName); 
		perror(myErrorMsg.ascii());
	theComFD = -1;
	return false;
  }

  doSmallSleep(200000L); /* Wait a bit (DTR raise) */
  DEBUG3("Serial::openDev('%s') as FD %d - Serial instance %p\n", aDeviceName.ascii(), theComFD, this);

  return true;
}




/* Write a null-terminated string to communication device */
bool SerialPort::writeDev(const QString& aString) 
{
	const char* myStringPtr = aString.ascii();
	long myLen = aString.length();
	int myTryCount = 0;
	
	if (theComFD == -1)
		return false;
	
	while (myLen > 0) 
	{
		int myResult =  write(theComFD, myStringPtr, myLen);
		if (myResult <= 0)
		{
			if (++myTryCount > 10)
			{
				closeDev();
				perror("Could not write to serial device");
				Popup::Critical(QObject::tr("Could not write to device (anymore)"));
				return false;
			}
			DEBUG4("write '%s' failed for the %d time, re = %d\n. Trying again.", 
					myStringPtr, myTryCount, myResult);
			doSmallSleep(10000);
			myResult = 0;
		}
		if (myResult > myLen) 
			myResult = myLen;	// (catch possible errors in write function)
		
		myLen -= myResult;
		myStringPtr += myResult;
	}
	doSmallSleep(20000);
	return true;
}




/** Gets a single byte from comm. device.
 *  @returns byte if available, -1 if none, 0 if byte should be there but isn't
 */
int SerialPort::getOneByte(void) 
{
	
	if (theComFD == -1)
	{
		return -1;
	}

	char  myChar = 0;
	struct timeval myTimeOut;
	myTimeOut.tv_sec  = 0L;
	myTimeOut.tv_usec = theSleepTime;
	fd_set myFDset;
	FD_ZERO(&myFDset);
	FD_SET(theComFD, &myFDset);
	int myResult = select(theComFD+1, &myFDset, NULL, NULL, &myTimeOut);
	if (myResult == -1)
	{
		perror("SerialPort::getOneByte::select");
		myChar = 0;
	}
	if (myResult != 0)
	{
		myResult=read(theComFD, &myChar, 1);
		if (myResult != 1)
		{
		  // nothing read
		  myChar=0;
		}
	}
	return myChar;
}


//-----------------------------------------------------------------
#define MAXTHESTRING 511

static char theString[MAXTHESTRING];

static char* convertToReadable(const QString& aString)
{
	unsigned int myDestPos   = 0;
	int mySourcePos = 0;
	for (; mySourcePos < aString.length();)
	{
		theString[myDestPos] = aString[mySourcePos].toAscii();
		if (theString[myDestPos] == '\r')
		{
			theString[myDestPos] = '\\';
			myDestPos ++;
			theString[myDestPos] = 'r';
		}
		if (theString[myDestPos] == '\n')
		{
			theString[myDestPos] = '\\';
			myDestPos ++;
			theString[myDestPos] = 'n';
		}
		if (theString[myDestPos] == '\0')
		{
			theString[myDestPos] = '\\';
			myDestPos ++;
			theString[myDestPos] = '0';
		}
		mySourcePos ++;
		myDestPos++;
		if (myDestPos > MAXTHESTRING-1)
			break;
	}
	theString[myDestPos]=0;
	return theString;
}


void SimulatedSerialPort::addSerialStatement(const QString& aQueryString, 
			const QString& aResponse)
{
	keydata* myPairPtr = new keydata;
	myPairPtr->key  = "\r"+aQueryString+"\r";
	myPairPtr->data = aQueryString+"\r\n" + aResponse+"\r\n";
	theQueryList.push_back(myPairPtr);
}


bool SimulatedSerialPort::openDev(
		__attribute__ ((unused)) const QString& aDevName, 
		__attribute__ ((unused)) int aPortSpeed, 
		__attribute__ ((unused)) long aSleepTime)
{
	theDevName = aDevName;
	return true;
}

bool SimulatedSerialPort::closeDev()
{
	return true;
}

bool SimulatedSerialPort::writeDev(const QString& aString)
{
	// let's look up the aString
	for (theCurrentItem = theQueryList.begin(); theCurrentItem != theQueryList.end(); theCurrentItem++)
		if ((*theCurrentItem)->key==aString)
			break;
	// did we find it?
	if (theCurrentItem == theQueryList.end())
	{
		// no :-(
		DEBUG2("SimulatedSerialPort::writeDev : '%s' not found\n", convertToReadable(aString));
		theItemIndex = -1;
		isAllSatisfied = false;
		return false;
	}
	
	DEBUG2("SimulatedSerialPort::writeDev : query \"%s\" found,\n", 
		convertToReadable((*theCurrentItem)->key));  
	DEBUG2("   answer will be: '%s'\n", convertToReadable((*theCurrentItem)->data));
	theItemIndex = 0;
	return true;
}

int  SimulatedSerialPort::getOneByte(void)
{
	if (theCurrentItem == theQueryList.end() || theItemIndex==-1)
	{
		return -1;
	}
	
	QChar myByte = (*theCurrentItem)->data[theItemIndex];
	theItemIndex++;
	
	if (theItemIndex > static_cast<signed int>((*theCurrentItem)->data.length()))
	{
		theQueryList.erase(theCurrentItem);
		theCurrentItem = theQueryList.end();
		theItemIndex = -1;
	}
	return myByte.toAscii();
}


//-----------------------------------------------------------------
LongSerialPort::~LongSerialPort() 
{
	DEBUG3("LongSerialPort::~LongSerialPort()\n");
	closeDev();
	Query::setSerial(NULL);
}

bool LongSerialPort::openDev(const QString& aDeviceName, int aSpeed, long aSleepTime)
{
	if (SerialPort::openDev(aDeviceName, aSpeed, aSleepTime))
	{
		Query::setSerial(this);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------
TempSerialPort::TempSerialPort()
{
	DEBUG3("TempSerialPort::TempSerialPort()\n");
	// save the current serial port of Query, so we can restore it later
	theOldPtr = Query::setSerial(this);
}

TempSerialPort::~TempSerialPort() 
{
	DEBUG3("TempSerialPort::~TempSerialPort()\n");
	closeDev();
	// let's reset the serialport pointer in Query
	// we should get a pointer to us back
	assert(Query::setSerial(theOldPtr) == this); 
}
