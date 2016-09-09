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

#ifndef VALIDATEPHONENUMBER_CLASS
#define VALIDATEPHONENUMBER_CLASS

#include "umtsmon_global.h"
#include <qvalidator.h>
#include <qstring.h>

/// validates a phone number 
class ValidatePhoneNumber : public QValidator
{
public:
	ValidatePhoneNumber(QObject* aParent, const char* aName = 0);

	/// validate returns Acceptable if ... guess what...
	virtual State validate(QString& anInputString, __attribute__ ((unused)) int& aPos) const
	{
		/* ALLOWED is:
		* - any number of whitespace at the beginning
		* - 0,1 or 2   times '+' at the beginning
		*   (FIXME: only 0 or 1 is actually allowed by the card)
		* - any number of (0-9 '-' '.' ' ' or tab)
		*   but no starting 0 when a '+' is there...
		*/
		QValidator::State myReturn = Acceptable;

		unsigned int myPosition=0;
		int myPlusCount = 0;
		int myNumberCount = 0;
		WhereAreWe myState = COUNTINGPLUSES;

		while(myPosition < anInputString.length())
		{
			QChar myChar = anInputString[myPosition];
			myPosition++;
			if (myChar.isSpace())
				continue;
			switch(myState)
			{
			case COUNTINGPLUSES:
				if (myChar == '+')
					myPlusCount++;
				else
				{
					// re-check this one again
 					myPosition--;
					myState=CHECKINGNUMBERS;
				}
				break;
			case CHECKINGNUMBERS:
				if (myChar == '0' && myPlusCount > 0 && myNumberCount == 0)
					myReturn = Invalid;
				if (myChar.isDigit())
					myNumberCount++;
				else
					myReturn = Invalid;
				break;
			}
		}

		if(myPlusCount > 2)
		{
			myReturn = Invalid;
		}

		if (myReturn == Acceptable)
		{
			if (myNumberCount < 8)
				myReturn = Intermediate;
		}

		return myReturn;
	};

	/// fixup... fixes the phone number...
	virtual void fixup(QString& anInputString) const
	{
		QString myGoodString;
		int myPlusCount=0;
		for (unsigned int i=0; i<anInputString.length(); i++)
		{
			QChar myChar = anInputString[i];
			if (myChar.isSpace())
				continue;
			if (myChar == '+')
			{
				if (myPlusCount < 1)
				{
					myPlusCount++;
					myGoodString += '+';
				}
			}
			else
				myGoodString += myChar;
		}
		anInputString = myGoodString;
	};


private:
	enum WhereAreWe
	{
		COUNTINGPLUSES,
		CHECKINGNUMBERS
	};

	bool isWhiteSpace(int aChar)
	{
		switch (aChar)
		{
		case ' ':
		case '-':
		case '.':
			return true;
		default:
			return false;
		}
	};

};


ValidatePhoneNumber::ValidatePhoneNumber(
		QObject* aParent, const char* aName)
	: QValidator(aParent, aName)
{
	DEBUG5("ValidatePhoneNumber\n");
}



#endif // defined VALIDATEPHONENUMBER_CLASS
