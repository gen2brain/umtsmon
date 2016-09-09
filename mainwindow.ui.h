/* umtsmon - a program to control/monitor your UMTS card
 * Copyright (C) 2006, 2007, 2008, 2009  Klaas van Gend, Christofer Wesseling
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

/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

#include "umtsmon_global.h"
#include "TheDeviceManagerSingleton.h"
#include "PPPConnection.h"
#include "Popup.h"
#include "UIState.h"

#include "qlabel.h"
#include "qlineedit.h"
#include "qtextedit.h"

#include <assert.h>


// it's actually defined in main.cpp
extern const char* theGeneralAutoConnectSettingName;


void mainwindow::init()
{
	theConnectionPtr = new DummyConnection();
	
	setCaption("UMTSmon");
	menu_help_about->setMenuText(     tr("&About %1").arg(APPNAME) );
	menu_connection_exit->setMenuText(tr("&Quit %1").arg(APPNAME) );
	Toolbar->setCaption(APPNAME" Toolbar");
	
	// FIXME: for now, this will automatically handle the SIMHandler.askforPIN().
	theUIStatePtr = new UIState(this);
	theConnectionPtr->attach(theUIStatePtr);

	// the autoconnect item is a toggle one
	menu_settings_AutoConnect->setToggleAction(true);
	menu_settings_AutoConnect->setOn(TheSettingsSingleton::getQSRef().readBoolEntry(theGeneralAutoConnectSettingName, false));
}


void mainwindow::showAboutDialog()
{
	DEBUG5("About dialog clicked\n");
	AboutDialogUmtsmon myDialog;
	myDialog.versionlabel->setText("<b>"APPNAME"</b> version <b>"APPRELEASE "" APPFLAVOUR "</b>");
	// no AT commands in this dialog - no exec() prevention needed
	myDialog.exec();
}

void mainwindow::showInfoDialog()
{
	DEBUG5("info dialog clicked\n");
	if (theUIStatePtr->getState() > UIState::REGISTERED)
	{
		Popup::Info(tr("Sorry, the Info dialog cannot be shown\nwhilst a PPP connection exists."), this);
		return;
	}
	if (theUIStatePtr->getState() < UIState::REGISTERING)
	{
		Popup::Info(tr("Sorry, the Info dialog cannot be shown right now."), this);
		return;
	}
	InfoDialogUmtsmon myDialog;
	myDialog.lbl_pppd_version->setText(theConnectionPtr->getPPPVersion());
	myDialog.exec();
}


void mainwindow::setPINmenu(PINState myPINState)
{
	bool isOn =      (myPINState == PIN_ENABLED);
	bool isEnabled = (myPINState == PIN_DISABLED);
	
    menu_pin_enable->setOn(isOn);
    menu_pin_enable->setEnabled(isEnabled);
    menu_pin_disable->setOn(isEnabled);
    menu_pin_disable->setEnabled(isOn);
    menu_pin_change->setEnabled(isOn);
}

void mainwindow::DisablePINProtection()
{
	SIMHandler mySIMHandler;
	EnterPIN myPIN;
	myPIN.enableValidator();					
	myPIN.setSIMHandlerPtr(&mySIMHandler);							
	myPIN.exec(); // shows the dialog, blocking all other..
	if(myPIN.result() == QDialog::Rejected)
		return; 
	if(!mySIMHandler.setPINActive(false,myPIN.pinField->text(),this))
		Popup::Warning(tr("Disabling PIN protection failed.\n"
		    "Try again, and be sure to use the correct current PIN!"), this);
	else
	{
		//switch the toggle state....
		setPINmenu(PIN_DISABLED);
		Popup::Info(tr("Disabling PIN protection was successful."), this);
	}
}

void mainwindow::EnablePINProtection()
{
	SIMHandler mySIMHandler;
	EnterPIN myPIN;
	myPIN.enableValidator();					
	myPIN.setSIMHandlerPtr(&mySIMHandler);							 
	myPIN.exec(); // shows the dialog, blocking all other..
	if(myPIN.result() == QDialog::Rejected) 
		return;
	if(!mySIMHandler.setPINActive(true,myPIN.pinField->text(),this))
		Popup::Warning(tr("Enabling PIN protection failed.\n"
		    "Try again, and be sure to use the correct current PIN!"), this);
	else
    {
        setPINmenu(PIN_ENABLED);
		Popup::Info(tr("Enabling PIN protection was successful."), this);
    }
}


void mainwindow::ChangePIN()
{
    DEBUG5("change PIN settings clicked\n");
    SIMHandler mySIMHandler;
          EnterPIN myCurrentPIN;
			myCurrentPIN.enableValidator();					
			myCurrentPIN.setSIMHandlerPtr(&mySIMHandler);							
			myCurrentPIN.errorMessage->setText(tr("Please enter your current PIN.")); 
			myCurrentPIN.exec(); // shows the dialog, blocking all other..	
		if(myCurrentPIN.result() == QDialog::Rejected) return;
			
		EnterPIN myNewPIN;
			myNewPIN.enableValidator();					
			myNewPIN.setSIMHandlerPtr(&mySIMHandler);							
			myNewPIN.errorMessage->setText(tr("Please enter your NEW pin.")); 
			myNewPIN.exec(); // shows the dialog, blocking all other..
		if(myNewPIN.result() == QDialog::Rejected) return;
		
		EnterPIN myNewPINsec;
			myNewPINsec.enableValidator();					
			myNewPINsec.setSIMHandlerPtr(&mySIMHandler);							
			myNewPINsec.errorMessage->setText(tr("Please enter your NEW pin again.")); 
			myNewPINsec.exec(); // shows the dialog, blocking all other..
		if(myNewPINsec.result() == QDialog::Rejected) return;

	if(!myNewPINsec.pinField->text().contains(myNewPIN.pinField->text() ) )
	{
		Popup::Warning(tr("Changing PIN failed.\n"
		    "New PIN and verify PIN were not same, please try again!"), this);
	}
	else if(! mySIMHandler.setNewPIN(myCurrentPIN.pinField->text(),myNewPIN.pinField->text(),this))  
	{
		Popup::Warning(tr("Changing PIN failed.\n"
		    "Try again, and be sure to use the correct current PIN!"), this);
	}
	else
	{
		Popup::Info(tr("PIN changed successfully."), this);
	}
}


void mainwindow::PushButtonCreateSMS_clicked()
{
	DEBUG5("mainwindow::PushButtonCreateSMS_clicked()\n");	
	if (checkRegistered()==true)
	{
		Popup::WarningWithMemory("SMSMayFailMsg", tr("The text message (SMS) part"
				" of umtsmon is not very stable. It may not work at all.\n"
				"Please set your expectations accordingly..."), this);
		smsdialog myDialog;
		ifATPortAvailableRunExec(&myDialog);
	}
	else
	{
		Popup::Warning(tr("You are not connected to a network yet.\n"
		  "SMS functions are unavailable."), this);
	}
}


void mainwindow::showPPPErrorLogsDialog()
{
	PPPErrorLogsDialog myPPPDialog;
	QStringList::Iterator it;
	if (theConnectionPtr->thePPPDStdOut.count() > 0)
	{
		for (it = theConnectionPtr->thePPPDStdOut.begin(); 
		     it != theConnectionPtr->thePPPDStdOut.end(); ++it )
		{
			myPPPDialog.StdOutTextBox->append(*it);
		}
	}
	else
		myPPPDialog.StdOutTextBox->append(
			tr("<<PPP did not provide any stdout information>>"));

	if (theConnectionPtr->thePPPDStdErr.count() > 0)
	{
		for (it = theConnectionPtr->thePPPDStdErr.begin(); 
		     it != theConnectionPtr->thePPPDStdErr.end(); ++it )
		{
			myPPPDialog.StdErrTextBox->append(*it);
		}
	}
	else
		myPPPDialog.StdErrTextBox->append(
			tr("<<PPP did not provide any stderr information>>"));

	myPPPDialog.exec();	
}


void mainwindow::updateNetworkData()
{
	DEBUG5("mainwindow::updateNetworkData()\n");
	const int mySecondsSinceRefresh=UIState::SEC_REFRESH;
	ConnectionInfo* myCIPtr = TheDeviceManagerSingleton::getConnectionInfoPtr();
	if (myCIPtr == NULL)
		return;

	// there's three situations here:
	// 1) minimized - display in title bar
	// 2) maximized, in network tab - update fields inside that tab
	// 3) maximized, not in network - do nothing
	
	theConnectionPtr->isConnectionActive();

	try { myCIPtr->refresh(); }
	catch (MonthRolloverException)
	{
		Popup::Warning(tr("Your computer just ran into a new month.\n"
					"Due to limitations of pppd, we cannot account new "
					"traffic to the new month. Please close the connection "
					"and reconnect at your earliest convenience."));
	}
	catch (OverThresholdException e)
	{
		if (e.getThreshReached()==OverThresholdException::TRAFFIC)
			Popup::Warning(tr("You are over your data limit treshold."));
		else
			Popup::Warning(tr("You are over your time limit treshold."));
	}
	
	// and update title bar or fields with data
	if (isMinimized())
	{
		DEBUG5("umtsmon minimized - change titlebar\n");
		QString myRT = myCIPtr->getRadiotype();
		QString myTotalName = APPNAME " " + myRT + " " + myCIPtr->getSignalQualityString(); 
		setCaption( myTotalName );
	}
	else
	{
		setCaption(APPNAME);
		// only the last one needs to have the LCDDisplay redraw
		theLCDDisplay->setStrength(myCIPtr->getSignalQuality(), false);
		theLCDDisplay->setRoaming( myCIPtr->isRoaming());
		theLCDDisplay->setOperatorName( myCIPtr->getOperatorName(), false);
		theLCDDisplay->setRadioType( myCIPtr->getRadiotype(), false );
		theLCDDisplay->setUpload(myCIPtr->getCounterBytesSent(), 
								 myCIPtr->getUploadDelta() / mySecondsSinceRefresh, false);
		theLCDDisplay->setDownload(myCIPtr->getCounterBytesReceived(),
								 myCIPtr->getDownloadDelta() / mySecondsSinceRefresh, false);
		theLCDDisplay->setConnectedTime(myCIPtr->getSecondsConnected(), false);

		// alternate in the display between the total time and total bytes
		static int theVisualCounter = 0;
		theVisualCounter++;
		if (theVisualCounter < 3)
			theLCDDisplay->setTotalTraffic(
							myCIPtr->getTotalTraffic(), 
							myCIPtr->getTrafficThresholdStatus(), true);
		else
		{
			theLCDDisplay->setTotalTime(
							myCIPtr->getTotalSecondsConnected(), 
							myCIPtr->getTimeThresholdStatus(), true);
			if (theVisualCounter >4)
				theVisualCounter = 0;
		}
	}
}


// no need to pass any state info - we can get that from
// the UIState class :-D
void mainwindow::setConnectButtons(void)
{
	bool isConnectEnabled    = false;
	bool isConnectOn         = false;
	bool isDisconnectEnabled = false;
	
	switch(theUIStatePtr->getState())
	{
	case UIState::NO_DEVICE:
	case UIState::DETECTING_DEVICE:
	case UIState::DEVICE_UNUSABLE:
	case UIState::SIM_CHECK:
	case UIState::REGISTERING:
	case UIState::PPPDISCONNECTING:
		// these seven states all have none of the buttons enabled
		break;
	case UIState::REGISTERED:
		isConnectEnabled = true;
		break;
	case UIState::PPPCONNECTED:
		isDisconnectEnabled = true;
		// deliberately no break here
	case UIState::PPPCONNECTING:
		isConnectOn = true;
		break;
	}

	// and set all UI items in the right state:
	menu_connection_connect->setOn( isConnectOn );	
	menu_connection_connect->setEnabled( isConnectEnabled );

	menu_connection_disconnect->setOn( !isConnectOn );
	menu_connection_disconnect->setEnabled( isDisconnectEnabled );

	toolbar_button_connect->setEnabled( isConnectEnabled );

	toolbar_button_disconnect->setEnabled( isDisconnectEnabled );

}


bool mainwindow::DisconnectPPP()
{
	DEBUG4("umtsmonmain::DisconnectPPP\n");
	theUIStatePtr->setState(UIState::PPPDISCONNECTING);
	theConnectionPtr->stopConnection();
	return true;
}

bool mainwindow::checkRegistered(bool isShowPopups)
{
	if (theUIStatePtr->getState()==UIState::REGISTERED)
		return true;
	
	if (theUIStatePtr->getState()==UIState::REGISTERING && isShowPopups)
			Popup::Info("There is no connection with a mobile network yet.\n"
			"Please wait a few more seconds. If the problem remains,"
			"try selecting another network operator.");
	
	return false;
}


bool mainwindow::ConnectPPP()
{
	DEBUG3("umtsmonmain::ConnectPPP\n");

	// check if we are registered on a network already
	if (checkRegistered()==false)
		return false;
	// we are - let's start the PPP!
	// startConnection doesn't block for more than 6 seconds 
	//   - so we can do relatively adequate progress reports
	ProgressDialog myProgress(this, tr("Setting up Internet Connection"));
	myProgress.setMaxTime(30);
	// connection will take time anyway, so better show progress from start
	myProgress.start(0);

	// FIXME: all the tricks are still here in the UI code :-(
	theUIStatePtr->setState(UIState::PPPCONNECTING);
	
	if(theConnectionPtr->startConnection()==false)
		goto problems;
	DEBUG2("thePPPConnection.startConnection() returned without indicating problems\n");

	while (theConnectionPtr->isConnectionStarting())
	{
		if (myProgress.runOneSecond() == false)
		{
			// Apparently, the user pressed cancel?!
			goto cleanup;
		}
	}
	myProgress.finish();
	if (theConnectionPtr->hasErrors())
		goto problems;
	emit theUIStatePtr->update();

	return true;
	
	//---------------------
problems:	
	DEBUG1("start PPP NOT successful :-(\n");
// FIXME: Do we still need this warning message?
	Popup::Warning(tr("Failed to start PPP.\n"
	       "Please re-run umtsmon from the commandline with -v5 and check the output."), this);
	
cleanup:
	sleep(1);
	DisconnectPPP();
	emit theUIStatePtr->update();
	return false;
}


void mainwindow::showProfileDialog()
{
	DEBUG5(" profile dialog button clicked\n");
	if (theUIStatePtr->getState() <= UIState::REGISTERED)
	{
		ManageProfiles myDialog;
		myDialog.exec();
	}
	else
	{
		// FIXME: it's better to adjust the button state 
		Popup::Warning(tr("Please do not change\nthe profile whilst connected."), this);
	}
}



void mainwindow::showNetworkOperatorDialog()
{
	DEBUG5(" network operator button clicked\n");

	// Only allow switching of operator if no connection active
	if (theUIStatePtr->getState() <= UIState::REGISTERED)
	{
		NetworkOperatorWizard myDialog;
		myDialog.exec();
		if(myDialog.result() == QDialog::Accepted)
			emit theUIStatePtr->update();
	}
}


/** executes the dialog (i.e. aDialogToExecPtr->exec()) 
 *    if either of these conditions is met:
 *      a) the card has more than one serial port
 *      b) there is no running PPP session.
 *    Otherwise it will throw a warning dialog.
 *  @param aDialogToExecPtr  QDialog subclass to run exec() on
 *  @returns  true if dialog was executed, false if not
 */
