/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2006  Klaas van Gend
 *                     (C) 2007  Christofer Wesseling, Klaas van Gend
 *                     (C) 2008  Klaas van Gend
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
#ifndef POPUP_H_
#define POPUP_H_

#include <qstring.h>
#include <qmessagebox.h>

// for exit(3)
#include <cstdlib>

/// DON'T use this function in your code!!!
extern void printAbortMessage(int aSignalNumber);


/** This is a class utility - it encapsulates three member functions to do 
 *  quick popup dialogs
 */
class Popup
{
public:
	/** pops up a message box with a critical message
	 *  PROGRAM EXECUTION WILL STOP AND THE APPLICATION WILL HALT
	 * @param aMessage: the Message
	 * @param currentWidget: the Widget below the Message - if Popup should appear in front of another form than the mainWindow
	 */
	static inline void Critical(const QString& anErrorMessage, QWidget* currentWidget = NULL)
	{
		DEBUG1("*** CRITICAL ERROR: %s\n",anErrorMessage.ascii());
		QMessageBox::critical (currentWidget, 
				APPNAME " critical error",
				anErrorMessage,
				QMessageBox::Abort |  QMessageBox::Default, 
				Qt::NoButton, 
				Qt::NoButton);
		// critical errors? we might want to explain the user on 
		// how to help solve this!
		printAbortMessage(0);
		// printAbortMessage doesn't exit - good!
		// leaves us room to explain why we actually stop
		DEBUG1("Critical Popup - application will halt\n");
		exit(10);	
	};
	
	/** pops up a message box with a informational message
	 *  program execution will continue afterwards
	 * @param aMessage: the Message
	 * @param currentWidget: the Widget below the Message - if Popup should appear in front of another form than the mainWindow 
	 */
	static inline void Info(const QString& aMessage, QWidget* currentWidget = NULL)
	{
		DEBUG2("Info Message: %s\n",aMessage.ascii());
		QMessageBox::information (currentWidget, 
				APPNAME " informational message",
				aMessage,
				QMessageBox::Ok |  QMessageBox::Default, 
				Qt::NoButton, 
				Qt::NoButton);
	};
	
	/** pops up a message box with a warning message
	 *  program execution will continue afterwards
	 * @param aMessage: the Message
	 * @param currentWidget: the Widget below the Message - if Popup should appear in front of another form than the mainWindow
	 */
	static inline void Warning(const QString& aMessage, QWidget* currentWidget = NULL)
	{
		DEBUG2("Warning Message: %s\n",aMessage.ascii());
		QMessageBox::warning (currentWidget, 
				APPNAME " warning",
				aMessage,
				QMessageBox::Ok |  QMessageBox::Default, 
				Qt::NoButton, 
				Qt::NoButton);
	};

	/** pops up a message box with a question message
	 *  if question answered YES, will return true, otherwise false
	 * @param aMessage: the Message
	 * @param currentWidget: the Widget below the Message - if Popup should appear in front of another form than the mainWindow
	 * @returns true if clicked Yes, false if no or 'escape'
	 */
	static inline bool YesNoQuestion(const QString& aMessage, QWidget* currentWidget = NULL)
	{
		DEBUG2("Yes/No Question: %s\n", aMessage.ascii());
		if (QMessageBox::question(currentWidget, 
				APPNAME " question",
				aMessage,
				QMessageBox::Yes | QMessageBox::Default, 
				QMessageBox::No, 
				Qt::NoButton) == QMessageBox::Yes)
			return true;
		else
			return false;
	};
	
	
	/** pops up a message box with a warning message
	 *  program execution will continue afterwards
	 *  the message box will contain a checkbox "do not show again", which can be
	 *  checked - and works as advertised :-)
	 * @param aName: unique name of the message which will be used to save the user's preference
	 * @param aMessage: the Message
	 * @param currentWidget: the Widget below the Message - if Popup should appear in front of another form than the mainWindow
	 */
	static void WarningWithMemory(const QString& aName, const QString& aMessage, QWidget* currentWidget = NULL);

	/** pops up a message box with a question message
	 *  if question answered YES, will return true, otherwise false
	 *  the message box will contain a checkbox "do not show again", which can be
	 *  checked - and works as advertised :-)
	 * @param aName: unique name of the message which will be used to save the user's preference
	 * @param aMessage: the Message
	 * @param currentWidget: the Widget below the Message - if Popup should appear in front of another form than the mainWindow
	 * @returns true if clicked Yes, false if no or 'escape'
	 */
	static bool YesNoQuestionWithMemory(const QString& aName, const QString& aMessage, QWidget* currentWidget = NULL);
	
}; 


#endif /*POPUP_H_*/
