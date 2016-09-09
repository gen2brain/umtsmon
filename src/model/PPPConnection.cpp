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

#include "umtsmon_global.h"

#include "PPPConnection.h"
#include "Query.h"
#include "Runner.h"
#include "Profile.h"
#include "TheDeviceManagerSingleton.h"
#include "TheSettingsSingleton.h"
#include "Popup.h"
#include "SerialPort.h"
#include "FileStuff.h"

#include <qregexp.h>
#include <qstring.h>
#include <sys/types.h>
#include <signal.h>
#include <cstdlib>

// the actual definition of thePPPDState
static ConnectionObserverInterface::ConnectionState  thePPPDState = ConnectionObserverInterface::STOPPED;
static pid_t theRunningPPPDPID = -1;


// the function to be called by atexit
// to make sure the PPPD daemon is shut down
static void run_at_exit(void)
{
	DEBUG5("PPPConnection run_at_exit() called\n");
	Runner myRunner;
	int myPID = myRunner.doesPPPDexist();
	if (myPID!=-1)
	{
		if (thePPPDState != ConnectionObserverInterface::NOT_OURS_RUNNING)
		{
			// thePPPDState is either RUNNING or STARTING
			DEBUG3("Killing PPPD - process %d\n", myPID);
			kill(myPID, SIGTERM);
			theRunningPPPDPID = -1;
		}
	}
}

static void run_at_signal(__attribute__ ((unused)) int aSignalNumber)
{
	run_at_exit();
	exit(1);
}


ConnectionBaseClass::ConnectionBaseClass(const ConnectionBaseClass& aRef)
	: ObserverManager<ConnectionObserverInterface>(aRef)
{
	atexit(run_at_exit);
	signal(SIGINT, run_at_signal);
	signal(SIGHUP, run_at_signal);

	// let's assume the worst
	setNewConnectionState(ConnectionObserverInterface::STOPPED);
}

ConnectionBaseClass::~ConnectionBaseClass(void)
{
	DEBUG4("ConnectionBaseClass::~ConnectionBaseClass(void)\n");
	// nothing else to do here
}


bool ConnectionBaseClass::chat(const Profile& aProfile) const
{
	// have to declare all variables here because I want to use goto...
	bool myResult = false;
	unsigned int myCID;
	QString myConnectStr;
	QString myAPN;
	
	DEBUG3("ConnectionBaseClass::chat() start\n");

	// AT&F: reset modem to default profile
	// FIXME: if this fails, we should do pccardctl eject and insert and retry
	if (sendChatString("AT&F")==false)
		goto bad_end;

	// ATE1: enable echo
	// FIXME: if this fails, we should do pccardctl eject and insert and retry
	if (sendChatString("ATE1")==false)
		goto bad_end;

	// build up the connectionstring
	// like "AT+CGDCONT=1,\"IP\",\"office.vodafone.nl\""
	//
	// security check: anAPN should not contain any quotes or spaces
	myAPN = aProfile.getAPN();
	if ( myAPN.contains("\"") || myAPN.contains("\"") ||
	     myAPN.contains("'") || myAPN.contains(" ") )
	{
		Popup::Warning(QObject::tr("Your APN contains illegal characters.\nPlease go back to your Profile page and change the APN."));
		DEBUG1("ERROR: APN contains illegal character\n");
		goto bad_end;
	}
	if (myAPN.isEmpty())
	{
		Popup::WarningWithMemory("NoAPNMsg", QObject::tr("Warning: You did not specify an 'APN' in your profile.\nThis may work ok, it may not - depending on your network operator."));
	}

	// myCID needs to be adjustable: different number for cell phones on USB or BT
	myCID = aProfile.getCID();
	myConnectStr = "AT+CGDCONT=";
		myConnectStr += QString::number(myCID);
		myConnectStr += ",\"IP\",\"" + myAPN + "\"";

	if (sendChatString(myConnectStr)==false)
		goto bad_end;

	// and call the actual dial
	myResult = dial(aProfile, myConnectStr);

 bad_end:
	DEBUG3("ConnectionBaseClass::chat() end with result: %d\n", myResult);
	return myResult;
}

