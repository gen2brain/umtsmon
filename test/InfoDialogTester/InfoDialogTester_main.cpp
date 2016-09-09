/* umtsmon - a program to control/monitor your UMTS card
 * Copyright (C) 2006, 2007  Klaas van Gend
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
#include "Query.h"
#include "SerialPort.h"

#include <qstring.h>

#include <assert.h>
#include <errno.h>

#include "../TestFrame.cpp"

// conveniently set theVerbosity to a number of your liking
int  theVerbosity = 2;

#include "FileStuff.h"


#include "infodialog.h"


// and the implementation of the members
#include "infodialog.ui.h"




int main(int argc, char** argv)
{
	// I'm not entirely sure if this is allowed and/or works
	errno = 0;
	
	// welcome to user
	printf("Test app is called '%s' version '%s'\n", APPNAME, APPRELEASE);

printf("\n*** INIT\n");
	QApplication a( argc, argv );

	SimulatedSerialPort mySP;
	Query::setSerial(&mySP);
	
	mySP.addSerialStatement("AT","OK");
	mySP.addSerialStatement("AT","ERROR");
	
printf("\n*** Motorola Sliver 7\n");

	// Moto SLVR L7 answer:
	mySP.addSerialStatement("AT+CGSN", "+CGSN: IMEI359411001731435\r\nOK\r\n");
	mySP.addSerialStatement("AT+CIMI", "204042920481060\r\n\r\nOK\r\n");
	mySP.addSerialStatement("AT+CGMI", "\xFF\xFF\r\r\n+CGMI: \"Motorola CE, Copyright 2000\"\r\n\r\nOK\r\n");
	mySP.addSerialStatement("AT+CGMM", "+CGMM: \"GSM900\",\"GSM1800\",\"GSM1900\",\"GSM850\",\"MODEL=L7\"\r\n\r\nOK\r\n");
	mySP.addSerialStatement("AT+CGMR", "+CGMR: \"R4513_G_08.B7.DCR_RB\"\r\n\r\nOK\r\n");
	
	InfoDialogUmtsmon mySL7;
	mySL7.init();
	check (mySL7.lbl_IMEI.text() == "IMEI359411001731435", "IMEI correct\n");
	check (mySL7.lbl_IMSI.text() == "204042920481060", "IMSI correct\n");
	check (mySL7.lbl_Manufacturer.text() == "Motorola CE, Copyright 2000", "Manufacturer correct\n");
	
#if 0
printf("\n*** Motorola 680E\n");

	// Moto 680E answers:
	mySP.addSerialStatement("AT+CGSN", "+CGSN: IMEI359411001731435\r\nOK\r\n");
	mySP.addSerialStatement("AT+CIMI", "204042920481060\r\n\r\nOK\r\n");
	mySP.addSerialStatement("AT+CGMI", "\xFF\xFF\r\r\n+CGMI: \"Motorola CE, Copyright 2000\"\r\n\r\nOK\r\n");
	mySP.addSerialStatement("AT+CGMM", "+CGMM: \"GSM900\",\"GSM1800\",\"GSM1900\",\"GSM850\",\"MODEL=L7\"\r\n\r\nOK\r\n");
	mySP.addSerialStatement("AT+CGMR", "+CGMR: \"R4513_G_08.B7.DCR_RB\"\r\n\r\nOK\r\n");
	
	InfoDialogUmtsmon my680;
	my680.init();
	check (my680.lbl_IMEI.text() == "IMEI359411001731435", "IMEI correct\n");
	check (my680.lbl_IMSI.text() == "204042920481060", "IMSI correct\n");
	check (my680.lbl_Manufacturer.text() == "Motorola CE, Copyright 2000", "Manufacturer correct\n");
#endif

printf("\n*** Option GT \n");

	// Option GT answers:
	mySP.addSerialStatement("AT+CGSN", "355201021053686,QL4A5C73PR\r\n\r\nOK\r\n");
	mySP.addSerialStatement("AT+CIMI", "204042920481060\r\n\r\nOK\r\n");
	mySP.addSerialStatement("AT+CGMI", "Option N.V.\r\n\r\nOK\r\n");
	mySP.addSerialStatement("AT+CGMM", "GlobeTrotter Fusion Quad Lite\r\n\r\nOK\r\n");
	mySP.addSerialStatement("AT+CGMR", "1.5.5  (Date: May 17 2005, Time: 14:55:44)\r\n\r\nOK\r\n");
	
	InfoDialogUmtsmon myGT;
	myGT.init();
	check (myGT.lbl_IMEI.text() == "355201021053686,QL4A5C73PR", "IMEI correct\n");
	check (myGT.lbl_IMSI.text() == "204042920481060", "IMSI correct\n");
	check (myGT.lbl_Manufacturer.text() == "Option N.V.", "Manufacturer correct\n");

	
	
	// keep this last for summary:
	finish();
	return 0;
}

void printAbortMessage(__attribute__ ((unused)) int aSignalNumber)
{
	printf("\n\n  *** " APPNAME " version " APPRELEASE " closed due to an"
	           " unrecoverable program error.\n"
	       "  *** Please e-mail all of the above output to"
	           " umtsmon-develop@lists.sourceforge.net,\n"
	       "  *** Or go to the " APPNAME " website and file a bug.\n\n\n"); 
	// do not call exit here - or the signal will not be handled correctly
}

