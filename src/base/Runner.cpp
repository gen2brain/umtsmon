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

#include "umtsmon_global.h"
#include "Runner.h"
#include "FileStuff.h"

#include <qstringlist.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <q3textstream.h>
#include <qdir.h>

#include <assert.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 


QString Runner::theSU;
QString Runner::thePPPD;
QString Runner::theCHAT;
QString Runner::thePCCARDCTL;
QString Runner::theUSBMODESWITCH;
QString Runner::theRUNNERTESTER;


static const char* PPPDSEARCHPATH = "/sbin:/usr/sbin:/bin:/usr/bin:/usr/local/bin";

Runner::Runner(void)
{
	// as all location strings are static, we only need to locate the binaries 
	// when the first constructor is run
	if (theSU.isEmpty())
		findBinaries();
	memset(theStdOutPipe, 0, sizeof(theStdOutPipe));
	memset(theStdErrPipe, 0, sizeof(theStdErrPipe));
}


QString   Runner::lookThroughPath(const QString& anAppName, const char* aPath = PPPDSEARCHPATH )
{
	DEBUG5("lookThroughPath(%s)\n", anAppName.ascii());
	
	QStringList myPathList = QStringList::split(":", aPath);
	QStringList::Iterator myI = myPathList.begin();
	for(; myI != myPathList.end(); ++myI)
	{
		// security: if the caller specified a custom search path,
		// let's only allow paths that start with /usr or /opt...
		QString myPath = *myI;
		QFile myFile ( myPath + "/" +  anAppName);
		if ( myPath.startsWith("/bin",true)==false &&
			 myPath.startsWith("/sbin",true)==false && 
			 myPath.startsWith("/usr",true)==false && 
			 myPath.startsWith("/opt",true)==false )
		{
			DEBUG5("  * ignoring entry '%s' as it is not in a safe path\n",
					myPath.ascii());
			if (myFile.exists())
				DEBUG5("   * HOWEVER, THE BINARY DID EXIST THERE\n");
			continue;
		}
		
		if (myFile.exists())
		{ 
			DEBUG5("  * found %s\n", myFile.name().ascii());
			return myFile.name();
		}
	}	
	DEBUG5("  *** searching %s really failed :-(\n", anAppName.ascii());
	return "";
}


bool Runner::findBinaries(void)
{
	DEBUG5("Runner::findBinaries()\n");
	bool isEverythingPresent = true;

	// for debugging / automated test purposes
	theRUNNERTESTER="./RunnerTester";
	
	///////////////////////////////////////
	// find pccardctl
	thePCCARDCTL = lookThroughPath("pccardctl"); // post-2.6.13
	if (thePCCARDCTL.isEmpty())
		thePCCARDCTL = lookThroughPath("cardctl");  // pre-2.6.13
	if (thePCCARDCTL.isEmpty())
	{
		DEBUG2("Helper binary pccardctl/cardctl is missing\n");
		isEverythingPresent = false;
	}
		
	///////////////////////////////////////
	// find PPPD and CHAT
	thePPPD = lookThroughPath("pppd");
	theCHAT = lookThroughPath("chat");
	if (thePPPD.isEmpty())
	{
		DEBUG2("Helper binary pppd is missing\n");
		isEverythingPresent = false;
	}

	///////////////////////////////////////
	// find kdesu/gnomesu, etc
	// we have to look on the regular PATH to find those
	// lookThroughPath contains some code against unsafe directories
	char* myPath = getenv("PATH");
	theSU = lookThroughPath("kdesu", myPath);
	if (theSU.isEmpty())
		theSU = lookThroughPath("gnomesu", myPath);
	if (theSU.isEmpty())
		theSU = lookThroughPath("gksu", myPath);
	if (theSU.isEmpty())
		theSU = lookThroughPath("kdesudo", myPath);
	if (theSU.isEmpty())
	{
		DEBUG2("Helper binary *su (i.e. any known GUI frontend to su) is missing\n");
		isEverythingPresent = false;
	}
		
	///////////////////////////////////////
	// find icon_switch and comparable
	// note that icon_switch itself is outdated, let's look for the
	// best option first.
	theUSBMODESWITCH = lookThroughPath("usb_modeswitch");
	if (theUSBMODESWITCH.isEmpty())
		theUSBMODESWITCH = lookThroughPath("huaweiAktBbo");
	if (theUSBMODESWITCH.isEmpty())
		theUSBMODESWITCH = lookThroughPath("icon_switch");
	
	if (isEverythingPresent)
		DEBUG5("Runner::findBinaries() completed successfully\n");
	return isEverythingPresent;
}