bool ConnectionBaseClass::isConnectionStarting(void)
{
	// if PPP is running, it is not starting anymore :-)
	if (isConnectionActive())
		return false;
	return thePPPDState == ConnectionObserverInterface::STARTING;
}

void ConnectionBaseClass::notifyObserver(ConnectionObserverInterface* myObserverPtr) const
{
	myObserverPtr->newConnectionState(thePPPDState);
}

bool 
ConnectionBaseClass::sendChatString(const QString& aChatString) const
{
	Query myQuery(aChatString);
	if (myQuery.runUntilDone() != Query::OK)
		return false;
	return true;
}


void ConnectionBaseClass::setNewConnectionState(ConnectionObserverInterface::ConnectionState aNewState)
{
	// only go through the burden of notify if state really changed
	if (aNewState != thePPPDState)
	{
		DEBUG3("ConnectionBaseClass::setNewPPPState state %d to %d\n", thePPPDState, aNewState);
		thePPPDState = aNewState;
		ConnectionObserverInterface* myObserverPtr = firstObserver();
		while (myObserverPtr != NULL)
		{
			notifyObserver(myObserverPtr);
			myObserverPtr = nextObserver();
		}
		
		if (aNewState == ConnectionObserverInterface::RUNNING)
		{
			// make sure to mark this profile successful for the future
			Profile myProfile;
			myProfile.setWasConnectionSuccess(true);
		}
	}
}

//############################################################################
static const QString theStaticDummyVersion(QObject::tr("No Connection Class defined yet"));

DummyConnection::DummyConnection(void)
	: ConnectionBaseClass(*this)
{
	DEBUG4("DummyConnection::DummyConnection(void)\n");
	isConnectionActive();
}

DummyConnection::~DummyConnection(void)
{
	DEBUG4("DummyConnection::~DummyConnection(void)\n");
	stopConnection();
}

bool DummyConnection::dial(
		__attribute__ ((unused)) const Profile& aProfile, 
		__attribute__ ((unused)) const QString& myConnectStr) const
{
	return false;
}

const QString& DummyConnection::getPPPVersion(void)
{
	return theStaticDummyVersion;
}

bool DummyConnection::isConnectionActive(void)
{
	setNewConnectionState(ConnectionObserverInterface::STOPPED);
	return false;
}

bool DummyConnection::startConnection(void)
{
	return false;
}

void DummyConnection::stopConnection(void)
{
}


//############################################################################


PPPConnection::PPPConnection(const ConnectionBaseClass& aRef)
		:	ConnectionBaseClass(aRef),
			theCIInhibitorPtr(NULL),
			hasOnlyOnePort(false),
			thePPPThreadPtr(NULL)
{
	// for some compatibility stuff, we need the exact version number of pppd
	Runner myRunner;
	myRunner.runCommand(Runner::PPPD, "--version");
	QString myTotalVersionNr = myRunner.getStdErrStringList()[0];
	int myFirst = myTotalVersionNr.find(QRegExp("\\d"), 0);
	int myLast  = myTotalVersionNr.find(QRegExp("\\d"), -1);
	thePPPVersionNumber = myTotalVersionNr.mid(myFirst,myLast-myFirst+1);
	DEBUG4("pppd version string '%s' results in '%s'.\n",
			myTotalVersionNr.ascii(),
			thePPPVersionNumber.ascii());

	isConnectionActive();
}


PPPConnection::~PPPConnection(void)
{
	DEBUG4("PPPConnection::~PPPConnection(void)\n");
	stopConnection();
}


void
PPPConnection::stopConnection(void)
{
	run_at_exit();
}


