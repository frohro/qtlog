/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007, 20.11.08, 08.07.09
********************************************************************** */

#include <QtGui>
#include <QtSql>
#include <QString>
#include "wadif.h"
#include "../qtlogDiag/version.h"


wadif::wadif() {
  state = 0;
}

wadif::~wadif() {
}

// ----------------------------------
void wadif::openRefListQso(int rows)
{
   qsoFields = rows;
   adh = new ADIFHEAD[rows];          // Speicher für einen QSO_Datensatz reservieren
   state = 0;
   count = 0;
}

// ---------------------------------
void wadif::openRefListQsl(int rows)
{
   qslFields = rows;
   adhq = new ADIFHEAD[rows];         // Speicher für einen QSO_Datensatz reservieren
}

// --------------------------------
void wadif::openRefListAwd(int rows)
{
   awdFields = rows;
   adhw = new ADIFHEAD[rows];         // Speicher für einen QSO_Datensatz reservieren
}

// -------------------------------
void wadif::closeRefLists()
{
   delete adh;                       // Speicher zurückgeben
   delete adhq;
   delete adhw;
}

// Fehler_status
// ------------------------------
int wadif::getState()
{
    return state;                   // Fehler_Status
}

// Fehler_protkoll zurückgeben
// ----------------------------
QString wadif::getErrorInfo()
{
  return Error;
}

// Anzahl importierter QSO's
// ---------------------------
int wadif::getCount()
{
   return count;
}



// englisch ??????????????????? !!!!!!!!!!!!!!!!!!!!!!!!
// ---------------------------------------------------------------------------
// ReferenzListe mit Spalten_Namen, UserFeldNamen und ADIf_referenzNamen laden
// ( 4_dimensionales Arryfeld ) zur Aufnahme des Datensatzes
// ---------------------------------------------------------------------------
void wadif::loadRefList()
{
    QSqlQuery query;
    qy = "SELECT dbfield,refnam,adifnam FROM refnamen WHERE type = '2' ORDER BY idn";
    query.exec(qy);
    openRefListQso(query.size());
    
    i = 0;
    while(query.next()) {                                   // Datensatz erzeugen
       n = 0;                                                        // Index_Felder
       adh[i].pos = i;                                               // index_Nr
       strcpy(adh[i].field,query.value(n++).toString().toAscii());   // dbSpalten_Namen
       strcpy(adh[i].refnam,query.value(n++).toString().toAscii());  // User_Name
       strcpy(adh[i].adifnam,query.value(n++).toString().toAscii()); // ADIF_Name
       strcpy(adh[i].value,"");                            // Datensatz_Feld_variable
       i++;
    }
    
 // englich ????????????? !!!!!!!!!!!!!!!!!!!
     // funqsl Tabelle
    qy = "SELECT dbfield,refnam,adifnam FROM refnamen WHERE type='3' OR type ='4' ORDER BY idn";
    query.exec(qy);
    openRefListQsl(query.size());
    i = 0;
    while(query.next()) {                                    // Datensatz erzeugen
       n = 0;                                                         // Index_Felder
       adhq[i].pos = i;                                               // index_Nr
       strcpy(adhq[i].field,query.value(n++).toString().toAscii());   // dbSpalten_Namen
       strcpy(adhq[i].refnam,query.value(n++).toString().toAscii());  // User_Name
       strcpy(adhq[i].adifnam,query.value(n++).toString().toAscii()); // ADIF_Name
       strcpy(adhq[i].value,"");                             // Datensatz_Feld_variable
       i++;
    }
}

// ---------------------------
void wadif::loadRefListAwd()
{
    // funawd Tabelle
    QSqlQuery query;
    qy = "SELECT atype,adiftype FROM wawdlist WHERE aset != '0' ORDER BY aset";
    query.exec(qy);
    openRefListAwd(query.size());                                                // Liste erzeugen
    i = 0;
    while(query.next()) {                                                        // values eintragen
       n = 0;                                                                    
       adhw[i].pos = i;                                                          // index_Nr
       strcpy(adhw[i].field, query.value(n++).toString().toAscii().toUpper());   // dbSpalten_Namen (atype)
       strcpy(adhw[i].refnam,"");                                                
       strcpy(adhw[i].adifnam, query.value(n++).toString().toAscii().toUpper()); // ADIF_Name (adiftype)
       strcpy(adhq[i].value,"");                                                  // kenner_variable
       i++;
    }
}

