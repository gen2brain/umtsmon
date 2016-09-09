/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2006,2007,2008  Klaas van Gend
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
#include "FileStuff.h"
#include "Popup.h"
#include "TheDeviceManagerSingleton.h"

#include "helperlistgui.h"

#include <qfile.h>
#include <qpixmap.h>
//Added by qt3to4:
#include <q3mimefactory.h>
//Added by qt3to4:
#include <q3mimefactory.h>

#include <sys/types.h>
#include <grp.h>
#include <unistd.h>
#include <assert.h>


/////////////////////////////////////////////////////////////////////////////
bool HelperData::fixup(void)
{
	DEBUG3("HelperApp::fixup()\n");
	return false;
}

/////////////////////////////////////////////////////////////////////////////
HelperApp::HelperApp(int aNumber, Runner::BinariesToRun aHelper,
		const QString& aVersion, QString aRemark) 
		: HelperData(aNumber), theHelper(aHelper), isBinaryMissing(false)
{
	Runner myRunner;
	
	theFullPath = myRunner.getPath(aHelper);
	isSUID   = myRunner.isSUID(aHelper)?(HelperData::YES):(HelperData::NO);
	theRemarks  = aRemark;
	theVersion  = aVersion;

	// It's debatable whether this code belongs here or in Runner...
	// For now it's here because this is the location where it concerns us
	switch(aHelper)
	{
	case Runner::RUNNERTESTER:
		theName = "INTERNAL ERROR";
		isSUID = HelperData::NOTAPPLICABLE;
		break;	
	case Runner::SOMESU:
		theName = "*su";
		isSUID = HelperData::NOTAPPLICABLE;
		break;
	case Runner::PPPD:
		theName = "pppd"; 	
		break;
	case Runner::CHAT:
		theName = "chat";
		break;
	case Runner::PCCARDCTL:
		theName = "(pc)cardctl";
		break;
	case Runner::USBMODESWITCH:
		theName = "USB mode switcher";
		break;
	}
	
	// do we see problems?
	if (theFullPath.isEmpty() || (!FileStuff::doesFileExist(theFullPath)))
	{
		isBinaryMissing = true;
		// binary isn't there :-(
		// in case of SOMESU and PPPD, that's killing
		// in case of PCCARDCTL or USBMODESWITCH, it may not even be relevant
		theFullPath = QObject::tr("!!! missing binary");
		switch(aHelper)
		{
		case Runner::SOMESU:
		case Runner::PPPD:
			// problem
			needsFix = true;
			break;
		case Runner::PCCARDCTL:
			// not a problem, we still didn't implement 'eject'
			break;
		case Runner::USBMODESWITCH:
			// let's put it this way: only if we find a binary
			// but it's not suid, it's a problem
			// if there's no binary, I don't care :-D
			break;
		case Runner::RUNNERTESTER:
		case Runner::CHAT:
			// *definitely* not a problem
			break;
		}
		DEBUG3("Binary %s is missing and that's %s.\n", theName.ascii(), needsFix?"major":"minor");
	}
	else
	{
		if (isSUID == NO && aHelper!=Runner::PCCARDCTL)
		{
			DEBUG1("binary '%s' is not SUID -> needs fixing\n", theFullPath.ascii());
			needsFix = true;
		}
	} 
}

void HelperApp::add(Q3ListView* aListViewPtr)
{
	DEBUG3("HelperApp::add\n");
	Q3ListViewItem* myQLVI = new Q3ListViewItem (aListViewPtr, QString::number(theNumber,10), theName);
	myQLVI->setOpen(true);
	
	Q3ListViewItem* mySubQLVI= new Q3ListViewItem (myQLVI, "", 
				theFullPath, theVersion, "", theRemarks);
	switch(isSUID)
	{
	case HelperData::YES:
		{
			QPixmap myPixmap( qPixmapFromMimeSource("ok.png") );
			mySubQLVI->setPixmap(3, myPixmap);
			break;
		}
	case HelperData::NO:
		{
			QPixmap myPixmap( qPixmapFromMimeSource("process-stop-16x16.png") );
			mySubQLVI->setPixmap(3, myPixmap);
			break;
		}
	case HelperData::NOTAPPLICABLE:
		mySubQLVI->setText(3, "n/a");
		break;
	}

	// in case the binary is missing, let's put up a sign
	if (isBinaryMissing)
	{
		QPixmap myPixmap( qPixmapFromMimeSource("process-stop-16x16.png") );
		mySubQLVI->setPixmap(1, myPixmap);
	}
	
}

