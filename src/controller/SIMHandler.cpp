/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2006  Klaas van Gend
 *           copyright (C) 2007  Christofer Wesseling
 *           copyright (C) 2008  Klaas van Gend
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
#include "enterpindialog.h"
#include "enterpukdialog.h"
#include "SIMHandler.h"
#include "ProgressDialog.h"
#include "Query.h"
#include "TheDeviceManagerSingleton.h"
#include "Popup.h"

#include <assert.h>
#include <qstring.h>
#include <qtranslator.h>

/** this string list contains all possible answers on the AT+CPIN question
 * table is taken from Chapter 8.3 in 3GPP TS 27.007 V7.0.0 (2006-03)
 * 
 */
static const char*  theCPIN_Answers[] =
{
	// Translators notice: NEVER translate these texts!!!
	"READY",
	"SIM PIN",
	"SIM PUK",
	"PH-SIM PIN",
	"PH-SIM PUK",
	"PH-FSIM PIN",
	"PH-FSIM PUK",
	"SIM PIN2",
	"SIM PUK2",
	"PH-NET PIN",
	"PH-NET PUK",
	"PH-NETSUB PIN",
	"PH-NETSUB PUK",
	"PH-SP PIN",
	"PH-SP PUK",
	"PH-CORP PIN",
	"PH-CORP PUK"
};


/** TRANSLATOR 
 *  this string list contains descriptions for the possible answers 
 *  on the AT+CPIN question
 *  Klaas took this list from the gsmlib/gsmctl man page
 *  and modified it to contain user understandable text... 
 *  (If he understood the message himself)
 *  
 *  Several of these messages should not appear in the wild (including the phone-to-* messages)
 */
static const char* theCPIN_Descriptions[] =
{
	QT_TR_NOOP("not pending for any password."),		// READY
	QT_TR_NOOP("waiting for the PIN code for the SIM card."),	// SIM PIN
	QT_TR_NOOP("waiting for the PUK code to unblock the SIM card again."), //SIM PUK
	QT_TR_NOOP("waiting phone-to-SIM card password to be given."),  //PH-SIM PIN
	QT_TR_NOOP("waiting phone-to-very SIM card password to be given."), // PH-SIM PUK
	QT_TR_NOOP("waiting phone-to-very first SIM card password to be given."), // PH-FSIM Pin
	QT_TR_NOOP("waiting phone-to-very first SIM card unblocking password to be given."), // PH-FSIM PUK
	QT_TR_NOOP("waiting SIM PIN2 to be given."), // SIM PIN2
	QT_TR_NOOP("waiting SIM PUK2 to be given."), // SIM PUK2
	QT_TR_NOOP("waiting network personalisation password to be given."),
	QT_TR_NOOP("waiting network personalisation unblocking password to be given."), // PH-NET PUK
	QT_TR_NOOP("waiting network subset personalisation password to be given."),
	QT_TR_NOOP("waiting network subset personalisation unblock code to be given."), // PH-NETSUB PUK
	QT_TR_NOOP("unknown (PH-SP PIN) - please contact author"),
	QT_TR_NOOP("unknown (PH-SP PUK) - please contact author"),
	QT_TR_NOOP("unknown (PH-CORP PIN) - please contact author"),
	QT_TR_NOOP("unknown (PH-CORP PUK) - please contact author")
};

SIMHandler::SIMHandler()
{
	// empty now
	;
}

void SIMHandler::askForPIN(void)
{
	DEBUG4("SIMHandler::askForPIN() has been called\n");
	SIMHandler::PIN_ReturnCode myPINReturn;
	// dont't let him out before the card signal: everything fine!
	while( (myPINReturn=isPinCodeRequired()) != SIMHandler::PIN_READY)
	{  
		DEBUG4("is Pin required: %s\n", getCPINDescription(myPINReturn).ascii());
		switch(myPINReturn)
		{
		case SIMHandler::PIN_READY:
			DEBUG5("No PIN required\n");
			break; // everythink fine!
		case SIMHandler::PIN_SIM:
			{		
				DEBUG5("We need a PIN!\n");
				EnterPIN myPIN;
				myPIN.enableValidator();					
				myPIN.setSIMHandlerPtr(this);								
				// shows the dialog, blocking all other..
				myPIN.exec(); 
				
				if (myPIN.result() == QDialog::Accepted)
				{
					// het think's he's reads? ok.. lets give him a try?
					if( setPIN(myPIN.getPIN(), &myPIN) == true )
						break;
				}
				else
				{
						// dialog rejected
						exit(35); // ok, he wants to get out
				}
				break;
			}
		case SIMHandler::PIN_PUK:
			{
				EnterPUK myPUK;
				myPUK.enableValidator();
				myPUK.setSIMHandlerPtr(this);
				myPUK.exec();
				if (myPUK.result() == QDialog::Rejected)
					 exit(35); // ok, he wants to get out
				break;
			}
		default:
			Popup::Critical( QObject::tr("SIM card is moaning about something I don't know about:\n" 
			            "\"%1\"\n"
						"Please insert the SIM into a regular phone and check it...").arg(getCPINDescription(myPINReturn)));
			assert(false);
		}
	} // end of while status != ok...	
}