// Übersetze : input db_Feldname -> return ADIF
// --------------------------------------
char * wadif::getAdifName(char * nam)
{
    int idx = 0;
    while( idx != qsoFields ) {
      if(strcmp(adh[idx].field,nam) == 0) break;
         idx++;
    }
    return adh[idx].adifnam;
}

// Übersetze : input ADIF -> return db_Feldname
// --------------------------------------------
char * wadif::getdbName(char * nam)
{
    int idx = 0;
    while( idx != qsoFields ) {
      if(strcmp(adh[idx].adifnam,nam) == 0) break;
         idx++;
    }
    if(idx != qsoFields)                // gefunden
      return adh[idx].field;
    else 
      return (char *)"";                // nicht gefunden
}

// Übersetze : input ADIF.qsl -> return db_Feldname
// ------------------------------------------------
char * wadif::getdbNameQsl(char * nam)
{
    int idx = 0;
    while( idx != qslFields ) {
      if(strcmp(adhq[idx].adifnam,nam) == 0) break;
         idx++;
    }
    if(idx != qslFields)                // gefunden
      return adhq[idx].field;
    else 
      return (char *)"";                // nicht gefunden
}

// Übersetze : input ADIF.awd -> return db_Feldname
// -------------------------------------------------
char * wadif::getdbNameAwd(char *nam)
{
   int idx = 0;
    while( idx != awdFields ) {
      if(strcmp(adhw[idx].adifnam,nam) == 0) break;
         idx++;
    }
    if(idx != awdFields)                // gefunden
      return adhw[idx].field;
    else 
      return (char *)"";                 // nicht gefunden
}

// kopiere Feldvarilable in name.[index].var
// ----------------------------------------
void wadif::setValue(char *nam, QString v)
{
     int idx = 0;
     while( idx != qsoFields ) {
       if(strcmp(adh[idx].field,nam) == 0) break;
          idx++;
     }
     strcpy(adh[idx].value,v.toAscii());
}

// prüfe ob in 'value' berets 'v' (B,E oder L) enthalten ist
// wenn ja; nichts tun, sonst 'v' anhängen
// Funktion für qslRvia und qslSvia
// ---------------------------------------------------------
void wadif::addQslFlag(char *nam, QString v)
{
  char sbuf[12];
  int i;
  int idx = 0;
  
     while( idx != qsoFields ) {
       if(strcmp(adh[idx].field,nam) == 0) break;
          idx++;
     }
     strcpy(sbuf,adh[idx].value);
     i = 0;
     while(i != 10) {
       if(sbuf[i] == (QChar)v[0]) break;
       if(i++ == 10) break;
     }
     if(i != 10) return;                     // B,E, oder L gefunden
     else
       strcat(adh[idx].value,v.toAscii());  // nicht gefunden, B,E oder L anhängen
}

// kopiere Feldvarilable in name.[index].var
// ----------------------------------------
void wadif::setValueQsl(char *nam, QString v)
{
     int idx = 0;
     while( idx != qsoFields ) {
       if(strcmp(adhq[idx].field,nam) == 0) break;
          idx++;
     }
     strcpy(adhq[idx].value,v.toAscii());
}

// kopiere Feldvarilable in name.[index].var
// ----------------------------------------
void wadif::setValueAwd(char *nam, QString v)
{
     int idx = 0;
     while( idx != awdFields ) {
       if(strcmp(adhw[idx].field,nam) == 0) break;
          idx++;
     }
     strcpy(adhw[idx].value,v.toAscii());
}

// input db_feldName return value
// -------------------------------------
QString wadif::getValue(char * fname)
{
     int idx = 0;
     while( idx != qsoFields ) {
       if(strcmp(adh[idx].field,fname) == 0) break;
          idx++;
     }
     return adh[idx].value;
}

// input db_feldName return value
// ----------------------------------
QString wadif::getValueQsl(char *nam)
{
    int idx = 0;
    while( idx != qslFields ) {
      if(strcmp(adhq[idx].field,nam) == 0) break;
         idx++;
    }
    return adhq[idx].value;
}

// =====================================================================


// Baue die Band_übersetzungs_tabelle
// zur convertierung systemband -> adif_band_name
// ---------------------------------------------------------------------
void wadif::setBandTable()
{
  QString sb, ab;
    QSqlQuery query;
    qy = "SELECT band,aband FROM wband WHERE work != 0";
    query.exec(qy);
    while(query.next()) {                       //
       n = 0;
       sb = query.value(n++).toString();        // system_band
       ab = query.value(n++).toString();        // adif_band
       
       toAdifBand.insert(sb,ab);                // sysBand -> AdifBand
       toSysBand.insert(ab,sb);                 // AdifBand -> sysBand
    }
}

