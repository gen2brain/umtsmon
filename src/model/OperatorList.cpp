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
#include "OperatorList.h"

#include "Query.h"
#include "ConnectionInfo.h"
#include "TheSettingsSingleton.h"

#include <assert.h>


OperatorList::OperatorList()
{
}

OperatorList::~OperatorList()
{
	while (!theOperatorList.empty())
	{
		delete theOperatorList.back();
		theOperatorList.pop_back();
	}
}
	
Operator* 
OperatorList::add(Operator* aNewOperator)
{
	Operator* myOp = findNetworkID(aNewOperator->getNetworkID());
	if (myOp == NULL)
	{
		theOperatorList.push_back(aNewOperator);
		return findNetworkID(aNewOperator->getNetworkID());
	}
	
	myOp->theRadioModes += "+" + aNewOperator->theRadioModes;
	
	// FORBIDDEN or CURRENT are stronger than AVAILABLE
	if (myOp->getIconType()!=Operator::CURRENT || myOp->getIconType()==Operator::FORBIDDEN)
		myOp->theIconType = aNewOperator->theIconType;
	
	return myOp;
}


void 
OperatorList::fill(const QString& anAnswer)
{
	QStringList  myList = QStringList::split("),", anAnswer);

	// in case of multiple answers on one line (e.g. Option)
	// we will iterate over all the answers here
	int i=0;
	for ( QStringList::Iterator myIt = myList.begin(); 
		myIt != myList.end(); ++myIt ) 
	{
		i=i+1;
		// kill everything up to the first '('
		QString myFullTelcoString = (*myIt).section("(",1,1);
		Operator* myNewOperatorPtr = new Operator(myFullTelcoString);
		if (myNewOperatorPtr->getIconType() != Operator::NOT_VALID)
			add(myNewOperatorPtr);
		else
			delete myNewOperatorPtr;
	}
}



Operator*
OperatorList::findNetworkID(unsigned long aNetworkID)
{
	TheOperatorListIterator i;
	for (i=begin();	i!=end(); i++)
	{
		if ((*i)->getNetworkID() == aNetworkID)
		{
			return *i;
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

static const char* theKnownOperators = "/KnownOperators/";


Operator::Operator(const QString& aFullTelcoString)
{
//	DEBUG5("Operator::Operator(%s)\n", aFullTelcoString.ascii());
//	DEBUG5("  allow: '%s'\n", aFullTelcoString.section(",",0,0).ascii());
//	DEBUG5("  full:  '%s'\n", aFullTelcoString.section(",",1,1).ascii());
//	DEBUG5("  short: '%s'\n", aFullTelcoString.section(",",2,2).ascii());
//	DEBUG5("  id:    '%s'\n", aFullTelcoString.section(",",3,3).ascii());
//	DEBUG5("  type:  '%s'\n", aFullTelcoString.section(",",4,4).ascii());

	switch (aFullTelcoString.section(",",0,0).toInt())
	{
	case 3: // forbidden
		theIconType = FORBIDDEN;
		break;
	case 1: // available
		theIconType = AVAILABLE;
		break;
	case 2:	// current
		theIconType = CURRENT;
		break;
	case 0:	// unknown -> denotes the end of the list
		theIconType = NOT_VALID;
		return;
	}

	theNetworkName = aFullTelcoString.section(",",1,1).replace("\"","");

	theNetworkID = 
			aFullTelcoString.section(",",3,3)
			.section("\"",1,1).toLong();

	theRadioModes =
		ConnectionInfo::getRadiotypeString(
			aFullTelcoString.section(",",4,4).toInt());
	
	TheSettingsSingleton::getQSRef().writeEntry( theKnownOperators + QString::number(theNetworkID), theNetworkName);
}


const Operator*
Operator::lookup (const QString& aNetworkID)
{
	QString myName = TheSettingsSingleton::getQSRef().readEntry( theKnownOperators + aNetworkID, "");
	IconType myType = AVAILABLE;
	if (myName.isEmpty())
	{
		myType = NOT_VALID;
		myName = "*" + aNetworkID + "*";
	}
	return new Operator(aNetworkID.toULong(), myName, myType);
}
