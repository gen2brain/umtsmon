/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2008  Klaas van Gend
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
#include "Popup.h" 
#include "popupwithmemory.h"
#include "yesnowithmemory.h"

#include <qstring.h>
#include <qlabel.h>

void 
Popup::WarningWithMemory(
		const QString& aName, 
		const QString& aMessage, 
		QWidget* currentWidget)
{
	DEBUG2("Warning with Memory Name: %s, Message: %s\n", aName.ascii(), aMessage.ascii());
	
	PopupWithMemory myDialog (currentWidget);
	
	// do we need to show?
	if (myDialog.setMemoryName(aName)==false)
	{
		DEBUG3("  Message not shown due to user preference\n");
		return;
	}

	// if yes: display the message
	myDialog.textLabel->setText(aMessage);
	myDialog.setCaption(APPNAME + QObject::tr(" message"));
	myDialog.exec();
};


bool 
Popup::YesNoQuestionWithMemory(
		const QString& aName, 
		const QString& aMessage, 
		QWidget* currentWidget)
{
	DEBUG2("YesNo with Memory Name: %s, Message: %s\n", aName.ascii(), aMessage.ascii());
	
	YesNoWithMemory myDialog (currentWidget);
	
	// do we need to show?
	if (myDialog.setMemoryName(aName)==false)
	{
		DEBUG3("  Message not shown due to user preference\n");
	}
	else
	{
		// if yes: display the message
		myDialog.textLabel->setText(aMessage);
		myDialog.setCaption(APPNAME + QObject::tr(" message"));
		myDialog.exec();
	}
	
	return myDialog.theResult;
};
