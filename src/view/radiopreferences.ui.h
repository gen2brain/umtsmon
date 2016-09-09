/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2007, 2009  Klaas van Gend
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
#include "radiopreferences_defines.h"
#include "TheDeviceManagerSingleton.h"
#include "TheSettingsSingleton.h"
#include "Popup.h"


void RadioPreferences::init()
{
	loadSettings();
}

void RadioPreferences::loadSettings()
{
	DEBUG5("RadioPreferences::loadSettings()\n");
	// do we have radio type settings anyway?
	DeviceRadioCapabilities& theDRCRef = TheDeviceManagerSingleton::getDeviceRadioCapsRef();
	bool hasRadio = theDRCRef.hasRadioPreferences();
	comboBoxRadioTypes->setEnabled(hasRadio);
	textLabelRadioExplanation->setEnabled(hasRadio);
	textLabelRadioTypes->setEnabled(hasRadio);
	// and populate the combobox in that case...
	if (hasRadio)
	{
		DEBUG4("***Has Radio Preferences\n");
		comboBoxRadioTypes->clear();
		comboBoxRadioTypes->insertStringList(theDRCRef.getRadioPreferences());
		comboBoxRadioTypes->setCurrentItem(
			TheSettingsSingleton::getQSRef().readNumEntry(
			THE_RADIO_PREF_ROOT_NAME THE_RADIOTYPE_SETTING, 
			theDRCRef.getDefaultRadioPreference()));
	}
}

void RadioPreferences::saveSettings()
{
	// writing the setting only makes sense when we have one...
	if (TheDeviceManagerSingleton::getDeviceRadioCapsRef().hasRadioPreferences())
	{
		TheSettingsSingleton::getQSRef().writeEntry(
		  THE_RADIO_PREF_ROOT_NAME THE_RADIOTYPE_SETTING,
		  comboBoxRadioTypes->currentItem()); 
	}
}


void RadioPreferences::comboBoxRadioType_activated(int anEntry)
{
    DEBUG5("RadioPrefs::comboBoxRadioType_activated: currently selected: '%s' is %d - %d\n\n", 
    		comboBoxRadioTypes->currentText().ascii(),
    		comboBoxRadioTypes->currentItem(), anEntry);

    if (!TheDeviceManagerSingleton::getDeviceRadioCapsRef().setRadioPreferences(anEntry))
    {
    	Popup::Warning(tr("Failed to set your radio technology preference."), this);
    }

}
