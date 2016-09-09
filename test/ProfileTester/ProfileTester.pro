# include a file with all the common stuff
include(../GenericTestPro.inc)

HEADERS	+= \
	../../src/base/TheSettingsSingleton.h \
	../../src/model/Profile.h \
	umtsmon_global.h 

SOURCES	+= \
	../../src/base/TheSettingsSingleton.cpp \
	ProfileTester_main.cpp 
# note that Profile.cpp is actually missing because it's #included by ProfileTester_main.cpp	
#The following line was inserted by qt3to4
QT +=  qt3support 
