/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007 / V1.5.07 20.01.2009
********************************************************************** */

// dump CSV format

#include <QtGui>
#include <QtSql>
#include "dump.h"

dump::dump() 
{
    state = 0;
}
dump::~dump() {
}

// ---------------------------------
void dump::setDumpPath(QString pstr)
{
  dpath = pstr;
}

// ----------------------------------
void dump::setTabName(QString tabname)
{
   tname = tabname;
}

// ---------------------------------
int dump::getState()
{
   return state;                 // FehlerStatus
}


// ------------------------------------------------------------
// DUMP - kompl. Datenbank QtLog-system
// ------------------------------------------------------------
void dump::dumpLogdb(QString filepath) 
{
 QString s, a, s2;
 int n, i, tcnt;
     QFile file(filepath);
     if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
         state = 2;
         return;
     }
     QTextStream out(&file);
     s = "mysqldump -d --add-drop-table -p";   // Tabellen_strukturen_file vom Server holen
     s += settings.value("dbpasswd").toString();
     s += " ";
     s += settings.value("dbname").toString();
     s += " > backup.sql";
     system(s.toAscii());
     
     s = "cp -bf backup.sql "+dpath;           // nach /path kopieren
     system(s.toAscii());
     system("rm backup.sql &");
     
     QSqlQuery query;                          // hole alle  Tabellen_Namen
     qy = "SHOW TABLES FROM "+settings.value("dbname").toString();
     query.exec(qy);
     tcnt = query.size();
     s = "";
     i = 0;
     while(query.next()) {                     // logFile_str bauen
       n = 0;
       a = query.value(n++).toString();        // Tabellen_namen aus der db holen
       list.insert(i++,a);                     // Liste mit Tabellen_namen aufbauen
       s += a+",";
    }
    
    s2 = s.setNum(tcnt);                       // Tabellen_Anzahl 
    n = 0;
    while( n != tcnt ) {
      s = list[n];
      tname = s;
      if(s.compare("qrzimage"))
         dumpTable(0,"dump");
      else
         dumpImage("dump");
      s2 += ",";
      s2 += list[n];
      n++;
    }
    s2 += "\n";
    out << s2;
    file.close();
}

// ---------------------------------------------------------------
// DUMP eine Datenbank_Tabelle
// ---------------------------------------------------------------
void dump::dumpTable( int tg, QString filetype)
{
  int nfields;
  int n, i;
  QString s, s2, tmp;
     s = dpath+tname+"."+filetype;
     QFile file(s);
     if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
         state = 2;
         return;
     }
     QTextStream out(&file);
     if( tg )
       qy = "SELECT * FROM "+tname+" ORDER BY day,btime";   // Tabelenselection
     else 
       qy = "SELECT * FROM "+tname;
     
     QSqlQuery query;                           // hole alle  Tabellen_Namen
     query.exec(qy);
     i = 0;
     while(query.next()) {                      // 1.Tabelle
        nfields = query.record().count();       // Anzahl Felder
        n = 0;
        tmp = "";
        while(n != nfields) {                   // Anzahl Felder lesen
          tmp += "'";
          if(tg) {
           tmp += "0";
           tg = 0;
          }
          else
            tmp += query.value(n).toString();   // Feld
          tmp += "'";
          n++;
          if( n == nfields ) 
              break;
          tmp += ",";
        }
        out << tmp << "\n";
     }
     file.close();
}

// DUMP Image_datenbank_tabelle
// -------------------------------------
void dump::dumpImage( QString filetype )
{
}


// -----------------------------------------------------------------
// RESTORE alle Tabellen - Leitdatei ist mysql_DUMP_struktur_file
// -----------------------------------------------------------------
int dump::restoreAll()
{
  QString p, t, c;
  int i, n, row;
     i = 0;
     n = 0;
     row = 0;
     p = dpath+"backup.sql";
     QSqlQuery query;                           // hole alle  Tabellen_Namen
     
     QFile f ( p );
     if(!f.open(QIODevice::ReadOnly))
        return 1;                               // FEHLER
     
    QTextStream s( &f );
    while(!s.atEnd()) {
        t = s.readLine();
        if(t.indexOf("DROP",0) != -1) {
           query.exec(t);
           i = t.lastIndexOf(' ',-1);
           n = t.indexOf(';',0);
           i++;
           i++;
           tname = t.mid(i, n -i -1);
        }
        else {
          if(t.indexOf("CREATE",0) != -1) {             // Anfang CREATE ....
            c = t;
            while(!s.atEnd()) {
              t = s.readLine();
              c += t;
              if((t.lastIndexOf(";",-1)) != -1) break;  // END_marker gefunden
            }
            t = dpath+tname+".dump";
            query.exec(c);                              // create tabelle
            restoreFile(t,tname,1);                     // RESTORE table
            row++;
          }
       }
   }
   
   // jetzt RESTORE alle gifs_files
   f.close();
   return row;
}

// Restore eine Tabelle - Leitdatei ist mysql_Dump_struktuere_file
// ---------------------------------------------------------------
int dump::restoreTable(QString table)
{
  QString p, t, c;
    QSqlQuery query;  
    state = 1;
    tname = table;
    p = dpath+"backup.sql";
    QFile f ( p );
    if(! f.open(QIODevice::ReadOnly))
         return 1;
    QTextStream s( &f );
    while(!s.atEnd()) {
      t = s.readLine();
      if(t.indexOf("DROP",0) != -1) {
         if(t.indexOf(tname,0) != -1) {                 // tabellen_name gefunden ?
            query.exec(t);                              // DROP table
            while( ! s.atEnd() ) {
              t = s.readLine();
              c += t;
              if((t.lastIndexOf(";",-1)) != -1) break;  // END_marker gefunden
            }
            query.exec(c);                              // CREATE table
            t = dpath+tname+".dump";
            restoreFile(t,tname,1);                     // RESTORE table
            break;
         }
        }
     }
      // hier qrzImage holen
     return 0;
}


// ----------------------------------------------------------------
// Ein Datenfile in Datenbabk zurückschreiben
// ----------------------------------------------------------------
void dump::restoreFile(QString datfile,QString tname,int del)
{
     QSqlQuery query;  
     if(del) {
        qy = "DELETE FROM "+tname;
        query.exec(qy);
     }
     qy = "LOAD DATA INFILE '"+datfile+"' REPLACE INTO TABLE "+tname;
     qy += " FIELDS TERMINATED BY ',' ENCLOSED BY ''''";
     query.exec(qy);
     state = 0;
}

// Alle Tabellen entfernen
// ---------------------------------------
//void dump::dropAll()
