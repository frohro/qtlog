######################################################################
# Automatically generated by qmake (2.01a) Di Mai 1 10:32:18 2007
# Project laDiag - DL1HBD 10.05.2007
######################################################################

TEMPLATE = app
LANGUAGE = C++

# Debug -  das bin_file wird ~12 x größer !
# CONFIG += qt debug
CONFIG += qt
QT += sql

unix {
  LIBS = -L/usr/lib
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

win32{
  DEFINES-=UNICODE
  target=qrzdb
}

# -- Input --
FORMS += qrz.ui

HEADERS += qrz.h

SOURCES += main.cpp\
           ../qtlogDiag/dirmngr.cpp\
           qrz.cpp

RESOURCES = ../qtlogRessourcen.qrc

unix::TARGET = ../bin/qrzdb

DEPENDPATH += .