const QString 
SIMHandler::getCPINDescription(PIN_ReturnCode aCode)
{
	assert(aCode > 0);
	QString myString;
	if(aCode > ERROR_BEYOND_LAST)
		myString = QObject::tr("Unknown error - please review logs");
	else
		myString = QObject::tr(theCPIN_Descriptions[aCode]);
	return myString;
}


SIMHandler::PIN_ReturnCode SIMHandler::isPinCodeRequired(QWidget* aWPtr)
{
	DEBUG5("SIMHandler::isPinCodeRequired\n");
	QString myPINAnswer;
		
	// create a Status Dialog....
	ProgressDialog myPD(aWPtr, QObject::tr("Waiting for SIM-Interface...")); 
	myPD.start(500);

	// wait up to 10 seconds for a positive result...
	// note: we can have three cases here
	//  1) AT+CPIN fails because the interface is still being initialised
	//     we retry in that case (that's what the loop is for)
	//  2) AT+CPIN fails for errors
	//     throw an error dialog and fail
	//  3) AT+CPIN takes considerable time to finish...
	//     this is solved by using runUntilDone
	myPD.setMaxTime(10);
	while (myPD.runOneSecond())
	{
		Query myQuery("AT+CPIN?");
		Query::ReturnCode myResult = myQuery.runUntilDone();
		if (myResult == Query::OK)
		{
			if (!myQuery.hasAnswer())
			{
				DEBUG1("work around for cards not returning SIM_READY but only OK.\n")
				return PIN_READY;
			}
			myPINAnswer = myQuery.getAnswer().first();
			myPD.finish();
			break;  // faster than 10 seconds.. great!
		}
		if (myResult == Query::ERROR)
		{
			// regular error without explanation
			if (!myQuery.hasAnswer())
			{
				myPD.finish();
				return ERROR_ERROR;
			}
			QString myLastAnswer = myQuery.getAnswer().last();
			// at least the Option cards really give 
			//      "+CME ERROR: SIM interface not started yet" error...
			if( !myLastAnswer.contains("not started yet") &&
			    !myLastAnswer.contains("SIM busy") )
				Popup::Critical(QObject::tr("SIM card handling resulted in this error:\n"
						"  \"%1\", sorry.\numtsmon will now close.").arg(myLastAnswer.replace("+CME ERROR: ","",false)));
		}
		DEBUG2("no SIM interface yet - sleeping \n");
		sleep(1); // wait 1 second...
	}
	// if you get here after the progress max TIME, the following will return an ERROR_UNKOWN...

	// clean up the PIN answer, as it might contain all kinds of "rubbish" 
	myPINAnswer.replace("\"","");
	myPINAnswer.replace("\'","");
	myPINAnswer.replace("\r","");
	myPINAnswer.replace("\n","");
	int myCode;
	for (myCode = 0; myCode < ERROR_BEYOND_LAST; myCode++)
	{
		QString myCPINString = "+CPIN: ";
		myCPINString += theCPIN_Answers[myCode];
		if (myPINAnswer == myCPINString)
			break;
	}

	// Fixup for "original Vodafone 3G" and "Solomon SCMi250u" cards
	// as this means SIM READY !!
	if ((myCode == PIN_PIN2 || myCode == PIN_PUK2) && 
			TheDeviceManagerSingleton::getDeviceCapsRef().needsPIN2workaround()==true)
	{
		DEBUG2("Vodafone3G/SolomonSCMi250u workaround: PIN_PIN2/PIN_PUK2 means PIN_READY.\n");
		myCode = PIN_READY;
	}

	// "magically", ERROR_UNKNOWN is equal to ERROR_BEYOND_LAST :-)
	return static_cast<PIN_ReturnCode>(myCode);
}



