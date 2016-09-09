/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2006 Christofer Wesseling
 *                     (C) 2008 Klaas van Gend
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
#include "FileStuff.h"
#include "ProgressDialog.h"
#include "Query.h"
#include "TheDeviceManagerSingleton.h"
#include <qfile.h>
#include <qtextstream.h>



void InfoDialogUmtsmon::init()
{
	// FIXME: the tables of modules&devices can't be sorted on other colums by the user BUG: 1616485
	//        it musn't be implemented at this position, it's just a reminder...
	DEBUG2("InfoDialogUmtsmon::init called\n");

	// create a Status Dialog....
	ProgressDialog myPD(this, tr("Fetching device information...")); 
	myPD.start(0);
	myPD.setMaxTime(6);

	lbl_IMEI->setText( get_Info_using_AT_Query("AT+CGSN") );
	myPD.runOneSecond();
	lbl_IMSI->setText(get_Info_using_AT_Query("AT+CIMI"));
	myPD.runOneSecond();
	lbl_Manufacturer->setText(get_Info_using_AT_Query("AT+CGMI"));
	myPD.runOneSecond();
	lbl_CardType->setText(get_Info_using_AT_Query("AT+CGMM"));
	myPD.runOneSecond();
	lbl_Revision->setText(get_Info_using_AT_Query("AT+CGMR"));

	DeviceCapabilities& myDevCapsRef = TheDeviceManagerSingleton::getDeviceCapsRef();
	lbl_AT_Port     ->setText( myDevCapsRef.getATSerialPortName());
	lbl_PPP_Port    ->setText( myDevCapsRef.getPPPSerialPortName());
	lbl_Driver_Name ->setText( myDevCapsRef.getDeviceClassName());
	
	// finish the progress dialog.. we're ready..
	myPD.finish();

	// read the system informations...
	lbl_System_Version->setText(FileStuff::getFileFirstLine("/proc/version"));
	readSystemInformations();

	connect(module_table->horizontalHeader(), SIGNAL(clicked(int)), this, SLOT(sort_module_table(int)));
	connect(device_table->horizontalHeader(), SIGNAL(clicked(int)), this, SLOT(sort_device_table(int)));
}


/**
* SLOT to sort the rows of the module_table depending on the selected column
* @param the section/column to sort
**/
void InfoDialogUmtsmon::sort_module_table(int section) {

	if (module_table->horizontalHeader()->sortIndicatorOrder() == Qt::AscendingOrder) {
		module_table->sortColumn(section,false,true);
	} else {
		module_table->sortColumn(section,true,true);
	}
	module_table->horizontalHeader()->adjustHeaderSize();
}

/**
* sLOT to sort the rows of the device_table depending on the selected column
* @param the section/column to sort
**/
void InfoDialogUmtsmon::sort_device_table(int section) {

	if (device_table->horizontalHeader()->sortIndicatorOrder() == Qt::AscendingOrder) {
		device_table->sortColumn(section,false,true);
	} else {
		device_table->sortColumn(section,true,true);
	}
	device_table->horizontalHeader()->adjustHeaderSize();
}


/**
* returns the result of the query
* @param the Query String
* @return the result of the query,  "error" if no result
**/
QString InfoDialogUmtsmon::get_Info_using_AT_Query( QString anAtCommand )
{		
	Query myQuery(anAtCommand);
	if (myQuery.runUntilDone() != Query::OK)
	{
		// no big deal, it happens, fail silently
		DEBUG2("Query.run() failed\n");	
		return "error";
	}
	if (!myQuery.hasAnswer())
		return tr("No answer from device.");
	QString myAnswer = myQuery.getAnswer().first();
	
	
	
	if (myAnswer.isEmpty() && myQuery.getAnswer().count()>1)
	{
		DEBUG5("FIRST ANSWER EMPTY - GET SECOND ONE\n");
		myAnswer = myQuery.getAnswer()[1];
	}

DEBUG5("myAnswer: '%s'\n", myAnswer.ascii());

	// if the answer contains a ':' in the first 6 characters, that probably means
	// a string like:   +CGSN: IMEI359411001731435
	// but also:        1.5.5  (Date: May 17 2005, Time: 14:55:44)
	int myFirstColonPosition = myAnswer.find(":"); 
	if (myFirstColonPosition > 0 && myFirstColonPosition < 6)
	{
		myAnswer = myAnswer.section(":",1,1);	
	}
	
	// if there are quotes inside the string, let's removed them if 2
	// e.g. Motorola SLVR L7 returns stuff like:
	// +CGMI: "Motorola CE, Copyright 2000"\r\n
	// and keep them if not 2:
	// +CGMM: "GSM900","GSM1800","GSM1900","GSM850","MODEL=L7"\r\n
	int myIndex = 0;
	int myQuoteCounter = -1;
	while (myIndex != -1)
	{
		myIndex = myAnswer.find("\"", myIndex+1);
		myQuoteCounter++;
	}
	if (myQuoteCounter == 2)
	{
		myAnswer = myAnswer.section("\"", 1,1);
	}

	
	return myAnswer.stripWhiteSpace();
}


