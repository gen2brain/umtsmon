# include a file with all the common stuff
include(../GenericTestPro.inc)

# contrary to e.g. SettingsTester, I don't need to
# copy the Runner.* files here because they will work
# regardless of the location they're compiled in.

HEADERS	+= ../../src/base/Runner.h \
	../../src/base/FileStuff.h \
	umtsmon_global.h

SOURCES	+= ../../src/base/Runner.cpp \
	../../src/base/FileStuff.cpp \
	RunnerTester_main.cpp
