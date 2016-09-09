# include a file with all the common stuff
include(../GenericTestPro.inc)

LIBS += -lusb

HEADERS	+= \
	../../umtsmon_global.h \
	../../src/controller/AutoDetect.h \
	../../src/base/FileStuff.h \
	../../src/model/Device.h \
	../../src/model/MonthlyTraffic.h \
	../../src/model/Profile.h \
	../../src/model/Query.h \
	../../src/base/SerialPort.h \
	../../src/base/TheSettingsSingleton.h \
	../../src/view/Popup.h

SOURCES	+= AutoDetectTester_main.cpp \
	../../src/controller/AutoDetect.cpp \
	../../src/model/ConnectionInfo.cpp \
	../../src/model/Device.cpp \
	../../src/model/MonthlyTraffic.cpp \
	../../src/model/Profile.cpp \
	../../src/model/Query.cpp \
	../../src/base/FileStuff.cpp \
	../../src/base/Runner.cpp \
	../../src/base/SerialPort.cpp \
	../../src/base/TheSettingsSingleton.cpp
	
