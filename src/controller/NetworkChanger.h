/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2006, 2007, 2008 Klaas van Gend
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

#ifndef NETWORKCHANGER_CLASS
#define NETWORKCHANGER_CLASS

#include "OperatorList.h"
#include "ConnectionInfo.h"

#include <qstring.h>

class Query;

/// created to find all networks and change over to a new one
class NetworkChanger
{
public:
	NetworkChanger(void) : theQueryPtr(NULL) {};

	virtual ~NetworkChanger(); 


public:
	/** Sets the device to search for the best operator itself
	 *  (i.e. the AT+COPS=0 command)
	 */
	bool setCardToAutomatic(void) const;
	
	/// the possible answers to AT+COPS?
	enum RegistrationMode {AUTOMATIC, MANUAL, DEREGISTER, SETONLY, MAN_AUTO, MODE_ERROR};
	
	/** retrieves the network registration mode from the device
	 * @returns the registration mode
	 */ 
	RegistrationMode getRegistrationState(void) const;
	
	/** sends the AT commands to start searching for networks.
	    @returns  true if successful in sending, false if an error occurred
	*/
	bool startSearching(void);

	/** returns true if the search is completed. 
	    If so, it will call parseAnswer()
        */
	bool isSearchCompleted(void);
	
	/// closes up anything related to the search
	void stopSearch(void);

	
	enum SwitchResults{ NOT_READY_YET, OK, IS_CURRENT, ERROR};
	
	/** asks the card to switch over to the specified network
	 *  @param  theNetworkID  theNetworkID from a CurrentNetworkItem
	 *  @returns  false if the network is forbidden
	 */
	SwitchResults startSwitchToNetworkID(unsigned long aNetworkID);

	/** returns true if the switching is completed. 
        */
	SwitchResults isSwitchCompleted(void);
	
	/** if and only if there is an error, this member can
	 *  take all the output and return it for display
	 *  @returns a QString with the error message, empty if none
	 */
	QString getErrorMessage(void);

public:
	OperatorList theOperatorList;
	
private:
	Query* theQueryPtr;

	/** if an instance of the ConnectionInfoInhibitor exists, 
	 * ConnectionInfo is prohibited from sending out AT commands. 
	 * That's convenient as extra AT commands during network selection
	 * might f*ck up the network selection.
	 * As this object will exist always if a NetworkChanger exists, we're
	 * done quickly :-)
	 */
	ConnectionInfoInhibitor myConnectionInfoInhibitor;
};


#endif // NETWORKCHANGER
