QT       += core network
QT       -= gui

TARGET = cgFamily
CONFIG   += dll
TEMPLATE = lib
VERSION	= 0.1.1

SOURCES += familysearch.cpp \
	familysearchreply.cpp

HEADERS += \
    familysearch.h \
	familysearch_p.h \
	familysearchreply.h \
	familysearchreply_p.h \
    cgfamily.h

DEFINES += CGFAMILY_EXPORTS

