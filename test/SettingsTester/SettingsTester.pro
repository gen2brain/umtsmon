# include a file with all the common stuff
include(../GenericTestPro.inc)

!exists(TheSettingsSingleton.h) {
	system(ln -s ../../src/base/TheSettingsSingleton.h .) {}
	system(ln -s ../../src/base/TheSettingsSingleton.cpp .) {}
}


HEADERS	+= TheSettingsSingleton.h \
	./umtsmon_global.h

SOURCES	+= TheSettingsSingleton.cpp \
	SettingsTester_main.cpp
#The following line was inserted by qt3to4
QT +=  qt3support 
