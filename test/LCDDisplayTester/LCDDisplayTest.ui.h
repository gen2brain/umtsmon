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

#include <stdio.h>

static const int theDownload = 1025;

void LCDDisplayTest::init(void)
{
	printf("LCDDisplayTest::init(void)\n");
	theLCDDisplayPtr->setRadioType("HSDPA");
	theLCDDisplayPtr->setRoaming(checkBox_Roaming->isChecked());
	
	checkBox_Roaming_update();
	spinBox_upload_update();
	
	emit spinBox1->setValue(13);
	comboBox1->setCurrentItem(1);
	combo_activated(comboBox1->currentText());
	comboBox_Profile_activated(comboBox_Profile->currentText());
}


void LCDDisplayTest::combo_activated(const QString& aNewString)
{
	printf("Form1::combo_activated changed to '%s'\n", aNewString.ascii());
	theLCDDisplayPtr->setOperatorName(aNewString);
}


void LCDDisplayTest::spin_changed( int aNewValue)
{
	printf("Form1::spinbox_changed to %d\n", aNewValue);
	theLCDDisplayPtr->setStrength(aNewValue);
}


void LCDDisplayTest::comboBox_Profile_activated(const QString& aNewString)
{
	printf("Form1::comboBox_Profile_activated changed to '%s'\n", aNewString.ascii());
	if (aNewString.find("empty") > 0)
		theLCDDisplayPtr->setProfile("");
	else
		theLCDDisplayPtr->setProfile(aNewString);
}


void LCDDisplayTest::spinBox_upload_update()
{
	int myLimitID = comboBox_limits->currentItem();
	printf("%s, %d\n", spinBox_upload->text().ascii(), myLimitID);
	theLCDDisplayPtr->setUpload(spinBox_upload->value(),
								spinBox_Speed->value());
	theLCDDisplayPtr->setDownload(2*spinBox_upload->value(),
								4*spinBox_Speed->value());
	theLCDDisplayPtr->setTotalTraffic(
		spinBox_upload->value()+theDownload, 
		static_cast<MonthlyTraffic::ThresholdStatus>(myLimitID));
}

void LCDDisplayTest::checkBox_Roaming_update()
{
	theLCDDisplayPtr->setRoaming(checkBox_Roaming->isChecked());
	// because this one doesn't update the display:
	comboBox_Profile_activated(comboBox_Profile->currentText());
}


void LCDDisplayTest::spinBox_ConnectedTime_update()
{
	theLCDDisplayPtr->setConnectedTime(spinBox_TimeConnected->value());
}

