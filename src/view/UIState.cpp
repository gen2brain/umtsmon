/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2008, 2009  Klaas van Gend
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

#include "UIState.h"

#include "TheDeviceManagerSingleton.h"
#include "ConnectionInfo.h"

// FIXME: this include should vanish - setting LCD should go through mainwindow
#include "LCDDisplay.h"

#include <qtimer.h>
#include <assert.h>

// member order in this file:
// * constructors 
// * destructor
// * all other members in alphabetical order


UIState::UIState(mainwindow* aMainWindowPtr) 
	:	QObject(aMainWindowPtr, "UIState instance"),
		theMainWindowPtr(aMainWindowPtr), 
		theState(NO_DEVICE),
		theUpdateTimer(NULL)
{
	startUpdateTimer();
	
	// only when both the user has requested autoconnect *and*
	// we know that in history there has been successful connetions,
	// enable autoconnect
	Profile myProfile;
	if (TheSettingsSingleton::getQSRef().readBoolEntry(theGeneralAutoConnectSettingName, false)
			&& myProfile.getWasConnectionSuccess())
		theAutoConnectState = AUTOCONNECT_ENABLED;
	else
		theAutoConnectState = AUTOCONNECT_DISABLED;
}	

UIState::~UIState() 
{
	stopUpdateTimer();
}

void UIState::handleSIMCHECK(void)
{
	// lets check the PIN-Status... so we can make the menu user friendly... :)
	SIMHandler mySIMHandler;
	mySIMHandler.askForPIN(); 
	if(!mySIMHandler.isPINProtectionEnabled())
	{
		DEBUG5(" PIN protection on SIM is disabled \n");		    
		//let the user see whats active and not...
		theMainWindowPtr->setPINmenu(mainwindow::PIN_DISABLED);
	}
	else
	{
		DEBUG5(" PIN protection on SIM is enabled \n");
		//let the user see whats active and not...
		theMainWindowPtr->setPINmenu(mainwindow::PIN_ENABLED);
	}
}


void 
UIState::newConnectionState(ConnectionState aNewState)
{
	DEBUG2("UIState::newConnectionState(%d) callback called\n", aNewState);
	switch (aNewState)
	{
	case STOPPED:
		// this callback also gets called when we register our callback 
		// with PPPConnection - so only act on it when we know it makes sense
		// in real life, this entry gets called when we disconnect OR when pppd is terminated
		// either through a crash or a signal
		if (getState() >= REGISTERED)
			setState(REGISTERED);
		break;
	case STARTING:
		assert (getState() >= REGISTERED);
		setState(PPPCONNECTING);
		break;
	case RUNNING:
	case NOT_OURS_RUNNING:
		assert (getState() >= REGISTERED);
		setState(PPPCONNECTED);
		break;
	case ERROR:
		assert (getState() >= REGISTERED);
		setState(PPPDISCONNECTING);
		emit theMainWindowPtr->showPPPErrorLogsDialog();
		break;
	}
}			


void UIState::setState(UIState::UmtsmonState aNewState)
{
	DEBUG2("\n\nUIState::setState(%d -> %d)\n", theState, aNewState);
	switch(aNewState)
	{
	case DEVICE_UNUSABLE:
	case NO_DEVICE:
		theState = NO_DEVICE;
		break;
	case DETECTING_DEVICE:
		theState = aNewState;
		if (TheDeviceManagerSingleton::me().tryDeviceDetection() == true)
		{
			theMainWindowPtr->setNewConnectionBasePtr();
			setState(SIM_CHECK);
		}
		else
			setState(NO_DEVICE);
		break;
	case SIM_CHECK:
		theState = aNewState;
		handleSIMCHECK();
		setState(REGISTERING);
		break;
	case REGISTERING:
		theState = aNewState;
		// NOTHING TO DO
		break;
	case REGISTERED:
		theState = aNewState;
		if (Profile::getProfileList().size() == 0)
		{
			Popup::WarningWithMemory("PleaseCreateProfile", tr("Before you connect, please create a \"profile\"."), theMainWindowPtr);
		}
		if (theAutoConnectState == AUTOCONNECT_ENABLED)
		{
			// FIXME: move to UIState
			theMainWindowPtr->ConnectPPP();
		}
		break;
	case PPPDISCONNECTING:
		// disable autoconnect for this session
		theAutoConnectState = AUTOCONNECT_CANCELLED;
		// deliberately no break; here
	case PPPCONNECTING:
	case PPPCONNECTED:
		// nothing to do - it's all still in mainwindow.ui.h
		theState = aNewState;
		break;
	}
}

