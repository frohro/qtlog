/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef DXLIST_H
#define DXLIST_H
//#include <QHash>
class dxlist
{
  public:
    dxlist();
    ~dxlist();
    QSqlQuery query;
    QString qy, s;
    QString logname;          
    void setLogname(QString);
    void setDxlistClear();
    void insertDxItem(int,QString, QString, QString, QString);

    QString ErrorStr;
    int x, errCnt;
    int laCnt;
    int getErrCnt();
    QString getErorrList();
    void clrErrStr();
    void clrErrCnt();
};
#endif 
