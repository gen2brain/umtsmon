/* profiledialog
 * Copyright (C) 2006  Christofer Wesseling
 * Copyright (C) 2007,2008  Christofer Wesseling, Klaas van Gend
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
#include <assert.h>

// load the Data into the Dialog:  
bool profiledialog::setProfile(QString aProfileName)
{
	assert(aProfileName.isEmpty()==false);
	// FIXME: Add check if aProfileName actually exists
	theProfile = Profile(aProfileName);
	// set the fields/checkboxen using the getters defaults from Profile...
	updateCurrentProfile();	
	return true;
}



void profiledialog::pushButtonSaveProfile_clicked()
{
	// do we have a username/password set? 
	// if not: let's warn the user.
	if (connection_username->text().stripWhiteSpace().isEmpty() ||
		connection_password->text().stripWhiteSpace().isEmpty())
	{
		// FIXME: a yes-no question with memory would work better here...
		Popup::WarningWithMemory(tr("empty user/pass"), 
				tr("Are you sure you want to leave username or password empty?<br>"
				   "If you do not specify a username and/or password, "
				   "connection with the operator may not work as expected.<br>"
				   "Most operators expect dummy (but non-empty) values here.") , this);
	}
	
	// is this the only profile? then set it as current profile
	if (Profile::getProfileList().empty())
		Profile::setActiveProfile(theProfile.getProfileName());

	//lets send the settings to de Profile... :)
	theProfile.setAPN(connection_apn->text().stripWhiteSpace());
	theProfile.setUsername(connection_username->text().stripWhiteSpace());
	theProfile.setPassword(connection_password->text().stripWhiteSpace());

	theProfile.setNoIpDefault(connection_routing_noipdefault->isChecked());
	theProfile.setIsDefaultRoute(connection_routing_defaultroute->isChecked());
	theProfile.setReplaceDefaultRoute(connection_routing_replaceDefaultRoute->isChecked());
	theProfile.setUsePeerDns(connection_routing_usepeerdns->isChecked());
	theProfile.setNoCompression(connection_noCompression->isChecked());
	theProfile.setNoCCP(connection_noCCP->isChecked());
	theProfile.setNoAuth(connection_noAuth->isChecked());

	theProfile.setWarnOnDataThreshold(checkBox_warn_data_threshold->isChecked());
	theProfile.setThresholdPercentage(spinBox_data_treshold->value());
	theProfile.setHomeDataLimit(spinBox_home_data_limit->value());
	theProfile.setRoamingDataLimit(spinBox_roaming_data_limit->value());

	theProfile.setHomeTimeLimit(spinBox_home_time_limit->value());
	theProfile.setRoamingTimeLimit(spinBox_roaming_time_limit->value());

	theProfile.save();
	accept();
}


void profiledialog::pushButton_Cancel_clicked()
{
	if (checkForUnsavedData())
		reject();
}


void profiledialog::pushButtonLoginHelp_clicked()
{
	Popup::Info(QObject::tr("<p align=\"center\"><b>APN info</b></p> <p align=\"left\">For internet-connection, most providers accept an empty APN-Name.<br>So you'll <b>not</b> have to fill in APN, username,password.</p> <p align=\"center\">If your provider requires an APN, you can find a list of those here:<br> <b>http://www.pinstack.com/carrier_settings_apn_gateway.html</b></p> <p align=\"left\">For Private APNs, please ask your provider for support!</p>"),this);
}



void profiledialog::enableButtonSaveProfile()
{
	if (testAPN(connection_apn->text()))
		pushButtonSaveProfile->setEnabled(true);
}

void profiledialog::disableButtonSaveProfile()
{
	pushButtonSaveProfile->setEnabled(false);
}


void profiledialog::updateCurrentProfile()
{
	//set the fields, getting defaults from Profile...
	lineEdit_profile_name->setText(theProfile.getProfileName());
	connection_username->setText( theProfile.getUsername() );
	connection_password->setText( theProfile.getPassword() );	
	connection_apn->setText( theProfile.getAPN() );	

	connection_routing_noipdefault->setChecked(theProfile.getNoIpDefault());
	connection_routing_defaultroute->setChecked(theProfile.getIsDefaultRoute());
	connection_noCompression->setChecked(theProfile.getNoCompression());
	connection_noCCP->setChecked(theProfile.getNoCCP());
	connection_noAuth->setChecked(theProfile.getNoAuth());
	connection_routing_replaceDefaultRoute->setChecked(theProfile.getReplaceDefaultRoute());
	connection_routing_usepeerdns->setChecked(theProfile.getUsePeerDns());

	checkBox_warn_data_threshold->setChecked(theProfile.getWarnOnDataThreshold());
	spinBox_data_treshold->setValue(theProfile.getThresholdPercentage());
	spinBox_home_data_limit->setValue(theProfile.getHomeDataLimit());
	spinBox_roaming_data_limit->setValue(theProfile.getRoamingDataLimit());
	spinBox_home_time_limit->setValue(theProfile.getHomeTimeLimit());
	spinBox_roaming_time_limit->setValue(theProfile.getRoamingTimeLimit());
	
	disableButtonSaveProfile();    
}


/** checks if there is unsaved data and pops up a warning if so
 *  @returns true if ok to continue (i.e. loose unsaved data)
 *           false if user asked to not continue
 */
bool profiledialog::checkForUnsavedData()
{
	// only accept if no unsaved changes or the user says OK.
	if (pushButtonSaveProfile->isEnabled())
	{
		if (QMessageBox::warning (this, 
				tr(APPNAME " warning"),
				tr("There are unsaved changes to your profile!"),
				QMessageBox::Ignore, 
				QMessageBox::Cancel | QMessageBox::Default, 
				QMessageBox::NoButton) == QMessageBox::Ignore)
		{
			return true;
		}
	}
	else
		return true;
	return false;
}

void profiledialog::dataChanged()
{
	DEBUG2("data of the profile has changed\n");
	emit enableButtonSaveProfile();
}

/// slot: called by the line edit to verify if the value is ok
void profiledialog::check_apn( const QString& aNewAPN)
{
	if (testAPN(aNewAPN))
		emit dataChanged();
	else
		emit disableButtonSaveProfile();
}

/// actual check whether the APN is correct
bool profiledialog::testAPN(const QString& aNewAPN)
{
	if ( aNewAPN.stripWhiteSpace().isEmpty())
		return false;

	if ( aNewAPN.contains("\"") || aNewAPN.contains("\"") || 
	     aNewAPN.contains("'") || aNewAPN.contains(" ") )
	{
		Popup::Warning(tr("Your APN contains illegal characters.\nYou cannot save your profile until you fix this."));
		return false;
	}
	return true;
}
