######################################################################
# Automatically generated by qmake (2.01a) fr. nov. 16 13:18:20 2007
######################################################################

TEMPLATE = app
LANGUAGE = C++

# Debug -  das bin_file wird ~12 x größer !
#CONFIG += qt debug                        
CONFIG += qt                               
QT += network                             

QT += sql

unix {
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}


DEPENDPATH += .
#QT = core network

HEADERS += downloadMgr.h

SOURCES += downloadMgr.cpp \
           main.cpp
        
TARGET = ../bin/downloadmgrdb

TRANSLATIONS += qtlogDownload_en.ts 
