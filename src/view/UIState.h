/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2008, 2009 Klaas van Gend
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

#ifndef UISTATE_H_
#define UISTATE_H_

#include "mainwindow.h"
#include <qobject.h>

/** this class maintains a state machine
 *  it observes changes in the connection and hardware and acts on those 
 * 
 *  This class only has one instance: owned and contained by mainwindow
 *  
 *  Note that this class is a friend class of mainwindow!
 */
class UIState : public QObject, public ConnectionObserverInterface
{
	Q_OBJECT
	
public:
	enum UmtsmonState
	{                       // (all entries starting with * are implemented)
		NO_DEVICE,          /// * start
		DETECTING_DEVICE,   /// * working on finding a device
		DEVICE_UNUSABLE,    /// big errors...
		SIM_CHECK,          /// * device present, working on SIM password checks
		REGISTERING,        /// device operational, not registered in network yet
		REGISTERED,         /// device operational, network found - ready to start PPP
		PPPCONNECTING,      /// 
		PPPCONNECTED,       /// fully operational - all connected & ppp running
		PPPDISCONNECTING,
	};

	enum AutoConnectStates 
	{
		AUTOCONNECT_DISABLED,   /// the current setting for autoconnect is disabled 
		AUTOCONNECT_ENABLED,    /// the current setting for autoconnect is enabled
		AUTOCONNECT_CANCELLED   /** autoconnect is enabled, but the user cancelled it
		                         *  therefore, for this session it is disabled */
	};
	AutoConnectStates theAutoConnectState;
	
public:
	/// constructor
	UIState(mainwindow* aMainWindowPtr);
	
	/// destructor
	virtual ~UIState();
	
	/** this is a callback from PPPConnection
	 *  the member will process the new state and switch states accordingly
	 *  
	 *  @param aNewState the new state of the PPP Process
	 */
	virtual void newConnectionState(ConnectionState aNewState);
	
	/** set the state, which will be communicated to all required subsystems
	 *  * @param aNewState the new state from enum UmtsmonState
	 */
	void setState(UmtsmonState aNewState);
	
	/// returns the state of umtsmon
	UmtsmonState getState(void) 
		{return theState;}
	
	/// time between two refreshes in seconds
	static const unsigned int SEC_REFRESH=3;
	
	/// number of refreshes between two device detects
	static const unsigned int DEVICE_DETECT_REFRESH_COUNT = 5;
	
public slots:
	void update(void);

private:
	mainwindow* theMainWindowPtr;

	UmtsmonState theState;
	
	void handleSIMCHECK(void);
	void startUpdateTimer(void);
	void stopUpdateTimer(void);
	
	/// this timer is used to run the updates from
	QTimer* theUpdateTimer;
	
	/// returns a spinner character in the order / - \ |  
	QChar spinner(void) const;
};


#endif /* UISTATE_H_ */
