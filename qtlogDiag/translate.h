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
    QHash <QString, QString> toUsr;    
    QHash <QString, QString> toDb;     
    QHash <QString, int> toBr;         
    QHash <QString, int> toBr2;        
    QHash <QString, QString> toMband;  
    QHash <QString, QString> toSband;  
    QHash <QString, int> toLog; 
    
    void installHashTable(int);
    void clearHashTable();
    int getLogTable(QString);
    
    QString getUsrField(QString);
    void setUsrField(QString,QString);
    QString getDbField(QString);
    
    int getFieldBr(QString);
    int getFieldBr2(QString);
    
    void installHashTableBand();
    QString getMyband(QString);
    QString getSysband(QString);
};
#endif 
