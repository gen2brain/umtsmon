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
#ifndef FILESTUFF_CLASS
#define FILESTUFF_CLASS

#include <qregexp.h>
#include <qstring.h>
#include <qdir.h>
#include <QListIterator>

class FileStuff
{
public:
	/** checks if a file contains a certain (sub)string.
	 *  @returns  false if file does not exist or on other errors, true if the substring was found
	 */ 
	static bool doesFileContainString(const QString& aFileName, const QString& aSubString);

	/** checks if the file exists
	 *  @returns  true if the file exists
	 */
	static bool doesFileExist(const QString& aFileName);

	/** reads the first line of the file
	 *  precondition: the file must exist - check with "doesFileExist()"
	 *  @returns   NULL if file does not exist or other errors, 
	 *               a QString with the first line (max 255 chars) otherwise
	*/
	static QString getFileFirstLine(const QString& aFileName);

	/** checks if the file is readable and/or writable by the user
	 *  @returns the corresponding value out of the enum ReadWrite. If file doesn't exist, returns NOTHING
	 */
	enum ReadWrite  { NOTHING, READ, WRITE, READWRITE };
	static ReadWrite isFileReadWrite(const QString& aFileName);

	/** checks if process is running
	 *  @param aProcessName process name to look for
	 *  @returns true if found at least once
	 */
	static bool checkForRunningProcess(const QString& aProcessName);

	/// this class abstracts the QDir class and iterating through its output
	class FindInDir
	{
	public:
		/** constructor
		 *  @param aDirName             directory where you want to iterate through
		 *  @param aNameFilter          wildcards to limit to certain names
		 *  @param aWhatEntriesToReturn limit to certain types of entries
		 * 			(examples are QDir::All, QDir::Dirs, QDir::Files)
		 * 			(see QDir::FilterSpec for the full list)
		 */
		FindInDir(const QString& aDirName, const QString& aNameFilter, 
		          QDir::FilterSpec aWhatEntriesToReturn = QDir::All);

        virtual ~FindInDir();
		
		/** returns the (first or next) entry in the directory,
		 *  returns an empty string if no (more) entries.
		 */
		QString getNextEntry(void);
	
	private:
		QDir	                  theDir;
        QFileInfoList             theFIListPtr;
        QListIterator<QFileInfo>* theFILIPtr;
	}; // end-of-class FindInDir


}; 

#endif // defined FILESTUFF_CLASS
