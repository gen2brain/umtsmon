TEMPLATE	= app
LANGUAGE	= C++

# install prefix
# can be overridden with PREFIX environment variable
PREFIX=/usr/local

CONFIG	+= qt warn_on release

LIBS	+= -lusb

DEFINES	+= QT_NO_ASCII_CAST

INCLUDEPATH	+= src/model src/view src/base src/controller

#CXXFLAGS += -ggdb3 -O0

HEADERS	+= umtsmon_global.h \
	src/base/FileStuff.h \
	src/base/Internationalisation.h \
	src/base/ObserverManager.h \
	src/base/Runner.h \
	src/base/SerialPort.h \
	src/base/TheSettingsSingleton.h \
	src/controller/AutoDetect.h \
	src/controller/NetworkChanger.h \
	src/controller/SIMHandler.h \
	src/controller/TheDeviceManagerSingleton.h \
	src/controller/ValidatePIN.h \
	src/controller/ValidatePhoneNumber.h \
	src/model/ConnectionInfo.h \
	src/model/Device.h \
	src/model/DeviceCapabilities.h \
	src/model/HelperList.h \
	src/model/MonthlyTraffic.h \
	src/model/OperatorList.h \
	src/model/PPPConnection.h \
	src/model/Profile.h \
	src/model/Query.h \
	src/view/LCDDisplay.h \
	src/view/Popup.h \
	src/view/ProgressDialog.h \
	src/view/radiopreferences_defines.h \
	src/view/SetLanguageDialog.h \
	src/view/UIState.h

SOURCES	+= main.cpp \
	src/base/FileStuff.cpp \
	src/base/Internationalisation.cpp \
	src/base/Runner.cpp \
	src/base/SerialPort.cpp \
	src/base/TheSettingsSingleton.cpp \
	src/controller/AutoDetect.cpp \
	src/controller/NetworkChanger.cpp \
	src/controller/SIMHandler.cpp \
	src/controller/TheDeviceManagerSingleton.cpp \
	src/model/ConnectionInfo.cpp \
	src/model/Device.cpp \
	src/model/HelperList.cpp \
	src/model/MonthlyTraffic.cpp \
	src/model/OperatorList.cpp \
	src/model/PPPConnection.cpp \
	src/model/Profile.cpp \
	src/model/Query.cpp \
	src/view/Popup.cpp \
	src/view/ProgressDialog.cpp \
	src/view/LCDDisplay.cpp \
	src/view/SetLanguageDialog.cpp \
	src/view/UIState.cpp

#The following line was changed from FORMS to FORMS3 by qt3to4
FORMS3	= src/view/newprofiledialog.ui \
	src/view/enterpindialog.ui \
	src/view/enterpukdialog.ui \
	src/view/aboutdialog.ui \
	src/view/infodialog.ui \
	src/view/smsdialog.ui \
	mainwindow.ui \
	src/view/profiledialog.ui \
	src/view/helperlistgui.ui \
	src/view/PPPErrorLogsDialog.ui \
	src/view/radiopreferences.ui \
	src/view/manageprofiles.ui \
	src/view/networkoperatorwizard.ui \
	src/view/popupwithmemory.ui \
	src/view/yesnowithmemory.ui

IMAGES	= images/22/address-book-new-22x22.png \
	images/22/document-new-22x22.png \
	images/22/document-save-22x22.png \
	images/22/network-transmit-receive-22x22.png \
	images/22/network-offline-22x22.png \
	images/22/network-wireless-22x22.png \
	images/22/mail-message-new-22x22.png \
	images/22/applications-system-22x22.png \
	images/22/view-refresh-22x22.png \
	images/22/umtsmon-22x22.png \
	images/16/address-book-new-16x16.png \
	images/16/applications-system-16x16.png \
	images/16/dialog-information-16x16.png \
	images/16/document-new-16x16.png \
	images/16/document-save-16x16.png \
	images/16/help-browser-16x16.png \
	images/16/mail-message-new-16x16.png \
	images/16/network-wireless-16x16.png \
	images/16/process-stop-16x16.png \
	images/16/system-log-out-16x16.png \
	images/16/system-search-16x16.png \
	images/16/umtsmon-16x16.png \
	images/16/user-trash-full-16x16.png \
	images/16/view-refresh-16x16.png \
	images/ok.png \
	images/LCDDisplay/LCDBackground.png \
	images/LCDDisplay/LCDSignal.png \
	images/Wizard/NetworkOperatorWizard.png

DISTFILES = \
	README \
	TODO \
	AUTHORS \
	COPYING

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

TRANSLATIONS = \
   i18n/umtsmon_de.ts \
   i18n/umtsmon_es.ts \
   i18n/umtsmon_id.ts \
   i18n/umtsmon_it.ts \
   i18n/umtsmon_nb_NO.ts \
   i18n/umtsmon_nl.ts \
   i18n/umtsmon_pl.ts \
   i18n/umtsmon_pt_BR.ts \
   i18n/umtsmon_pt_PT.ts \
   i18n/umtsmon_ru.ts \
   i18n/umtsmon_sv.ts

# build translations
translations_qm = $$replace($${TRANSLATIONS}, .ts, .qm)

lupdate.target=$${TRANSLATIONS}
lupdate.commands=lupdate -verbose umtsmon.pro
lupdate.depends=$${HEADERS} $${SOURCES} $${FORMS} umtsmon.pro

lrelease.target=$${translations_qm}
lrelease.commands=lrelease -verbose umtsmon.pro
lrelease.depends=$${TRANSLATIONS}

aboutdialog.target=src/view/aboutdialog.ui
aboutdialog.commands=sed -e '/@AUTHORS@/rAUTHORS' -e '/@COPYING@/rCOPYING' src/view/aboutdialog.base.ui > src/view/aboutdialog.ui && sed -e '/@COPYING@/d' -e '/@AUTHORS@/d' -i src/view/aboutdialog.ui
aboutdialog.depends=AUTHORS COPYING src/view/aboutdialog.base.ui

QMAKE_EXTRA_TARGETS += lupdate lrelease 
QMAKE_EXTRA_TARGETS += aboutdialog
POST_TARGETDEPS = $${lupdate.target} $${lrelease.target}

# make install rules
env_prefix=$$(PREFIX)

!isEmpty(env_prefix) {
	PREFIX=$${env_prefix}
}

target.path=$${PREFIX}/bin

trans.files=$${translations_qm}
trans.path=$${PREFIX}/share/umtsmon/translations

desktop.files=umtsmon.desktop
desktop.path=$${PREFIX}/share/applications

icon128.files=images/128/umtsmon-128x128.png
icon128.path=$${PREFIX}/share/icons/hicolor/128x128/apps

INSTALLS += target trans desktop icon128

QMAKE_CLEAN += \
	src/view/aboutdialog.ui \
	$${translations_qm} \
	umtsmon
#The following line was inserted by qt3to4
QT +=  qt3support 
#The following line was inserted by qt3to4
CONFIG += uic3

