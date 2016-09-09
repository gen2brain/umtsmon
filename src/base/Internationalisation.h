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

#ifndef INTERNATIONALISATION_H_
#define INTERNATIONALISATION_H_

#include <qapplication.h>
#include <qstringlist.h>

// Forward declarations
class QTranslator;


/** this class contains all code to verify, find and load a translation file
 *  for umtsmon
 */ 
class Internationalisation
{
public:
	/// constructor
	Internationalisation(QApplication* aQApplicationPtr)
		: theApplicationPtr(aQApplicationPtr), theLocale("") {};
	
	/// virtual destructor
	virtual ~Internationalisation() {;};
	
	
	/// get the currently running Locale
	QString getLocale(void)
		{ if (theLocale.isEmpty()) return "en_US"; else return theLocale; };

	/** searches for all available translations
	 *  @returns a QStringList contaning all supported i18n codes
	 */
	const QStringList getAllSupportedLocales(void);
	
	/** called by main() and SetLanguageDialog only
	 *  Try the suggested locale
	 *  @param aSuggestedLocale   locale to validate and attempt to load
	 *  @returns true if a suggestion was valid and installed
	 */
	bool tryLocale(const QString& aSuggestedLocale);

	/** called by main() only 
	 *  Try the saved setting from config file - if there is one
	 *  @returns true if a valid setting was found and installed
	 */
	bool trySavedLocale(void);
	
private:
	QApplication* theApplicationPtr;
	QString theLocale;
	
	/// the directory containing all translations
	QString theTranslationsDirectory;
	
	/** internal member - some sanity checks
	 *  @param aLocale	locale to check
	 *  @returns true if all sanity checks passed OK
	 */
	bool validateLocale(const QString& aLocale);

	/** internal member - actually sets the locale defined in theLocale
	 *  @returns true if setting "theLocale" succeeded
	 */
	bool setLocale(void);
	
	/// writes locale setting to config file
	void saveLocale(void);
	
	/** tries different locations for a locale file and creates the corresponding
	 *  QTranslator. 
	 *     the i18n file usually looks like:  aFileName+"_"+aLocale+".qm"
	 *  @param aFileName  base for file name
	 *  @param aLocale    base for locale
	 *  @returns   pointer to QTranslator instance if success, or NULL on problems
	 */	
	QTranslator* createTranslator(const QString& aFileName, const QString& aLocale);
	
	/** (use only by createTranslator)
	 *  try to load a translator with the filename and path
	 */
	bool tryLoad(QTranslator* aTranslatorPtr, const QString& aFileName, const QString& aPath);
};


#endif /*INTERNATIONALISATION_H_*/
