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
#include "Internationalisation.h"
#include "TheSettingsSingleton.h"
#include "FileStuff.h"

#include <qapplication.h>
#include <qtranslator.h>
#include <qtextcodec.h>
#include <qregexp.h>

#include <assert.h>
#include <stdlib.h>

bool 
Internationalisation::tryLoad(QTranslator* aTranslatorPtr, const QString& aFileName, const QString& aPath)
{
	DEBUG5(" . searching for %s.qm in %s\n", aFileName.ascii(), aPath.ascii());
	assert(aTranslatorPtr!=NULL);
	if (aTranslatorPtr->load( aFileName, aPath))
	{
		DEBUG4(" .. found in '%s'\n", aPath.ascii());
		theTranslationsDirectory= aPath;
		return true;
	}
	return false;
}


QTranslator* 
Internationalisation::createTranslator(const QString& aFileName, const QString& aLocale)
{
	DEBUG4("loadTranslator('%s', '%s')\n", aFileName.ascii(), aLocale.ascii());

	// basic sanity check first
	if (aFileName.isEmpty() || aLocale.isEmpty())
		return NULL;
	if (validateLocale(aLocale) == false && !aLocale.isEmpty())
	{
		DEBUG1("aLocale '%s' denied\n", aLocale.ascii());
		return NULL;
	}

	// Fix up the locale if required
	// i.e. pt_pt is not allowed, should be pt_PT :-(
	QString myLocale;
	if (aLocale.length()>3)
	{
		myLocale = aLocale.left(2).lower() + "_" + aLocale.right(aLocale.length()-3).upper();
	}
	else
		myLocale = aLocale.lower();
	
	QTranslator* myTranslator = new QTranslator(0);
	QString myQMFile = aFileName + "_" + myLocale;
	DEBUG5("Translation filename: '%s'\n", myQMFile.ascii());	
	// try in several locations: 
	//  note that QTranslator itself already tries several permutations, 
	//  including aLocale equal to "de" if "de_AT" is not found  
	if (tryLoad(myTranslator, myQMFile, "."))
		goto done;
	if (tryLoad(myTranslator, myQMFile, "i18n"))
		goto done;
	if (tryLoad(myTranslator, myQMFile, "/usr/local/share/" + aFileName + "/translations"))
		goto done;
	if (tryLoad(myTranslator, myQMFile, "/usr/share/" + aFileName + "/translations"))
		goto done;
	if (tryLoad(myTranslator, myQMFile, "/usr/lib/qt3/translations"))
		goto done;

	// failed:
	DEBUG5(" ... NOT FOUND\n");
	delete myTranslator;
	myTranslator = NULL;
	
	// just one extra: if "pt" doesn't work, try for "pt_PT"
	// don't be afraid of a little recursion :-D
	if (aLocale.length() == 2)
		return createTranslator(aFileName, aLocale + "_" + aLocale);
	return NULL;
	
done:
	theLocale = myLocale;
	return myTranslator;
}



const QStringList
Internationalisation::getAllSupportedLocales()
{
	QStringList mySupported;
	
	// is theTranslationsDirectory filled in?
	// if not: let's try to do that ourselves... 'nl' should exist :-D
	if (theTranslationsDirectory.isEmpty())
	{
		QString myTempLocale = theLocale;
		delete createTranslator(APPNAME, "nl");
		theLocale = myTempLocale;
	}
	// is it still empty? Not good...
	if (theTranslationsDirectory.isEmpty()==false)
	{
		QString myFileName;
		FileStuff::FindInDir myDir = FileStuff::FindInDir(theTranslationsDirectory, 
					"*.qm", QDir::Files);
		while( !( (myFileName=myDir.getNextEntry()) .isEmpty()) )
		{
			// remove everything before 'mon_' and everything after '.'
			QString myLang = QStringList::split("mon_", myFileName)[1];
			myLang = QStringList::split(".", myLang)[0];
			mySupported.push_back(myLang);
			DEBUG5("Found i18n file %s\n", myLang.ascii());
		}
	}

	// whatever happens, we support (American) English
	mySupported.push_back("en_US");
		
	DEBUG5("Internationalisation::getAllSupportedLocales returns a list with %d entries\n", 
			mySupported.count());
	return mySupported;
}



void 
Internationalisation::saveLocale(void)
{
	assert(!theLocale.isEmpty());
	assert(validateLocale(theLocale));
	TheSettingsSingleton::getQSRef().writeEntry(theLanguageSetting, theLocale);
}


bool 
Internationalisation::setLocale(void)
{
	QTranslator* myQTTranslator = createTranslator("qt", theLocale);
	QTranslator* myOwnTranslator = createTranslator(APPNAME, theLocale);
	if (myOwnTranslator == NULL)
		return false;

	// it makes only sense to install the QT translator if we have our own
	// translator. We might have our own translator without a QT translator
	if (myQTTranslator != NULL)
	{
		theApplicationPtr->installTranslator(myQTTranslator);
		DEBUG3("Will use specified (%s) locale for QT-internal strings\n", theLocale.ascii());
// FIXME: outcommenting the next two lines fixes i18n within valgrind...
//		delete myQTTranslator;
//		myQTTranslator = NULL;
	}
	else
		DEBUG3("Will use default (en_US) locale for QT-internal strings\n");
		

	theApplicationPtr->installTranslator(myOwnTranslator);
	DEBUG3("Will use specified (%s) locale for " APPNAME " strings\n", theLocale.ascii());
// FIXME: outcommenting the next two lines fixes i18n within valgrind...
//	delete myOwnTranslator;
//	myOwnTranslator = NULL;
	return true;
}


bool 
Internationalisation::tryLocale(const QString& aSuggestedLocale)
{
	// If ever someone wants to make e.g. en_GB, this has to change
	if (aSuggestedLocale.left(2) == "en")
	{
		DEBUG5("\"en\" is default language - no need to do something\n");
		theLocale = "en_US";
		return true;
	}

	if (validateLocale(aSuggestedLocale)==true)
	{
		theLocale = aSuggestedLocale;
		if (setLocale())
		{
			saveLocale();
			return true; 
		}
	}
	return false;
}


bool 
Internationalisation::trySavedLocale(void)
{
	QString mySaved = TheSettingsSingleton::getQSRef().readEntry(theLanguageSetting, "");
	if (mySaved.isEmpty())
	{
		DEBUG3("No locale suggestion found in config file\n");
		return false;
	}
	DEBUG5("Trying locale '%s' from settings file...\n", mySaved.ascii());
	return tryLocale(mySaved);
}



bool 
Internationalisation::validateLocale(const QString& aLocale)
{
	// allowed formats are:   nl, nl_NL or nl_nl. 
	// No numbers, no dots, etc.
	QRegExp myRegExp_nl("^[a-z]{2}$");
	QRegExp myRegExp_nl_nl("^[a-z]{2}_[a-zA-Z0-9.\\-@]{0,15}$");
	if (myRegExp_nl.search(aLocale) == 0)
		return true;
	if (myRegExp_nl_nl.search(aLocale) == 0)
		return true;
	return false;
}