bool
PPPConnection::isConnectionActive(void)
{
	// let's try to get a PID for a running pppd
	// without reading through all of /proc every time
	pid_t myPID = -1;
	// if the running pppd process ID still exists,
	// we do not need to check for a *new* pppd pid
	if (theRunningPPPDPID>0
		&& FileStuff::doesFileExist("/proc/"+QString::number(theRunningPPPDPID)+"/stat"))
	{
		myPID = theRunningPPPDPID;
	}
	else
	{
        // if there is a routing table set up for pppX, a pppd is running
        if (FileStuff::doesFileContainString("/proc/net/route", QString("ppp"))
        		&& thePPPDState!=ConnectionObserverInterface::STOPPED)
        {
			myPID = Runner::doesPPPDexist();
        }
	}

	switch (thePPPDState)
	{
	case ConnectionObserverInterface::ERROR:
		setNewConnectionState(ConnectionObserverInterface::STOPPED);
		break;
	case ConnectionObserverInterface::STOPPED:
		if (myPID!=-1)
		{
			DEBUG2("PPPConnection::isPPPConnected someone else started a pppd\n");
			setNewConnectionState(ConnectionObserverInterface::NOT_OURS_RUNNING);
		}
		if (theRunningPPPDPID!=-1)
		{
			// shouldn't happen
			DEBUG1("PPPConnection::isPPPConnected state changed to STOPPED "
			       "without updating theRunningPPPDPID\n");
			assert(false);
			theRunningPPPDPID=-1;
		}
		break;
	case ConnectionObserverInterface::STARTING:
		if (thePPPThreadPtr == NULL)
		{
			// something went wrong ?!?
			setNewConnectionState(ConnectionObserverInterface::STOPPED);
			thePPPDStdErr.push_back(APPNAME " internal Error: not thePPPThreadPtr");
			break;
		}
		if (thePPPThreadPtr->isDone())
		{
			DEBUG2("isDone is true\n");
			// so, was it successful or not?
			if (myPID == theRunningPPPDPID)
			{
				// euh? finished *and* still running?
				// that means we have a zombie
				//    - process is dead, we have to reap it...
				myPID = -1;
			}
			if (myPID == -1)
			{
				theRunningPPPDPID=-1;
				thePPPDStdOut = thePPPThreadPtr->getStdOut();
				thePPPDStdErr = thePPPThreadPtr->getStdErr();
				if(thePPPVerbosity > 1)
				{
					QStringList::Iterator it;
					DEBUG4("*****output of PPP to stdout\n");
					for (it = thePPPDStdOut.begin(); it != thePPPDStdOut.end(); ++it )
					{
				        DEBUG4("* %s\n", (*it).ascii());
					}
					DEBUG4("*****end-of-output\n");
					DEBUG4("*****output of PPP to stderr\n");
					for (it = thePPPDStdErr.begin(); it != thePPPDStdErr.end(); ++it )
					{
				        DEBUG4("* %s\n", (*it).ascii());
					}
					DEBUG4("*****end-of-output\n");
				}
				setNewConnectionState(ConnectionObserverInterface::ERROR);
			}
			else
			{
				theRunningPPPDPID = myPID;
				setNewConnectionState(ConnectionObserverInterface::RUNNING);
			}
			DEBUG3("thread end - wait and cleanup\n");
			delete thePPPThreadPtr;
			thePPPThreadPtr = NULL;
		}
		else
		{
			DEBUG3("PPPDThread still running\n");
		}
		break;
	case ConnectionObserverInterface::RUNNING:
		if (myPID==-1 || myPID!=theRunningPPPDPID)
		{
			DEBUG2("Our PPPD was killed by someone\n");
			theRunningPPPDPID=-1;
			setNewConnectionState(ConnectionObserverInterface::STOPPED);
		}
		break;
	case ConnectionObserverInterface::NOT_OURS_RUNNING:
		if (myPID==-1)
		{
			DEBUG2("Someone else's PPPD was killed\n");
			setNewConnectionState(ConnectionObserverInterface::STOPPED);
		}
		break;
	}

	DEBUG5("The PPPDState is %d\n", thePPPDState);

	// we need the ConnectionInfoInhibitor here if we have only one serial port
	// and we're not in 'stopped'.
	// note that PPPThread and startConnection also have CIIs to make sure that we
	// don't run into simulatenous accessess.
	if (hasOnlyOnePort)
	{
		if (thePPPDState==ConnectionObserverInterface::STOPPED)
		{
			if (theCIInhibitorPtr!=NULL)
			{
				TheDeviceManagerSingleton::resetDevice();
				delete theCIInhibitorPtr;
				theCIInhibitorPtr=NULL;
			}
		}
		else
		{
			if (theCIInhibitorPtr==NULL)
			{
				theCIInhibitorPtr = new ConnectionInfoInhibitor;
			}
		}
	}

	if (thePPPDState==ConnectionObserverInterface::STOPPED
			|| thePPPDState==ConnectionObserverInterface::STARTING)
		return false;
	else
		return true;
}


