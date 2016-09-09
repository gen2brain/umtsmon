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
	../../src/model/OperatorList.h \
	../../src/view/Popup.h \
	../../src/base/SerialPort.h \
	../../src/base/TheSettingsSingleton.h

SOURCES	+= AutoDetectTester_main.cpp \
	../../src/controller/AutoDetect.cpp \
	../../src/model/ConnectionInfo.cpp \
	../../src/model/Device.cpp \
	../../src/model/MonthlyTraffic.cpp \
	../../src/model/Profile.cpp \
	../../src/model/Query.cpp \
	../../src/model/OperatorList.cpp \
	../../src/view/Popup.cpp \
	../../src/base/FileStuff.cpp \
	../../src/base/Runner.cpp \
	../../src/base/SerialPort.cpp \
	../../src/base/TheSettingsSingleton.cpp

#The following line was changed from FORMS to FORMS3 by qt3to4
FORMS3	= ../../src/view/popupwithmemory.ui \
	../../src/view/yesnowithmemory.ui
	
#The following line was inserted by qt3to4
QT +=  qt3support 
#The following line was inserted by qt3to4
CONFIG += uic3
