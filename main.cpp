/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2006, 2008  Klaas van Gend
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

#include "mainwindow.h"
#include "TheDeviceManagerSingleton.h"
#include "Runner.h"
#include "Popup.h"

#include "Internationalisation.h"

#include <qapplication.h>
#include <qmessagebox.h>
#include <qtextcodec.h>
#include <assert.h>
#include <signal.h>
#include <stdlib.h>

// forward declarations
void popupAbortMessageBox(int);
void printAbortMessage(int);


static const int MAXLONGCOMMANDSIZE  = 20;
static const int MAXSHORTCOMMANDSIZE =  4;

// oops global variable declared here...
// let's make sure they do not multiply :-)
int  theVerbosity = 1;
int  thePPPVerbosity = 0;

static char* theLocale = NULL;

static bool setDefProfile(char* anArgument)
{
	DEBUG5("setDefProfile: '%s'\n", anArgument);
	
	// all argument checking is done in in the class :-D
	if (Profile::setActiveProfile(anArgument) == false)
	{
		printf("ERROR: Profile \"%s\" does not exist\n", anArgument);
		exit(2);
	}
	return true;
}

static bool setForceAutodetection( __attribute__ ((unused)) char* anArgument)
{
	TheDeviceManagerSingleton::me().setForceAutodetection();
	return true;
}

static bool setLocale(char* anArgument)
{
	// will be parsed/verified later in main()
	theLocale = anArgument;
	return true;
}

static bool setAutoConnect(__attribute__ ((unused)) char* anArgument)
{
	DEBUG5("setautoconnect\n");
	TheSettingsSingleton::getQSRef().writeEntry(theGeneralAutoConnectSettingName, true);
	return true;
}

static bool setNoAutoConnect(__attribute__ ((unused)) char* anArgument)
{
	DEBUG5("setNOautoconnect\n");
	TheSettingsSingleton::getQSRef().writeEntry(theGeneralAutoConnectSettingName, false);
	return true;
}

static bool setSerial(char* anArgument)
{
	DEBUG5("setSerial: '%s'\n", anArgument);

	// no need to do advanced parameter checking
	//     - that'll be done later on, right? 
			
	// is there a comma in? in that case we neet to set both AT and PPP
	QStringList myList = QStringList::split(',', anArgument);
	
	switch (myList.count())
	{
		case 1:
			TheDeviceManagerSingleton::me().setSuggestedAT(myList[0]);
			TheDeviceManagerSingleton::me().setSuggestedPPP("");
			TheDeviceManagerSingleton::me().setSuggestedDeviceType(DeviceCapabilities::GENERIC_USER_SERIAL);
			break;
		case 2:
			TheDeviceManagerSingleton::me().setSuggestedAT(myList[0]);
			TheDeviceManagerSingleton::me().setSuggestedPPP(myList[1]);
			TheDeviceManagerSingleton::me().setSuggestedDeviceType(DeviceCapabilities::GENERIC_USER_SERIAL);
			break;
		default:
			printf("ERROR: supply max 2 devices to --serial\n");
			exit(2);
			break;
	}
	
	return true;
}

static bool setVerbosity(char* anArgument)
{
	DEBUG1("setVerbosity: '%s'\n", anArgument);
	
	// the argument should be a number. Let's parse.
	int myNumber = atoi(anArgument);
	if (myNumber < 1 || myNumber > MAX_VERBOSITY)
	{
		printf("ERROR: verbosity argument should be between 1 and 5\n");
		return false;
	}
	theVerbosity = myNumber;
	DEBUG2("set verbosity level to %d\n", theVerbosity);

	// check if we have a second argument after a comma (ppp-verbosity)
	char* comma = strchr(anArgument, ',');
	if (comma != NULL)
	{
		myNumber = atoi(comma + 1);
		if (myNumber < 0 || myNumber > 2)
		{
			printf("ERROR: PPP verbosity argument should be between 0 and 2\n");
			return false;
		}
		thePPPVerbosity = myNumber;
	}
	else
	{
		// verbosity > 2 => pppverbosity = 1; verbosity > 4 => pppverbosity = 2
		thePPPVerbosity = (theVerbosity - 1) / 2;
	}
	DEBUG2("set PPP verbosity level to %d\n", thePPPVerbosity);
	
	return true;
}

