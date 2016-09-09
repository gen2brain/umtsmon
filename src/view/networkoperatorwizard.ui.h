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

/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

#include "umtsmon_global.h"
#include <assert.h>

#include <q3listview.h>
//Added by qt3to4:
#include <q3mimefactory.h>
#include <QPixmap>
//Added by qt3to4:
#include <q3mimefactory.h>

enum NetwOpWizardPages
{
	WELCOME,
	WHOPICKS,
	GETNWSTRENGTHS,
	SEARCHING,
	SELECT,
	CONNECTING,
	FINISH
};


void NetworkOperatorWizard::init()
{
    DEBUG5("NetworkOperatorWizard::init()\n");
    myConnectTimer = NULL;
    mySearchTimer = NULL;
    
	connect(table_operators->horizontalHeader(), SIGNAL(clicked(int)), this, SLOT(sort_table_operators(int)));
}

void NetworkOperatorWizard::back()
{
    DEBUG5("NetworkOperatorWizard::back()\n");
    int myCurrentPageNumber = indexOf(currentPage());
    assert (myCurrentPageNumber>=0 && myCurrentPageNumber<pageCount());
    NetwOpWizardPages myNextPageNumber = WELCOME;
    switch (static_cast<NetwOpWizardPages>(myCurrentPageNumber))
    {
    case WELCOME:
    case WHOPICKS:
    	myNextPageNumber = WELCOME;
    	break;
    case GETNWSTRENGTHS:
    	myNextPageNumber = WHOPICKS;
    	break;
    case SEARCHING:
    	// this situation only happens when the startSearch() runs
    	// into trouble and emits "back".
    	// therefore:
    	myNextPageNumber = GETNWSTRENGTHS; 
    	break;
    case SELECT:
    	// skip SEARCHING
    	myNextPageNumber = GETNWSTRENGTHS;
    	break;
    case CONNECTING:
    	// this situation only happens when the connect process runs into
    	// trouble (e.g. timeout)
    	// therefore:
    	myNextPageNumber = SELECT; 
    	break;
    	// should never happen
    	assert(false);
    case FINISH:
    	if (radioButton_card->isChecked())
	    	// back to where we came from
	    	myNextPageNumber = WHOPICKS;
    	else
	    	// skip CONNECTING;
	    	myNextPageNumber = SELECT;
    	break;
    };
    gotoPage(myNextPageNumber);
}    
    

// under some circumstances, it is possible that calls to next()
// are nested. The code doesn't really know how to cope with it, but
// the side effects are fortunately rather limited :-)
void NetworkOperatorWizard::next()
{
	DEBUG5("NetworkOperatorWizard::next()\n");
	int myCurrentPageNumber = indexOf(currentPage());
	assert (myCurrentPageNumber>=0 && myCurrentPageNumber<pageCount());
	NetwOpWizardPages myNextPageNumber = FINISH;
	switch (static_cast<NetwOpWizardPages>(myCurrentPageNumber))
	{
	case WELCOME:
		myNextPageNumber = WHOPICKS;
		break;
	case WHOPICKS:
		if (radioButton_card->isChecked())
		{
			// switch device and finish
			if (myNetworkChanger.setCardToAutomatic() == false)
			{
				Popup::Warning("Switching the device to automatic operator selection failed. Please retry", this);
				myNextPageNumber = WHOPICKS;
			}
			else
				myNextPageNumber = FINISH;
			break;
		}
		myNextPageNumber = GETNWSTRENGTHS;
		break;
	case GETNWSTRENGTHS:
		myNextPageNumber = SEARCHING;
		break;
	case SEARCHING:
		myNextPageNumber = SELECT;
		break;
	case SELECT:
		myNextPageNumber = CONNECTING;
		break;
	case CONNECTING:
		myNextPageNumber = FINISH;
		break;
	case FINISH:
		// should never happen
		assert(false);
		myNextPageNumber = FINISH;
		break;
	};
	gotoPage(myNextPageNumber);
}    


