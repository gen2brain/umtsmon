/* umtsmon - a program to control/monitor your UMTS card
 * Copyright (C) 2006  Christofer Wesseling
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
#include "ProgressDialog.h"

#include <qstring.h>
#include <time.h>
#include <unistd.h>



ProgressDialog::ProgressDialog(QWidget* aParent, const QString& aTitle)
	: Q3ProgressDialog(aTitle, QObject::tr("Abort"), 1, aParent, "ProgressDialog-text2", TRUE)
{
	setProgress( 0 );
	setCaption(aTitle);
}

void
ProgressDialog::start(int aNumberOfMiliSecondsToWait)
{
	setMinimumDuration(aNumberOfMiliSecondsToWait);
	qApp->processEvents();
}


bool
ProgressDialog::runOneSecond(void)
{
	setProgress( progress()+1 );
	DEBUG5("ProgressDialog::runOneSecond of % 3d, % 3d has passed\n", progress(), totalSteps()-1 );
	if ( progress() >= totalSteps()-1 )
		return false;
	qApp->processEvents();
	sleep(1);
	return (wasCanceled()==false);
}

void 
ProgressDialog::finish(void)
{
	// we will show a visible finish of the progress dialog here
	// but we should only do such thing if the dialog was visible
	// in the first place
	if (progress() <= minimumDuration()*1000)
	 	return;
	
	struct timespec sleep_time;
        sleep_time.tv_sec=0;
        sleep_time.tv_nsec=30*1000000; // show each status 30ms 

	int myStepSize = (totalSteps()-progress())/10+1;
	while( (totalSteps()-progress()) >1 )
	{
		int myProgress = progress()+myStepSize;
		if (myProgress > totalSteps()-1)
			myProgress = totalSteps()-1;
		setProgress( myProgress );
		qApp->processEvents();
		nanosleep(&sleep_time,NULL);  // so the user is able to see it...
	}
	qApp->processEvents();
	
}
