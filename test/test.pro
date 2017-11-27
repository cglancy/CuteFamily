QT += core testlib network
QT -= gui

TARGET = cgFamilyTest
CONFIG += testcase 

TEMPLATE = app

SOURCES += familytest.cpp

HEADERS += familytest.h \
	familysearchsecret.h

INCLUDEPATH += ../src

CONFIG(debug, debug|release) {
    LIBS += -L../src/debug -lcgFamily0
    PRE_TARGETDEPS += ../src/debug/cgFamily0.dll
}
else {
    LIBS += -L../src/release -lcgFamily0
    PRE_TARGETDEPS += ../src/release/cgFamily0.dll
}
