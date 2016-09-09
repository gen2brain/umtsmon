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
#include "HelperList.h"

#include <qapplication.h>
#include <qstring.h>

#include <assert.h>
#include <errno.h>

#include "../TestFrame.cpp"

// conveniently set theVerbosity to a number of your liking
int  theVerbosity = 5;
//const char* theWasRoamingSettingName         = "/General/WasRoaming";





int main( int aArgC, char* aArgV[] )
{
	// I'm not entirely sure if this is allowed and/or works
	errno = 0;
	
	// welcome to user
	printf("Test app is called '%s' version '%s'\n", APPNAME, APPRELEASE);

chapter("INIT\n");

	QApplication myApp( aArgC , aArgV );
	
chapter("TEST GUI - let device choose\n")

	HelperList myList1;
	check (myList1.testAndRun() != HelperList::QUIT, "user did not select exit");

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