bool mainwindow::ifATPortAvailableRunExec( QDialog* aDialogToExecPtr)
{
	DEBUG5("mainwindow::ifATPortAvailableRunExec()\n");
	// test for condition A)
	if (TheDeviceManagerSingleton::getDeviceCapsRef().isSinglePort()==false)
	{
		if (aDialogToExecPtr!=NULL)	
			aDialogToExecPtr->exec();
		return true;
	}

	// test for condition B)
	DEBUG5("only one port available - check if PPP is running?\n");
	return ifPPPInactiveRunExec(aDialogToExecPtr);
}

/** executes the dialog (i.e. aDialogToExecPtr->exec()) 
 *    IF AND ONLY IF there is no running PPP session.
 *    Otherwise it will throw a warning dialog.
 *  @param aDialogToExecPtr  QDialog subclass to run exec() on
 *  @returns  true if dialog was executed, false if not
 */
bool mainwindow::ifPPPInactiveRunExec(QDialog* aDialogToExecPtr)
{
	DEBUG5("mainwindow::ifPPPInactiveRunExec()\n");
	if (theUIStatePtr->getState()==UIState::REGISTERED)
	{
		ConnectionInfoInhibitor myParallelATPreventer;
		if (aDialogToExecPtr!=NULL)	
			aDialogToExecPtr->exec();
		return true;
	}
	Popup::Warning(tr("PPP is active, cannot run this in parallel."), this);
	return false;
}