static bool help( __attribute__ ((unused)) char* anArgument)
{
	printf(APPNAME " " APPRELEASE "" APPFLAVOUR "\n\nhelp text\n\n");
	printf(" --help              gives this help text\n");
	printf(" -h                  gives this help text\n");
	printf(" --force-autodetect  force autodetection (don't use info from config file)\n");
	printf(" --serial <at>[,ppp] will prevent autodetection of devices and use serial port <at>[,<ppp>]\n");
	printf(" -s <at>[,<ppp>]     will prevent autodetection of devices and use serial port <at>[,<ppp>]\n");
	printf(" --verbosity <lvl>[,ppplvl] set verbosity, 1=little (default), %d=all\n", MAX_VERBOSITY);
	printf(" -v <lvl>[,ppplvl]   set verbosity\n");
	printf(" --language <locale> set the language, e.g. nl or pt_BR\n");
	printf(" --autoconnect       set autoconnect to true (persistant!)\n");
	printf(" --no-autoconnect    set autoconnect to false (persistant!)\n");
	printf(" --default-profile <profilename>   sets the default profile to connect with (persistant!)\n");
	printf("\n(parameters in <> are required, parameters in [] are optional)\n");
	printf("\n"); 
	exit(1);
}

/// the formatting for theArgsTable
struct ArgsStruct
{
	char theFullCommand [MAXLONGCOMMANDSIZE];
	char theShortCommand[MAXSHORTCOMMANDSIZE];
	bool needsArgument;
	bool (*theFunctionPtr)(char* anArgument);
};

static ArgsStruct theArgsTable[] =
{ 
//    long,   short, has args, function name
	{ "--help", "-h", false, help, },
	{ "--serial", "-s", true, setSerial, },
	{ "--verbosity", "-v", true, setVerbosity, }, 
	{ "--force-autodetect", "", false, setForceAutodetection, },
	{ "--language", "", true, setLocale, },
	{ "--autoconnect", "", false, setAutoConnect, },
	{ "--no-autoconnect", "", false, setNoAutoConnect, },
	{ "--default-profile", "-p", true, setDefProfile, },
//	keep this one last
	{ "\0", "\0", false, NULL, },
};

/** compare strings. 
 *  @returns 		If 0, no match, if positive nonzero: the character at this index is extra
 */
static int compareStrings(const char* anArgument, const char* aCommand)
{
	int i=0;
	while(true)
	{
		// we're at the end of the command - a match, maybe with info left...
		if (aCommand[i] == 0)
			return i;
		// the argument is shorter than the command - cannot be a match :-(
		if (anArgument[i] == 0)
			break;
		if (anArgument[i] != aCommand[i])
			break;
		i++;
	}

	// if we get here, it was no match :-(			
	return 0;
}


