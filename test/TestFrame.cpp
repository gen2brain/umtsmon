/* umtsmon - a program to control/monitor your UMTS card
 * Copyright (C) 2006  Klaas van Gend
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

#include <unistd.h>
#include <stdlib.h>

#include <qstring.h>

// conveniently set theVerbosity to a number of your liking
extern int  theVerbosity;

int theNumberOfOKs = 0;
int theNumberOfFAILs = 0;

#define testmsg(format, ...)	printf("    " format, ## __VA_ARGS__);
#define chapter(format, ...)	printf("\n** " format, ## __VA_ARGS__);

void finish(int anExit=0)
{
	int myTotal = theNumberOfOKs + theNumberOfFAILs;
	assert(myTotal > 0);
	printf("\nSummary:\n--------\n");
	printf("Number of OKs  : % 3d, (% 4d %%)\n",
		   theNumberOfOKs, (100*theNumberOfOKs)/myTotal);
	printf("Number of FAILs: % 3d, (% 4d %%)\n\n",
		   theNumberOfFAILs, (100*theNumberOfFAILs)/myTotal);
	exit(anExit);
}


bool check(bool aCondition, const QString& aMessage, bool abortIfWrong=false)
{
	if (aCondition)
	{
		QString myTotalLine ="* \033[1m\033[32mOK\033[m  : " + aMessage;
		printf (myTotalLine.ascii());
		theNumberOfOKs++;
	}
	else
	{
		printf(("* \033[1m\033[31mFAIL: " + aMessage + "\033[m").ascii());
		testmsg("the strerror might help: '%s'\n", strerror(errno));
		theNumberOfFAILs++;
		
		if (abortIfWrong)
		{
			printf("*****************************\nFATAL FAIL\n");
			finish(35);
		}
	} 
	
	return aCondition;
}
