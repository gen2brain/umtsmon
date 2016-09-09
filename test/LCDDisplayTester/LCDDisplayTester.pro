# include a file with all the common stuff
include(../GenericTestPro.inc)

HEADERS	+= LCDDisplay.h \
	../../umtsmon_global.h

SOURCES	+= main.cpp \
	LCDDisplay.cpp

#The following line was changed from FORMS to FORMS3 by qt3to4
FORMS3	= LCDDisplayTest.ui

IMAGES	= LCDBackground.png \
	LCDSignal.png

!exists(LCDBackground.png) {
	system(ln -s ../../images/LCDDisplay/LCDBackground.png .) {}
	system(ln -s ../../images/LCDDisplay/LCDSignal.png .) {}
	system(ln -s ../../src/view/LCDDisplay.h .) {}
	system(ln -s ../../src/view/LCDDisplay.cpp .) {}
	system(ln -s ../../i18n .) {}
}
#The following line was inserted by qt3to4
QT +=  qt3support 
#The following line was inserted by qt3to4
CONFIG += uic3

