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

#include "FileStuff.h"
#include <qdir.h>
#include <qstring.h>
#include <qregexp.h>
//Added by qt3to4:
#include <Q3TextStream>

QString
FileStuff::getFileFirstLine(const QString& aFileName)
{
	QFile myFile(aFileName);
	if (myFile.open(QIODevice::ReadOnly))
	{
	   	Q3TextStream in(&myFile);
		QString myLine = in.readLine().stripWhiteSpace();
		myFile.close();
		return myLine;
	}
	return NULL;	
}

bool
FileStuff::doesFileContainString(
	const QString& aFileName, 
	const QString& aSubString)
{
	QFile myFile(aFileName);
	if (myFile.open(QIODevice::ReadOnly) != true)
	{
		return false;
	}

	QString myLine;
	bool myResult = false;
   	Q3TextStream in(&myFile);
	while(!in.atEnd())
	{
		myLine = in.readLine().stripWhiteSpace();
		// perform substring searching
		if (aSubString.indexOf(myLine,0) != -1)
		{
			myResult = true;
			break;	// breaks while loop
		}
	}
	myFile.close();
	return myResult;
}

bool 
FileStuff::doesFileExist(const QString& aFileName)
{ 
	return QFile::exists(aFileName);
}

FileStuff::ReadWrite 
FileStuff::isFileReadWrite(const QString& aFileName)
{
	QFile myFile(aFileName);
	if (myFile.open(QIODevice::ReadWrite))
	{
		myFile.close();
		return READWRITE;
	}
	if (myFile.open(QIODevice::ReadOnly))
	{
		myFile.close();
		return READ;
	}
	if (myFile.open(QIODevice::WriteOnly))
	{
		myFile.close();
		return WRITE;
	}
	return NOTHING;
}


bool 
FileStuff::checkForRunningProcess(const QString& aProcessName)
{
	// retrieve a sorted list of all entries in the proc filesystem
	QString myBaseDirName="/proc/";
	QDir myProcDir(myBaseDirName);
	myProcDir.setFilter( QDir::Dirs );
	
	const QFileInfoList myList = myProcDir.entryInfoList();
	QListIterator<QFileInfo> myIterator( myList );
	
	while ( myIterator.hasNext() ) 
	{
	    const QFileInfo myFileInfo = myIterator.next();

		// if the dirname contains any non-number characters, including dots, skip it.
		QRegExp myNonNumberRegExp("\\D");
		if (myFileInfo.fileName().contains(myNonNumberRegExp))
			continue;
		// and check for the ProcessName on the command line			
		QString myCmdName = myBaseDirName + myFileInfo.fileName() + "/cmdline";
		if (FileStuff::doesFileContainString(myCmdName, aProcessName) == true)
			return true;
	}

	return false;
}



FileStuff::FindInDir::FindInDir(
			const QString& aDirName, 
			const QString& aNameFilter, 
			QDir::FilterSpec aWhatEntriesToReturn)
{
	theDir.setPath(aDirName);
	theDir.setNameFilter(aNameFilter);
	theDir.setFilter(aWhatEntriesToReturn);
	
	theFIListPtr = theDir.entryInfoList();
    theFILIPtr = new QListIterator<QFileInfo>( theFIListPtr );
}

FileStuff::FindInDir::~FindInDir()
{
    delete theFILIPtr;
}

QString 
FileStuff::FindInDir::getNextEntry(void)
{
	const QFileInfo myEntryInfo = theFILIPtr->next();
	if (!myEntryInfo.exists())
		return "";
	QString myFileName = myEntryInfo.absFilePath();
	return myFileName;
}
