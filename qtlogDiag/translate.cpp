/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

// translate.cpp - 20.06.2007
// NamenConverter: Uebersetze db.feldnamen nach User_Feldnamen und zurueck

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
 int b, b2;
 QString s;
 
    QSqlQuery query; 
    qy = "SELECT dbfield,refnam,breite,br2,type FROM refnamen WHERE type!=0 ORDER BY idn"; // Tabelle Feldnamen lesen
    query.exec(qy);
    i = query.size();
    while(query.next()) {
       i = 0;
       d = query.value(i++).toString();  // db_field
       u = query.value(i++).toString();  // usr_field
       b = query.value(i++).toInt();     // FeldBreite
       b2 = query.value(i++).toInt();    // b2_FeldBreite qsl_Tabelle
      
       toUsr.insert(d,u);                // db_name   -> user_name - Uebersetzungstabelle
       toDb.insert(u,d);                 // user_name -> db_name
       toBr.insert(u,b);                 // usr_name  -> feld_breite
       toBr2.insert(u,b2);               // usr_name  -> b2_feld_breite -  qsl_Tabelle
       toLog.insert(d,query.value(i++).toInt()); // AWD_type 2=fun , 3=funsql
    }
}

// --- wird nicht verwendet -----------------------------------
void translate::clearHashTable()
{
       toUsr.clear();                // db_name   -> user_name - Uebersetzungstabelle
       toDb.clear();                 // user_name -> db_name
       toBr.clear();                 // usr_name  -> feld_breite
       toBr2.clear();                // usr_name  -> b2_feld_breite qsl_Tabelle
}

// -----------------------------------------------------------
void translate::setUsrField(QString dbfield, QString user)
{
    toUsr.insert(dbfield,user); 
}

//  - return USR_Feldnamen
// -----------------------------------------------------------
QString translate::getUsrField(QString dbfield)
{
     QHash<QString, QString>::const_iterator in = toUsr.find(dbfield);
     return in.value();
}

// - return DB_Feldnamen
// ----------------------------------------------------------
QString translate::getDbField(QString usrfield)
{
    QHash<QString, QString>::const_iterator i = toDb.find(usrfield);
    if(i == toDb.end())
       return "?";                          
    return i.value();                       
}

// - return Feld_breite
int translate::getFieldBr(QString usrfield)
{
    QHash<QString, int>::const_iterator i = toBr.find(usrfield);
    return i.value();
}

// - return Feld_breite_2 (qsl_tabelle)
int translate::getFieldBr2(QString usrfield)
{
    QHash<QString, int>::const_iterator i = toBr2.find(usrfield);
    return i.value();
}

// - return index <= 36 fun - >=37 funsql
// -----------------------------------------------------------
int translate::getLogTable(QString dbfield)
{
    QHash<QString, int>::const_iterator i = toLog.find(dbfield);
    return i.value();
}

// BAND_übersetzung_Tabelle aufbauen
// ----------------------------------
void translate::installHashTableBand()
{
 QString b, mb;
    QSqlQuery query; 
    qy = "SELECT band,mband FROM wband WHERE work != 0"; 
    query.exec(qy);
    i = query.size();
    while(query.next()) {
        i = 0;
        b = query.value(i++).toString();     // sysBand
       mb = query.value(i++).toString();     // myBand
       toMband.insert(b,mb);                 // sysBand -> myBand
       toSband.insert(mb,b);                 // myBand  -> sysBand
    }
}

// - return MyBand
// ----------------------------------------------------------
QString translate::getMyband(QString sysband)
{
    QHash<QString, QString>::const_iterator i = toMband.find(sysband);
    if(i == toMband.end())
       return "?";
    return i.value();                        
}

// - return sysBand
// ----------------------------------------------------------
QString translate::getSysband(QString myband)
{
   QHash<QString, QString>::const_iterator i = toSband.find(myband);
   if(i == toMband.end())
      return "?";
   return i.value();
}
