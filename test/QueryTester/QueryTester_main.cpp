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
int  theVerbosity = 5;





int main()
{
	// I'm not entirely sure if this is allowed and/or works
	errno = 0;
	
	// welcome to user
	printf("Test app is called '%s' version '%s'\n", APPNAME, APPRELEASE);

printf("\n*** INIT\n");

	SimulatedSerialPort mySP;
	mySP.addSerialStatement("AT","OK");
	mySP.addSerialStatement("AT","ERROR");
	
	Query::setSerial(&mySP);

	{
	Query myQuery("AT");
	check(myQuery.runUntilDone() == Query::OK, "Query was OK\n");
	}
printf("\n*** PART2\n");
	
	{
	Query myQuery("AT");
	check(myQuery.runUntilDone() == Query::ERROR, "Query was not OK - as expected\n");
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

