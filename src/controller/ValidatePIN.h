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

#ifndef VALIDATEPIN_CLASS
#define VALIDATEPIN_CLASS

#include "umtsmon_global.h"
#include <qvalidator.h>
#include <qstring.h>

/// validates the PIN
class ValidatePIN : public QValidator
{
public:
	ValidatePIN(QObject* aParent, const char* aName = 0);
	
	/// validate returns Acceptable if ... guess what...
	virtual State validate(QString& anInputString, __attribute__ ((unused)) int& aPos) const
	{
		/* ALLOWED is:
		* ints: 0-9
		* 4 to 8 digits....
		*/

		unsigned int myPosition=0;

		while(myPosition < anInputString.length())
		{
			
			QChar myChar = anInputString[myPosition];
			myPosition++;
			if (!myChar.isNumber())
				return Invalid;
		}
		return Acceptable;
	};

};

inline ValidatePIN::ValidatePIN( QObject* aParent, const char* aName) : QValidator(aParent, aName)
{
	DEBUG5("Validate PIN\n");
}

#endif // defined VALIDATEPIN_CLASS
