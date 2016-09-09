/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2008 Klaas van Gend
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

#ifndef OPERATORLIST_H_
#define OPERATORLIST_H_

#include <qstring.h>
#include <list>

// forward declaration
class Operator;

// some typedefs to ease this class' interface
typedef std::list<Operator*> TheOperatorList;
typedef TheOperatorList::iterator TheOperatorListIterator;


class OperatorList
{
public:
	OperatorList(void);
	virtual ~OperatorList(void);

	TheOperatorListIterator begin(void)
		{ return theOperatorList.begin(); };
	TheOperatorListIterator end(void)
		{ return theOperatorList.end(); };
	size_t size(void)  
		{ return theOperatorList.size(); };

	
	/** fill the operator list from the answer from a single line of an AT+COPS=? query
	 *  depending on the card that generated the output, it can contain multiple operators
	 * 
	 *  if the operator already exists, its name string will 
	 *  be appended to contain the new radio type 
	 */
	void fill(const QString& anAnswer);
	
	/** searches the current list for the network ID and returns a pointer to the Operator instance
	 * @returns pointer to the found instance or NULL if not found
	 */
	Operator* findNetworkID(unsigned long aNetworkID);

protected:
	/// the real list of available networks
	TheOperatorList theOperatorList;

	/** adds the info of the Operator to the list
	 *  if the operator already exists (checked through the ID),
	 *  it will merge the info
	 *  @param aNewOperator  pointer to Operator instance containing the new data
	 *  @returns   pointer to the Operator class finally containing the data
	 */
	Operator* add(Operator* aNewOperator);

};



class Operator
{
public:
	/** constructor - to be called with a single, full AT+COPS=? entry.
	 */
	Operator(const QString& aFullTelcoString);
	
	/** "kind of a" constructor - initialises an Operator class based on the
	 *  networkid - by looking it up in the settings file.
	 *  @param aNetworkID   QString containing numerical value of network ID 
	 *                      (number to identify a mobile operator). 
	 *  @returns the resulting Operator instance - it is the responsibility of
	 *           the caller to delete this instance 
	 *  If aNetworkID is unknown, the Icon type will be NOT_VALID and the 
	 *  NetworkName will be empty.                       
	 */
	static const Operator* lookup (const QString& aNetworkID);
			
	virtual ~Operator(void) {;};
	
	enum IconType{ NOT_VALID=0, CURRENT, AVAILABLE, FORBIDDEN};
	
	IconType  getIconType(void)	
		{return theIconType;};
	
	const QString&  getNetworkName(void) const
		{ return theNetworkName;};
	
	const QString&  getRadioModes(void) const
		{ return theRadioModes;};

	/** @returns the network ID. 
	 *  We've observed network IDs to be in the 10,000-400,000 range only
	 */
	unsigned long	getNetworkID(void) const
		{return theNetworkID;};
	
private:
	IconType      theIconType;
	QString       theNetworkName;
	QString       theRadioModes;
	unsigned long theNetworkID;

	friend class OperatorList;
	
	/// private default constructor - not defined, not accessible
	Operator(void);
	/// private constructor - only accessible internally
	Operator(unsigned long aNetworkID, const QString& aName, IconType aType)
		: theIconType(aType), theNetworkName(aName), theNetworkID(aNetworkID) {;};
};

#endif /*OPERATORLIST_H_*/