bool
PPPConnection::startConnection()
{
	// just for this startup, create a Profile - which should be the currently selected profile
	Profile myProfile;
 
	theSerialPortName = TheDeviceManagerSingleton::getDeviceCapsRef().getPPPSerialPortName();
	assert(!theSerialPortName.isEmpty());

	// check if AT and PPP are the same device
	if (TheDeviceManagerSingleton::getDeviceCapsRef().getATSerialPortName() == theSerialPortName)
		hasOnlyOnePort = true;
	else
		hasOnlyOnePort = false;

	// just as a precaution, some sanity checks.
	// we will be checking if a PPP connection is already running.
	// This shouldn't be the case - that's why there is an assert(false).
	// however, if this happens in a release build, just return false.
	DEBUG3("PPPConnection::startPPP entry\n");
	if (isConnectionActive())
	{
		// if happens in debug build: make a lot of noise
		// if happens in release build: just error out of here
		DEBUG2("PPPConnection::startPPP, PPP is already connected ?!?\n");
		stopConnection();
		sleep(1);
		// this shouldn't happen - crash big time on debug builds
		assert(isConnectionActive()==false);
		// and a fall-through prevention for release builds
		if (isConnectionActive())
			return false;
	}

	// remove everything that might be left from previous connections
	assert(thePPPThreadPtr==NULL);
	thePPPDStdOut.clear();
	thePPPDStdErr.clear();


	// from now on until the exit of this member, prevent
	// ConnectionInfo from interfering with the AT serial port
	setNewConnectionState(ConnectionObserverInterface::STARTING);
	assert(theCIInhibitorPtr == NULL);
	if (hasOnlyOnePort)
		theCIInhibitorPtr = new ConnectionInfoInhibitor();

	// we cannot connect if this is not set
	if ( theSerialPortName .isEmpty())
	{
		DEBUG2("PPPConnection::startPPP - no PPP device ?!?\n");
		return false;
	}

	// let's create a new serial port, that will temporarily
	// (as long as this temp class exists) replace the original serial
	// port for queries. As this temp object will be deleted upon exit of this
	// member, the original state will be returned automatically
	TempSerialPort myTempPort;
	if (myTempPort.openDev(theSerialPortName)==false)
	{
		DEBUG2("PPPConnection::startPPP - cannot open port '%s' ?!?\n", theSerialPortName.ascii());
		return false;
	}

	// let's do the CHATting  :-) 
	//     (using the temp serial instance)
	if (!chat(myProfile))
	{
		DEBUG2("PPPConnection::startPPP - chatting failed ?!?\n");
		return false;
	}

	// let's assemble the command line for pppd and send it to the logs
	QStringList myPPPArguments = assembleArgumentList(myProfile);
	QStringList::const_iterator i = myPPPArguments.begin();
	DEBUG4("Assembled PPP string is: '");
	for (; i != myPPPArguments.end(); i++)
		DEBUG4NT("%s ", (*i).ascii());
	DEBUG4NT("'\n");

	setNewConnectionState(ConnectionObserverInterface::STARTING);
	thePPPThreadPtr = new PPPThread(myPPPArguments);
	thePPPThreadPtr->start();
	// if there is a PPP running now, it's ours.
	// but it might take a while before all scheduling is done,
	// so let's wait for it...
	theRunningPPPDPID = -1;
	for (int i=0; i<5 && theRunningPPPDPID==-1; i++)
	{
		DEBUG5("Waiting for ppp PID\n");
		sleep(1);
		theRunningPPPDPID = Runner::doesPPPDexist();
		DEBUG5("Found PPP PID %d\n", theRunningPPPDPID);
	};
	return true;
}

bool PPPConnection::dial(const Profile& aProfile, const QString& myConnectStr) const
{
	QString myDialStr = "";
	unsigned int myCID = aProfile.getCID();
	if ( aProfile.getCIDPrefix().stripWhiteSpace().isEmpty() )
		myDialStr = "ATD*99***" + myConnectStr.number(myCID) + "#";
	else
		myDialStr = "ATD" + aProfile.getCIDPrefix() + QString::number(myCID) + "#";

	DEBUG3("Dial String is: %s \n", myDialStr.ascii() );
	Query myQuery(myDialStr);
	// don't expect an OK following this one...
	// however, we must catch an "ERROR"
	if (myQuery.run()== Query::ERROR)
	{
		DEBUG1("ATD returned ERROR - connection failed\n");
		return false;
	}
	return true;
}


