/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

#include "umtsmon_global.h"

#include "FileStuff.h"
#include "Runner.h"
#include "HelperList.h"

#include <qfile.h>
#include <stdlib.h>

void helperlistgui::init(void)
{
	theHelperListPtr = NULL;
}

void helperlistgui::pushButton_Fixit_clicked()
{
	if (theHelperListPtr->fixIt())
    	accept();
    else
		theHelperListPtr->addListToWidget();
}

void helperlistgui::pushButton_Quit_clicked()
{
	DEBUG3("Quit button in helperlistgui clicked - app will exit\n");
	exit(0);
}


void helperlistgui::pushButton_Ignore_clicked()
{
	// we could have made this connection directly, but this is more clear
	reject();
}

void helperlistgui::setHelperListPtr(HelperList* aPtr)
{
	theHelperListPtr = aPtr;
}


void helperlistgui::clearList(void)
{
	helperListView->clear();
}

void helperlistgui::add(HelperData* aDataPtr)
{
	aDataPtr->add(helperListView);
}
