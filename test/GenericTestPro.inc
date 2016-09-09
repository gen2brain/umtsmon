# This is an include file that defines generic parameters for all test programs.
# it is included in each *.pro file at the beginning.

TEMPLATE	= app
LANGUAGE	= C++

### uncomment these two lines for compiling with code coverage instrumentation
#QMAKE_CXXFLAGS_DEBUG += -fprofile-arcs -ftest-coverage
#QMAKE_LFLAGS_DEBUG += -fprofile-arcs -ftest-coverage


CONFIG	+= qt warn_on debug

INCLUDEPATH +=../.. ../../src/base ../../src/model ../../src/view ../../src/controller

unix {
  UI_DIR = ../../.ui
  MOC_DIR = ../../.moc
  OBJECTS_DIR = .obj
}
