/* umtsmon - a program to control/monitor your UMTS card
 * Copyright (C) 2006, 2007  Klaas van Gend
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
#include "Runner.h"

#include <qstring.h>
#include <qstringlist.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qdatetime.h>

#include <assert.h>
#include <errno.h>

#include "../TestFrame.cpp"

// conveniently set theVerbosity to a number of your liking
int  theVerbosity = 5;


int runChild(void)
{
	fprintf(stdout, "hello on stdout 1\n");
	fprintf(stderr, "hello on stderr 1\n");

	sleep(1);

	fprintf(stdout, "hello on stdout 2\n");
	fprintf(stderr, "hello on stderr 2\n");

	sleep(3);
	return 0;
}



int main(int argc, __attribute__ ((unused)) char* argv[])
{
	if (argc==2)
		return runChild();
		
	// I'm not entirely sure if this is allowed and/or works
	errno = 0;
	
	// welcome to user
	printf("Test app is called '%s' version '%s'\n", APPNAME, APPRELEASE);
	// and check if the APPNAME is correct
	check(QString(APPNAME) != QString("umtsmon"), "Tester is not called umtsmon\n", true);
	
	Runner myRunner;
	check(myRunner.amIRoot()==false, "This app is not run as root - good\n", true);

	printf("*** warning: the next test takes approx 6 seconds to complete\n");
	check(myRunner.runCommand(Runner::RUNNERTESTER,"child") == 0, "the stdout/stderr test returned ok\n");
	check(myRunner.getStdOutStringList().size()==2, "two strings on stdout\n");
	check(myRunner.getStdErrStringList().size()==2, "two strings on stderr\n");

	// let's try to create a file "hello" in /
	check(myRunner.runCommand(Runner::SOMESU,"/usr/bin/touch /hello") == 0,
		"(kde|gnome)su returned success\n", true);
	check(QFile::exists("/hello"), "/hello indeed exists\n");
	QFileInfo myFI("/hello");
	check( myFI.lastModified() >= QDateTime::currentDateTime().addSecs(-5),
		"/hello was created/modified during the last 5 seconds\n");

	//try to execute some commands in an shell 
	//first attempt - all commands in one string
	check(myRunner.runCommand(Runner::SOMESU,"/bin/sh -c \"echo hello >> /hello; wc -l /hello >/hello2;\"") == 0,
		"(kde|gnome)su returned success for shell exec, first attempt\n");
	check(QFile::exists("/hello2"), "/hello2 exists\n");
	QFileInfo myFI2("/hello2");
	check( myFI2.lastModified() >= QDateTime::currentDateTime().addSecs(-5),
		"/hello2 was created/modified during the last 5 seconds\n        This is known to work with \"kdesudo\"\n");

	//second attempt
	QStringList myCommands = "/bin/sh";
	myCommands += "-c";
	myCommands += "echo hello >> /hello; wc -l /hello >/hello3";
	check(myRunner.runCommand(Runner::SOMESU,myCommands)==0,
		"(kde|gnome)su returned success for shell exec, second attempt\n");
	check(QFile::exists("/hello3"), "/hello3 exists\n");
	QFileInfo myFI3("/hello3");
	check( myFI3.lastModified() >= QDateTime::currentDateTime().addSecs(-5),
		"/hello3 was created/modified during the last 5 seconds\n        This is known to work with \"gksu\"\n");

	// let's check if pccardctl actually is SUID, 
	// because otherwise the 'eject' won't work
	if (!myRunner.isSUID(Runner::PCCARDCTL))
	{
		// uh-oh, we need to suid it...
		check(myRunner.runCommand(Runner::SOMESU,
			"/bin/chmod +s "+myRunner.getPath(Runner::PCCARDCTL) ) == 0,
			"setting suid of pccardctl succeeded\n");
	}

	// let's try to eject a PCMCIA card
	check (myRunner.runCommand(Runner::PCCARDCTL, "eject") == 0,
			"pccardctl succeeded in eject\n");

	myRunner.runCommand(Runner::PPPD, "--help");
	printf(" --- first line output of `pppd --help`: '%s'\n", 
				myRunner.getStdErrStringList()[0].ascii());
	check(myRunner.getStdErrStringList()[0].contains("version"), "pppd shows version\n");


	// keep this last for summary:
	finish();
	return 0;
}
