/* umtsmon - a program to cotrol/monitor your UMTS card
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

#include "umtsmon_global.h"
#include "SetLanguageDialog.h"
#include "Internationalisation.h"
#include "Popup.h"

#include <qpushbutton.h>
#include <qlabel.h>
#include <q3buttongroup.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include <qstringlist.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

SetLanguageDialog::SetLanguageDialog( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    setSizeGripEnabled(true);
    theDialogLayout = new Q3VBoxLayout( this, 11, 6, "DialogLayout"); 

    textLabel1 = new QLabel( this, "textLabel1" );
    theDialogLayout->addWidget( textLabel1 );

    theRadioButtonGroup = new Q3ButtonGroup( this, "RadioButtonGroup" );
    theRadioButtonGroup->setColumnLayout(0, Qt::Vertical );
    theRadioButtonGroup->layout()->setSpacing( 6 );
    theRadioButtonGroup->layout()->setMargin( 11 );
    theRadioButtonGroupLayout = new Q3VBoxLayout( theRadioButtonGroup->layout() );
    theRadioButtonGroupLayout->setAlignment( Qt::AlignTop );
    theDialogLayout->addWidget( theRadioButtonGroup );

    thePushButtonLayout = new Q3HBoxLayout( 0, 0, 6, "PushButtonLayout"); 
    Horizontal_Spacing2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    thePushButtonLayout->addItem( Horizontal_Spacing2 );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setAutoDefault(true);
    buttonOk->setDefault(true);
	buttonOk->setEnabled(false);
    thePushButtonLayout->addWidget( buttonOk );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setAutoDefault(true);
    thePushButtonLayout->addWidget( buttonCancel );
    theDialogLayout->addLayout( thePushButtonLayout );
    languageChange();
    resize( QSize(100, 209).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( buttonOk_clicked() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

void SetLanguageDialog::languageChange()
{
	// the strings in the radio buttons are missing here -
	// don't translate them
    setCaption( tr( "Set Language" ) );
    textLabel1->setText( tr( "Please select your language. \n"
    		"This setting will be active after a restart." ) );
    theRadioButtonGroup->setTitle( tr( "Available languages" ) );
    buttonOk->setText( tr( "&OK" ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    buttonCancel->setText( tr( "&Cancel" ) );
    buttonCancel->setAccel( QKeySequence( QString::null ) );
}

void SetLanguageDialog::buttonOk_clicked()
{
	// figure out which radio button is selected
	// and find it's name
	QString myLocale = theRadioButtonGroup->selected()->text();
	DEBUG3("We will use locale '%s'\n", myLocale.ascii());
	
	if (theInternationalisationPtr->tryLocale(myLocale))
	{
		// yay!
		DEBUG3("Setting new locale succeeded\n");
		accept();
	}
	else
	{
		DEBUG3("Setting new locale failed\n");
		Popup::Warning(tr("Selecting the language failed ?!?"), this);
		// do not run accept()...
	}
}

void SetLanguageDialog::radioButton_clicked()
{
	DEBUG3("user clicked a radio button\n");
	buttonOk->setEnabled(true);
	buttonOk->setFocus();
}

void SetLanguageDialog::setI18nPtr(Internationalisation* aI18nPtr)
{
	theInternationalisationPtr = aI18nPtr;
	
	// make a copy of the list and sort it
	QStringList myLangList = aI18nPtr->getAllSupportedLocales();
	qSort(myLangList);
	
	QString myCurrentLang = aI18nPtr->getLocale();
	DEBUG5("My current language = '%s'\n", myCurrentLang.ascii());
	
	QStringList::Iterator it;
	for (it = myLangList.begin(); it != myLangList.end(); ++it )
	{
		// I'm not sure what this kludge is good for, but const char* is the
		// only acceptable parameter for a button name. 
		// We could of course have named all buttons "button", but...
		QString myButtonName = "button_" + (*it);
		QRadioButton* myRadioButtonPtr = new QRadioButton( theRadioButtonGroup, myButtonName.ascii() );
		// don't attempt to translate the language names - no tr() here!
		myRadioButtonPtr->setText(*it);
		
		if ((*it) == myCurrentLang)
			myRadioButtonPtr->setChecked(true);
		// hook up the button to the signal handler to enable the OK button
	    connect( myRadioButtonPtr, SIGNAL( clicked() ), this, SLOT( radioButton_clicked() ) );
		theRadioButtonGroupLayout->addWidget( myRadioButtonPtr );
	}
}