const QString& Runner::getPath(BinariesToRun aProgram)
{
	switch(aProgram)
	{
	case SOMESU:
		return theSU;
	case PPPD:
		return thePPPD;
	case CHAT:
		return theCHAT;
	case PCCARDCTL:
		return thePCCARDCTL;
	case USBMODESWITCH:
		return theUSBMODESWITCH;
	case RUNNERTESTER:
		return theRUNNERTESTER;
	}

	// UH-OH if we get here, something very wrong...
	assert(false);
	
	// in case of a release build... let's limit the damage
	// we cannot return "" as that will be a temp object
	const static QString myEmpty = "";
	return myEmpty;
}
	
bool Runner::isSUID(BinariesToRun aProgram)
{
	DEBUG5("Runner::isSUID()\n");
	if (amIRoot())
	{
		DEBUG2("isSUID: i am root => SUID checks skipped\n");
		return true;
	}

	struct stat myBuf;
	if (stat(getPath(aProgram).ascii(), &myBuf) != 0)
	{
		return false;
	}

	// if the file is not owned by root
	// then I don't care for checking suid bits
	if ( myBuf.st_uid != 0 )
		return false;

	// and the real check itself
	if ( myBuf.st_mode & S_ISUID )
		return true;

	// if we're still here - it is not SUID	
	return false;
}

	
int Runner::doesPPPDexist(void)
{
	QString myPPPDPath = getPath(PPPD);
	
	QDir myProc("/proc");
	myProc.setFilter( QDir::Dirs );
	myProc.setNameFilter ( "[0-9]*" );
	
	const QFileInfoList myList = myProc.entryInfoList();
	QListIterator<QFileInfo> myIterator( myList );
	
	while ( myIterator.hasNext() ) 
	{
	    const QFileInfo myFileInfo = myIterator.next();
		QString myDirName = myFileInfo.fileName();
	
		// be warned: if you don't use the .ascii()
		// myCmdLine will contain all arguments as well, separated by \0...
		QString myCmdLine = FileStuff::getFileFirstLine("/proc/"+myDirName+"/cmdline").ascii();
		if (myCmdLine == myPPPDPath || myCmdLine == "pppd") 
			return myDirName.toLong(NULL, 10);
	}
	return -1;
}


bool Runner::amIRoot(void)
{
	if ( geteuid()==0 || getuid()==0 )
		return true;
	else
		return false;
}




int  Runner::runCommand(BinariesToRun aProgram, 
			QStringList anArgumentStringList, 
			bool isToDetach)
{
	DEBUG5("Runner::runCommand(%d, list[%d items], %d)\n", 
			aProgram, anArgumentStringList.size(), isToDetach);
	
	if (getPath(aProgram).isEmpty())
	{
		DEBUG2("Runner::runCommand - attempting to run command with empty path\n");
		return 1;
	}

	// if we are running pccardctl or icon_switch and they are not suid, we need 
	// to run them through SOMESU
	if (aProgram==PCCARDCTL || aProgram==USBMODESWITCH)
	{
		if (!isSUID(aProgram))
		{
			// FIXME: I'd prefer to keep the QStringList as it is!  	
			QString myNewArgumentString = getPath(aProgram)+" "+ anArgumentStringList.join(" "); 
			return runCommand(SOMESU, myNewArgumentString.split(" "));
		}
	}

	// for *su, we need to apply an extra parameter "-c" that specifies that the next 
	// argument contains the command to execute
	if (aProgram == SOMESU)
	{
		if (! theSU.endsWith("gksu") )
		{
			anArgumentStringList.push_front("-c");
		}
		else
		{
			// but gksu does not like that extra "-c"
			// gksu likes "--" instead
			anArgumentStringList.push_front("--");
		}
	}

	// setup the pipes and clean the stringlists
	if (pipe(theStdOutPipe)!=0 || pipe(theStdErrPipe)!=0)
	{
		assert(false);
		return 1;
	}
	theStdOutTexts.clear();
	theStdErrTexts.clear();
		
	// start a second (child) process
	pid_t myChildPID = fork();
	if (myChildPID == -1)
	{
		DEBUG1("fork() returns error\n");		
		return -2;
	}
	
	// only the child process executes the command
	if (myChildPID == 0)
		execChild(aProgram, anArgumentStringList);

	// if we're still here, we're the parent...
	DEBUG3("INSIDE PARENT, uid=%d, pid=%d\n", getuid(), getpid());
	
	// Close STDERR for write
	close(theStdErrPipe[1]);
	// Close the write end of STDOUT
	close(theStdOutPipe[1]);

	// and the parent waits for the child to finish...
	DEBUG5("Parent with pipes to not detached child\n");
	char buffer[BUFSIZ+1];
	memset(buffer, '\0', sizeof(buffer));
	
	int myStatus = -1;

	// just to make sure that we read everything, we check on 3 different conditions to be met
	// 1) stdout is empty
	// 2) stderr is empty
	// 3) waitpid thinks the app has died
	int hasAppDied = 0;
	while (hasAppDied != 7)
	{
		fd_set myReadFDs;
		FD_ZERO(&myReadFDs);
		FD_SET(theStdOutPipe[0], &myReadFDs);
		FD_SET(theStdErrPipe[0], &myReadFDs);

		struct timeval myTimeVal;
		myTimeVal.tv_sec = 1;
		myTimeVal.tv_usec= 0;
		int n = (theStdOutPipe[0]>theStdErrPipe[0])?theStdOutPipe[0]:theStdErrPipe[0];
		int myRetVal = select(n+1, &myReadFDs, NULL, NULL, &myTimeVal);

		if (myRetVal == -1)
		{
			perror("select()");
			break;
		}

		if (FD_ISSET(theStdOutPipe[0], &myReadFDs))
			if (readPipe(theStdOutTexts, theStdOutPipe[0]) == false) 
				hasAppDied |= 1;

		if (FD_ISSET(theStdErrPipe[0], &myReadFDs))
			if (readPipe(theStdErrTexts, theStdErrPipe[0]) == false) 
				hasAppDied |= 2;

		// has program state finished? if yes: that's one out of 3 things 
		// to stop the while loop
		if (waitpid(myChildPID, &myStatus, WNOHANG | WUNTRACED) > 0)
			hasAppDied |= 4;

		// sleep for 10 ms
		struct timespec sleep_time;
		sleep_time.tv_sec=0;
		sleep_time.tv_nsec=10*1000*1000; 
		nanosleep(&sleep_time,NULL);
		DEBUG5("child read pipe loop end, flags: %d\n", hasAppDied);
	}

	// Close STDOUT/STDERR for reading
	close(theStdOutPipe[0]);
	close(theStdErrPipe[0]);

	// the child is dead now
	myChildPID = -1;
	return myStatus;
} 