QStringList PPPConnection::assembleArgumentList(const Profile& aProfile) const
{
  	QStringList myPPPArguments;
  	myPPPArguments.push_back("idle");   myPPPArguments.push_back("7200");
	if (aProfile.getNoAuth())
		myPPPArguments.push_back("noauth");
  	myPPPArguments.push_back("asyncmap"); myPPPArguments.push_back("0");

	// PPP related stuff
	// we only ask PPP to detach if connection is successfully up
	myPPPArguments.push_back("updetach");
	if (thePPPVerbosity > 0)
	{
		myPPPArguments.push_back("dump");
		myPPPArguments.push_back("debug");
		myPPPArguments.push_back("debug");
		myPPPArguments.push_back("debug");
	}

	// modem-related stuff
	myPPPArguments.push_back("460800");
	myPPPArguments.push_back("lock");
	myPPPArguments.push_back("crtscts");
	myPPPArguments.push_back("modem");
	myPPPArguments.push_back(theSerialPortName);

	// header compressions and such
	myPPPArguments.push_back("noipx");
	if (aProfile.getNoCompression())
	{
		myPPPArguments.push_back("novj");
		myPPPArguments.push_back("nobsdcomp");
	}
	if (aProfile.getNoCCP())
		myPPPArguments.push_back("noccp");

	// routing related stuff
	if (aProfile.getIsDefaultRoute())
		myPPPArguments.push_back("defaultroute");
	if (aProfile.getReplaceDefaultRoute())
		myPPPArguments.push_back("replacedefaultroute");
	if (aProfile.getUsePeerDns())
		myPPPArguments.push_back("usepeerdns");
	if (aProfile.getNoIpDefault())
		myPPPArguments.push_back("noipdefault");

	// login info
	if (!aProfile.getUsername().isEmpty())
	{
		myPPPArguments.push_back("user");
		myPPPArguments.push_back(aProfile.getUsername());
	}
	if (!aProfile.getPassword().isEmpty())
	{
		myPPPArguments.push_back("password");
		myPPPArguments.push_back(aProfile.getPassword());
	}
	
	// users can specify a ppp peer to call which in turn
	// will make pppd include /etc/ppp/peers/<name> .
	// the good news is that this file allows for priviledged options
	// without pppd having to run as root - that's convenient!
	// (of course, this file can only be created by root)
	if (!aProfile.getCallName().isEmpty())
	{
		myPPPArguments.push_back("call");
		myPPPArguments.push_back(aProfile.getCallName());
	}

	return myPPPArguments;
}




//############################################################################

PPPConnection::PPPThread::PPPThread(const QStringList& anArgumentList)
	: QThread(), theArgumentList(anArgumentList), theReturnValue(-1), isRunning(false)
{
	DEBUG2("PPPConnection::PPPThread::PPPThread()\n");
}


void PPPConnection::PPPThread::run()
{
	DEBUG2("PPPThread::run, before runCommand\n");
	theReturnValue = theRunner.runCommand(Runner::PPPD, theArgumentList);
	DEBUG2("PPPThread::run, after runCommand\n");
	// end of the thread - we're done
	isRunning = false;
}



//############################################################################
static const QString theStaticHSOVersion(QObject::tr("HSO device - no pppd required"));
static const QString theEtcResolvLocation("/etc/resolv.conf"); 
static const QString theEtcResolvBackupLocation("/etc/resolv.conf." APPNAME);


HSOConnection::HSOConnection(const ConnectionBaseClass& aRef)
	:	ConnectionBaseClass(aRef)
{
	DEBUG4("HSOConnection::HSOConnection(void)\n");
	isConnectionActive();
}


HSOConnection::~HSOConnection(void)
{
	DEBUG4("HSOConnection::~HSOConnection(void)\n");
	stopConnection();
}


