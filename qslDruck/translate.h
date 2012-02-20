/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef TRANSL_H
#define TRANSL_H
#include <QHash>
class translate
{
  public:
    translate();
    ~translate();
    int i;
    int n;
    QString qy;
/*
    QHash <QString, QString> toUsr;    
    QHash <QString, QString> toDb;     
    QHash <QString, int> toBr;         
    void installHashTable();
    QString getUsrField(QString);
    QString getDbField(QString);
    int getFieldBr(QString);
*/
    QHash <QString, QString> toMband;  
    QHash <QString, QString> toSband;  
    void installHashTableBand();
    QString getMyband(QString);
    QString getSysband(QString);
};
#endif 
