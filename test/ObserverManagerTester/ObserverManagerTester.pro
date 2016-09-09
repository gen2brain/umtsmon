# include a file with all the common stuff
include(../GenericTestPro.inc)

HEADERS	+= ../../src/base/ObserverManager.h \
	../../umtsmon_global.h

SOURCES	+= ObserverManagerTester_main.cpp

#The following line was inserted by qt3to4
QT +=  qt3support 
