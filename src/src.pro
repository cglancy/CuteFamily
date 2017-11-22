QT       += core network
QT       -= gui

TARGET = CuteFamily
CONFIG   += dll
TEMPLATE = lib
VERSION	= 0.1.1

SOURCES += familysearch.cpp \
	familysearchreply.cpp

HEADERS += \
    familysearch.h \
	familysearchreply.h \
    cutefamily.h

DEFINES += CUTEFAMILY_EXPORTS