bool HSOConnection::dial(
		__attribute__ ((unused)) const Profile& aProfile, 
		__attribute__ ((unused)) const QString& myConnectStr) const
{
	// this will actually initiate the connection
	sendChatString("AT_OWANCALL=1,1,0");
	
	// now we are going to poll - there will be a lot of output to parse...
	// FIXME: unfortunately, the current poll code will not rerender the UI
	//        we need to move this piece of code into isConnectionActive()
	while(1)
	{
		switch (runOWANCALL())
		{
		case OWAN_FAILED:
		case OWAN_DISCONNECTED:
			DEBUG3("not good - HSOConnect::dial()ing failed...\n");
			return false;
		case OWAN_CONNECTED:
			DEBUG4("yay, HSOConnect::dial()ing worked\n");
			return true;
		case OWAN_INSETUP:
			// let's wait for a second and try again...
			break;
		}
		sleep(1);
	}
}


const QString& 
HSOConnection::getPPPVersion(void)
{ 
	return theStaticHSOVersion; 
}


bool 
HSOConnection::isConnectionActive(void)
{
	// we only use AT commands here.
	// but these only are available once the SerialPort has been set up...
	if (Query::hasSerial()==false)
	{
		DEBUG4("HSOConnection::isConnectionActive - no serial yet\n");
		setNewConnectionState(ConnectionObserverInterface::STOPPED);
		return false;
	}
	
	ResultsOWANCALL myCallStatus = runOWANCALL();
	DEBUG4("HSOConnection::isConnectionActive - myCallStatus: %d, thePPPDState: %d\n", myCallStatus, thePPPDState);
	switch(myCallStatus)
	{
	case OWAN_FAILED:
	case OWAN_DISCONNECTED:
		if (thePPPDState==ConnectionObserverInterface::RUNNING || 
		    thePPPDState==ConnectionObserverInterface::NOT_OURS_RUNNING) 
		{
			// we just disconnected :-(
			stopNetworking();
		}
		return false;
	case OWAN_CONNECTED:
		return true;
	case OWAN_INSETUP:
		// still in progress ... nothing to do ...
		return true;
	}
	
	// there's no way we can get here... but ok... gcc thinks we can :-(
	return false;
}


HSOConnection::ResultsOWANCALL HSOConnection::runOWANCALL(void) const
{
	Query myQuery("AT_OWANCALL?");
	if (myQuery.runUntilDone() != Query::OK)
		return OWAN_FAILED;
	if (myQuery.hasAnswer()==true)
	{
		// answer will have 10 lines, but we're only interested in the first line
		// it should look like this:  "_OWANCALL: 1, 2, 0" where the '2' is:
		// 0 = Disconnected, 
		// 1 = Connected, 
		// 2 = In setup,  
		// 3 = Call setup failed
		QString myResult = myQuery.getAnswer().first().section(" ",2,2);
		if (myResult.isEmpty())
			return OWAN_FAILED;
		char myFirstChar = myResult.ascii()[0]; 
		switch (myFirstChar)
		{
		case '0':
			return OWAN_DISCONNECTED;
		case '1':
			return OWAN_CONNECTED;
		case '2':
			return OWAN_INSETUP;
		case '3':
			return OWAN_FAILED;
		default:
			DEBUG4("HSOConnection::runOWANCALL unexpected response was '%c'\n", myFirstChar);
			break;
		}
	}
	DEBUG3("HSOConnection::runOWANCALL failed\n");
	return OWAN_FAILED;	
}


bool
HSOConnection::startConnection()
{
	// just for this startup, create a Profile - which should be the currently selected profile
	Profile myProfile;
	
	// TODO: add check for DeviceType here...
 
	// we will be checking if a connection is already running.
	// This shouldn't be the case - that's why there is an assert(false).
	// however, if this happens in a release build, just return false.
	DEBUG3("HSOConnection::startConnection entry\n");

	// from now on until the exit of this member, prevent
	// ConnectionInfo from interfering with the AT serial port
	ConnectionInfoInhibitor myCI;
	setNewConnectionState(ConnectionObserverInterface::STARTING);

	if (isConnectionActive())
	{
		return true;
	}

	// let's do the CHATting :-)
	if (!chat(myProfile))
	{
		DEBUG2("PPPConnection::startPPP - chatting failed ?!?\n");
		return false;
	}
	return startNetworking(myProfile);
	
	// yay, everything worked!!!
	return true;
}