bool SIMHandler::setPIN(const QString& aPIN, QWidget* aWPtr)
{
	return getQueryReturnOK( "AT+CPIN=\"" + aPIN + "\"", aWPtr);
}


	
bool SIMHandler::setPUK(const QString& aPUK, const QString& aNewPIN, QWidget* aWPtr)
{ 
	//lets ensure that the sim reallay wants a puk...
	 for (int i=3; i>0; i--)
	 {
	 	DEBUG5("SimHandler::setPUK invalid pin\n");
		Query myQuery("AT+CLCK=\"SC\",0,\""  APPNAME  "\"");
		myQuery.run();
	 }
	 
	return getQueryReturnOK( "AT+CPIN=\"" + aPUK + "\",\""+aNewPIN+"\"", aWPtr );
}


bool SIMHandler::getQueryReturnOK(const QString& aQueryString, QWidget* aWPtr)
{
	bool myReturn = false;
	int  myOriginalVerbosity = theVerbosity;
	
	// we need some secrecy here: let's suspend logging until later
	if (theVerbosity>2)
	{
		DEBUG2("suspending detailed logging until PIN/PUK is set\n");
		theVerbosity=2;
	}
	
	Query myQuery(aQueryString);
	Query::ReturnCode myResult = myQuery.run();
	if (myResult == Query::OK)
	{
		myReturn = true;
		goto the_end;
	}
	if (myResult == Query::ERROR)
		goto the_end;

	// create a Status Dialog....
	{
		ProgressDialog myPD(aWPtr, QObject::tr("Trying to set PIN/PUK")); 
		myPD.start(500);
	
		// wait up to 12 seconds for a positive result...
		myPD.setMaxTime(12);
		while (myPD.runOneSecond())
		{
			myResult = myQuery.receiveMore(); 
			if ( myResult == Query::OK)
			{
				myReturn = true;
				myPD.finish();
				break;
			}
			DEBUG2("sleeping ..\n");
		}
	}
	
the_end:
	if (theVerbosity != myOriginalVerbosity)
	{
		DEBUG2("PIN/PUK finished: Resuming log at verbosity %d\n", myOriginalVerbosity);
		theVerbosity = myOriginalVerbosity;
	}
	return myReturn;
}


bool SIMHandler::isPINProtectionEnabled(void)
{
	DEBUG4("SIMHandler::isPINProtectionEnabled\n");
	
	// +CLCK: 0  -> PIN is off
	// +CLCK: 1  -> PIN is on
	Query myQuery("AT+CLCK=\"SC\",2");
	// lets wait for an answer..
	Query::ReturnCode myResult = myQuery.runUntilDone();
	if (myResult == Query::OK)
	{
		if (myQuery.hasAnswer())
			return( myQuery.getAnswer().last().contains("+CLCK: 1") );
		else
			return false; 
	}
	if (myResult == Query::ERROR)
	{
		// e.g. the LG KU380 Cellphone does this
		// it probably means nothing serious - let's assume it means
		// the phone doesn't expect a PIN code. 
		DEBUG3("Phone probably didn't expect a PIN question - assuming no PIN needed\n");
		return false;
	}
	
	// anything else?
	// probably no PIN required...
	return false;
}

bool SIMHandler::setPINActive(bool aPINActiceStatus, const QString &aCurrentPIN, QWidget* aWPtr)
{
	DEBUG5("SIMHandler::setPINActive\n");
	
	QString myStatus = "1";
	if(aPINActiceStatus == false)
		myStatus = "0";
	
	return getQueryReturnOK("AT+CLCK=\"SC\"," + myStatus + ",\"" + aCurrentPIN + "\"", aWPtr);
}

bool SIMHandler::setNewPIN(const QString &aCurrentPIN, const QString &aNewPIN, QWidget* aWPtr)
{
	DEBUG5("SIMHandler::setNewPIN \n");
	return getQueryReturnOK("AT+CPWD=\"SC\","+aCurrentPIN+","+aNewPIN, aWPtr);
}