// ----------------------------------------
void wadif::clearBandHash()
{
     toAdifBand.clear();
     toSysBand.clear();
}

// input system_band_val -> return ADIF_band_val
// -----------------------------------------------------------
QString wadif::getAdifBand(QString sysband)
{
    QHash<QString, QString>::const_iterator i = toAdifBand.find(sysband);
    if(i == toAdifBand.end())
       return "?";                           // FEHLER
    return i.value();                        // return adif_Band_val
}

// input ADIF_band -> return sys_band
// ----------------------------------------------------------
QString wadif::getSysBand(QString aband)
{
    QHash<QString, QString>::const_iterator i = toSysBand.find(aband);
    if(i == toSysBand.end())
       return "?";                           // FEHLER
    return i.value();                        // return sys_Band_val
}



// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// EXPORT 
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// EX - Erzeuge : header_Namen für SELECT und hash_tabelle zur Conv. dbfield -> AdifName
void wadif::createAdifTableHeader()
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
       df = query.value(n++).toString();        // dbfield
       an = query.value(n++).toString();        // adifName
       tmp += df; 
       toAdif.insert(df,an);                    // hashtabelle füllen
   }
   
   n = 0;                                       // erweiterter Datensatz qsl
   qy = "SELECT dbfield,adifnam FROM refnamen WHERE adifnam != '' AND type='3' ORDER BY idn";
   query.exec(qy);
   while(query.next()) {
      n = 0;
      etmp += ",";
      df = query.value(n++).toString();         // dbfield
      an = query.value(n++).toString();         // adifName
      toAdif.insert(df,an);                     // adifName
      etmp += df;
   }
}

// EX - Erzeuge :  AWD_typen Uebersetzungstabelle
// ---------------------------------------------------------
void wadif::loadAdifAwdTable(QString awd,QString addadif)
{
    toAdifCustom.insert(awd,addadif);
}

// EX - convert : input dbfield -> return ADIF_feld_name
// ----------------------------------------------------------
QString wadif::getAdifName(QString dbfield)
{
    QHash<QString, QString>::const_iterator i = toAdif.find(dbfield);
    if(i == toAdif.end())
       return "?";                           // FEHLER nicht gefunden
    return i.value();                        // return adif_name
}

// EX - convert : input dbfield -> return ADIF_feld_name
// ----------------------------------------------------------
QString wadif::getAdifCustom(QString custom)
{
    QHash<QString, QString>::const_iterator i = toAdifCustom.find(custom);
    if(i == toAdifCustom.end())
       return "?";                           // FEHLER nicht gefunden
    return i.value();                        // return adif_name
}

// EX - lösche den Inhalt in adif_hash_tabelle
// ----------------------------------------------------------
void wadif::clearExpHash()
{
    toAdif.clear();
    toAdifCustom.clear();
}



