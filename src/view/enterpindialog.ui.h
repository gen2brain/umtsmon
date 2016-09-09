/* umtsmon - a program to control/monitor your UMTS card
 * Copyright (C) 2006  Klaas van Gend
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
#include "SIMHandler.h"

#include <qstring.h>

#include <unistd.h>

void EnterPIN::init(void)
{
    DEBUG5("EnterPIN::init called\n");
    mySIMHandlerPtr = NULL;
}


void EnterPIN::buttonOk_clicked()
{
	DEBUG5("validate\n");

	//min 4 digits.
	if ( pinField->text().length() >= 4 )
	{
	    // let the caller know, that the users wants to give him the data.. accept :)
	    accept();
	}
	else
	{
		errorMessage->setText(tr("This is not a valid PIN code."));
		thePIN = "";
	}

	// if we got here, something went wrong. 
	// Either was the PIN code wrong, or it was not a valid one
	//    (because the only true way out of here is the accept() above,
	//     not calling accept() means that the dialog will stay open)
}

void EnterPIN::setSIMHandlerPtr( SIMHandler *anSIMHandlerPtr )
{
	mySIMHandlerPtr = anSIMHandlerPtr;
}


/// enables the validator. validator just works if init() is finished :(
void EnterPIN::enableValidator()
{
	pinField->setValidator(new ValidatePIN(pinField));
}


QString EnterPIN::getPIN()
{
    return  pinField->text();
}

