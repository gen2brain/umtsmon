# include a file with all the common stuff
include(../GenericTestPro.inc)

!exists(infodialog.ui.h) {
        system(ln -s ../../src/view/infodialog.ui.h .) {}
        system(ln -s ../../src/view/infodialog.ui .) {}
}


HEADERS	+= ../../src/model/Query.h \
	../../umtsmon_global.h \
	../../src/base/SerialPort.h \
	../../src/view/ProgressDialog.h \
	../../src/base/FileStuff.h \
	infodialog.h

SOURCES	+= InfoDialogTester_main.cpp \
	../../src/model/Query.cpp \
	../../src/base/FileStuff.cpp \
	../../src/base/SerialPort.cpp \
	../../src/model/ConnectionInfo.cpp \
	../../src/view/ProgressDialog.cpp

#FORMS   = \
#        ./infodialog.ui