void mainwindow::showRadioPreferences()
{
	DEBUG1("   showRadioPreferences dialog requested\n");

	//  this window is modeless - one can keep it open
	RadioPreferences myDialog;
	myDialog.exec();
	myDialog.disconnect();
}


void mainwindow::menu_settings_SetLanguage_activated()
{
	SetLanguageDialog myDialog;
	myDialog.setI18nPtr(myI18n);
	myDialog.exec();
}

void mainwindow::setI18nRef( Internationalisation& anI18nRef )
{
	myI18n = &anI18nRef;
}


void mainwindow::menu_settings_AutoConnect_activated()
{
	Profile myProfile;
	if (myProfile.getWasConnectionSuccess())
		TheSettingsSingleton::getQSRef().writeEntry(theGeneralAutoConnectSettingName,
			menu_settings_AutoConnect->isOn());
	else
		Popup::Info(tr("You can only enable autoconnect"
			" once you have made a successful connection."), this);
}

void mainwindow::setNewConnectionBasePtr(void)
{
	// this function is called by UIState
	// if so - we'd better update the Connection pointer
	ConnectionBaseClass* newPtr;
	if (TheDeviceManagerSingleton::getDeviceCapsRef().getDeviceType()
			== DeviceCapabilities::OPTION_HSO)
		newPtr = new HSOConnection(*theConnectionPtr);
	else
		newPtr = new PPPConnection(*theConnectionPtr);
	delete theConnectionPtr;
	theConnectionPtr = newPtr;
}
