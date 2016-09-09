# include a file with all the common stuff
include(../GenericTestPro.inc)

HEADERS	+= \
	../../src/base/TheSettingsSingleton.h \
	../../src/model/Profile.h \
	../../src/model/MonthlyTraffic.h \
	umtsmon_global.h 

SOURCES	+= \
	../../src/base/TheSettingsSingleton.cpp \
	../../src/model/Profile.cpp \
	MonthlyTrafficTester_main.cpp 
# note that MonthlyTraffic.cpp is actually missing because it's #included by MonthlyTrafficTester_main.cpp	
#The following line was inserted by qt3to4
QT +=  qt3support 
