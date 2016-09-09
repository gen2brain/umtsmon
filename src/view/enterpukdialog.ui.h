/* umtsmon - a program to control/monitor your UMTS card
 * Copyright (C) 2006  Christofer Wesseling
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
#include "Popup.h"

#include <qstring.h>

void EnterPUK::init(void)
{
    DEBUG5("Enter PUK & new PIN::init called\n");
    mySIMHandlerPtr = NULL;
}

void EnterPUK::OK_Button_clicked()
{
	DEBUG5("validate PUK:\n");
	
	bool isOK = false;



	if ( pinField->text().length() >= 4 )
		if ( pukField->text().length() >= 8 )
			isOK = true;
			
	if (isOK == false)
	{		
		errorMessage->setText(tr("PIN or PUK was not valid!")); 
	}
	else
	{
		// if everything is fine...
		if (mySIMHandlerPtr->setPUK(pukField->text(),pinField->text(), this)==true)
		{
			accept();
		}
		else
		{
			switch(mySIMHandlerPtr->isPinCodeRequired(this))
			{
			case SIMHandler::PIN_SIM:
	
				// now he seems to want the pin...
				// no yet implemented...can this case happen? -> christofer: No!
				Popup::Critical(tr("PIN needed after PUK?\nThat's unexpected!\nPlease mail info to: umtsmon-develop@sourceforge.net"));
				exit(35);
				break;
 			case SIMHandler::PIN_READY:
				// after PUK was set, Option-cards gives us a "READY"
				errorMessage->setText(tr("success :-)"));
				sleep(1);
				this->accept();
					
				break;
			case SIMHandler::PIN_PUK:
				errorMessage->setText(tr("PUK was not correct."));
				break;
			default:
				Popup::Critical(tr("Something went wrong in the PIN procedures\numtsmon cannot recover\nPlease try a restart and/or\ninsert the SIM card in a GSM phone"));
				exit(35);
				break;
		}
	}
}

// end everthing is fine...
}

void EnterPUK::setSIMHandlerPtr( SIMHandler *anSIMHandlerPtr )
{
	mySIMHandlerPtr = anSIMHandlerPtr;
}

/// enables the validator. validator just works if init() is finished :(
void EnterPUK::enableValidator()
{
	pukField->setValidator(new ValidatePIN(pukField));
	pinField->setValidator(new ValidatePIN(pinField));
}
