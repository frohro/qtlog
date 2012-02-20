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



void quadi::setBandTable()
{
  QString sb, ab;
    QSqlQuery query;
    qy = "SELECT band,aband FROM wband WHERE work != 0";
    query.exec(qy);
    while(query.next()) {                        //
       n = 0;
       sb = query.value(n++).toString();         
       ab = query.value(n++).toString();         
       toAdifBand.insert(sb,ab);                 
       toSysBand.insert(ab,sb);                  
    }
}

void quadi::clearBandHash()
{
     toAdifBand.clear();
     toSysBand.clear();
}


QString quadi::getAdifBand(QString sysband)
{
    QHash<QString, QString>::const_iterator i = toAdifBand.find(sysband);
    if(i == toAdifBand.end())
       return "?";                           
    return i.value();                        
}




void quadi::createAdifTableHeader()
{
 QString df, an, q;
 int n;
   QSqlQuery query; 
   qy = "SELECT dbfield,adifnam FROM refnamen WHERE adifnam != '' AND type='2' ORDER BY idn";
   query.exec(qy);
   query.next();                                
   n = 0;
   tmp = "";                                    
   df = query.value(n++).toString();            
   an = query.value(n++).toString();            
   tmp += df;                                   
   toAdif.insert(df,an);                        
   while(query.next()) {                        
       n = 0;
       tmp += ",";
       df = query.value(n++).toString();        
       an = query.value(n++).toString();        
       tmp += df; 
       toAdif.insert(df,an);                    
   }
   n = 0;                                       
   qy = "SELECT dbfield,adifnam FROM refnamen WHERE adifnam != '' AND type='3' ORDER BY idn";
   query.exec(qy);
   while(query.next()) {
      n = 0;
      etmp += ",";
      df = query.value(n++).toString();         
      an = query.value(n++).toString();         
      toAdif.insert(df,an);                     
      etmp += df;
   }
}


void quadi::loadAdifAwdTable(QString awd,QString addadif)
{
    toAdifCustom.insert(awd,addadif);
}


QString quadi::getAdifName(QString dbfield)
{
    QHash<QString, QString>::const_iterator i = toAdif.find(dbfield);
    if(i == toAdif.end())
       return "?";                           
    return i.value();                        
}


QString quadi::getAdifCustom(QString custom)
{
    QHash<QString, QString>::const_iterator i = toAdifCustom.find(custom);
    if(i == toAdifCustom.end())
       return "?";                           
    return i.value();                        
}


void quadi::clearExpHash()
{
    toAdif.clear();
    toAdifCustom.clear();
}
 

//--------------------------------------------------------------
int quadi::getCount()
{
   return count;
}

void quadi::setDayForm(int idx)
{
   dform = idx;       
}

void quadi::setTimeForm(int idx)
{
     tform = idx;    
}




void quadi::doExport(QString log, QString qystr, QString fpath, QString Awd, int lpref, int leor)
{
 QString p, dfeld, awd;
 int col, idx;
 QString Call, Name, Cept, val, v, id;
    QString lo = log;                                
    state = 0;
    p = fpath;
    QFile file(p);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
         state = 2;
        return;
    }
    QTextStream out(&file);
    n = lpref;                                       
    n = leor;                                        
    if(Awd.compare("DLD") == 0)
        awd = "DOK";
    QSqlQuery query;
    query.exec(qystr);
    n = query.size();                                
    col = query.record().count();                    
    
    p = "** ADIF V2.0 file exportet by QtLog ";
    p += VERSION;
    p += " ** LOG: - logs:"+s.setNum(n)+"  date:";
    QDate d = QDate::currentDate();                  
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
    while(query.next() ) {                           
      idx = 0;
      p = "";
      while( idx != col ) {                          
       n = 0;                                        
       dfeld = query.record().fieldName(idx);        
       val = query.value(idx).toString();            
       if(dfeld.compare("id") == 0) {                
          id = val;
       }
       else 
        if(dfeld.compare("cept") == 0) { 
           Cept = val;                                
        }
       else
        if(dfeld.compare("rufz") == 0) {              
          Call = val;
          if(Cept.count() != 0) {
              val = Cept+Call;                        
              p += "<CALL:"+v.setNum(val.count())+">"+val+" ";
           }
           else 
             p += "<CALL:"+v.setNum(Call.count())+">"+Call+" ";
        }
       else 
        if(dfeld.compare("day") == 0) {              
            val.remove(QChar('-'), Qt::CaseInsensitive)+" ";
            p += "<"+getAdifName(dfeld)+":"+v.setNum(val.count())+":d>"+val+" ";
        }
       else 
        if((dfeld.compare("qslrd") == 0) || (dfeld.compare("qslsd") == 0)) {  
           val.remove(QChar('-'), Qt::CaseInsensitive);
           if(val.count() != 0) 
             p += "<"+getAdifName(dfeld)+":"+v.setNum(val.count())+":d>"+val+" ";
        }
       else 
        if(dfeld.compare("btime") == 0) {            
           val.remove(QChar(':'), Qt::CaseInsensitive);
           if(tform == 2)                            
              val = val.left(4);
           p += "<"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val+" ";
        }
       else
        if(dfeld.compare("etime") == 0) {            
           val.remove(QChar(':'), Qt::CaseInsensitive);
           if(tform == 2)                            
             val = val.left(4);
           p += "<"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val+" ";
        }
       else
        if(dfeld.compare("band") == 0) {
           val = getAdifBand(val);                  
           p += "<"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val+" ";
        }
        else
         if(dfeld.compare("ntext") == 0) {
           if(val.count() != 0) {
             val = val.simplified();                
             p += "<"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val+" "; 
           }
        }
       else
         if(dfeld.compare("ktext") == 0) {
           if(val.count() != 0) {
             val = val.simplified();                
             p += "<"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val+" "; 
           }
        }
        else
         if(dfeld.compare("dxcc") == 0) {
            if(lpref) {                             
               if(val.count() != 0) {               
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
       idx++;                                      
     }

    p += "<eor>\n";                                 
    out << p;                                       
    count++;
  }                                                 
  out << "<EOF>\n";
  file.close();
}