/// reads systeminformations into the info dialog
void InfoDialogUmtsmon::readSystemInformations()
{
	DEBUG5("InfoDialogUmtsmon::readSystemInformations()\n");
// System modules
    QString line;
	QFile f;

	f.setName( "/proc/modules" );
	f.open(QIODevice::ReadOnly);
    QTextStream in(&f);
        
    line = in.readLine();
	while(!line.isNull())
	{
        line = line.stripWhiteSpace();
		// insert into table..
		/// new row at line 0
		module_table->insertRows(0);
		//insert into row,col,value 
		module_table->setText(0,0, line.section(" ",0,0) );
		module_table->setText(0,1, line.section(" ",1,1) );
		module_table->setText(0,2, line.section(" ",2,2) );

        line = in.readLine();
	}
	f.close();
	module_table->horizontalHeader()->setStretchEnabled(-1, true);
	module_table->sortColumn(0,true,true); // sort on module_names, whole row
	module_table->adjustColumn(0);
	module_table->adjustColumn(1);
	module_table->adjustColumn(2);

	
//System devices
	f.setName( "/proc/devices" );
	f.open(QIODevice::ReadOnly);
    QTextStream in2(&f);

    line = in.readLine();  // first line is just "Character devices:"
	while(!line.isNull())
	{
        line = in.readLine();
		// insert into table..
		/// new row at line 0
		if( ( line.left(3).isEmpty() ) ) // we have reached the last device.. :)
			break; 
		
		device_table->insertRows(0);
		//insert into row,col,value 
		device_table->setText(0,0, line.section(" ",0,0) ); // up to digit 3
		device_table->setText(0,1, line.section(" ",1) ); //from digit 4 to end..
	}
	f.close();
	device_table->horizontalHeader()->setStretchEnabled(-1, true);
	device_table->sortColumn(1,true,true); // sort on device-name, whole row
	device_table->adjustColumn(0);
	device_table->adjustColumn(1);
	
// Software dependency versions
	// let's try to read /etc/Suse-release /etc/redhat-release, etc...
	// unfortunately, also something called '/etc/lsb-release' exists :-(
	FileStuff::FindInDir myDirReader("/etc","*-release", QDir::Files);
	QString myReleaseFileName;
	while ( (myReleaseFileName=myDirReader.getNextEntry()) != "")
	{
		DEBUG5("FILE: %s\n", myReleaseFileName.ascii());
		if (myReleaseFileName != "/etc/lsb-release")
			break;
	}
	DEBUG5("   myReleaseFileName: %s\n", myReleaseFileName.ascii());
	if (myReleaseFileName!="")
	{
		lbl_distro_release->setText(FileStuff::getFileFirstLine(myReleaseFileName));
		DEBUG5("   release contents: %s\n", lbl_distro_release->text().ascii());
	}
	// the pppd version is written in mainwindow.h through setPPPDversion
}



void InfoDialogUmtsmon::setPPPDversion(QString aVersionString)
{

	DEBUG5("InfoDialogUmtsmon::setPPPDversion(\"%s\")\n", aVersionString.ascii());
	lbl_pppd_version->setText(aVersionString);
}
