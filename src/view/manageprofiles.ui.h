/* ManageProfiles
 * Copyright (C) 2006  Christofer Wesseling
 * Copyright (C) 2007  Christofer Wesseling, Klaas van Gend
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
#include "profiledialog.h"

#include <qlineedit.h>

// load the Data into the Dialog:  
void ManageProfiles::init()
{
	// add the profiles to the gui
	listBox_profiles->clear(); 
	listBox_profiles->insertStringList(Profile::getProfileList());	

	// select the default Profile in the combo
	if (listBox_profiles->count() > 0)
	{
		unsigned int i=0;
		Profile myDefaultProfile;
		while(i < listBox_profiles->count())
		{
			if (listBox_profiles->text(i) == myDefaultProfile.getProfileName())
			{
				listBox_profiles->setCurrentItem(i);
				listBox_profiles->changeItem(
					QPixmap::fromMimeSource("ok.png"), 
					listBox_profiles->text(i), i);
			}
			i++;
		}
		enableButtons(true);
	}
	else
		enableButtons(false);
}


void ManageProfiles::enableButtons(bool isEnabled) 
{
	pushButton_edit_profile  ->setEnabled(isEnabled);
	pushButton_set_active    ->setEnabled(isEnabled);
	pushButton_delete_profile->setEnabled(isEnabled);
}


void ManageProfiles::pushButton_add_profile_clicked()
{
	NewProfileDialog myNewPrfDialog(this); //"this" means, put it in front of "this"-dialog	
	myNewPrfDialog.exec();
	if(myNewPrfDialog.result() == QDialog::Rejected)
	{
		DEBUG4("NewProfile: User clicked cancel\n");
		return; 
	}
	QString myNewProfileName = myNewPrfDialog.newProfileName->text();
	DEBUG2("New profile name: '%s'\n", myNewProfileName.ascii());

	// FIXME: check if profile name already exists
	
	// create the new profile and fill it up with default values 
	Profile myNewProfile(myNewProfileName);
	if (myNewProfile.getAPN().isEmpty())
		myNewProfile.setAPN(tr("please_replace_me"));
	myNewProfile.save();

	// insert into listbox
	init();
	
	// enable our new item in the list box
	listBox_profiles->setCurrentItem(listBox_profiles->findItem(myNewProfileName));
	// and run to the edit profile dialog 
	emit pushButton_edit_profile_clicked();
}


void ManageProfiles::pushButton_delete_profile_clicked()
{
	// figure out which profile to delete
	int myItemNr = listBox_profiles->currentItem();
	QString myProfileToDelete = listBox_profiles->currentText();
	
	DEBUG2("Request to delete profile %d: '%s'\n", myItemNr, myProfileToDelete.ascii());

	Profile myProfile(myProfileToDelete);
	myProfile.remove();

	listBox_profiles->removeItem(myItemNr);
	
	// and refresh the listBox
	init();	
}

void ManageProfiles::pushButton_edit_profile_clicked()
{
	// figure out which profile to edit
	QString myProfileToEdit = listBox_profiles->currentText();
	
	DEBUG2("Request to edit profile '%s'\n", myProfileToEdit.ascii());
	
	profiledialog myDialog;
	if (myDialog.setProfile(myProfileToEdit) == false)
		return;
	myDialog.exec();
	
	// and refresh the listBox
	init();	
}


void ManageProfiles::pushButton_set_default_profile()
{
// 	// figure out which profile to set default
	QString myProfileToSet = listBox_profiles->currentText();
	DEBUG2("Request to set profile as active '%s'\n", myProfileToSet.ascii());
	Profile::setActiveProfile(myProfileToSet);
	// refresh dialog contents
	init();
}
