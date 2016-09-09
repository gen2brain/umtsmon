# include a file with all the common stuff
include(../GenericTestPro.inc)

HEADERS	+= \
	../../src/model/Query.h \
	../../umtsmon_global.h \
	../../src/base/SerialPort.h

SOURCES	+= QueryTester_main.cpp \
	../../src/model/Query.cpp \
	../../src/base/SerialPort.cpp
#The following line was inserted by qt3to4
QT +=  qt3support 