QChar UIState::spinner(void) const
{
	// this member is not really an exemplary C++ member,
	// but it works :-)
	
	static int myState = 0;
	myState++;
	switch(myState)
	{
	case 1:
		return '/';
	case 2:
		return '-';
	case 3:
		return '\\';
	default:
		myState = 0;
		return '|';
	}
}

void UIState::startUpdateTimer(void)
{
	DEBUG5("***startUpdateTimer\n");
	if (theUpdateTimer == NULL)
	{
		// setup the refreshtimer for network statistics and signal quality
		theUpdateTimer = new QTimer(this, "refreshtimer");
		connect(theUpdateTimer, SIGNAL(timeout()), SLOT(update()));
		theMainWindowPtr->updateNetworkData();
		theUpdateTimer->start(SEC_REFRESH * 1000, FALSE);
	}
	else
	{
		DEBUG1("refreshtimer not null?!?");
	}
	// whether we set a timer or just killed it - let's refresh now
	theMainWindowPtr->updateNetworkData();
}

void UIState::stopUpdateTimer()
{
	DEBUG5("stopUpdateTimer\n");
	if (theUpdateTimer != NULL)
	{
		theUpdateTimer->stop();
		delete theUpdateTimer;
		theUpdateTimer = NULL;
	}
}

void UIState::update(void)
{
	DEBUG2("***UIState::update(void), state=%d\n", theState);
	switch(theState)
	{
	case NO_DEVICE:
		theMainWindowPtr->setConnectButtons();
		// only run autodetect every DEVICE_DETECT_REFRESH_COUNTxSEC_REFRESH
		// (except the first time)
		static int myCounter = 1;
		myCounter--;
		if (myCounter>0)
		{
			// in these case, it doesn't make sense to update network data
			theMainWindowPtr->theLCDDisplay->setOperatorName(tr("No device (yet)... (%1)").arg(myCounter), true);
			break;
		}
		myCounter = DEVICE_DETECT_REFRESH_COUNT;
		setState(DETECTING_DEVICE);
		// deliberately no break here...
	case DETECTING_DEVICE:
		// in these case, it doesn't make sense to update network data
		theMainWindowPtr->theLCDDisplay->setOperatorName(tr("Detecting device %1").arg(spinner()), true);
		break;
	case SIM_CHECK:
		// in these case, it doesn't make sense to update network data
		theMainWindowPtr->theLCDDisplay->setOperatorName(tr("SIM check %1").arg(spinner()), true);
		break;
	case DEVICE_UNUSABLE:
		// in these case, it doesn't make sense to update network data
		theMainWindowPtr->theLCDDisplay->setOperatorName(tr("No device"), true);
		break;
	case REGISTERING:
	{
		// ask a ConnectionInfo if we are operator connected already
		ConnectionInfo* myCIPtr = TheDeviceManagerSingleton::getConnectionInfoPtr();
		assert(myCIPtr != NULL);
		
		// none of the exceptions can be thrown here as we're not connected 
		myCIPtr->refresh();
		if (myCIPtr->isDeviceRegistered())
			setState(REGISTERED);
		theMainWindowPtr->theLCDDisplay->setOperatorName(tr("Registering %1").arg(spinner()), true);
		break;
	}
	case REGISTERED:
		theMainWindowPtr->setConnectButtons();
		theMainWindowPtr->updateNetworkData();
		break;
	case PPPCONNECTING:
		theMainWindowPtr->setConnectButtons();
		theMainWindowPtr->updateNetworkData();
		theMainWindowPtr->theLCDDisplay->setOperatorName(tr("Connecting %1").arg(spinner()), true);
		break;
	case PPPCONNECTED:
		theMainWindowPtr->setConnectButtons();
		theMainWindowPtr->updateNetworkData();
		break;
	case PPPDISCONNECTING:
		theMainWindowPtr->setConnectButtons();
		theMainWindowPtr->updateNetworkData();
		theMainWindowPtr->theLCDDisplay->setOperatorName(tr("Disconnecting %1").arg(spinner()), true);
		break;
	}
}
