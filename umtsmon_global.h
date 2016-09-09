/* umtsmon - a program to control/monitor your UMTS card
 * This file copyright (C) 2006,2007,2008,2009  Klaas van Gend
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

#ifndef UMTSMON_GLOBAL
#define UMTSMON_GLOBAL

#define APPNAME "umtsmon"
#define APPRELEASE "0.9.72"
/* for your special versioning needs, normally empty */
#ifndef APPFLAVOUR
#define APPFLAVOUR ""
#endif

/* Debug verbosity
 * level 1 = very little information, autodetect KILLING
 * level 2 = level 1 + autodetect GOOD/BAD + some general messages on behaviour
 * level 3 = level 2 + autodetect INFO
 * level 4 = level 3 + AT commands and answers
 * level 5 = all debugging, including entry of functions under scrutiny
 * level 6 = timing info on serial port
 */
#include <stdio.h>
#include <time.h>
extern int theVerbosity;	// actually defined in main.cpp
extern int thePPPVerbosity;
#define MAX_VERBOSITY 6
// note: the below is actually GCC-specific, see "info:/gcc/Variadic Macros"
#define DEBUG1(format, ...)	{if (theVerbosity>=1) printf("##P1 t=%03ld: " format, time(NULL)%1000, ## __VA_ARGS__);}
#define DEBUG2(format, ...)	{if (theVerbosity>=2) printf("##P2 t=%03ld: " format, time(NULL)%1000, ## __VA_ARGS__);}
#define DEBUG3(format, ...)	{if (theVerbosity>=3) printf("##P3 t=%03ld: " format, time(NULL)%1000, ## __VA_ARGS__);}
#define DEBUG4(format, ...)	{if (theVerbosity>=4) printf("##P4 t=%03ld: " format, time(NULL)%1000, ## __VA_ARGS__);}
#define DEBUG4NT(format, ...)	{if (theVerbosity>=4) printf(format, ## __VA_ARGS__);}
#define DEBUG5(format, ...)	{if (theVerbosity>=5) printf("##P5 t=%03ld: " format, time(NULL)%1000, ## __VA_ARGS__);}
#define DEBUG6(format, ...)	{if (theVerbosity>=6) printf("##P6 t=%03ld: " format, time(NULL)%1000, ## __VA_ARGS__);}


#endif // UMTSMON_GLOBAL
