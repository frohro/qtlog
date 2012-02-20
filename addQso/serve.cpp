/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#include "addQso.h"
#include "serve.h"
Serve::Serve()
{
  i = 0;
  n = 0;
}
Serve::~Serve()
{
}
/*


void translate::installHashTable()
{
 QString d, u;
 int b;
    QSqlQuery query; 
    qy = "SELECT dbfield,refnam,breite FROM refnamen WHERE type=2"; 
    query.exec(qy);
    i = query.size();
    while(query.next()) {
       i = 0;
       d = query.value(i++).toString();  
       u = query.value(i++).toString();  
       b = query.value(i++).toInt();     
       toUsr.insert(d,u);                
       toDb.insert(u,d);                 
       toBr.insert(u,b);                 
    }
}
*/
/*


QString translate::getUsrField(QString dbfield)
{
     QHash<QString, QString>::const_iterator in = toUsr.find(dbfield);
     return in.value();
}


QString translate::getDbField(QString usrfield)
{
    QHash<QString, QString>::const_iterator i = toDb.find(usrfield);
    return i.value();
}

int translate::getFieldBr(QString usrfield)
{
    QHash<QString, int>::const_iterator i = toBr.find(usrfield);
    return i.value();
}
*/


void Serve::installHashTableBand()
{
 QString b, mb;
    QSqlQuery query; 
    qy = "SELECT band,mband FROM wband WHERE work !=0"; 
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


QString Serve::getMyband(QString sysband)
{
    QHash<QString, QString>::const_iterator i = toMband.find(sysband);
    if(i == toMband.end())
       return "?";
    return i.value();                        
}


QString Serve::getSysband(QString myband)
{
    QHash<QString, QString>::const_iterator i = toSband.find(myband);
   if(i == toMband.end())
      return "?";
   return i.value();
}