bool HelperApp::fixup(void)
{
	DEBUG3("HelperApp::fixup()\n");
	
	if (isBinaryMissing)
	{
		Popup::Warning(QObject::tr("%1 couldn't find binary %2,"
			" which is required for full operation.").arg(APPNAME).arg(theName));
		return false;
	}
	
	QString myCommand = "chown root:root " + theFullPath + " && chmod u+s " + theFullPath;
	DEBUG2("The fixup is: '%s'\n", myCommand.ascii());	
	Runner myRunner;
   	myRunner.runCommand(Runner::SOMESU,  myCommand.split(" "));
	needsFix = myRunner.isSUID(theHelper)==false;
	isSUID = (!needsFix)?(HelperData::YES):(HelperData::NO);

	// needsFix is true when there's something to be fixed
	// we return here if everything was fixed - that's the opposite
	return !needsFix; 
}



/////////////////////////////////////////////////////////////////////////////
HelperDev::HelperDev(int aNumber, const QString& anATPort, const QString& aPPPPort) :
		HelperData(aNumber), theATPort(anATPort), thePPPPort(aPPPPort)
{			
	needsFix=false;
	if (FileStuff::isFileReadWrite(anATPort)!=FileStuff::READWRITE)
	{
		DEBUG1("Device %s is not read/write -> needs fix\n", anATPort.ascii());
		needsFix = true;
	}
	if (FileStuff::isFileReadWrite(aPPPPort)!=FileStuff::READWRITE)
	{
		DEBUG1("Device %s is not read/write -> needs fix\n", aPPPPort.ascii());
		needsFix = true;
	}
}


Q3ListViewItem* HelperDev::newLVI(Q3ListViewItem* aQLVI, 
				const QString& aPortName)
{
	QString myRemarks;
	bool isOK = false;
	
	if (FileStuff::doesFileExist(aPortName))
	{
		switch(FileStuff::isFileReadWrite(aPortName))
		{
		case FileStuff::READ:
			myRemarks = QObject::tr("user does not have write access");
			isOK = false;
			break;
		case FileStuff::WRITE:
			myRemarks = QObject::tr("user does not have read access");
			isOK = false;
			break;
		case FileStuff::READWRITE:
			myRemarks = QObject::tr("read/write access is OK");
			isOK = true;
			break;
		case FileStuff::NOTHING:
			myRemarks = QObject::tr("user does not have read nor write access "
			            "(or something else has a lock)");
			isOK = false;
			break;
		}
	}
	else
	{
		isOK = false;
		myRemarks = QObject::tr("not found on file system");
	}

	Q3ListViewItem* mySubQLVI  = new Q3ListViewItem (aQLVI, "", 
				aPortName, "n/a", "n/a", myRemarks);
	if (isOK)
	{
		QPixmap myPixmap( qPixmapFromMimeSource("ok.png") );
		mySubQLVI->setPixmap(4, myPixmap);
	}
	else
	{
		QPixmap myPixmap( qPixmapFromMimeSource("process-stop-16x16.png") );
		mySubQLVI->setPixmap(4, myPixmap);
	}
	return mySubQLVI;
}