int main( int argc, char **argv )
{
	
	QApplication myQApp( argc, argv );

	puts( APPNAME " version " APPRELEASE "" APPFLAVOUR " ." );

	// perform command line parsing
	// skip argv[0] because we know what's in there
	for ( int i = 1; i < myQApp.argc(); i++ )
	{
		int  myMatchPoint = 0;
				
		char* myArgvPtr = myQApp.argv()[i];
		ArgsStruct* myTEntry;

		for (int j = 0; theArgsTable[j].theFullCommand[0]!='\0'; j++)
		{
			myTEntry = &(theArgsTable[j]);

			myMatchPoint = compareStrings(myArgvPtr, myTEntry->theFullCommand);
			if (myMatchPoint)
				break;
			myMatchPoint = compareStrings(myArgvPtr, myTEntry->theShortCommand);
			if (myMatchPoint)
				break;
		} // end of the 'j' loop

		if (myMatchPoint)
		{
			char* anArgument = NULL;

			// there are several cases:
			//  1)  -s 3
			//  2)  -s3
			//  3)  -s=3
			//  4)  --serial=3
			//  5)  --serial 3
			//  6)  --help     (no arguments)
			//  7)  -h         (no arguments)

			// cases 1,2,3,4,5:
			if (myTEntry->needsArgument)
			{
				// is there extra data in this argument (i.e. cases 2,3,4)
				if (myArgvPtr[myMatchPoint] != 0)
				{
					// is it a '=' (case3,4) - then advance one character
					if (myArgvPtr[myMatchPoint] == '=')
						myMatchPoint++;
					anArgument = &myArgvPtr[myMatchPoint];
				}
				else
				{
					// case 1,5:

					// is there a next argument to parse?
					if ( (i+1) < myQApp.argc())
					{
						anArgument = myQApp.argv()[i+1];
						i++;
					}
					else
						anArgument = NULL;
				}
			}
			
			
			if (myTEntry->needsArgument==true && anArgument==NULL)
			{
				printf("parsing error: missing argument to '%s'!\n", myArgvPtr);
				exit(3);
			}
			if (myTEntry->theFunctionPtr(anArgument) == false)
			{
				printf("...\nExiting gracefully\n\n");
				help(NULL);
				// this should never be reached:
				exit(1);
			}
		}
		else
		{
			printf("ERROR: unknown argument '%s'\n", myArgvPtr);
			exit(2);
		}
	} // end of the 'i' loop

	// If a user who doesn't run umtsmon from the command line hits an assert,
	// the program stops without a visible UI message.
	// let's fix that here: if no verbosity is set, register a SIGABRT handler to pop up a dialog
	if (theVerbosity <= 1)
	{
		printf("installing GUI SIGABRT handler\n");
		signal(SIGABRT, popupAbortMessageBox);
	}
	else
	{
		// also register a handler explaining what to do 
		// if the users enabled lots of verbosity 
		if (theVerbosity >= 5)
		{
			printf("installing text SIGABRT handler\n");
			signal(SIGABRT, printAbortMessage);
		}		
	}

	// Setup translations
	// * we may have a valid theLocale (i.e. specified on command line), we should check it
	// * we may have a valid locale as a setting, we should check it
	// * we may have a valid locale in the environment
	Internationalisation myI18n( &myQApp );
	bool myI18nResult = false;
	if (theLocale!=NULL)
	{
		myI18nResult = myI18n.tryLocale(theLocale);
		if (myI18nResult == false)
		{
			DEBUG1("ERROR: your specified locale (\"%s\") is not valid\n", theLocale);
			exit(1);
		}
	}
	else
		myI18nResult = myI18n.trySavedLocale();
	if (myI18nResult == false)
		myI18nResult = myI18n.tryLocale(QTextCodec::locale());
	if (myI18nResult == false)
		DEBUG5("Running on the default (en_US) locale\n");
	

	Popup::WarningWithMemory("WelcomeMsg", QObject::tr("Welcome to umtsmon."));
	
	// check for rootness - useful for the logs if users do odd things
	if (Runner::amIRoot())
		DEBUG1(APPNAME " is running as root!!!\n");

	////////////////////////////////////
	// and start the application
	// do not forget to pass on some initialised classes
	mainwindow w;
	myQApp.setMainWidget( &w );
	w.setI18nRef(myI18n);
	w.show();
	return myQApp.exec();
}





void popupAbortMessageBox(__attribute__ ((unused)) int aSignalNumber)
{
	static bool isFirstTimeHere=true;

	if ( !isFirstTimeHere )
		return;
	isFirstTimeHere=false;

	QMessageBox::critical (NULL, 
			APPNAME " assert error",
			APPNAME " ran into a problem and unfortunately had to close. "
				"It is highly likely that if you run " APPNAME "from a command line, like:\n"
				APPNAME " -v5\n"
				"you will get information on what went wrong, including"
				" information on how to help us solve it.\n"
				"We thank you in advance for helping us improve " APPNAME "!",
			QMessageBox::Ok |  QMessageBox::Default, 
			QMessageBox::NoButton, 
			QMessageBox::NoButton);
	// do not call exit here - or the signal will not be handled correctly
}

void printAbortMessage(__attribute__ ((unused)) int aSignalNumber)
{
	printf("\n\n  *** " APPNAME " version " APPRELEASE "" APPFLAVOUR " closed due to an"
	           " unrecoverable program error.\n"
	       "  *** Please e-mail all of the above output to"
	           " umtsmon-develop@lists.sourceforge.net,\n"
	       "  *** Or go to the " APPNAME " website and file a bug.\n\n\n"); 
	// do not call exit here - or the signal will not be handled correctly
}
