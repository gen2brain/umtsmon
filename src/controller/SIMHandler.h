/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2006  Klaas van Gend
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

#ifndef SIMHANDLER_CLASS
#define SIMHANDLER_CLASS

#include <qstring.h>

// forward declarations
class Query;
class Device;
class QWidget;

/** the SIMHandler class contains everything related to the SIM card,
 *  including PIN and PUK code handling
 * 
 *  At this moment, there is no persistant data in this class, it is more
 *  a "class utility" than a real class.
 */
class SIMHandler
{
public:

	/** this enum contains numerical IDs for all possible answers
	*  on a AT+CPIN question
	*  FIXME: Add pointer to where this list was taken from
	*
	*  (the corresponding const text strings are in SIMHandler.cpp:
	*   static const char*  theCPIN_Answers[] )
	*/
	enum PIN_ReturnCode
	{
		PIN_READY =  0,
		PIN_SIM,
		PIN_PUK,
		PIN_PH_SIM,
		PIN_PH_SIM_PUK,
		PIN_PH_FSIM,
		PIN_PH_FSIM_PUK,
		PIN_PIN2,
		PIN_PUK2,
		PIN_PH_NET,
		PIN_PH_NET_PUK,
		PIN_PH_NETSUB,
		PIN_PH_NETSUB_PUK,
		PIN_PH_SP,
		PIN_PH_SP_PUK,
		PIN_CORP,
		PIN_CORP_PUK,
		// from here the codes are *not* ETSI/3GPP defined, but added by Klaas:
		ERROR_BEYOND_LAST,
		ERROR_SIMINTERFACE_NOT_STARTED_YET = 200,
		ERROR_ERROR,
		ERROR_UNKNOWN = ERROR_BEYOND_LAST
	};

	/// constructor
	SIMHandler(void); 
	/// destructor
	virtual ~SIMHandler(void) {}; 

	/// checks wheter PIN/PUK is needed and starts the needed dialogs
	void askForPIN(void);

	/** checks if a PIN code must be entered before the SIM can be accessed
	* @param  aWPtr  pointer to the Widget in front of which to put a Progress Dialog
	* @return returns wheter a PIN or PUK or something else is needed
	*/	
	PIN_ReturnCode isPinCodeRequired(QWidget* aWPtr=NULL);

	
	/** tries to set the puk and a new PIN - and control if it was succsessfully
	* @param  aPuk   the PUK
	* @param  newPIN the new PIN
	* @param  aWPtr  pointer to the Widget in front of which to put a Progress Dialog
	* @returns true: new PIN has been set..  false: cant't set new PIN (wrong PUK, incompatible PIN or AT error)
	*/
	bool setPUK(const QString& aPUK, const QString& newPIN, QWidget* aWPtr=NULL);	
	
	/** tries to set the pin and check if it was succsessfully
	* @param  aPIN  the new PIN
	* @param  aWPtr pointer to the Widget in front of which to put a Progress Dialog
	* @return true: setting the pin was successfully, false: PIN incorrect, or AT error 
	*/
	bool setPIN(const QString& aPIN, QWidget* aWPtr=NULL);

	/** checks if a PIN is needed to access the sim-card
	* @return true: ask-for.PIN is active, user have to enter a pin to access the sim-card false: no pin will be asked 
	*/
	bool isPINProtectionEnabled(void);
	
	/** trys to set the ask-for-pin status
	* @param wished pin-active-status
	* @param current pin
	* @param aWPtr  pointer to the Widget in front of which to put a Progress Dialog
	* @return true: change pinstatus succesfully, false: change PIN status failed 
	*/
	bool setPINActive(bool aPINActiceStatus, const QString &aCurrentPIN, QWidget* aWPtr);
	
	/** changes the actial pin to a new one
	* @param current pin
	* @param new pin
	* @param aWPtr  pointer to the Widget in front of which to put a Progress Dialog
	* @return true: pin changed, false: change pin failed 
	*/
	bool setNewPIN(const QString &aCurrentPIN, const QString &aNewPIN, QWidget* aWPtr);
	

private:

	/** Return a description for PIN RETURN Codes
	* @param  PIN_ReturnCode aCode
	* @return QString
	*/
	const QString getCPINDescription(PIN_ReturnCode aCode);


	/** Returns if the Result of the Query was "OK" or not...
	* @param  aQueryString the Query, for which you want to check the result...
	* @param  aWPtr pointer to the Widget in front of which to put a Progress Dialog
	* @return true: when te last query had returned an "OK", false if not...
	*/
	bool getQueryReturnOK(const QString& aQueryString, QWidget* aWPtr=NULL);
};


#endif // SIMHANDLER_CLASS