void HelperDev::add(Q3ListView* aListViewPtr)
{
	Q3ListViewItem* myQLVI = new Q3ListViewItem (aListViewPtr, 
				QString::number(theNumber, 10), 
				QObject::tr("Modem Communication Ports"));
	myQLVI->setOpen(true);

	//FIXME, here are probably two small, one-time-only memory leaks:
	__attribute__ ((unused)) Q3ListViewItem* mySubQLVI_AT  = newLVI(myQLVI, theATPort);
	__attribute__ ((unused)) Q3ListViewItem* mySubQLVI_PPP = newLVI(myQLVI, thePPPPort);
}


bool HelperDev::fixDev(const QString& aPort)
{
	Runner myRunner;
	if (aPort.isEmpty())
		return false;
	if (FileStuff::isFileReadWrite(aPort)!=FileStuff::READWRITE)
	{
		QString myCommand = "chmod uog+rw " + aPort;
    	myRunner.runCommand(Runner::SOMESU,  myCommand.split(" "));
	}
    return FileStuff::isFileReadWrite(aPort)==FileStuff::READWRITE;
}

bool HelperDev::fixup(void)
{
	DEBUG3("HelperDev::fixup()\n");
	bool theResult = true;

	if (fixDev(theATPort)==false)
		theResult = false;
	if (fixDev(thePPPPort)==false)
		theResult = false;

	needsFix = !theResult;
	return theResult;
}


/////////////////////////////////////////////////////////////////////////////
HelperGroup::HelperGroup(int aNumber) :
		HelperData(aNumber)
{			
	needsFix=false;
    createGroupList();
}

void HelperGroup::createGroupList()
{
	DEBUG5("HelperGroup::createGroupList()\n");
	gid_t *myGroupArray;
	unsigned int myNumberOfGroups;
	unsigned long myMaxNumberGroups = sysconf(_SC_NGROUPS_MAX) + 1;
	myGroupArray = (gid_t*)(malloc(myMaxNumberGroups * sizeof (gid_t)));
	myNumberOfGroups = getgroups(myMaxNumberGroups, myGroupArray);
	
	// according to the man page, the egid *might* be missing 
	if(myNumberOfGroups < myMaxNumberGroups)
	{
		myGroupArray[myNumberOfGroups] = getegid();
		myNumberOfGroups++;
	}

	// and transform the array into a QStringList
	for(unsigned int i=0; i<myNumberOfGroups; i++)
	{
		struct group *myGroup = getgrgid(myGroupArray[i]);
		DEBUG6("group: %d = %s\n", myGroupArray[i], myGroup->gr_name);
		theGroupList.push_back(myGroup->gr_name);
	}
}

void HelperGroup::add(Q3ListView* aListViewPtr)
{
	bool isRoot = Runner::amIRoot();
	QString myGroupText;
	QString myGroupHelp;
	if (isRoot)
		myGroupText = QObject::tr("You run as root - no need for groups");
	else
	{
		myGroupText = QObject::tr("Member of the following user groups");
		myGroupHelp = QObject::tr("usually one needed to access the device");
	}
	
	Q3ListViewItem *myQLVI = new Q3ListViewItem(aListViewPtr, QString::number(theNumber, 10), myGroupText, "", "", myGroupHelp);
    myQLVI->setOpen(true);
	bool isOK = isRoot;
    if (!isRoot)
	{
		Q3ListViewItem* mySubQLVI_dialout  = newLVI(myQLVI, "dialout", &isOK);
		Q3ListViewItem* mySubQLVI_uucp = newLVI(myQLVI, "uucp", &isOK);

		if (isOK)
		{
			QPixmap myPixmap( qPixmapFromMimeSource("ok.png") );
			mySubQLVI_dialout->setPixmap(4, myPixmap);
			mySubQLVI_uucp->setPixmap(4, myPixmap);
		}
		else
		{
			QPixmap myPixmap( qPixmapFromMimeSource("process-stop-16x16.png") );
			mySubQLVI_dialout->setPixmap(4, myPixmap);
			mySubQLVI_uucp->setPixmap(4, myPixmap);
		}
	}
    needsFix = !isOK;
}

