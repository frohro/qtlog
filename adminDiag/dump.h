/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef DUMP_H
#define DUMP_H
class dump {
 public:
  dump();
  ~dump();
    QString qy;
    QSettings settings;
    QList<QString> list;
    void setTabName(QString );
    void setDumpPath(QString);
    int getState();
  
    void dumpLogdb(QString);                 
    void dumpTable(int,QString);             
    void dumpImage(QString);       	     
  
  
    int restoreAll();
    int restoreTable(QString);
    void restoreFile(QString, QString, int );
 protected:
    int state;
    QString tname;
    QString dpath;
};
#endif
