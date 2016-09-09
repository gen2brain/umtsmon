/* umtsmon - a program to control/monitor your UMTS card
 * Copyright (C) 2006,2007,2008  Klaas van Gend
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

#ifndef QUERY_CLASS
#define QUERY_CLASS


#include <qstring.h>
#include <qstringlist.h>
#include <qmutex.h>

// forward declaration
class SerialPort;

/** Holds the modem query plus its result. 
 *  It also parses parts of the results for common error conditions.
 */ 
class Query
{
public:
	/** constructor for Query.
	 *  @param aString - query, will be appended with terminator character
	 */
	Query(const QString& aQueryString)
		: theQueryString(aQueryString), hasLock(false), isFirstRun(true)
			{};

	virtual ~Query();

	/// contains the return code from the modem on the query
	enum ReturnCode
	{
		OK,
		ERROR,
		NO_CARRIER,
		NO_DEVICE,
		NO_LOCK,
		UNKNOWN_ERROR
	};

	/** call this member to actually run the query
	 *  @return NO_LOCK if failed to obtain the lock
	 *          - in that case, the Query hasn't run yet, retry is possible.
	 */ 
	ReturnCode run();

	/// returns the answer from the modem - split by line
	QStringList& getAnswer();

	/// returns true if there is an answer	
	bool hasAnswer()
		{ return !theAnswer.empty();};

	/** send even more data. 
	 *  @param aString - string to send, will be appended with terminator character
	 *  @return  true if successful, false if not
	 */
	bool sendMore(const QString& aString);

	/** receive even more data
	 *  this function will clear theAnswer first and also clear theQueryString first
	 */
	ReturnCode receiveMore(void); 

	/** sets the serial port to use 
	 *  (static member - valid for all Queries!)
	 *  Should only be called by the SerialPort class
	 * @param aSerialPortPtr the new serial port pointer
	 * @returns the old serial port pointer - if there was one
	 */
	static SerialPort* setSerial(SerialPort* aSerialPortPtr);
	
	/// returns true if a SerialPort has been set
	static bool hasSerial(void);

	/** call this member to actually run the query and keep waiting 
	 *  until "OK" "ERROR" or similar is received.
	 *  DO NOT USE THIS FUNCTION IF YOU EXPECT THE ANSWER TO TAKE MORE THAN HALF A SECOND
	 *  in that case: write a loop and use receiveMore()
	 *  @return NO_LOCK if failed to obtain the lock
	 *          - in that case, the Query hasn't run yet, retry is possible.
	 */ 
	ReturnCode runUntilDone();


private:
	/** read a string from the device
	 *  will retry 3 times
	 *  @returns QString with the info read - it can be multiple lines
	 */
	QString receiveStringRetry(void) const;

	/** read a string from the device
	 *  will attempt only once
	 *  @returns QString with the info read - it can be multiple lines
	 */
	QString receiveStringOnce(void) const;

	/// interpret the last line from the response (OK/ERROR/etc)
	ReturnCode interpretResult(void);

	/** check if first line equals "QUERY+\r", 
	 *  and remove it if so. 
	 */
	bool checkQuery(void);

	/** internal call actually running the query
	 *  @return NO_LOCK if failed to obtain the lock
	 *          - in that case, the Query hasn't run yet, retry is possible.
	 */ 
	ReturnCode realRun(void);

private:
	QString       theQueryString;
	QStringList   theAnswer;
	
	/** This Mutex is needed to prevent two queries from running in parallel.
	 *  this can happen if e.g. you are searching for a network to connect to
	 *  and in the mean time iconify the program.
	 *  
	 *  The basic assumption in this code is that the Mutex is locked on first
	 *  action (i.e. run())  and unlocked on destroy.
	 */ 
	QMutex		  theSerialPortMutex;
	/** Unfortunately, it is possible that run() cannot lock the mutex.
	 *  in that case, run will return NO_LOCK, but we need to know not to unlock
	 *  the mutex in the destructor.
	 */
	bool		  hasLock;

	bool isFirstRun;
};


#endif // defined QUERY_CLASS
