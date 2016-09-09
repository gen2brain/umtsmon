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
#include "ObserverManager.h"

#include <qstring.h>

#include <assert.h>
#include <errno.h>

#include "../TestFrame.cpp"

// conveniently set theVerbosity to a number of your liking
int  theVerbosity = 5;



/** this class is a pure interface class - used to call the Observer
 *  in case something happens in the state of the onderlying class
 */
class OBS
{
public:
	virtual void notify(void) = 0;
	virtual ~OBS() {;};
};



/** Observer1 is a class interested in observing the changes, so
 *  it derives from OBS.
 */
class Observer1 : public OBS
{
public:
	Observer1(void) : count(0) {};
	
	void notify(void)
	{
		count++;
	};
	int count;
};



/** OMTester is the actual class implementing ObserverManager
 *  and it is a friend - so it can look at the internals of that class as well
 */
class OMTester : public ObserverManager<OBS> 
{
public:

	OMTester() {DEBUG1("OMTester constructor\n");};
	virtual ~OMTester() {DEBUG1("OMTester destructor\n");};
	
	virtual void notifyObservers(void)
	{
		OBS* myObserverPtr = firstObserver();
		while (myObserverPtr != NULL)
		{
			myObserverPtr->notify();
			myObserverPtr = nextObserver();
		}
	};

	virtual void notifyObserver(OBS* anObserver) const 
	{
		anObserver->notify();
	};
	
	int getObserverCount(void)
	{
		return theObserverList.size();
	};
	
	// because it inherits from ObserverManager, it also has
	// attach and detach :-)
};




int main()
{
	// I'm not entirely sure if this is allowed and/or works
	errno = 0;
	
	// welcome to user
	printf("Test app is called '%s' version '%s'\n", APPNAME, APPRELEASE);

chapter("INIT\n");

	OMTester theOMTester;
	check (theOMTester.getObserverCount() == 0, "no observers yet\n", true);

	Observer1 theObserver1;
	check (theObserver1.count == 0, "theObserver1 initialised correctly\n", true);
	Observer1 theObserver2;
	check (theObserver2.count == 0, "theObserver2 initialised correctly\n", true);

chapter("Attach & Detach first observer\n");

	theOMTester.attach(&theObserver1);
	check (theOMTester.getObserverCount() == 1, "one observer attached\n", true);
	check (theObserver1.count == 1, "theObserver1 has been notified once\n", true);
	
	theOMTester.notifyObservers();
	check (theObserver1.count == 2, "theObserver1 has been notified\n", true);
	check (theOMTester.getObserverCount() == 1, "one observer attached\n", true);
	
	theOMTester.detach(&theObserver1);
	check (theOMTester.getObserverCount() == 0, "no observer attached\n", true);
	check (theObserver1.count == 2, "theObserver1 has not been notified again\n", true);

	theOMTester.notifyObservers();
	check (theObserver1.count == 2, "theObserver1 has not been notified again\n", true);
	check (theOMTester.getObserverCount() == 0, "no observer attached\n", true);

	theObserver1.count = 0;
	theObserver2.count = 0;

chapter("Attach & Detach two observers\n");

	theOMTester.attach(&theObserver1);
	theOMTester.attach(&theObserver2);
	check (theOMTester.getObserverCount() == 2, "two observers attached\n", true);

	theOMTester.notifyObservers();
	check (theObserver1.count == 2, "theObserver1 has been notified\n", true);
	check (theObserver2.count == 2, "theObserver2 has been notified\n", true);
	check (theOMTester.getObserverCount() == 2, "two observer attached\n", true);

	theOMTester.detach(&theObserver1);
	check (theOMTester.getObserverCount() == 1, "only one observer left attached\n", true);
	check (theObserver1.count == 2, "theObserver1 has not been notified again\n", true);
	check (theObserver2.count == 2, "theObserver1 has not been notified again\n", true);

	theOMTester.notifyObservers();
	check (theObserver1.count == 2, "theObserver1 has NOT been notified\n", true);
	check (theObserver2.count == 3, "theObserver2 has been notified\n", true);
	check (theOMTester.getObserverCount() == 1, "only one observer left attached\n", true);

chapter("Double entries check\n");
	theObserver1.count = 0;
	theObserver2.count = 0;
	theOMTester.attach(&theObserver1);
	theOMTester.attach(&theObserver2);
	check (theOMTester.getObserverCount() == 3, "three observers attached\n", true);
	
	// so we now have #2 #1 #2 attached...
	
	theOMTester.notifyObservers();
	// observer 1: once notified on attach, once on notify
	check (theObserver1.count == 2, "theObserver1 has been notified twice\n", true);
	// observer 1: once notified on attach, twice on notify - the first attach was before the count was set to 0!
	check (theObserver2.count == 3, "theObserver2 has been notified three times\n", true);

	theOMTester.notifyObserver(&theObserver2);
	check (theObserver1.count == 2, "theObserver1 has NOT been notified again\n", true);
	check (theObserver2.count == 4, "theObserver2 has been notified three times now\n", true);

	theOMTester.detach(&theObserver2);
	check (theOMTester.getObserverCount() == 1, "only one observer left attached\n", true);
	theOMTester.notifyObservers();
	check (theObserver1.count == 3, "theObserver1 has been notified again\n", true);
	check (theObserver2.count == 4, "theObserver2 has not been notified again\n", true);

chapter("Test Copy Constructor\n");
	OMTester theOMTester2(theOMTester);

	// notifying the new copy should work
	theOMTester2.notifyObservers();
	check (theObserver1.count == 4, "theObserver1 has been notified again\n", true);
	check (theObserver2.count == 4, "theObserver2 has not been notified again\n", true);

	// but the old one should still work, too
	// and anything happening to the old one doesn't happen to the new one!
	theOMTester.detach(&theObserver1);
	theOMTester.notifyObservers();
	check (theObserver1.count == 4, "theObserver1 has not been notified again\n", true);
	check (theObserver2.count == 4, "theObserver2 has not been notified again\n", true);

	// the new one should be unaffected
	theOMTester2.attach(&theObserver2);
	check (theObserver2.count == 5, "theObserver2 has been notified due to attach\n", true);
	theOMTester2.notifyObservers();
	check (theObserver1.count == 5, "theObserver1 has been notified again\n", true);
	check (theObserver2.count == 6, "theObserver2 has been notified again\n", true);

	// keep this last for summary:
	finish();
	return 0;
}