bool
HSOConnection::startNetworking(const Profile& aProfile)
{
	DEBUG2("HSOConnection::startNetworking(void)\n");

	// yet another sanity check
	if (runOWANCALL()!=OWAN_CONNECTED)
	{
		assert(false);
		return false;
	}
	
	// 	If that went well, we can issue AT_OWANDATA?. 
 	//  This will return the IP settings you can use to	configure the hso0 interface
	Query myDataQuery("AT_OWANDATA?");
	if (myDataQuery.runUntilDone() != Query::OK)
		return false;

	// 	The settings which will be returned are : 
	//       _OWANDATA: 1, <gateway>, <IP>, <DNS 1>, <DNS 2>, <NBNS1>, <NBNS2>, <speed>
	if (myDataQuery.hasAnswer()==false)
		return false;
	QStringList myDATAargs = QStringList::split(", ", myDataQuery.getAnswer().first(), true);
	QString myIP   = myDATAargs[1];
	QString myDNS1 = myDATAargs[3];
	QString myDNS2 = myDATAargs[4];
	
	// let's assemble the string that needs to be executed...
	
	// ifconfig is always necessary
	QString myRootRun = 
		"/sbin/ifconfig hso0 " + myIP + " netmask 255.255.255.255 up; ";
	
	// only add default route if specified in the profile 
	if (aProfile.getIsDefaultRoute())
		myRootRun += 
		"/sbin/route add default dev hso0; ";

	// if /etc/resolv.conf's first line contains umtsmon, just overwrite it
	if (FileStuff::getFileFirstLine(theEtcResolvLocation).contains(APPNAME)==false)
	{
		DEBUG5("%s file is not written by us, so let's save it...\n", theEtcResolvLocation.ascii());
		myRootRun += 
		"mv " + theEtcResolvLocation + " " + theEtcResolvBackupLocation + "; ";
	}
	else
		DEBUG5("%s file is already touched by us - let's overwrite it...\n", theEtcResolvLocation.ascii());

	// and create the new resolv.conf
	myRootRun +=
		"echo \"# umtsmon temporary resolv.conf\" > " + theEtcResolvLocation + "; " 
		"echo \"nameserver " + myDNS1 + "\" >> " + theEtcResolvLocation + "; "
		"echo \"nameserver " + myDNS2 + "\" >> " + theEtcResolvLocation;

printf("\n***********STRING*************\n%s\n***********STRING*************\n\n", myRootRun.ascii());
	Runner myRunner;
	myRunner.runCommand(Runner::SOMESU, myRootRun);
	
	setNewConnectionState(ConnectionObserverInterface::RUNNING);
	return true;
}


void 
HSOConnection::stopConnection(void)
{
	DEBUG2("HSOConnection::stopConnection(void)\n");
	stopNetworking();

	// this will kill the actual radio connection
	sendChatString("AT_OWANCALL=1,0,0");

}

void 
HSOConnection::stopNetworking(void)
{
	DEBUG1("************HSOConnection::stopNetworking(void)\n");
	// let's assemble the string that needs to be executed...
	
	// network definitely must go down
	QString myRootRun;
	// do we need to rewrite the /etc/resolv.conf file???
	if (FileStuff::doesFileExist(theEtcResolvBackupLocation))
	{
		DEBUG5("There is a backup resolv.conf\n");
		if (FileStuff::getFileFirstLine(theEtcResolvLocation).contains(APPNAME))
		{
			DEBUG5("And the current resolv.conf was written by umtsmon\n")
			myRootRun +=
			"mv " + theEtcResolvBackupLocation + " " + theEtcResolvLocation + "; ";
		}
	}
	
	// this ifconfig down will automatically kill the route
	myRootRun += 
			"/sbin/ifconfig hso0 down; ";
	
printf("\n***********STRING*************\n%s\n***********STRING*************\n\n", myRootRun.ascii());
	Runner myRunner;
	myRunner.runCommand(Runner::SOMESU, myRootRun);

	setNewConnectionState(ConnectionObserverInterface::STOPPED);
}
