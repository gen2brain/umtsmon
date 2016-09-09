/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2007  Klaas van Gend
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

#ifndef HELPERLIST_H_
#define HELPERLIST_H_

#include "Runner.h"
#include <qstring.h>
#include <q3listview.h>

// forward declarations
class helperlistgui;



class HelperData
{
public:
	HelperData(int aNumber) : theNumber(aNumber), needsFix(false) {};
	virtual ~HelperData() {};

	/// @returns true if this item is incorrectly configured
	bool isNeedingFix(void) {return needsFix;};
	
	/** adds QListViewItem(s) based on the data in the class to the widget
	 *  @param aListViewPtr	pointer to the widget to add the items to
	 */
	virtual void add(Q3ListView* aListViewPtr) = 0;
	
	/** the code to actually fix the problem, 
	 *  @returns true if problem fixed
	 */
	virtual bool fixup(void) = 0;
	
protected:
	/// this int is an ID to sort on in the dialog
	int     theNumber;
	QString theName;
	QString theFullPath;
	QString theVersion;
	
	enum SUIDState {YES, NO, NOTAPPLICABLE};
	SUIDState isSUID;
	QString theRemarks;

	bool needsFix;
};



class HelperApp : public HelperData
{
public:
	HelperApp(int aNumber, Runner::BinariesToRun aHelper,
		const QString& aVersion="version", QString aRemark="");
		
	virtual ~HelperApp() {};
	virtual bool fixup(void);
	virtual void add(Q3ListView* aListViewPtr);
	
private:
	Runner::BinariesToRun theHelper;
	bool	isBinaryMissing;
};


class HelperDev : public HelperData
{
public:
	HelperDev(int aNumber, const QString& anATPort, const QString& aPPPPort);
		
	virtual ~HelperDev() {};
	virtual bool fixup(void);
	virtual void add(Q3ListView* aListViewPtr);
	
private:
	/// (internal function) creates a new ListViewitem for port aPortName
	Q3ListViewItem* newLVI(Q3ListViewItem* aQLVI, const QString& aPortName);
	bool fixDev(const QString& aPort);

	QString theATPort;
	QString thePPPPort;
};


class HelperGroup : public HelperData
{
public:
	HelperGroup(int aNumber);
		
	virtual ~HelperGroup() {};

	virtual void add(Q3ListView* aListViewPtr);

	virtual bool fixup(void);
private:
	/** (internal function) creates a new "sub"QListViewItem 
	 * @param aQLVI      theQListViewItem to create a sub item for
	 * @param aGroupName the group to create the item for
	 * @param isOKPtr    (OUTPUT) becomes true if aGroupName found in theGroupList
	 * @return a pointer to the newly created QLVI
	 */
    Q3ListViewItem *newLVI(Q3ListViewItem *aQLVI, 
    		const QString & aGroupName,
    		bool* isOKPtr);

    /// assembles everything for the group list
    void createGroupList();
    
    QStringList theGroupList;
};



class HelperList
{
public:
	HelperList() {};
	virtual ~HelperList();
	
	enum TestResults {OK, FIXED=OK, CONTINUE, QUIT};
	TestResults testAndRun(void);
	
	/// returns true if anything needs fixing
	bool needsFixing(void);

	/// fixup the problem that this instance found	
	bool fixIt(void);
	
	/** adds all the HelperApps to the HelperDataList
	 *  note that it does not create the entry for the device nodes (ports)
	 *  to use - that's done in setSerialPorts()
	 */
	void createHelperList(void);
	
	
	void addListToWidget(void);
	
private:
	/// Runner class to do our stuff
	Runner theRunner;
	
	/// pointer to the QT Dialog associated to this class
	helperlistgui* theGUIPtr;
	
	typedef std::list<HelperData*> HelperDataList;
	HelperDataList theHelperDataList;
};

#endif /*HELPERLIST_H_*/