// ************************************************************
// doExport (20.11.08)
// logFile, pathDateiname,??
// ************************************************************
void wadif::doExport(QString log, QString fpath, int lpref)
{
 QString p, dfeld;
 int col, idx;
 QString Call, Name, Cept, val, v, id;
    state = 0;
    p = fpath+log+".adi";
    QFile file(p);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
         state = 2;
        return;
    }
    QTextStream out(&file);
    n = lpref;                                        // dummy
    QSqlQuery query;
    qy = "SELECT id,cept,"+tmp+etmp;                  // QUERY bauen SELECT + header_felder
    qy += " FROM "+log+"om LEFT JOIN ("+log+"qsl,"+log+") ON (omid=oid AND qsoid=id)";
    qy += " ORDER BY day";
    //qDebug() << qy;
    
    query.exec(qy);
    n = query.size();                                 // Tabellen_größe
    col = query.record().count();                     // Anzahl Tabellen_felder
    
    p = "** ADIF V2.0 file exportet by QtLog V1.5.07 ** LOG:"+log+" - logs:"+s.setNum(n)+"  date:";
    QDate d = QDate::currentDate();                   // erzeuge das Datum von heute
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
           Cept = val;                               // Cept
        }
       else
        if(dfeld.compare("rufz") == 0) {             // Call
          Call = val;
          if(Cept.count() != 0) {
              val = Cept+Call;                       // bilde cept/Call
              p += "<CALL:"+v.setNum(val.count())+">"+val;
           }
           else 
             p += "<CALL:"+v.setNum(Call.count())+">"+Call;
        }
       else 
        if(dfeld.compare("day") == 0) {              // QSO_datum
            val.remove(QChar('-'), Qt::CaseInsensitive);
            p += " <"+getAdifName(dfeld)+":"+v.setNum(val.count())+":d>"+val;
        }
       else 
        if((dfeld.compare("qslrd") == 0) || (dfeld.compare("qslsd") == 0)) {   // Datum QSL_erhalten ..
           val.remove(QChar('-'), Qt::CaseInsensitive);
           if(val.count() != 0) 
             p += " <"+getAdifName(dfeld)+":"+v.setNum(val.count())+":d>"+val;
        }
       else 
        if(dfeld.compare("btime") == 0) {             // QSO_Start
           val.remove(QChar(':'), Qt::CaseInsensitive);
           p += " <"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val;
        }
       else
        if(dfeld.compare("etime") == 0) {             // QSO_Ende
           val.remove(QChar(':'), Qt::CaseInsensitive);
           p += " <"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val;
        }
       else
        if(dfeld.compare("band") == 0) {
           val = getAdifBand(val);                   // Band getAdifBand(QString sysband)
           p += " <"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val;
        }
        else
         if(dfeld.compare("ntext") == 0) {
           if(val.count() != 0) {
             val = val.simplified();                 // entferne '\n', '\v', '\f', '\r', and ' 
             p += " <"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val; 
           }
        }
       else
         if(dfeld.compare("ktext") == 0) {
           if(val.count() != 0) {
             val = val.simplified();                 // entferne '\n', '\v', '\f', '\r', and ' '.
             p += " <"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val; 
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
            p += " <"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val;
        }
       else
        if((dfeld.compare("lotqslrd") == 0) || (dfeld.compare("lotqslsd") == 0)) {
            val.remove(QChar('-'), Qt::CaseInsensitive);
            if(val.count() != 0)
              p += " <"+getAdifName(dfeld)+":"+v.setNum(val.count())+":d>"+val;
        }
       else {
         if(val.count() != 0) {                      // Leer_felder nicht übernehmen
           p += " <"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val; 
         }
       }
       idx++;                                        // nächstes Feld
     }
//--
    
    qy = "SELECT awtype,awkenner FROM "+log+"awd WHERE qid="+id;
    QSqlQuery query(qy);
    n = query.size();
    while(query.next()) {
       n = 0;
       dfeld = query.value(n++).toString();         // zB DLD
       val = query.value(n++).toString();           // zB E33
       p += " <"+getAdifCustom(dfeld)+":"+v.setNum(val.count())+">"+val; 
    }                                               // while daten_satz_felder
    p += " <eor>\n";                                // Datensatz fertig 
    out << p;                                       // Datensatz in Datei übernehmen
    count++;
  }                                                 // while log_tabelle
  out << "<EOF>\n";
  file.close();
}


