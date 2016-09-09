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

#ifndef PROGRESSDIALOG_H_
#define PROGRESSDIALOG_H_


#include <q3progressdialog.h>
#include <qapplication.h>

//forward declaration of QString
class QString;

/**
 *  This ProgressDialog class hides some of the querks of the QT class
 *  it also provides us with a convenient "once per second" polling mechanism
 * 
 * Depending on your wishes:
 *  * show from the start and kill it when success 
 *  * show from start and rapid progress to 100% once success
 *  * show delayed and don't show at all if it finishes before a certain time span
 * See the start(), setMaxTime() and finish() members for more explanation.
 */ 
class ProgressDialog : protected Q3ProgressDialog
{ 

public:
	/// constructor
	ProgressDialog(QWidget* aParent, const QString& aTitle);

	/// destructor
	virtual ~ProgressDialog() {};

	/**Sets the Max Time in seconds so reach 100%
	 * (in reality it sets te amount of total steps, we wait 1 second per step....)
	 * @param aValue amount of seconds to wait
	 */
	void setMaxTime( int aValue )  {setTotalSteps(aValue+1);};

	/** start the dialog, 
	 *  @param aNumberOfMiliSecondsToWait wait for this time before actually showing, 
	 *  0 means show immediately.
	 */
	void start(int aNumberOfMiliSecondsToWait);

	/** Makes one step, and waits after that one second..
	 *  @return true if still OK, false if user cancelled OR if the Max Time (see above) is reached.
	 */
	bool runOneSecond(void);

	/** GUI fixup: If there is not other visible indication to the user that a
	 *  task finished correctly, run this member to have the progress bar go to
	 *  100% (and wait a little while) first.
	 *  Note that if the time hasn't progressed beyond the aNumberOfMiliSecondsToWait
	 *  as set by the start() member, this member will not show anything. 
	 */
	void finish(void);

private:

};
#endif // defined ProgressDialog CLASS
