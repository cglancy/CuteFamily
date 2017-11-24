QT       += core testlib network
QT       -= gui

TARGET = TestCuteFamily
CONFIG   += testcase 

TEMPLATE = app

SOURCES += \
    testcutefamily.cpp

HEADERS += familysearchsecret.h \
    testcutefamily.h

INCLUDEPATH += ../src

CONFIG(debug, debug|release) {
    LIBS += -L../src/debug -lCuteFamily0
    PRE_TARGETDEPS += ../src/debug/CuteFamily0.dll
}
else {
    LIBS += -L../src/release -lCuteFamily0
    PRE_TARGETDEPS += ../src/release/CuteFamily0.dll
}