bool HelperGroup::fixup(void)
{
	DEBUG3("HelperGroup::fixup()\n");
	// FIXME: to be implemented
	return false;
}

Q3ListViewItem* HelperGroup::newLVI(Q3ListViewItem* aQLVI, 
				const QString& aGroupName, bool* isOKPtr)
{
	QString myMessage;
	if (theGroupList.contains(aGroupName))
	{
		myMessage = QObject::tr("is member");
		*isOKPtr = true;
	}
	else
		myMessage = QObject::tr("not a member - not fixable by umtsmon");

	Q3ListViewItem* mySubQLVI  = new Q3ListViewItem (aQLVI, "", 
				aGroupName, "n/a", "n/a", myMessage);
	return mySubQLVI;
}

// ###########################################################################
// ###########################################################################
// ###########################################################################
// ###########################################################################
// ###########################################################################


HelperList::~HelperList()
{
	DEBUG3("HelperList::~HelperList\n");
	while(theHelperDataList.size()!=0)
	{
		delete (*(theHelperDataList.begin()));
		theHelperDataList.pop_front();
	}
}

HelperList::TestResults HelperList::testAndRun(void)
{
	DEBUG3("HelperList::testAndRun(void)\n");

	createHelperList();
	
	// if nothing to fix - don't create window
	if (needsFixing()==false)
		return OK;

	// create gui and handle everything
	theGUIPtr = new helperlistgui();
	theGUIPtr->show();
	theGUIPtr->setHelperListPtr(this);
	addListToWidget();
	theGUIPtr->exec();
	return OK;
}

bool HelperList::needsFixing(void)
{
	DEBUG3("HelperList::needsFixing\n");
	HelperDataList::iterator i = theHelperDataList.begin(); 
	for (; i!=theHelperDataList.end(); i++)
	{
		if ( (*i)->isNeedingFix() )
			return true;
	}
	return false;
}


bool HelperList::fixIt(void)
{
	DEBUG3("HelperList::fixIt\n");
	
	bool wasAllFixed = true;
	
	HelperDataList::iterator i = theHelperDataList.begin(); 
	for (; i!=theHelperDataList.end(); i++)
	{
		if ( (*i)->isNeedingFix() )
		{
			if ( ((*i)->fixup()) == false)
				wasAllFixed = false;
		}
	}
	return wasAllFixed;
}

	
void HelperList::createHelperList(void)
{
	DEBUG3("HelperList::createHelperList()\n");
	// create the appropriate HelperAdata-derived structs
	theHelperDataList.push_back(new HelperApp(1, Runner::PPPD, "", QObject::tr("required if you want to make connections")));

	QString thePccardctlStdOut = "";
	if (!theRunner.getPath(Runner::PCCARDCTL).isEmpty())
	{
		theRunner.runCommand(Runner::PCCARDCTL, QStringList("--version"));
		thePccardctlStdOut = theRunner.getStdOutStringList()[0];
	}
	theHelperDataList.push_back(new HelperApp(2, Runner::PCCARDCTL, 
					thePccardctlStdOut,
					QObject::tr("only required for PCMCIA cards")));
	
	theHelperDataList.push_back(new HelperApp(3, Runner::SOMESU));

 	theHelperDataList.push_back(new HelperApp(4, Runner::USBMODESWITCH,
					"", QObject::tr("if this app is present, you probably need it.")));

	theHelperDataList.push_back(new HelperDev(5, 
			TheDeviceManagerSingleton::getDeviceCapsRef().getATSerialPortName(), 
			TheDeviceManagerSingleton::getDeviceCapsRef().getPPPSerialPortName()));
	
	theHelperDataList.push_back(new HelperGroup(6));
}

void HelperList::addListToWidget(void)
{
	theGUIPtr->clearList();
	HelperDataList::iterator i = theHelperDataList.begin(); 
	for (; i!=theHelperDataList.end();i++)
		theGUIPtr->add(*i);
}

