/* umtsmon - a program to control/monitor your UMTS card
 * Copyright (C) 2008  Klaas van Gend
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
#ifndef SETLANGUAGEDIALOG_H_
#define SETLANGUAGEDIALOG_H_

#include <qvariant.h>
#include <qdialog.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3HBoxLayout>
#include <QLabel>

class Q3VBoxLayout;
class Q3HBoxLayout;
class QSpacerItem;
class QLabel;
class Q3ButtonGroup;
class QRadioButton;
class QPushButton;
class Internationalisation;

/// this dialog allows a user to change the language for the UI
class SetLanguageDialog : public QDialog
{
	Q_OBJECT

public:
	SetLanguageDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );

	virtual ~SetLanguageDialog()
	    {// no need to delete child widgets, Qt does it all for us
		};

	/** pass the Internationalisation instance
	 *  loads the dialog with all languages and allows for setting it later
	 */
	void setI18nPtr(Internationalisation* aI18nPtr);
	
private:
    QLabel* textLabel1;
    Q3ButtonGroup* theRadioButtonGroup;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    Q3VBoxLayout* theDialogLayout;
    Q3VBoxLayout* theRadioButtonGroupLayout;
    Q3HBoxLayout* thePushButtonLayout;
    QSpacerItem* Horizontal_Spacing2;
    Internationalisation* theInternationalisationPtr;

private slots:
	/// Sets the strings of the subwidgets using the current language.
	void languageChange();
	/// when OK button is clicked, make sure the new setting is used and saved
	void buttonOk_clicked();
	/// when a radio button is clicked, enable OK button
	void radioButton_clicked();
};

#endif /*SETLANGUAGEDIALOG_H_*/
