# include a file with all the common stuff
include(../GenericTestPro.inc)

!exists(networkoperatorwizard.ui.h) {
        system(ln -s ../../src/view/networkoperatorwizard.ui.h .) {}
        system(ln -s ../../src/view/networkoperatorwizard.ui .) {}
}

HEADERS	+= \
	../../src/model/ConnectionInfo.h \
	../../umtsmon_global.h \
	../../src/model/MonthlyTraffic.h \
	../../src/controller/NetworkChanger.h \ 
	../../src/model/OperatorList.h \
	../../src/view/Popup.h \
	../../src/model/Profile.h \
	../../src/model/Query.h \
	../../src/base/SerialPort.h \
	../../src/base/TheSettingsSingleton.h

SOURCES	+= \
	NetworkOperatorWizardTester_main.cpp \
	../../src/model/ConnectionInfo.cpp \
	../../src/model/MonthlyTraffic.cpp \
	../../src/controller/NetworkChanger.cpp \
	../../src/model/OperatorList.cpp \
	../../src/model/Profile.cpp \
	../../src/model/Query.cpp \
	../../src/base/SerialPort.cpp \
	../../src/base/TheSettingsSingleton.cpp

FORMS   = \
        ./networkoperatorwizard.ui

IMAGES = ../../images/Wizard/NetworkOperatorWizard.png \
	../../images/ok.png \
	../../images/16/process-stop-16x16.png
