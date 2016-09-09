/* umtsmon - a program to control/monitor your UMTS card
 * Copyright (C) 2007  Klaas van Gend
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

#include <qvariant.h>
#include <qdialog.h>
#include <qtable.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QTabWidget;
class QWidget;
class QLabel;
class QGroupBox;



class TestLabel
{
public:
	TestLabel* operator ->(void)	{return this;}

	void setText(QString aString)	{theString = aString; DEBUG2("setText(\"%s\")\n", aString.ascii());};
	QString text(void)			{return theString;};
private:
	QString	theString;
};


class TestTable
{
public:
	TestTable* operator ->(void)	{return this;}
	void insertRows(__attribute__ ((unused)) int a1)	{;};
	void sortColumn(__attribute__ ((unused)) int a1, 
	                __attribute__ ((unused)) bool a2,
	                __attribute__ ((unused)) bool a3)		{;};
	void adjustColumn(__attribute__ ((unused)) int a1)						{;};
	void setText(__attribute__ ((unused)) int a1, 
	             __attribute__ ((unused)) int a2, 
	             __attribute__ ((unused)) QString aString)	{;};
	QHeader * horizontalHeader () const {static QHeader myHeader; return &myHeader;};
};



class InfoDialogUmtsmon : public QDialog
{
    Q_OBJECT

public:
    InfoDialogUmtsmon() {};
    virtual ~InfoDialogUmtsmon() {};

    QTabWidget* tabWidget3;
    QWidget* TabPage;
    TestLabel lbl_CardType;
    TestLabel lbl_IMSI;
    TestLabel lbl_IMEI;
    TestLabel lbl_Manufacturer;
    TestLabel lbl_Revision;
    QWidget* TabPage_2;
    TestLabel lbl_System_Version;
    TestTable device_table;
    TestTable module_table;
	TestLabel lbl_pppd_version;
	TestLabel lbl_distro_release;

public slots:
	void sort_module_table(int section);
	void sort_device_table(int section);
	
public:
    void init();
    QString get_Info_using_AT_Query( QString anAtCommand );
    void readSystemInformations();
    void setPPPDversion(QString aVersionString);
};

