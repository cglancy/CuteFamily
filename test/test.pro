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
    LIBS += -L./debug -lCuteFamily0
    PRE_TARGETDEPS += ./debug/CuteFamily0.dll
}
else {
    LIBS += -L./release -lCuteFamily0
    PRE_TARGETDEPS += ./release/CuteFamily0.dll
}
