# include a file with all the common stuff
include(../GenericTestPro.inc)

!exists(helperlistgui.ui.h) {
        system(ln -s ../../src/view/helperlistgui.ui.h .) {}
        system(ln -s ../../src/view/helperlistgui.ui .) {}
}

LIBS	+= -lusb

HEADERS	+= \
	../../umtsmon_global.h \
	../../src/controller/AutoDetect.h \
	../../src/model/ConnectionInfo.h \	
	../../src/model/Device.h \
	../../src/base/FileStuff.h \
	../../src/model/HelperList.h \
	../../src/model/MonthlyTraffic.h \
	../../src/model/OperatorList.h \
	../../src/view/Popup.h \
	../../src/model/Profile.h \
	../../src/model/Query.h \
	../../src/base/Runner.h \
	../../src/base/SerialPort.h \
	../../src/controller/TheDeviceManagerSingleton.h \
	../../src/base/TheSettingsSingleton.h

SOURCES	+= \
	HelperTester_main.cpp \
	../../src/controller/AutoDetect.cpp \
	../../src/model/ConnectionInfo.cpp \	
	../../src/model/Device.cpp \
	../../src/base/FileStuff.cpp \
	../../src/model/HelperList.cpp \
	../../src/model/MonthlyTraffic.cpp \
	../../src/model/OperatorList.cpp \
	../../src/model/Query.cpp \
	../../src/model/Profile.cpp \
	../../src/base/Runner.cpp \
	../../src/base/SerialPort.cpp \
	../../src/controller/TheDeviceManagerSingleton.cpp \
	../../src/base/TheSettingsSingleton.cpp

FORMS   = \
        ./helperlistgui.ui

IMAGES = ../../images/Wizard/NetworkOperatorWizard.png \
	../../images/ok.png \
	../../images/16/process-stop-16x16.png
