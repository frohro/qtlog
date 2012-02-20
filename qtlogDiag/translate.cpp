/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */


#include <QtGui>
#include <QtSql>
#include "translate.h"
translate::translate()
{
  i = 0;
  n = 0;
}
translate::~translate()
{
}
//- Refnamen_Tabelle aus DB lesen - Hash_Tabellen aufbauen
//---------------------------------------------------------
void translate::installHashTable()
{
 QString d, u;
 int b, b2, idx;
 QString s;
    QSqlQuery query; 
    idx = 1;
    qy = "SELECT dbfield,refnam,breite,br2,type FROM refnamen WHERE type!=0 ORDER BY idn"; 
    query.exec(qy);
    i = query.size();
    while(query.next()) {
       i = 0;
       d = query.value(i++).toString();  
       u = query.value(i++).toString();  
       b = query.value(i++).toInt();     
       b2 = query.value(i++).toInt();    
      
       toUsr.insert(d,u);                
       toDb.insert(u,d);                 
       toBr.insert(u,b);                 
       toBr2.insert(u,b2);               
       toLog.insert(d,query.value(i++).toInt()); 
    }
}

void translate::clearHashTable()
{
       toUsr.clear();                
       toDb.clear();                 
       toBr.clear();                 
       toBr2.clear();                
}

void translate::setUsrField(QString dbfield, QString user)
{
    toUsr.insert(dbfield,user); 
}


QString translate::getUsrField(QString dbfield)
{
     QHash<QString, QString>::const_iterator in = toUsr.find(dbfield);
     return in.value();
}


QString translate::getDbField(QString usrfield)
{
    QHash<QString, QString>::const_iterator i = toDb.find(usrfield);
    if(i == toDb.end())
       return "?";                          
    return i.value();                       
}

int translate::getFieldBr(QString usrfield)
{
    QHash<QString, int>::const_iterator i = toBr.find(usrfield);
    return i.value();
}

int translate::getFieldBr2(QString usrfield)
{
    QHash<QString, int>::const_iterator i = toBr2.find(usrfield);
    return i.value();
}


int translate::getLogTable(QString dbfield)
{
    QHash<QString, int>::const_iterator i = toLog.find(dbfield);
    return i.value();
}


void translate::installHashTableBand()
{
 QString b, mb;
    QSqlQuery query; 
    qy = "SELECT band,mband FROM wband WHERE work != 0"; 
    query.exec(qy);
    i = query.size();
    while(query.next()) {
        i = 0;
        b = query.value(i++).toString();     
       mb = query.value(i++).toString();     
       toMband.insert(b,mb);                 
       toSband.insert(mb,b);                 
    }
}


QString translate::getMyband(QString sysband)
{
    QHash<QString, QString>::const_iterator i = toMband.find(sysband);
    if(i == toMband.end())
       return "?";
    return i.value();                        
}


QString translate::getSysband(QString myband)
{
   QHash<QString, QString>::const_iterator i = toSband.find(myband);
   if(i == toMband.end())
      return "?";
   return i.value();
}
