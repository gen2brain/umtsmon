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
#include "Query.h"
#include "SerialPort.h"
#include "Popup.h"

#include "networkoperatorwizard.h"

#include <qapplication.h>
#include <qstring.h>

#include <assert.h>
#include <errno.h>

#include "../TestFrame.cpp"

// conveniently set theVerbosity to a number of your liking
int  theVerbosity = 5;





int main( int aArgC, char* aArgV[] )
{
	// I'm not entirely sure if this is allowed and/or works
	errno = 0;
	
	// welcome to user
	printf("Test app is called '%s' version '%s'\n", APPNAME, APPRELEASE);

chapter("INIT\n");

	SimulatedSerialPort mySP;
	Query::setSerial(&mySP);
	
	LongSerialPort myRealSP;
	myRealSP.openDev("/dev/ttyUSB1", 115200);
	
	QApplication myApp( aArgC , aArgV );

	
chapter("TEST GUI - let device choose\n")
	if (true)
	{
	// Answer that it already is automatic mode and expect the user to confirm that again.
	mySP.addSerialStatement("AT+COPS?", "+COPS: 0,0,\"vodafone NL\",2\r\nOK\r\n\r\n");
	mySP.addSerialStatement("AT+COPS=0", "OK\r\n\r\n");
	NetworkOperatorWizard myNOW;
	Popup::Info("Contrary to most test binaries, this one requires the user to click a few buttons.\nPlease click Next> Next> Finish", NULL);
	myNOW.show();
	check(myNOW.exec() == QDialog::Accepted, "the user clicked FINISH\n", true);
	check(mySP.wasAllSatisfied(), "no unexpected Queries arose\n", true);
	check(mySP.isAllUsed(), "all expected Queries have happened\n", true);
	}


chapter("TEST GUI - let user choose\n")
	{
	// this is one of the test strings from NetworkChangerTester, enhanced with some more
	// delays - to make the GUI look more realistic
	// I also removed the Huawei test string as it doesn't matter right now
	mySP.addSerialStatement("AT+COPS?", "+COPS: 1,0,\"vodafone NL\",2\r\nOK\r\n\r\n");
	mySP.addSerialStatement("AT+COPS=?", "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
		"+COPS: (1,\"vodafone NL\",\"voda NL\","
		"\"20404\",0),(2,\"vodafone NL\",\"voda NL\",\"20404\",2),(1,\"T-Mobile NL\","
		"\"TMO NL\",\"20416\",0),(3,\"NL KPN\",\"NL KPN\",\"20408\",0),(3,\"NL KPN\","
		"\"NL KPN\",\"20408\",2),(3,\"Orange NL\",\"Orange\",\"20420\",2),"
		"(1,\"T-Mobile NL\",\"TMO NL\",\"20416\",2),(3,\"Orange NL\",\"Orange\","
		"\"20420\",0),,(0,1,3,4),(0,1,2)\r\n"
		"OK\r\n\r\n");
	mySP.addSerialStatement("AT+COPS=1,2,\"20416\"", 
		"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF+COPS: 2,0,\"T-Mobile NL\",2\r\n"
		"OK\r\n\r\n");
	NetworkOperatorWizard myNOW;
	Popup::Info("Please click Next> Next> Next> Orange OK T-Mobile Next> Finish", NULL);
	myNOW.show();
	check(myNOW.exec() == QDialog::Accepted, "the user clicked FINISH\n", true);
	check(mySP.wasAllSatisfied(), "no unexpected Queries arose\n", true);
	check(mySP.isAllUsed(), "all expected Queries have happened\n", true);
	}

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