void NetworkOperatorWizard::gotoPage(int aPageNumber)
{
    assert (aPageNumber>=0 && aPageNumber<pageCount());
	showPage(page(aPageNumber));
    switch (static_cast<NetwOpWizardPages>(aPageNumber))
    {
    case WELCOME:
    	setBackEnabled(currentPage(),false);
    	break;
    case WHOPICKS:
    	setBackEnabled(currentPage(),true);
    	switch(myNetworkChanger.getRegistrationState())
    	{
    	case NetworkChanger::MANUAL:
    	case NetworkChanger::MAN_AUTO:
    		emit radioButton_user->setChecked(true);
    		break;
    	case NetworkChanger::DEREGISTER:
    	case NetworkChanger::SETONLY:
    	case NetworkChanger::MODE_ERROR:
    		DEBUG1("Registration mode set to weird stuff\n");
    		// and walk gracefully into the next one:
    	case NetworkChanger::AUTOMATIC:
    		emit radioButton_card->setChecked(true);
    		break;
    	}
    	
    	break;
    case GETNWSTRENGTHS:
    	break;
    case SEARCHING:
    	setBackEnabled(currentPage(),false);
    	setNextEnabled(currentPage(),false);
    	emit startSearch();
    	break;
    case SELECT:
    	setBackEnabled(currentPage(),true);
    	// will be enabled when the user clicks on a table entry
    	setNextEnabled(currentPage(),false);
    	emit updateTable();
    	break;
    case CONNECTING:
    	setBackEnabled(currentPage(),false);
    	setNextEnabled(currentPage(),false);
    	emit startConnect();
    	break;
    case FINISH:
    	setBackEnabled(currentPage(),true);
    	setNextEnabled(currentPage(),false);
    	setFinishEnabled(page(FINISH), true);
    	break;
    };
	setHelpEnabled(currentPage(), false);
}   


void NetworkOperatorWizard::operatorSelected()
{
	DEBUG5("NetworkOperatorWizard::operatorSelected()\n");
	
	// if an operator is FORBIDDEN, don't enable next>
	long myID = table_operators->text(table_operators->currentRow(),3).toLong();
	Operator* myOp = myNetworkChanger.theOperatorList.findNetworkID(myID);
	assert(myOp != NULL);
	if (myOp == NULL)
		return;
	
	DEBUG5("user picked operator %ld\n", myID);
	if (myOp->getIconType()==Operator::FORBIDDEN)
	{
		Popup::Info("You are not allowed to connect to " + myOp->getNetworkName(), this);
		setNextEnabled(currentPage(),false);
	}
	else
		setNextEnabled(currentPage(),true);
}

// 20 seconds = 20/0.3 = 66 steps
static const int MAX_TIMEOUT_CONNECT = 66;

void NetworkOperatorWizard::startConnect()
{
	DEBUG5("NetworkOperatorWizard::startConnect()\n");
	// We can get the required info from the table - the fourth column
	// after all, the user clicked on the line, right?
	QString myID = table_operators->text(table_operators->currentRow(),3);
	NetworkChanger::SwitchResults myResults = myNetworkChanger.startSwitchToNetworkID(myID.toLong());
	switch(myResults)
	{
	case NetworkChanger::IS_CURRENT:
	case NetworkChanger::OK:
		// instant switching, not really likely to happen...
		// unless e.g. the user selected the network he's already in
		DEBUG5("immediate is current / ok\n");
		emit next();
		return;
		break;
	case NetworkChanger::ERROR:
		// uh-oh
		DEBUG5("immediate error\n");
		emit back();
		return;
		break;
	case NetworkChanger::NOT_READY_YET:
		// handled outside switch
		break;
	}
	
	assert(myConnectTimer == NULL);
	if (myConnectTimer != NULL)
		return;
	myConnectCount = 0;
	myConnectTimer = new QTimer(this, "connecttimer");
	connect(myConnectTimer, SIGNAL(timeout()), SLOT(progressConnect()));
	myConnectTimer->start(300, FALSE);
	emit progressConnect();
}

void NetworkOperatorWizard::progressConnect()
{
	DEBUG5("NetworkOperatorWizard::progressConnect()\n");
	
	NetworkChanger::SwitchResults myResults = myNetworkChanger.isSwitchCompleted();
	switch (myResults)
	{
	case NetworkChanger::IS_CURRENT:
	case NetworkChanger::OK:
		// success
		stopTimer(&myConnectTimer);
		DEBUG5("progress - iscurrent/ok\n");
		emit next();
		break;
	case NetworkChanger::ERROR:
	{
		// uh-oh
		DEBUG5("progress - error\n");
		stopTimer(&myConnectTimer);
		// one of the arguments of the query should be an ERROR.
		QString myError = myNetworkChanger.getErrorMessage();
		myNetworkChanger.stopSearch();
		Popup::Warning("The following error occured:\n" + myError, this);
		emit back();
		break;
	}
	case NetworkChanger::NOT_READY_YET:
		// handled outside switch
		break;
	}
	
	myConnectCount++;
	progressBar_connecting->setProgress(myConnectCount, MAX_TIMEOUT_CONNECT);
	if (myConnectCount > MAX_TIMEOUT_CONNECT)
	{
		stopTimer(&myConnectTimer);
		emit next();
	}
}


