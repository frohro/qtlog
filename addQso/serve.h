/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef SERVE_H
#define SERVE_H
#include <QHash>
class Serve
{
  public:
    Serve();
    ~Serve();
    int i;
    int n;
    QString qy;
    //QHash <QString, QString> toUsr;    
    //QHash <QString, QString> toDb;     
    //QHash <QString, int> toBr;         
    QHash <QString, QString> toMband;  
    QHash <QString, QString> toSband;  
   
   
   
   
    void installHashTableBand();
    QString getMyband(QString);
    QString getSysband(QString);
};
#endif 
