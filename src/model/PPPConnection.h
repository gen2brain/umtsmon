/* umtsmon - a program to control/monitor your UMTS card
 * Copyright (C) 2006,2007,2008  Klaas van Gend
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

#ifndef PPPCONNECTION_H_
#define PPPCONNECTION_H_

#include "Runner.h"
#include "ObserverManager.h"
#include "ConnectionInfo.h"
#include <qstring.h>
#include <qstringlist.h>
#include <qthread.h>

// forward declarations
class Profile;
class DeviceCapabilities;

/** implement this interface and attach yourself to the appropriate
 *  Connection if you want to hear when the ConnectionState changes
 */
class ConnectionObserverInterface
{
public:
	enum ConnectionState
	{
		STOPPED,
		STARTING,
		RUNNING,
		NOT_OURS_RUNNING,
		ERROR
	};

	/** implement this and attach to the Connection to get info
	 *  about state changes of the Connection
	 */
	virtual void newConnectionState(ConnectionState aNewState) = 0;
	
	/// required but not used: virtual destructor
	virtual ~ConnectionObserverInterface() {};
};

/** abstract base class for Connections
 *  two derived classes exist: PPPConnection and HSOConnection
 */
class ConnectionBaseClass :  
		public ObserverManager<ConnectionObserverInterface>
{
public:
	/// constructor
	ConnectionBaseClass(const ConnectionBaseClass& aRef);
	 
	/// destructor
	virtual ~ConnectionBaseClass(void);
	
	/** starts the Connection
	 *  @returns true if succesful
	 */
	virtual bool startConnection(void) = 0;
	
	/// stops the Connection, does nothing if there is none
	virtual void stopConnection(void) = 0;

	/// @returns a string containing the pppd version, e.g. "2.4.4"
	virtual const QString& getPPPVersion(void) = 0;

	bool isConnectionStarting();
	
	virtual bool hasErrors() {return thePPPDStdErr.count()!=0;};
	
	/// stringlist with all the stdout of the most recent PPP run
	QStringList thePPPDStdOut;
	/// stringlist with all the stderr of the most recent PPP run
	QStringList thePPPDStdErr;

	/** returns true if a Connection exists
	 *  in the case of ppp, it is not very sharp - it will return true if ANY
	 *  pppd is running, not just if *our* pppd is running
	 */
	virtual bool isConnectionActive(void) = 0;

protected:
	/** implementation of the pure virtual member in ObserverManager;
	 *  the implementation should perform the notification of the observer pointed to.
	 */
	virtual void notifyObserver(ConnectionObserverInterface* anObserver) const;

	/** cache the serial port to do the PPP connection on
	 *  Note that we will still use the AT port for the dialling AT commands
	 */	
	QString theSerialPortName;

protected:
	void setNewConnectionState(ConnectionObserverInterface::ConnectionState aNewState);

	/** runs the 'chat', i.e. a series of queries to prepare the modem
	 *  for the connection 
	 *  it will use the currently setup serialport - if that 
	 *  happens to be a TempSerial, that's not a problem :-)
	 *  @param aProfile   the profile to take data from
	 *  @returns true if successful
	 */
	bool chat(const Profile& aProfile) const;
	
	/** creates and runs the ATD (dial string)
	 * 
	 * @param aProfile
	 * @param myConnectStr
	 * @return
	 */
	virtual bool dial(const Profile& aProfile, const QString& myConnectStr) const = 0;
	
	/** sends an actual line of the chat and waits for the answers
	 * @param aChatString the data to send
	 * @returns true if reply was "OK".
	 */
	bool sendChatString(const QString& aChatString) const;
	
private:
	/** assign operator is private and not implemented
	 *  to prevent duplication of this class instance (EffC++ item27)
	 *  DO NOT IMPLEMENT THIS MEMBER
	 */
	ConnectionBaseClass& operator=(const ConnectionBaseClass& aRef);
};


// ##########################################################################
		
/** The DummyConnection class is a dummy, empty connection class
 */
class DummyConnection :  
		public ConnectionBaseClass
{
public:

	/// constructor
	DummyConnection(void);

	/// destructor
	virtual ~DummyConnection(void);
	
	/// inherited & overriden from ConnectionBaseClass
	virtual bool startConnection(void);

	/// inherited & overriden from ConnectionBaseClass
	virtual void stopConnection(void);

	/** @returns a string containing the pppd version...
	 *  for HSOConnection, it is "HSO device - no pppd required"  
	 */
	virtual const QString& getPPPVersion(void);

	/// inherited & overriden from ConnectionBaseClass
	virtual bool isConnectionActive(void);

private:
	/// inherited & overriden from ConnectionBaseClass
	virtual bool dial(const Profile& aProfile, const QString& myConnectStr) const;
};

