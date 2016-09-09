/* umtsmon - a program to control/monitor your UMTS card
 * Copyright (C) 2006,2007,2008 Klaas van Gend
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

#ifndef RUNNER_H
#define RUNNER_H

#include <qstring.h>
#include <qstringlist.h>


/** Runner is a class to start external programs with 
 *  Just instantiate the class and run a command and delete the class afterwards
 *  Settings like where to find the binaries are kept static - so creation/deletion of the class
 *  is not really expensive.
 * 
 *  WARNING: do not keep two detached class instances in parallel !!! 
 */
class Runner
{
public:

	Runner();
	virtual ~Runner() {};

	enum BinariesToRun
	{
		SOMESU,
		PPPD,
		CHAT,
		PCCARDCTL,
		USBMODESWITCH,
		RUNNERTESTER // do not touch - only for RunnerTester
	};

	static const QString& getPath(BinariesToRun aProgram);
	
	/** returns true if aProgram is suid root
	 *  @param AProgram   entry from the enum to check for suid
	 *  @returns true if suid, false if not (or not found)
	 */ 
	static bool isSUID(BinariesToRun aProgram);
	
	/// returns true if umtsmon is run as root or is suid root
	static bool amIRoot(void);

	/**  runs a program with specified argument string
	 *   the program will be run *without* any environment or search path
	 *   and the calling thread will be paused until the program returns
	 *   @param aProgram choose from the enum the program to run
	 *   @param anArgumentStringList  the arguments to the program
	 *          in case of SOMESU, this will binary plus args in a single item
	 *   @returns  the return value of the program
	 */
	int  runCommand(BinariesToRun aProgram, QStringList anArgumentStringList)
		// asking for out-of-process operation is not possible for external callers
		{ return runCommand(aProgram, anArgumentStringList, false); };

	/**  checks if the PPPD process exists
	 *   @returns -1 if process does not exist or return PID if found
	 */
	static int  doesPPPDexist(void);

	const QStringList& getStdOutStringList(void) const
		{return theStdOutTexts;};

	const QStringList& getStdErrStringList(void) const
		{return theStdErrTexts;};

private:
	/** this function tries to locate e.g. kdesu/gnomesu, pppd, chat, pccardctl
	 *  on a fixed, pre-defined path to make sure that we use the reliable ones
	 *  Note: this is the actual function setting the internal strings below
	 *  @returns true if everything went OK, false on errors
	 */
	bool findBinaries(void);

	/** checks if I am allowed to run a certain command
	 *  i.e. the x bit is set for either me, my group of the whole world
	 *  @param anExecutable  the name of the executable to test
	 *  @returns  true if I'm allowed to execute it
	 */
	bool isExecutableByMe(const QString& anExecutable) const;

	/** looks through the specified path or the default search path
	 *  /bin:/usr/bin:/sbin:/usr/sbin for the application
	 *  @param anAppName the name of the binary to look for
	 *  @param aPath  (optional) the path to search on - note that anyting not
	 *                in /opt or /usr will be ignored
	 *  @returns an empty QString if nothing found, or the path to the binary
	 *           plus the appname if found
	 */
	QString lookThroughPath(const QString& anAppName, const char* aPath);

	/**  runs a program with specified argument string
	 *   the program will be run *without* any environment or search path
	 *   and the calling thread will be paused until the program returns
	 *   @param aProgram choose from the enum the program to run
	 *   @param anArgumentStringList  the arguments to the program
	 *   @param isToDetach  true if you do not want to wait for the program to end
	 *   @returns  the return value of the program
	 */
	int  runCommand(BinariesToRun aProgram, QStringList anArgumentStringList, bool isToDetach);

	/** this member is executed in the child process of Runner.
	 *  THIS MEMBER SHOULD ONLY BE CALLED FROM runCommand()!!!
	 *   @param aProgram choose from the enum the program to run
	 *   @param anArgumentStringList  the arguments to the program
	 */  
	void execChild(BinariesToRun aProgram, QStringList anArgumentStringList);

private:
	static QString theSU;
	static QString thePPPD;
	static QString theCHAT;
	static QString thePCCARDCTL;
	static QString theUSBMODESWITCH;
	static QString theRUNNERTESTER;


	/// used for creating a pipe between the program and Runner
	int theStdOutPipe[2];
	/// used for creating a pipe between the program and Runner
	int theStdErrPipe[2];
	
	/** read all lines from a pipe into a string list
	 *  @param aStringList  string list to store read data into
	 *  @param anFD         file descriptor to read from
	 *  @returns true if data read, false if not
	 */
	bool readPipe(QStringList& aStringList, int anFD);

	/// contains the output over stdout of a run program
	QStringList theStdOutTexts;
	/// contains the output over stderr of a run program
	QStringList theStdErrTexts;
};


#endif // RUNNER_H
