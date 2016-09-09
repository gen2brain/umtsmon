/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2009  Klaas van Gend
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

bool YesNoWithMemory::setMemoryName(const QString aName)
{
	assert( !aName.isEmpty() );
	theUniqueName = "/ShowPopupWithMemory/" + aName;
	theResult = TheSettingsSingleton::getQSRef().readBoolEntry( theUniqueName+".answer", false);

	// true means we are going to show the dialog
	return TheSettingsSingleton::getQSRef().readBoolEntry( theUniqueName+".showagain", true);
}

void YesNoWithMemory::buttonYES_clicked()
{
	theResult = true;
	button_clicked();
}

void YesNoWithMemory::buttonNO_clicked()
{
	theResult = false;
	button_clicked();
}

void YesNoWithMemory::button_clicked()
{
	// if box is checked, that means the user doesn't want to see the box again
	// i.e. we need to write false
	assert( !theUniqueName.isEmpty() );
	
	TheSettingsSingleton::getQSRef().writeEntry( theUniqueName+".showagain", checkBoxDoNotShowAgain->isChecked()==false);
	TheSettingsSingleton::getQSRef().writeEntry( theUniqueName+".answer", theResult);
	
	accept();
}