// ====================================================================================
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// IMPORT 
// logFile, path_Dateiname, qsltype, rig, home_id
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void wadif::doImport(QString log,QString fpath, int qslmode, QString rig, int hId)
{
 QString val, token, prog;
 QString Call, qsoDate;
 QString t, z;
 int idx, n, oid, id, sekcnt, vl, len;
 bool ok;
 char buf[255];
    sekcnt = 10;                                      // Sekundenzähler 10..59
    oid = 0;
    QSqlQuery query;
    QFile datei( fpath );
    if(! datei.open(QIODevice::ReadOnly)) {
        state = 1;
        return;                                       // FEHLER
    }
    count = 0;
    QTextStream istream( &datei);
    
    // Header lesen ----------------------------------
    while( istream.atEnd() != true ) {                // HEADER '<eoh>' suchen
        len = 0;
        while(len == 0) {                             // leere zeilen ueberspringen
           t = istream.readLine(0);
           len = t.count();
           if(len != 0) break;                        // zeile mit text gefunden
        }
        idx = 0;                                      // text auswerten
        while(len != idx)
          if(t[idx++] == QChar('<')) break;           // suche '<'
        z = "";
        if(len != idx) {                              // '<' gefunden, text ist gueltig
           while(t[idx] != QChar(':') && t[idx] != QChar('>')) z += t[idx++]; 
           if(t[idx] == QChar('>')) {                 // '>' gefunden, dann ist token ohne text
              z = z.toUpper();                        // token immer groß
              if(z.compare("EOH") == 0) break;        // header_ende ?
           }
           else {                                     
              val = "";                               // ':' gefunden, es gibt text
              idx++;
              if(z.compare("PROGRAMID") == 0)         // wenn ProgrammID
                while(t[idx] != QChar('>')) val += t[idx++]; 
              vl = val.toInt(&ok,10);
              idx++;                                  
              val = "";
              n = vl;                                 // text:_laenge
              while(n-- != 0) val += t[idx++];        // text lesen
              prog = val;
           }
        }
    }
    // Header gelesen 
    
    // ------------ jetzt DATA ---------------------
    Error = "";
    count = 0;                                            // Datensatz Zähler
    while(istream.atEnd() == FALSE) {                     // ADIF_text bis Datei_ende bearbeiten
        i = 0;
        while(i != qsoFields) strcpy(adh[i++].value,"");  // value(datenfelder) QSO_tabelle loeschen
        i = 0;
        while(i != qslFields) strcpy(adhq[i++].value,""); // value(datenfelder) QSL_tabelle loeschen
        i = 0;
        while(i != awdFields) strcpy(adhw[i++].value,""); // value(datenfelder) AWD_tabelle loeschen

        z = istream.readLine(0); // daten_zeile lesen und bearbeiten. Zeile ist: stream ohne '\n' or '\l\r',
                                 // daten_token oder Zeilenanteile, daten_zeile_ende ist <eor> im stream.
        if(z.compare("<EOF>") == 0) break;                // File_ende ?

        while(z.count() == 0) {                 // check: es koennte eine leer_zeile vor <eor> sein
           if(istream.atEnd() == TRUE) break;   // oder eine leer_zeile mitten in der text_zeile (Ucx)
           z = istream.readLine(0);             // suche text
        }
        
        if(istream.atEnd() == TRUE) break;      // ok. ist File_ende
                                              
        len = z.count();                        
        vl = 0;
        Lcn  = "";
        Cqz  = "";
        Ituz = "";
        Dxcc = "";
        t = "";
        n = 0;
        idx = 0;                                     // zeilen_index 
        while( 1 ) {                                 // 1. zeile aus datastream
            while(len != idx) {
               if(z[idx++] == QChar('<')) break;     // gefunden
            }
            if(len == idx) {                         // das war eine leere_restzeile oder
                z = istream.readLine(0);   // zeile war token oder zeilen_anteil; neue zeile (token) lesen
                len = z.count();
                if(len == 0)                         // war leer_zeile
                   z = istream.readLine(0);          // n. zeile lesen
                len = z.count();
                idx = 0;
                while(z[idx] != QChar('<')) idx++;   // suche token_anfang
                idx++; 
            } 
            
            t = "";
            while(z[idx] != QChar(':') && z[idx] != QChar('>')) t += z[idx++]; // ADIF_token_text
            if(z[idx] == QChar('>')) {               // steht am Textende '>' ist token ohne Text
                t = t.toUpper();                     // wenn ja; immer groß
                if((t.compare("EOR") == 0) || (t.compare("EOF") == 0)) // check auf zeilen od. Datei_ENDE
                  break;
            }
            if(t.compare("EOF") == 0) break;         // * schluss *
            token = t;                               // ADIF_token sichern
            t = "";
            idx++;
            while(z[idx] != QChar('>') && z[idx] != QChar(':')) t += z[idx++]; // text_laenge holen
            vl = t.toInt(&ok,10);                    // laenge Datenstring sichern
    
            if(z[idx] == QChar(':')) {               // ist Datum_type <QSL_DATE:8:d>
              idx++;                                 // z.Z ':' überspringen
              idx++;                                 // z.Z 'd' überspringen
            }
            idx++;                                   // '>' überspringen
            t = "";
            n = vl;                                  // nutz_datenstring_laenge
            while(n-- != 0) t += z[idx++];           // nutz_daten lesen
            val = t;
                                                     // token + data bearbeitem
            token = token.toUpper();                 // ADIF_wort immer in groß_Buchstaben
            strcpy(buf,token.toAscii());             // ADIF_name nach ascii
            strcpy(buf,getdbName(buf));              // adif_name nach db_Spalten_name übersetzen
	    
             // <token:n>val im *QSO_Datensatz* gefunden - jetzt namen checken, variable ist in val
            if(strlen(buf) != 0) {          // falls db_spalten_name gefunden, value bearbeiten sonst überspringen      
                if(strcmp(buf,"rufz") == 0) {        // ist es das rufzeichen ?
                  n = val.indexOf("/");              // check Cept_prefix im val
                  if(n != -1) {                      // flush gefunden
                    if( n <= 4 ) {                   // ja ist cept ( es gibt auch prefixe grösser 4 !! )
                      s = val.left(n);               // ja: cept_pref sichern
                      s += "/";                      // flush anhängen
                      val.remove(0,n +1);            // cept_pref vom Rufzeiche entfernen
                      setValue((char *)"cept",s);    // CEPT_prefix setzen
                    }
                  }
                  setValue((char *)"rufz",val);   // Rufzeichen setzen ( ohne cept )
                  Call = val;                     // Rufz merken
                }
                else
                 if(strcmp(buf,"dxcc") == 0) {    // convertiere ADIF_Nr->la_pref
                   n = val.toInt(&ok,10);         // wenn dxcc numerisch
                   if(ok == true )                // wenn Zahl, convertiere nach pref
                     qy = "SELECT lcn,cqz,ituz,ldxcc FROM tla WHERE adif="+s.setNum(n);
                   else
                     qy = "SELECT lcn,cqz,ituz,ldxcc FROM tla WHERE ldxcc='"+val+"'";
                   query.exec(qy);
                   if(query.size() != 0 ) {             // prefix oder adif_nr gefunden
                      query.next();
                      Lcn  = query.value(0).toString(); // hole: cn aus Landestabelle
                      Cqz  = query.value(1).toString(); //       cqz
                      Ituz = query.value(2).toString(); //       ituz
                      Dxcc = query.value(3).toString(); //       dxcc_prefix
                      setValue(buf,Dxcc);               // setze:DXCC_pref -alpha_type
                   }
                   else {                               // nicht gefunden
                     Error += " Call "+getValue((char *)"rufz");
                     Error += " dxcc_prefix < "+val;
                     Error += " > nicht gefunden\n";    // tr() ???
                     state = 1;                         // es ist ein Fehler aufgetreten
                   }
                 }
                else
                 if(strcmp(buf,"band") == 0) {         // check Band
                   val = val.toUpper();
                   val = getSysBand(val);              // übersetze Band_variable
                   setValue(buf,val);                  // setze band
                 }
                else
                 if(strcmp(buf,"day") == 0) {          // check Qso_Tag
                    setValue(buf,val);                 // Tag setzen
                    qsoDate = val;                     // Datun merken
                 }
                else 
                  if(strcmp(buf,"btime") == 0) {       // check Qso_Start_zeit
                    if(vl != 6) {                      // falls Zeit 6.stellig
                      sekcnt++;                        // Sekunden +1
                      if(sekcnt == 60)                 // sek. min 10 <-> 60 max
                        sekcnt = 10;
                      val.append(s.setNum(sekcnt));    // Zeit auf 8.stellig erweitern
                    }
                    setValue(buf,val);                 // setze Zeit
                  }
                else
                  if(strcmp(buf,"etime") == 0) {       // Qso_Ende_zeit
                     if(vl != 6) {
                       sekcnt++;                       // Sekunden +1
                       if(sekcnt == 60)
                         sekcnt = 10;
                       val.append(s.setNum(sekcnt));   // Zeit 8.stellig
                     }
                     setValue(buf,val);                // setze Zeit
                  }
                else 
                  
                  if(strcmp(buf,"qslr") == 0) {  // ---------// QSL_r - Büro --------------------
		     if(val.compare("N") == 0)
		       val = "R";
		     else {
			if(val.compare("Y") == 0)            // QSL eralten ?
			  addQslFlag((char *)"qslRvia","B"); // QSLr_via 'B'
		     }
                     setValue(buf,val);                      // R, Y, I
	          }
                else
                  if(strcmp(buf,"qsls") == 0) {              // qsl_s
		     if( !qslmode )                          // qslMode = 0 ( daten aus contest )
		       val = "";
		     else {                                  // qslMode = 1 ( daten aus anderem LOG ) - wie Import
		       if(val.compare("N") == 0) {
		         val = "R";
			 setValue((char *)"prflg","B");      // QSL_Karte drucken - ja;
		       }
		       else
			 if(val.compare("Y") == 0)           // QSL gesendet - ja;
		          addQslFlag((char *)"qslSvia","B"); // QSLs_via 'B'
                     }
                     setValue(buf,val);                      // val = "", R, Y, I
                  }
                else {
                 setValue(buf,val);                          // gefunden, default - value in arry eintragen
                } 
             }
             else {  // -----------------------------------  // QSL * LOTW
	      strcpy(buf,token.toAscii());
              strcpy(buf,getdbNameQsl(buf));                 // adif_feldname übersetzen
              if(strlen(buf) != 0) {                          // falls Spalten_name gefunden, bearbeiten
                 if(strcmp(buf,"eqsls") == 0) {              // eQSL * eqsl_s    
		   if(val.compare("N") == 0)                 // (UcxL) N = noch nicht gesendet
                      val = "R";                             // setze R_equested
                    else {
                      if(val.compare("Y") == 0) {
                        addQslFlag((char *)"qslSvia","E");   // QSLs_via 'E'
                        setValueQsl(buf,val);                // val eintragen R, Y
		      }
		    }
		 }
	         else                                        // eQSL_r
                  if(strcmp(buf,"eqslr") == 0) {
                    if(val.compare("N") == 0)                // (UcxL) N = noch nicht erhalten
                      val = "R";                             // setze Requested
                    else {
                      if(val.compare("Y") == 0) {            // QSL erhalten ?
                        addQslFlag((char *)"qslRvia","E");   // QSLr_via 'E'
                        setValueQsl(buf,val);                // val eintragen R, Y
		      }
		    }
	        }
	        
	        else       // -------------------------------// LOTW * lotqsl_r
		if(strcmp(buf,"lotqslr") == 0) {             // ---------------
                   if(val.compare("N") == 0)                 // (UcxL) n = nicht erhalten
                      val = "R";                             // setze R_equested
                   else {
                     if(val.compare("Y") == 0) {             // QSL erhalten ?
                       addQslFlag((char *)"qslRvia","L");    // QSLr_via 'L'
                       setValueQsl(buf,val);                 // val eintragen R, Y
		     }
		   }
	        }
		else                                         // lotqsl_s
		if(strcmp(buf,"lotqsls") == 0) {
                   if(val.compare("N") == 0)                 // (UcxL) N = noch nicht erhalten
                      val = "R";                             // setze R_equested
                   else {
                     if(val.compare("Y") == 0) {             // QSL gesendet ?
                       addQslFlag((char *)"qslSvia","L");    // QSLs_via 'L'
                       setValueQsl(buf,val);                 // val eintragen R, Y
		     }
		   }
	        }
	        else
	         setValueQsl(buf,val);                        // default
	      }
              else {   // ----------------------------- AWD_daten_satz
                strcpy(buf,token.toAscii());
                strcpy(buf,getdbNameAwd(buf));         // adif_feldname nach dbname übersetzen
                if(strlen(buf) != 0) {                 // AWD_Type gefunden
                  setValueAwd(buf,val);                // value eintragen
                }
                else {                                 // Fehler_text setzen
                  Error += " Call "+getValue((char *)"rufz");
                  Error += " "+token;
                  Error += " text < "+val;
                  Error += " > nicht gefunden\n";      // tr() ??
                  state = 1;                           // es ist ein Fehler aufgetreten
                }
              } 
            }   // ein tocken bearbeitet
         }      // WHILE( 1 ) <eor> nicht erreicht
         
//--     ENDE_Adif_data_zeile -- <EOR> gefunden
         
          // fehlende Values nachtragen 
         if(getValue((char *)"dxcc").count() == 0) {   // falls dxcc_feld leer
           s = getValue((char *)"rufz");               // hole Rufzeichen
           n = 1;
           z = s.left(n);
           while(getla( z ) == 0) {
              if(n >= 4) {                             // unbekannt
                Error += "dxcc fuer Rufzeichen "+s+" nicht gefunden\n";
                state = 1;                             // es ist ein Fehler aufgetreten
               break;
              }
              z = s.left(++n);
           }
           setValue((char *)"cn",Lcn);
           setValue((char *)"cqz",Cqz);
           setValue((char *)"ituz",Ituz);
           setValue((char *)"dxcc",Dxcc);
         }
         
         if(getValue((char *)"cn").count() == 0) {     // falls Cont fehlt
            setValue((char *)"cn",Lcn);                // setze continent
         }
         
         if(getValue((char *)"cqz").count() == 0) {   // falls CQ_zone fehlt
            setValue((char *)"cqz",Cqz);              // setze..CQzone
         }
         
         if(getValue((char *)"ituz").count() == 0) {  // falls ITU_zone fehlt 
            setValue((char *)"ituz",Ituz);            // setze ..zone
         }
         
         if(getValue((char *)"etime").count() == 0) {             // falls Ende_zeit fehlt
            setValue((char *)"etime", getValue((char *)"btime")); // setze = bzeit
         }
         
         s = s.setNum(hId);                          // portabel_id
         strcpy(buf,s.toAscii());
         setValue((char *)"rig",rig);                // ( ist für DRUCK erforderlich )
         setValue((char *)"qline",(char *)"KW");     // kein ADIF_token - könnte leer bleiben
         setValue((char *)"quarter",buf);            // ist ADIF_import_portabel_type
         setValue((char *)"op",(char *)"0");         // Operator intern_value z.Z.= 0
// ------ DEBUG
/*
         n = 0;    // TEST Data ausgeben
         i = 0;
         while(i != qsoFields) {
            qDebug() << adh[i].pos <<  adh[i].field << adh[i].adifnam << adh[i].value;
            i++;
         }
         n = 0;
         i = 0;
         while(i != qslFields) {
           qDebug() << adhq[i].pos <<  adhq[i].field << adhq[i].adifnam << adhq[i].value;
           i++;
         }
         n = 0;
         i = 0;
         while(i != awdFields) {
           qDebug() << adhw[i].pos <<  adhw[i].field << adhw[i].adifnam << adhw[i].value;
           i++;
         }
         //datei.close(); 
         //return;        
*/
// ----- DEBUG
         QSqlQuery query;
	 
// --    OM --------------------------------------------  alle LOG_Datensatze erzeugen
         qy = "SELECT omid FROM "+log+"om WHERE rufz='";
         qy += Call;
         qy += "'";
         query.exec(qy);
         n = query.size();
         if(n == 0 ) {                                    // ist der OM neu ?
            qy = "INSERT INTO "+log+"om VALUES (0,'";     // ja;
            qy += Call;
            qy += "','";
            qy += adh[2].value;
            qy += "')";
            QSqlQuery query;
            query.exec(qy);
            oid = query.lastInsertId().toInt();           // Datensatz merken
         }
         else {
          query.next();
          oid = query.value(0).toInt();
         } 

         // --    QSO ------------------------------------------
         n = 3;                         // starte auf index_pos_3
         qy = "INSERT INTO "+log+" VALUES (0,"+s.setNum(oid)+",'";
         qy += adh[0].value;            // cept
         while(n != qsoFields -3 ) {    // alle felder übernehmen
           qy += "','";
           qy += adh[n++].value;
         }
         qy += "',";
         qy += adh[n++].value;          // quarter
         qy += ",";
         qy += adh[n++].value;          // Operator
         qy += ")";
         query.exec(qy);                // Datensatz eintragen
         id = query.lastInsertId().toInt();
	 
// --    QSL -------------------------------------------
         n = 0;                         // starte auf index_pos_0  
         qy = "INSERT INTO "+log+"qsl VALUES (0,"+s.setNum(id)+",'";
         qy += Call;
         qy += "','"+qsoDate;
         while(n != qslFields) {         // alle felder übernehmen
           qy += "','";
           qy += adhq[n++].value;
         }
         qy += "')";
         query.exec(qy);                 // Datensatz eintragen
         // qDebug() << query.lastError();
	 
// --    AWD -------------------------------------------
         n = 0;
         while(n != awdFields) {             // alle felder prüfen und übernehmen
          if(strlen(adhw[n].value) != 0) {   // value eingetragen ?
             qy = "INSERT INTO "+log+"awd VALUES (0,"+s.setNum(id)+",'";
             qy += Call;
             qy += "','";
             qy += adhw[n].field;
             qy += "','";
             qy += adhw[n].value;
             qy += "')";
             query.exec(qy);               // Datensatz eintragen
           }
           n++;                            // nächsten awd_type
         }
         count++;                          // 1 Datensatz kompl. bearbeitet - QSO_zähler +1
/*
         if(count == 1) {                   // TEST einen Datensatz bearbeiten
          datei.close();
          return;
         }
*/
     }
     datei.close();
     return;
}

// -------------------------------------------------
int wadif::getla(QString str)
{
  int i;
   
    i = 0;
    QSqlQuery query;                                       // setze query_instanz
    qy = "SELECT pref FROM tpref WHERE spref='"+str+"'";   // suche Land zum Prefix
    query.exec(qy);
    i = query.size();                                      // id > 0 Prefix gefunden
    if( i ) {                                              // wenn id = 1 Land gefunden
       query.next();
       s = query.value(0).toString();
       qy = "SELECT lcn,cqz,ituz,ldxcc FROM tla WHERE ldxcc='"+s+"'";
       query.exec(qy);
       query.next();
       Lcn  = query.value(0).toString();  // hole cn aus Landestabelle
       Cqz  = query.value(1).toString();  //      cqz
       Ituz = query.value(2).toString();  //      ituz
       Dxcc = query.value(3).toString();  //      dxcc_prefix
      }
      return i;
}
