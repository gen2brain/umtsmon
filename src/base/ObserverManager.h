/* umtsmon - a program to control/monitor your UMTS card
 * Copyright (C) 2007  Klaas van Gend
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

#ifndef OBSERVERMANAGER_CLASS
#define OBSERVERMANAGER_CLASS

#include <list>

/** The ObserverManager is a class that - once inherited from - 
 *  will take care of the administration associated to observers
 *  to your state.
 *  0) you have a class A that has state changes
 *  1) create your observer interface - usually an abstract base class
 *  2) implement the observer interface in class B that needs to be
 *     aware of the state changes
 *  3) use attach() and detach() to add/remove class B's new interface
 *  4) in class A, create a notify() that uses firstObserver() and
 *     nextObserver() to cycle through all attached observers
 *
 *  See also the (slightly abstracted) test/example implementation code 
 *  in ObserverManagerTester
 */
template<class ObserverInterface> 
class ObserverManager
{
public:
	ObserverManager() 
	 : theObserverListIterator(theObserverList.begin()) 
		{DEBUG3("ObserverManager constructor\n");};
	virtual ~ObserverManager()
		{DEBUG3("ObserverManager destructor\n");};
	
	/** attach yourself as an ObserverInterface
	 *  if you attach yourself twice, you will be notified twice
	 *  @param anObserver	pointer to yourself implementing the ObserverInterface
	 */
	void attach(ObserverInterface* anObserver)
	{
		DEBUG5("ObserverManager::attach(%p)\n", anObserver);
		if (anObserver != NULL)
			theObserverList.push_back(anObserver);
		// list change - invalidate our iterator
		theObserverListIterator = theObserverList.begin();
		// and give our observer it's initial state
		notifyObserver(anObserver);	
	};
	
	/** detach yourself as an ObserverInterface
	 *  note: calling detach will remove *all* your ObserverInterfaces, so if you were
	 *  attached twice, both entries will vanish
	 *  @param anObserver	pointer to yourself implementing the ObserverInterface
	 */
	void detach(ObserverInterface* anObserver)
	{
		DEBUG5("ObserverManager::detach(%p)\n", anObserver);
		if (anObserver != NULL)
			theObserverList.remove(anObserver);
		// list change - invalidate our iterator
		theObserverListIterator = theObserverList.begin();	
	};

protected:	
	/** pure virtual function, to be implemented by the inherited class
	 *  the implementation should perform the notification of the observer pointed to.
	 */
	virtual void notifyObserver(ObserverInterface* anObserver) const = 0;
	
	/** this member returns a pointer to the first Observer that is attached.
	 *  this is definitely not re-entrant safe code
	 */
	ObserverInterface* firstObserver(void)
	{ 
		theObserverListIterator = theObserverList.begin(); 
		if (theObserverListIterator != theObserverList.end()) 
			return *theObserverListIterator; 
		else
			return NULL;
	};

	/** this member returns a pointer to the next Observer that is attached.
	 *  Only call *after* you called firstObserver().
	 *  this is definitely not re-entrant safe code
	 */
	ObserverInterface* nextObserver(void)
	{ 
		theObserverListIterator++; 
		if (theObserverListIterator != theObserverList.end()) 
			return *theObserverListIterator; 
		else
			return NULL;
	};
	
	/** Copy constructor - we need to make sure that our member is copied!
	 * 
	 */
	ObserverManager(const ObserverManager& anOldManager)	 
		: theObserverListIterator(theObserverList.begin()) 
	{
		DEBUG3("ObserverManager COPY constructor\n");
		// note: we only need to *copy* - it's a list of pointers!
		theObserverList = anOldManager.theObserverList;
	}
	
private:
	/** in order for the automated Tester, we allow this one class to view 
	 *  our private data - if you want to add a different class, you probably
	 *  haven't understood the design pattern completely yet.
	 */
	friend class OMTester;

	typedef std::list<ObserverInterface*> ObserverList;
	
	/// this is the actual list of pointers to observers
	ObserverList theObserverList;

	/// this iterator is used for the first/nextObserver() calls 
	typename ObserverList::iterator theObserverListIterator;

	
	/** assign operator is private and not implemented
	 *  to prevent duplication of this class instance (EffC++ item27)
	 *  DO NOT IMPLEMENT THIS MEMBER
	 */
	ObserverManager& operator=(const ObserverManager& anOldManagerRef);	 

};


#endif // defined OBSERVERMANAGER_CLASS
