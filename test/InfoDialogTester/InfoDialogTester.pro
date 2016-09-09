# include a file with all the common stuff
include(../GenericTestPro.inc)

!exists(infodialog.ui.h) {
        system(ln -s ../../src/view/infodialog.ui.h .) {}
        system(ln -s ../../src/view/infodialog.ui .) {}
}

LIBS	+= -lusb

HEADERS	+= ../../src/model/Query.h \
	../../umtsmon_global.h \
	../../src/base/SerialPort.h \
	../../src/base/FileStuff.h \
	../../src/base/TheSettingsSingleton.h \
	../../src/base/Runner.h \
	../../src/model/HelperList.h \
	../../src/model/MonthlyTraffic.h \
    ../../src/model/Profile.h \
	../../src/model/OperatorList.h \
	../../src/model/Device.h \
	../../src/view/Popup.h \
	../../src/view/ProgressDialog.h \
	../../src/controller/AutoDetect.h \
	infodialog.h

SOURCES	+= InfoDialogTester_main.cpp \
	../../src/model/Query.cpp \
	../../src/base/FileStuff.cpp \
	../../src/base/SerialPort.cpp \
	../../src/base/TheSettingsSingleton.cpp \
	../../src/base/Runner.cpp \
	../../src/model/HelperList.cpp \
	../../src/model/MonthlyTraffic.cpp \
	../../src/model/ConnectionInfo.cpp \
    ../../src/model/Profile.cpp \
	../../src/model/OperatorList.cpp \
	../../src/model/Device.cpp \
	../../src/controller/AutoDetect.cpp \
	../../src/controller/TheDeviceManagerSingleton.cpp \
	../../src/view/Popup.cpp \
	../../src/view/ProgressDialog.cpp

#The following line was changed from FORMS to FORMS3 by qt3to4
FORMS3   = \
        ../../src/view/popupwithmemory.ui \
	    ../../src/view/yesnowithmemory.ui \
        ./helperlistgui.ui
#The following line was inserted by qt3to4
QT +=  qt3support 
#The following line was inserted by qt3to4
CONFIG += uic3
