/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5.07  - 20.01.09
********************************************************************** */

#include <QtGui>
#include <QtSql>
#include <QString>
#include "quAdif.h"
#include "version.h"
quadi::quadi() {
  state = 0;
}
quadi::~quadi() {
}

// Baue die Band_übersetzungs_tabelle
// zur convertierung systemband -> adif_band_name
// ---------------------------------------------------------------------
void quadi::setBandTable()
{
  QString sb, ab;
    QSqlQuery query;
    qy = "SELECT band,aband FROM wband WHERE work != 0";
    query.exec(qy);
    while(query.next()) {                        //
       n = 0;
       sb = query.value(n++).toString();         // system_band
       ab = query.value(n++).toString();         // adif_band
       
       toAdifBand.insert(sb,ab);                 // sysBand -> AdifBand
       toSysBand.insert(ab,sb);                  // AdifBand -> sysBand
    }
}

// --------------------------------------
void quadi::clearBandHash()
{
     toAdifBand.clear();
     toSysBand.clear();
}

// input system_band_val -> return ADIF_band_val
// -----------------------------------------------------------
QString quadi::getAdifBand(QString sysband)
{
    QHash<QString, QString>::const_iterator i = toAdifBand.find(sysband);
    if(i == toAdifBand.end())
       return "?";                        // Fehler     
    return i.value();                     // return adif_Band_val   
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// EXPORT 
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// EX - Erzeuge : header_Namen für SELECT und hash_tabelle zur Conv. dbfield -> AdifName
void quadi::createAdifTableHeader()
{
 QString df, an, q;
 int n;
   QSqlQuery query; 
   qy = "SELECT dbfield,adifnam FROM refnamen WHERE adifnam != '' AND type='2' ORDER BY idn";
   query.exec(qy);
   query.next();                                // 1.Feld
   n = 0;
   tmp = "";                                    // baue String headerfelder
   df = query.value(n++).toString();            // dbfield
   an = query.value(n++).toString();            // adifName
   tmp += df;                                   // 1,Feld im String
   toAdif.insert(df,an);                        // 1.pos hashtabelle
   
   while(query.next()) {                        // die restl.Felder -baue hashtabelle
       n = 0;
       tmp += ",";
       df = query.value(n++).toString();         // dbfield
       an = query.value(n++).toString();         // adifName
       tmp += df; 
       toAdif.insert(df,an);                     // insert hashtable
   }
   n = 0;                                       
   qy = "SELECT dbfield,adifnam FROM refnamen WHERE adifnam != '' AND type='3' ORDER BY idn";
   query.exec(qy);
   while(query.next()) {
      n = 0;
      etmp += ",";
      df = query.value(n++).toString();         // dbfield
      an = query.value(n++).toString();         // adifName
      toAdif.insert(df,an);                     // insert hashtabelle
      etmp += df;
   }
}

// EX - Erzeuge :  AWD_typen Uebersetzungstabelle
// ---------------------------------------------------------
void quadi::loadAdifAwdTable(QString awd,QString addadif)
{
    toAdifCustom.insert(awd,addadif);
}

// EX - convert : input dbfield -> return ADIF_feld_name
// ----------------------------------------------------------
QString quadi::getAdifName(QString dbfield)
{
    QHash<QString, QString>::const_iterator i = toAdif.find(dbfield);
    if(i == toAdif.end())
       return "?";                           
    return i.value();                        
}

// EX - convert : input feld -> return ADIF_feld_name
// ----------------------------------------------------------
QString quadi::getAdifCustom(QString custom)
{
    QHash<QString, QString>::const_iterator i = toAdifCustom.find(custom);
    if(i == toAdifCustom.end())
       return "?";                           
    return i.value();                        
}

// EX - lösche den Inhalt in adif_hash_tabelle
// ------------------------------------------------------------
void quadi::clearExpHash()
{
    toAdif.clear();
    toAdifCustom.clear();
}
 
// Anzahl exportierter Datesätze
//--------------------------------------------------------------
int quadi::getCount()
{
   return count;
}

// Datumformat
void quadi::setDayForm(int idx)
{
   dform = idx;       
}

// Zeitformat 
void quadi::setTimeForm(int idx)
{
     tform = idx;    
}


// *******************************************************************
// doExport (20.11.08) -20.01.09 - ADIF
// 
// *******************************************************************
void quadi::doExport(QString log, QString qystr, QString fpath, QString Awd, int lpref, int leor)
{
 QString p, dfeld, awd;
 int col, idx;
 QString Call, Name, Cept, val, v, id;
 
    QString lo = log;                              // dummy     
    state = 0;
    p = fpath;
    QFile file(p);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
         state = 2;
        return;
    }
    QTextStream out(&file);
    n = lpref;                                     // dummy   
    n = leor;                                      // dummy
    if(Awd.compare("DLD") == 0)
        awd = "DOK";
    
    QSqlQuery query;
    query.exec(qystr);
    n = query.size();                                // Tabellen_größe
    col = query.record().count();                    // Anzahl Tabellen_felder
    
    p = "** ADIF V2.0 file exportet by QtLog ";
    p += VERSION;
    p += " ** LOG: - logs:"+s.setNum(n)+"  date:";
    QDate d = QDate::currentDate();                  // erzeuge das Datum von heute
    p += s.setNum(d.year());
    p += "-";
    p += s.setNum(d.month());
    p += "-";
    p += s.setNum(d.day());
    p += "\n";
    p += "<PROGRAMID:5>QtLog";
    p += "\n";
    p += "<eoh>\n";
    out << p;
    count = 0;
    while(query.next() ) {                           // -- data_table_loop --
      idx = 0;
      p = "";
      while( idx != col ) {                          // -- data_header_loop --
       n = 0;                                        // ----------------------
       dfeld = query.record().fieldName(idx);        // dbfeld_name pos(col)
       val = query.value(idx).toString();            // Feld_Data_value(col)
       if(dfeld.compare("id") == 0) {                // ist ID
          id = val;
       }
       else 
        if(dfeld.compare("cept") == 0) { 
           Cept = val;                                // Cept
        }
       else
        if(dfeld.compare("rufz") == 0) {              // Callsign
          Call = val;
          if(Cept.count() != 0) {
              val = Cept+Call;                        // bilde cept/Callsign
              p += "<CALL:"+v.setNum(val.count())+">"+val+" ";
           }
           else 
             p += "<CALL:"+v.setNum(Call.count())+">"+Call+" ";
        }
       else 
        if(dfeld.compare("day") == 0) {              // QSO_datum
            val.remove(QChar('-'), Qt::CaseInsensitive)+" ";
            p += "<"+getAdifName(dfeld)+":"+v.setNum(val.count())+":d>"+val+" ";
        }
       else 
        if((dfeld.compare("qslrd") == 0) || (dfeld.compare("qslsd") == 0)) {  // Datum QSL_erhalten ..
           val.remove(QChar('-'), Qt::CaseInsensitive);
           if(val.count() != 0) 
             p += "<"+getAdifName(dfeld)+":"+v.setNum(val.count())+":d>"+val+" ";
        }
       else 
        if(dfeld.compare("btime") == 0) {            // QSO_Start
           val.remove(QChar(':'), Qt::CaseInsensitive);
           if(tform == 2)                            // hhmm
              val = val.left(4);
           p += "<"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val+" ";
        }
       else
        if(dfeld.compare("etime") == 0) {            // QSO_Ende
           val.remove(QChar(':'), Qt::CaseInsensitive);
           if(tform == 2)                            // hhmm
             val = val.left(4);
           p += "<"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val+" ";
        }
       else
        if(dfeld.compare("band") == 0) {
           val = getAdifBand(val);                  // Band getAdifBand(QString sysband)
           p += "<"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val+" ";
        }
        else
         if(dfeld.compare("ntext") == 0) {
           if(val.count() != 0) {
             val = val.simplified();                 // entferne '\n', '\v', '\f', '\r', and ' '
             p += "<"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val+" "; 
           }
        }
       else
         if(dfeld.compare("ktext") == 0) {
           if(val.count() != 0) {
             val = val.simplified();                // entferne '\n', '\v', '\f', '\r', and ' '.
             p += "<"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val+" "; 
           }
        }
        else
         if(dfeld.compare("dxcc") == 0) {
            if(lpref) {                             // dxcc als Adif_Nr exporieren
               if(val.count() != 0) {               // nur wenn dxcc vorhanden
                 QSqlQuery query;
                 qy = "SELECT adif FROM tla WHERE ldxcc='"+val+"'";
                 query.exec(qy);
                 query.next(); 
                 val = query.value(0).toString(); 
               }
            }
            p += "<"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val+" ";
        }
       else
        if((dfeld.compare("lotqslrd") == 0) || (dfeld.compare("lotqslsd") == 0)) {
            val.remove(QChar('-'), Qt::CaseInsensitive);
            if(val.count() != 0)
              p += "<"+getAdifName(dfeld)+":"+v.setNum(val.count())+":d>"+val+" ";
        }
       else {
         if(val.count() != 0) {                     //-Leer_felder nicht übernehmen
           if(dfeld.indexOf("awkenner") != -1) {
               p += "<"+awd+":"+s.setNum(val.count())+">"+val+" ";
           }
           else
            p += "<"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val+" "; 
         }
       }
       idx++;                                      // nächstes Feld
     }

    p += "<eor>\n";                                 // Datensatz fertig 
    out << p;                                       // Datensatz in Datei übernehmen
    count++;
  }                                                 // while log_tabelle
  out << "<EOF>\n";
  file.close();
}
