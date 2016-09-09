# include a file with all the common stuff
include(../GenericTestPro.inc)

HEADERS	+= \
	../../src/model/Query.h \
	../../src/model/Profile.h \
	../../src/model/OperatorList.h \
	../../src/model/MonthlyTraffic.h \
	../../umtsmon_global.h \
	../../src/base/TheSettingsSingleton.h \
	../../src/base/SerialPort.h \
	../../src/controller/NetworkChanger.h

SOURCES	+= NetworkChangerTester_main.cpp \
	../../src/model/Query.cpp \
	../../src/model/Profile.cpp \
	../../src/model/OperatorList.cpp \
	../../src/model/MonthlyTraffic.cpp \
	../../src/controller/NetworkChanger.cpp \
	../../src/base/TheSettingsSingleton.cpp \
	../../src/base/SerialPort.cpp \
	../../src/model/ConnectionInfo.cpp
