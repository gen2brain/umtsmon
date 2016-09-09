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
#include "NetworkChanger.h"

#include <qstring.h>

#include <assert.h>
#include <errno.h>

#include "../TestFrame.cpp"

// conveniently set theVerbosity to a number of your liking
int  theVerbosity = 5;





int main()
{
	// I'm not entirely sure if this is allowed and/or works
	errno = 0;
	
	// welcome to user
	printf("Test app is called '%s' version '%s'\n", APPNAME, APPRELEASE);

chapter("INIT\n");

	SimulatedSerialPort mySP;
	Query::setSerial(&mySP);
	
	mySP.addSerialStatement("AT","OK");
	mySP.addSerialStatement("AT","ERROR");
	
chapter("Simple test\n");
	{
	// short answer (modified Netherlands output) to test basics first
	mySP.addSerialStatement("AT+COPS=?", "\xFF\xFF+COPS: (2,\"vodafone NL\",\"vodafone"
			"-NL\",\"20404\",0),,(0, 1, 3, 4),(0-2)\r\nOK");
	
	NetworkChanger myNWC;
	myNWC.startSearching();
	check(myNWC.isSearchCompleted() == false, "Searching takes a while\n");
	check(myNWC.isSearchCompleted() == true, "Searching is finished\n");
	check(myNWC.theOperatorList.size() == 1, "one entry in the list\n");
	// analyse the first (and only) entry
	check((*myNWC.theOperatorList.begin())->getNetworkID() == 20404, "correct network ID\n");
	check((*myNWC.theOperatorList.begin())->getIconType() == Operator::CURRENT, "correct icon type\n");
	}

chapter("Huawei E220 with trailing info\n");
	{
	// this ent
	mySP.addSerialStatement("AT+COPS=?", "\xFF\xFF+COPS: (1,\"vodafone NL\",\"voda NL\","
			"\"20404\",0),(2,\"vodafone NL\",\"voda NL\",\"20404\",2),(3,\"T-Mobile NL\","
			"\"TMO NL\",\"20416\",0),(3,\"NL KPN\",\"NL KPN\",\"20408\",0),(3,\"NL KPN\","
			"\"NL KPN\",\"20408\",2),(3,\"Orange NL\",\"Orange\",\"20420\",2),"
			"(3,\"T-Mobile NL\",\"TMO NL\",\"20416\",2),(3,\"Orange NL\",\"Orange\","
			"\"20420\",0),,(0,1,3,4),(0,1,2)\r\n"
			"OK\r\n\r\n"
			"^RSSI:23\r\n");
	NetworkChanger myNWC;
	myNWC.startSearching();
	check(myNWC.isSearchCompleted() == false, "Searching takes a while\n");
	check(myNWC.isSearchCompleted() == true, "Searching is finished\n");
	check(myNWC.theOperatorList.size() == 4, "four entries in the list\n");
	// analyse the first entry
	check((*myNWC.theOperatorList.begin())->getNetworkID() == 20404, "correct network ID\n");
	check((*myNWC.theOperatorList.begin())->getIconType() == Operator::CURRENT, "correct icon type\n");
	}
	
chapter("Complex test - Qualcomm style \n");
	{
	// the complete answer for The Netherlands - Qualcomm style
	mySP.addSerialStatement("AT+COPS=?", "\xFF\xFF"
			"+COPS: (2,\"vodafone NL\",\"vodafone-NL\",\"20404\",0),"
			"(3,\"NL-KPN\",\"NL-KPN\",\"20408\",0),"
			"(3,\"NL-KPN\",\"NL-KPN\",\"20408\",2),"
			"(3,\"T-Mobile-NL\",\"TMO-NL\",\"20416\",0),"
			"(3,\"Orange-NL\",\"Orange\",\"20420\",0),"
			"(3,\"NL-Telfort\",\"NL-Tlfrt\",\"20412\",0),"
			",(0, 1, 3, 4),(0-2)\r\n"
			"OK");
	NetworkChanger myNWC;
	myNWC.startSearching();
	check(myNWC.isSearchCompleted() == false, "Searching takes a while\n");
	check(myNWC.isSearchCompleted() == true, "Searching is finished\n");
	check(myNWC.theOperatorList.size() == 5, "Five entries in the list (one double)\n");
	// analyse the second entry (NL-KPN with both UMTS and GRPS)
	Operator* myKPN =	*(++myNWC.theOperatorList.begin());
	check(myKPN->getNetworkID() == 20408, "second entry has correct network ID\n");
	check(myKPN->getIconType() == Operator::FORBIDDEN, "second entry has correct icon type\n");
	check(myKPN->getNetworkName() == "NL-KPN", "Correct network name");
	check(myKPN->getRadioModes() == "GPRS+UMTS", "UMTS and GPRS are integrated\n");
	}
	
chapter("Complex test - Novatel XU870 style \n");
	{
	// the complete answer for Germany - Novatel XU870 style 
	mySP.addSerialStatement("AT+COPS=?", "\xFF\xFF"
		"+COPS: (1,\"Vodafone.de\",\"voda DE\",\"26202\",0)\r\n"
		"+COPS: (2,\"Vodafone.de\",\"voda DE\",\"26202\",2)\r\n"
		"+COPS: (3,\"E-Plus\",\"E-Plus\",\"26203\",2)\r\n"
		"+COPS: (3,\"T-Mobile D\",\"TMO D\",\"26201\",2)\r\n"
		"+COPS: (3,\"T-Mobile D\",\"TMO D\",\"26201\",0)\r\n"
		"+COPS: (3,\"E-Plus\",\"E-Plus\",\"26203\",0)\r\n"
		"+COPS: (3,\"o2 - de\",\"o2 - de\",\"26207\",2)\r\n"
		"+COPS: (3,\"o2 - de\",\"o2 - de\",\"26207\",0)\r\n"
		"OK");
	NetworkChanger myNWC;
	myNWC.startSearching();
	check(myNWC.isSearchCompleted() == false, "Searching takes a while\n");
	check(myNWC.isSearchCompleted() == true, "Searching is finished\n");
	check(myNWC.theOperatorList.size() == 4, "Four entries in the list (all entries double)\n");
	// the first entry has both states CURRENT and AVAILABLE - that should become CURRENT
	check((*myNWC.theOperatorList.begin())->getIconType() == Operator::CURRENT, "First entry has correct icon type\n");
	check((*myNWC.theOperatorList.begin())->getNetworkID() == 26202, "First entry has correct network ID\n");
	}

chapter("Complex test - GC79 style \n");
	{
	// the complete answer for The Netherlands (summer 2006) - Sony GC79 style 
	mySP.addSerialStatement("AT+COPS=?", "\xFF\xFF"
		"+COPS: (2,\"Orange\",\"Orange NL\",\"20420\"),,(0,1,2,3,4),(0,1,2)\r\n"
		"+COPS: (3,\"vodafone NL\",\"vodafone NL\",\"20404\"),,(0,1,2,3,4),(0,1,2)\r\n"
		"+COPS: (3,\"T-Mobile NL\",\"T-Mobile NL\",\"20416\"),,(0,1,2,3,4),(0,1,2)\r\n"
		"+COPS: (3,\"NL KPN\",\"KPN\",\"20408\"),,(0,1,2,3,4),(0,1,2)\r\n"
		"+COPS: (3,\"NL Telfort\",\"Telfort\",\"20412\"),,(0,1,2,3,4),(0,1,2)\r\n"
		"OK");
	NetworkChanger myNWC;
	myNWC.startSearching();
	check(myNWC.isSearchCompleted() == false, "Searching takes a while\n");
	check(myNWC.isSearchCompleted() == true, "Searching is finished\n");
	check(myNWC.theOperatorList.size() == 5, "Five entries in the list\n");
	check((*myNWC.theOperatorList.begin())->getNetworkID() == 20420, "First entry has correct network ID\n");
	}
	

chapter("Complex test - Novatel U740 from 'gromeck' style \n");
	{
	// the complete answer for Germany from Gromeck 
	mySP.addSerialStatement("AT+COPS=?", "\xFF\xFF"
		"\r\n"
		"+COPS: (0,\"T-Mobile D\",\"TMO D\",\"26201\",2)\r\n"
		"+COPS: (2,\"o2 - de\",\"o2 - de\",\"26207\",2)\r\n"
		"+COPS: (1,\"T-Mobile D\",\"TMO D\",\"26201\",0)\r\n"
		"+COPS: (1,\"E-Plus\",\"E-Plus\",\"26203\",0)\r\n"
		"+COPS: (1,\"Vodafone.de\",\"voda DE\",\"26202\",0)\r\n"
		"+COPS: (0,\"Vodafone.de\",\"voda DE\",\"26202\",2)\r\n"
		"+COPS: (0,\"E-Plus\",\"E-Plus\",\"26203\",2)\r\n"
		"\r\n"
		"OK\r\n");
	NetworkChanger myNWC;
	myNWC.startSearching();
	check(myNWC.isSearchCompleted() == false, "Searching takes a while\n");
	check(myNWC.isSearchCompleted() == true, "Searching is finished\n");
	check(myNWC.theOperatorList.size() == 4, "Four entries in the list\n");
	check((*myNWC.theOperatorList.begin())->getNetworkID() == 26207, "First entry has correct network ID\n");
	}

chapter("Complex test - Novatel U630\n");
	{
	// the complete answer for The Netherlands from Klaas 
	mySP.addSerialStatement("AT+COPS=?", "\xFF\xFF"
		"\r\n"
		"+COPS: (2,\"vodafone NL\",\"voda NL\",\"20404\")\r\n"
		"+COPS: (3,\"NL KPN\",\"NL KPN\",\"20408\")\r\n"
		"+COPS: (3,\"T-Mobile NL\",\"TMO NL\",\"20416\")\r\n"
		"+COPS: (1,\"Orange NL\",\"Orange\",\"20420\")\r\n"
		"\r\n"
		"OK\r\n");
	NetworkChanger myNWC;
	myNWC.startSearching();
	check(myNWC.isSearchCompleted() == false, "Searching takes a while\n");
	check(myNWC.isSearchCompleted() == true, "Searching is finished\n");
	check(myNWC.theOperatorList.size() == 4, "Four entries in the list\n");
	check((*myNWC.theOperatorList.begin())->getNetworkID() == 20404, "First entry has correct network ID\n");

	// note that this block doesn't end but continues in the next chapter...

chapter("Let's connect\n");
	mySP.addSerialStatement("AT+COPS=1,2,\"20420\"","\xFF\xFF"
			"\r\n"
			"\xd\xa^MODE:3,3\xd\xa\xd\xa+CME ERROR: no network service\xd\xa");
	NetworkChanger::SwitchResults myResult = myNWC.startSwitchToNetworkID(20420);
printf("result: %d\n", myResult);	
	check( myResult == NetworkChanger::NOT_READY_YET,
			"connecting takes time\n");
	check(myNWC.isSwitchCompleted() == NetworkChanger::NOT_READY_YET,
			"connecting takes more time\n");
	check(myNWC.isSwitchCompleted() == NetworkChanger::ERROR,
			"connecting failed... (which was expected)\n");
printf("error: '%s'\n", myNWC.getErrorMessage().ascii());	
	check(myNWC.getErrorMessage().contains("CME"), "the correct piece of the answer was returned as error message\n");
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

