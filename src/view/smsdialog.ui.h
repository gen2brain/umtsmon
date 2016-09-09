/* umtsmon - a program to control/monitor your UMTS card
 * Copyright (C) 2007  Klaas van Gend, Christofer Wesseling
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
** You should not define a constructor or destructor in this file.s
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
#include "umtsmon_global.h"
#include "Popup.h"
#include "Query.h"
#include "ValidatePhoneNumber.h"
#include "ConnectionInfo.h"
#include "ProgressDialog.h"
#include <assert.h>



void smsdialog::init()
{
    smsToNumber->setValidator(new ValidatePhoneNumber(smsToNumber));
}

void smsdialog::smsTextChanged()
{
	int myNumberOfChars = smsMessage->length();
	if (myNumberOfChars < 0)
		myNumberOfChars = 0;
	smsCharInMessages->setText(QString::number(myNumberOfChars));
	
	// FIXME: there's a magic number here... 
	smsCharLeft->setText(QString::number(162-myNumberOfChars));
}

void smsdialog::pushButton_sendSMS()
{
	// prevent ConnectionInfo from interfering
	// for as long as this class is in scope.
	ConnectionInfoInhibitor myInhibitor;
	
	{
		Query myQuery("AT+CPMS=\"SM\"");
		if (myQuery.runUntilDone() != Query::OK)
		{
			DEBUG1("THE RESULT WAS NOT GOOD!\n");
			assert(false);
		}
	}
	{
		Query myQuery("AT+CMGF=1");
		if (myQuery.runUntilDone() != Query::OK)
		{
			DEBUG1("THE RESULT WAS NOT GOOD!\n");
			assert(false);
		}
	}

	//  thanks to the 'ValidatorPhoneNumber' class, the input is already OK
	//  apart from spaces and a surplus of starting pluses...
	QString mySMSNumber = smsToNumber->text();
	ValidatePhoneNumber myValidator(NULL);
	myValidator.fixup(mySMSNumber);
	if (mySMSNumber.length() > 0)
	{
		bool isOK = false;
		ProgressDialog myPD(this, tr("sending SMS..."));
		myPD.setMaxTime(12);
		myPD.start(0);		// show immediately
		Query myQuery("AT+CMGS=\"" + mySMSNumber +"\"");
		// the card does not answer "OK" here, it prompts for a message
		myQuery.run();
		if (!myQuery.hasAnswer())
			goto skipTheRest;
		// pop off the "AT+CMGS=..." echo
		myQuery.getAnswer();
		if (myQuery.getAnswer().first() != "> ")
		{
			DEBUG1("THE RESULT WAS NOT GOOD!\n");
			assert(false);
		}

		// send message and a closing Ctrl-Z
		myQuery.sendMore(smsMessage->text()+ QChar(26));
		while ((isOK = myPD.runOneSecond()))
		{
			Query::ReturnCode myCode = myQuery.receiveMore();
			if (myCode == Query::OK)
			{
				DEBUG5("sent sms: '%s'\n",  myQuery.getAnswer().first().ascii());
				break;
			}
			if (myCode == Query::ERROR)
			{
				DEBUG1("Received error from card on SMS:-(\n");
				isOK = false;
				break;
			}
		}
skipTheRest:
		myPD.finish();
		if (isOK)
		{
			Popup::Info(tr("SMS sent (according to the device)."));				
		}
		else
		{
		    Popup::Warning(tr("SMS was not sent!"));
		}
	}
}