bool Runner::readPipe(QStringList& aStringList, int anFD)
{
	QFile myFile;
    QString line;
	myFile.open(QIODevice::ReadOnly, anFD);
    Q3TextStream in(&myFile);
        
    line = in.readLine();
    if (line.length() == 0 )
    {
        myFile.close();
        return false;
    }

    line = line.stripWhiteSpace();
    aStringList.push_back(line);

    while (!in.atEnd())
    {
        line = in.readLine();
        line = line.stripWhiteSpace();
        aStringList.push_back(line);
    }

    myFile.close();
    return true;
}	

////////////////////////////////////////////////////////////////////////////
void Runner::execChild(BinariesToRun aProgram, 
				QStringList anArgumentStringList)
{
	DEBUG3("INSIDE CHILD, uid=%d, pid=%d\n", getuid(), getpid());
	DEBUG5("INSIDE CHILD, Prog: %s\n", getPath(aProgram).ascii());
	int myDebugArgs = anArgumentStringList.size();
	for (int i=0; i < myDebugArgs; i++)
	{
		DEBUG5("INSIDE CHILD,  arg%02d: '%s'\n",i,anArgumentStringList[i].ascii());
	}

	// connect stdout to the pipe and close the original stuff

	// Close the Child process' STDOUT 
	// and close the accompanying pipe for reading
	close(STDOUT_FILENO);
	int myDummy = dup(theStdOutPipe[1]);
	close(theStdOutPipe[0]);
	close(theStdOutPipe[1]);

	// Close the Child process' STDERR 
	// and close the accompanying pipe for reading
	close(STDERR_FILENO);
	myDummy = dup(theStdErrPipe[1]);
	close(theStdErrPipe[0]);
	close(theStdErrPipe[1]);

	QString myCmd = getPath(aProgram);
	int myNumArgs = anArgumentStringList.size();
	char** myArgv = new char*[myNumArgs+2];
	myArgv[0] = const_cast<char*>(myCmd.ascii());
	for (int i=1; i < myNumArgs+1; i++)
	{
		myArgv[i] = const_cast<char*>(anArgumentStringList[i-1].ascii());
	}
	myArgv[myNumArgs+1] = NULL;

	// FIXME: we apparently need several things from 'environ'...
	// But I'd prefer to just limit the environment to DISPLAY...
	// alas: it doesn't work then :-(
	execve(myArgv[0], myArgv, environ);
	
	// as execve shouldn't return, we shouldn't get here
	// but to prevent static code checkers from complaining, 
	// let's just be correct
	delete myArgv;
		
    DEBUG5("INSIDE CHILD,  myDummy:: '%d'\n", myDummy);
	
	// don't use exit() here, right after a fork() one uses _exit().
	_exit(127);
}


