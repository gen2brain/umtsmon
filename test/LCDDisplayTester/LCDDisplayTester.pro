# include a file with all the common stuff
include(../GenericTestPro.inc)

HEADERS	+= LCDDisplay.h \
	../../umtsmon_global.h

SOURCES	+= main.cpp \
	LCDDisplay.cpp

FORMS	= LCDDisplayTest.ui

IMAGES	= LCDBackground.png \
	LCDSignal.png

!exists(LCDBackground.png) {
	system(ln -s ../../images/LCDDisplay/LCDBackground.png .) {}
	system(ln -s ../../images/LCDDisplay/LCDSignal.png .) {}
	system(ln -s ../../src/view/LCDDisplay.h .) {}
	system(ln -s ../../src/view/LCDDisplay.cpp .) {}
	system(ln -s ../../i18n .) {}
}