// ##########################################################################
		
/** The PPPConnection class initiates, monitors and manages the PPP daemon and
 *  the connection it makes. 
 */
class PPPConnection :  
		public ConnectionBaseClass
{
public:

	/// constructor
	PPPConnection(const ConnectionBaseClass& aRef);

	/// destructor
	virtual ~PPPConnection(void);
	
	/// inherited & overriden from ConnectionBaseClass
	virtual bool startConnection(void);

	/// inherited & overriden from ConnectionBaseClass
	virtual void stopConnection(void);

	/// @returns a string containing the pppd version, e.g. "2.4.4"
	virtual const QString& getPPPVersion(void)
		{ return thePPPVersionNumber; };

	ConnectionInfoInhibitor* theCIInhibitorPtr;

	/// inherited & overriden from ConnectionBaseClass
	virtual bool isConnectionActive(void);

private:
	/// true if AT and PPP serial ports are the same device
	bool hasOnlyOnePort;

	/// a string containing the PPP version
	QString thePPPVersionNumber;
	
	/** assembles a QStringList with all arguments to be given to PPPD
	 *  taking input from various sources, including the profile.
	 *  @param aProfile   the profile to take data from
	 *  @returns a QStringList with arguments
	 */
	QStringList assembleArgumentList(const Profile& aProfile) const;

protected:
	/// inherited & overriden from ConnectionBaseClass
	virtual bool dial(const Profile& aProfile, const QString& myConnectStr) const;
	
private:
	/** this nested class creates the separate execution thread that
	 *  will fork/exec into ppp - to make sure that the main thread
	 *  keeps running
	 *  FIXME: This might move into Runner later...
	 */ 
	class PPPThread : public QThread
	{
	public:
		PPPThread(const QStringList& anArgumentList);
		virtual ~PPPThread() {DEBUG2("~PPPThread()\n"); QThread::wait(); };
		
		/** I re-implemented this one because I suspect the QT one
		 *  (called finished())  to be misbehaving
		 *  @returns true if the thread has ended or hasn't started yet
		 */
		virtual bool isDone()	{ return !isRunning;};
		
		/// reimplemented from QThread to ensure that isDone is working
		virtual void start()	{isRunning=true; QThread::start();};
		
		const QStringList& getStdOut() const {return theRunner.getStdOutStringList();};
		const QStringList& getStdErr() const {return theRunner.getStdErrStringList();};

	protected:
		QStringList theArgumentList;

		/// this member contains the actual thread code
		virtual void run();
		
		int theReturnValue;
		bool isRunning;
		Runner theRunner;
		
		/// as long as thePPPThread exists, no info updates
		ConnectionInfoInhibitor theCII;
	}; // end-of-PPPThread declaration
	
	PPPThread* thePPPThreadPtr;
};

// ##########################################################################
		
/** The HSOConnection class initiates, monitors and manages connections
 *  made by HSO devices 
 */
class HSOConnection :  
		public ConnectionBaseClass
{
public:

	/// constructor
	HSOConnection(const ConnectionBaseClass& aRef);

	/// destructor
	virtual ~HSOConnection(void);
	
	/// inherited & overriden from ConnectionBaseClass
	virtual bool startConnection(void);

	/// inherited & overriden from ConnectionBaseClass
	virtual void stopConnection(void);

	/** @returns a string containing the pppd version...
	 *  for HSOConnection, it is "HSO device - no pppd required"  
	 */
	virtual const QString& getPPPVersion(void);

	/// inherited & overriden from ConnectionBaseClass
	virtual bool isConnectionActive(void);

protected:
	/// inherited & overriden from ConnectionBaseClass
	virtual bool dial(const Profile& aProfile, const QString& myConnectStr) const;
	
private:
	enum ResultsOWANCALL 
	{
		OWAN_DISCONNECTED = 0,
		OWAN_CONNECTED = 1,
		OWAN_INSETUP = 2,
		OWAN_FAILED = 3
	};		

	/** runs a AT_OWANCALL? query
	 * 
	 * @return the result of the query, which is of the enum ResultsOWANCALL type
	 */
	ResultsOWANCALL runOWANCALL(void) const;

	/** runs all networking cruft - ifconfig, resolv.conf, route
	 * 
	 * @return true if successful
	 */
	bool startNetworking(const Profile& aProfile);

	/// stops all networking cruft - ifconfig, resolv.conf
	void stopNetworking(void);
};

#endif /*PPPCONNECTION_H_*/