// 150 seconds = 150/0.3 = 500 steps
static const int MAX_TIMEOUT_SEARCH = 500;

void NetworkOperatorWizard::startSearch()
{
	DEBUG5("NetworkOperatorWizard::startSearch()\n");
	assert(mySearchTimer == NULL);
	if (mySearchTimer != NULL)
		return;
	mySearchCount = 0;
	mySearchTimer = new QTimer(this, "connecttimer");
	connect(mySearchTimer, SIGNAL(timeout()), SLOT(progressSearch()));
	mySearchTimer->start(300, FALSE);
	
	if (myNetworkChanger.startSearching() == false)
	{
		// we get here if the card immediately returned with an error,
		// e.g. if the PIN code has not been entered or 
		// if the PIN has not propagated through all of the card's subsystems 
		// yet - see Bug # 1527401.
		stopTimer(&mySearchTimer);
		Popup::Warning("The card returned ERROR when asking to start searching. You might want to try again",this);
		emit back();
		return;
	}
	emit progressSearch();
}

void NetworkOperatorWizard::progressSearch()
{
	DEBUG5("NetworkOperatorWizard::progressSearch()\n");
	
	if (myNetworkChanger.isSearchCompleted())
	{
		stopTimer(&mySearchTimer);
		emit next();
		return;
	}
	
	mySearchCount++;
	progressBar_searching->setProgress(mySearchCount, MAX_TIMEOUT_SEARCH);
	if (mySearchCount > MAX_TIMEOUT_SEARCH)
	{
		stopTimer(&mySearchTimer);
		Popup::Warning("The device didn't respond in time - selecting the operator probably failed...",this);
		emit back();
	}
}


//void NetworkOperatorWizard::startTimer(QTimer* aTimerPtr, void* aSlot())
//{
//	assert(aTimerPtr == NULL);
//	if (aTimerPtr != NULL)
//		return;
//	myConnectCount = 0;
//	aTimerPtr = new QTimer(this, "connecttimer");
//	connect(aTimerPtr, SIGNAL(timeout()), SLOT(aSlot));
//	aTimerPtr->start(300, FALSE);
//}


void NetworkOperatorWizard::stopTimer(QTimer** aTimerPtrPtr)
{
	(*aTimerPtrPtr)->stop();
	delete (*aTimerPtrPtr);
	*aTimerPtrPtr = NULL;
}

void NetworkOperatorWizard::updateTable()
{
    DEBUG5("NetworkOperatorWizard::updateTable()\n");
    
    // empty the QTable
    while(table_operators->numRows() > 0)
    	table_operators->removeRow(0);

	if (myNetworkChanger.theOperatorList.size()==0)
	{
		Popup::Info(tr("No network operators were found.\nYou might want to try again."), this);
		emit back();
		return;
	}

	// create the pixmaps (will be copied upon entry in the table)
	QPixmap myOK( qPixmapFromMimeSource("ok.png") );
	QPixmap myForbidden( qPixmapFromMimeSource("process-stop-16x16.png") );

	// fill the table
	for (TheOperatorListIterator myI=myNetworkChanger.theOperatorList.begin();
		myI != myNetworkChanger.theOperatorList.end(); myI++)
	{
		table_operators->insertRows(0);
		table_operators->setText(0,0, (*myI)->getNetworkName() );
		table_operators->setText(0,1, (*myI)->getRadioModes() );
		table_operators->setText(0,3, QString::number((*myI)->getNetworkID()) );

		switch((*myI)->getIconType())
		{
		case Operator::NOT_VALID:
		case Operator::FORBIDDEN:
			table_operators->setPixmap(0, 0, myForbidden);
			break;
		case Operator::AVAILABLE:
			break;
		case Operator::CURRENT:
			table_operators->setPixmap(0, 0, myOK);
			break;
		}
	}
	emit sort_table_operators(0);
}



/**
* SLOT to sort the rows of the table depending on the selected column
* @param the section/column to sort
**/
void NetworkOperatorWizard::sort_table_operators(int aSection) 
{

	if (table_operators->horizontalHeader()->sortIndicatorOrder() == Qt::AscendingOrder) 
	{
		table_operators->sortColumn(aSection,false,true);
	} 
	else 
	{
		table_operators->sortColumn(aSection,true,true);
	}
	table_operators->horizontalHeader()->adjustHeaderSize();
}
