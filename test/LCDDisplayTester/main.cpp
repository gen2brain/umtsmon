#include "LCDDisplayTest.h"
#include "umtsmon_global.h"

#include <qapplication.h>
#include <qtranslator.h>
#include <qtextcodec.h>

#include <stdio.h>


int main( int argc, char **argv )
{
	
	QApplication a( argc, argv );

	puts( "LCDDisplayTest" );

      // translation file for Qt
        QTranslator myQTTranslator(0);
        myQTTranslator.load( QString( "qt_" ) + QTextCodec::locale(), "." );
        a.installTranslator( &myQTTranslator );

        // translation file for application strings
        QTranslator myAppTranslator( 0 );
        myAppTranslator.load( QString( APPNAME "_" ) + QTextCodec::locale(), "i18n" );
        a.installTranslator( &myAppTranslator );

	LCDDisplayTest w;
	w.show();
	return w.exec();
}
