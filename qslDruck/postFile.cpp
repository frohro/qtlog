/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007 / 02.02.2009
********************************************************************** */

#include <QtGui>
#include <QtSql>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "postFile.h"
#include "qslPr.h"

PostFile::PostFile()
:settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
    q  = new Qso(sizeof(Qso));       
    r  = new Rig(sizeof(Rig));       
    qk = new Qslk(sizeof(Qslk));     
    ktop = qk;
    err = 0;
}

PostFile::~PostFile()
{
    delete(q);
    delete(r);
    Qslk * temp;
    while(ktop->next != 0) {
      temp = ktop->next;
      delete(ktop);
      ktop = temp;
    }
    delete(ktop);
}

int PostFile::GetState()
{
  return err;
}

QString PostFile::GetErrMessage()
{
  return ErrInfo;
}


void PostFile::LoadMedium( QString name)
{
 int i, x;
 QString s, b;
    QSqlQuery query;
    trans.installHashTableBand();
    
    
    qy = "SELECT * FROM prmedium WHERE traeger='"+name+"' AND sel=1";
    query.exec(qy);
    while( query.next()) {
       i = 0;
       traeger = query.value(i++).toString().toAscii(); 
       form = query.value(i++).toString().toAscii();    
       form = form.toUpper();
       s = query.value(i++).toString();
       sel = s.toInt();                     
       s = query.value(i++).toString();
       breite = s.toFloat( 0 );             
       s = query.value(i++).toString();
       hoehe = s.toFloat( 0 );              
       s = query.value(i++).toString();
       col = s.toInt();                     
       s = query.value(i++).toString();
       row = s.toInt();                     
       s = query.value(i++).toString();
       randx = s.toFloat( 0 );              
       s = query.value(i++).toString();
       randy = s.toFloat( 0 );              
       s = query.value(i++).toString();
       zwx = s.toFloat( 0 );                
       s = query.value(i++).toString();
       zwy = s.toFloat( 0 );                
       s = query.value(i++).toString();     
       x = s.indexOf("=",0);
       s.remove(0,x+1);
       x = s.indexOf(',',0);
       b = s.left(x);
       tposx = b.toFloat( 0 );              
       x = s.indexOf("=",0);
       s.remove(0,x+1);
       tposy = s.toFloat( 0 );              
       printerStr = query.value(i++).toString(); 
       s = query.value(i++).toString();
       zuf = s.toInt();                     
       s = query.value(i++).toString();
       frame = s.toFloat();                 
       s = query.value(i++).toString();     
       just = s.toInt();
    }
    
    
    qy = "SELECT * FROM qslk WHERE ktype LIKE '"+form+"%'";
    
    query.exec(qy);
    while(query.next()) {
      i = 0;
      strcpy(qk->Name,query.value(i++).toString().toAscii());
      strcpy(qk->Ktype,query.value(i++).toString().toAscii());
      strcpy(qk->Dear,query.value(i++).toString().toAscii());
      strcpy(qk->Well,query.value(i++).toString().toAscii());
      strcpy(qk->Cqz,query.value(i++).toString().toAscii());
      strcpy(qk->Text1,query.value(i++).toString().toAscii());
      strcpy(qk->Text2,query.value(i++).toString().toAscii());
      strcpy(qk->Text3,query.value(i++).toString().toAscii());
      qk = qk->next = new Qslk(sizeof(Qslk));
    }
    qk->next = 0;
}



void PostFile::Label( QString lf, int id, QString logfile, int label, int sqsl,int qslcnt, char *datum)
{
  QString s;
  char buf[12];
  char logf[20];
  int i;
     strcpy(logf,logfile.toAscii());
     log = logfile;
     QSqlQuery query;
     
     
     qy = "SELECT rufz,name,dxcc,cept,day,btime,band,mode,freq,";
     qy += "qslr,rsts,qslmgr,rig,txpwr,rxpwr,ktext,quarter,op,qinfo ";
     qy += "FROM "+logfile+"om LEFT JOIN ("+logfile+"qsl"+","+logfile+") ON (omid=oid AND qsoid=id) ";
     qy += "WHERE id="+s.setNum(id);
     query.exec(qy);
     i = 0;
     query.next();
     strcpy(q->Call,query.value(i++).toString().toAscii());      
     strcpy(q->Name,query.value(i++).toString().toAscii());      
     strcpy(q->Dxcc,query.value(i++).toString().toAscii());      
     strcpy(q->Cept,query.value(i++).toString().toAscii());      
     strcpy(q->Day,query.value(i++).toString().toAscii());       
     strcpy(q->Btim,query.value(i++).toString().toAscii());      
     s = query.value(i++).toString();                            
     strcpy(q->Band,trans.getMyband(s).toAscii());               
     strcpy(q->Mod,query.value(i++).toString().toAscii());       
     strcpy(q->Freq,query.value(i++).toString().toAscii());      
     strcpy(q->Qslr,query.value(i++).toString().toAscii());      
     strcpy(q->Rsts,query.value(i++).toString().toAscii());      
     strcpy(q->Qslmgr,query.value(i++).toString().toAscii());    
     strcpy(q->Rig,query.value(i++).toString().toAscii());       
     strcpy(q->TxPwr,query.value(i++).toString().toAscii());     
     strcpy(q->RxPwr,query.value(i++).toString().toAscii());     
     strcpy(q->Ktext,query.value(i++).toString().toAscii());     
     q->QuarterId = atoi(query.value(i++).toString().toAscii()); 
     q->Operator = atoi(query.value(i++).toString().toAscii());  
     strcpy(q->Swl,query.value(i++).toString().toAscii());       
     
     
     qy = "SELECT * FROM wrig WHERE rigtype='";
     qy += q->Rig;
     qy += "'";
     query.exec(qy);
     numr = query.size();
     i = 0;
     if(numr) {
       query.next();
       strcpy(r->Rigtype,query.value(i++).toString().toAscii());
       strcpy(r->Trans,query.value(i++).toString().toAscii());
       strcpy(r->Ant,query.value(i++).toString().toAscii());
       strcpy(r->Op,query.value(i++).toString().toAscii());
       strcpy(r->Spez,query.value(i++).toString().toAscii());
     }
     else {
        err = 1;
        ErrInfo = "\nABBRUCH\n-----------------------------------------------------\nRIG - ";
        ErrInfo += q->Rig;
        ErrInfo += " - in der DB nicht bekannt.\n\nKorr. Rig-Eintrag im Qso ";
        ErrInfo += q->Call;
        strcpy(r->Rigtype,"");
        strcpy(r->Trans,"");
        strcpy(r->Ant,"");
        strcpy(r->Op,"");
        strcpy(r->Spez,"");
     }
      
      
      strcpy(userCall,settings.value("Call").toString().toAscii());
      strcpy(userName,settings.value("Name").toString().toAscii());
      strcpy(qth,settings.value("Qth").toString().toAscii());
      strcpy(hdxcc,settings.value("Dxcc").toString().toAscii());
      strcpy(locator,settings.value("Locator").toString().toAscii());
      strcpy(mail,settings.value("Mail").toString().toAscii());
      strcpy(www,settings.value("www").toString().toAscii());
      
      
      strcpy(operatorCall,settings.value("Call").toString().toAscii());
      strcpy(buf,form.toAscii());                    
      
      
      k = ktop;
      while(k->next != 0) {
         if(strcmp(q->Dxcc,k->Name) == 0 ) {         
           if(strcmp(buf,k->Ktype) == 0 ) break;
         }
         k = k->next;
      }
      
      
      if(strcmp(q->Dxcc,k->Name) != 0) {             
         k = ktop;                                   
         while(k->next != 0) {
           if(strcmp(k->Name,"DEF") == 0) {          
              if(strcmp(buf,k->Ktype) == 0 ) break;
           }
           k = k->next;
         }
      }

     if(form.compare("L",Qt::CaseSensitive) == 0) {
         create_Label(lf, label, sqsl, qslcnt, datum);     
     }
     else
      if(form.compare("LSWL",Qt::CaseSensitive) == 0) {
         create_Label(lf, label, sqsl, qslcnt, datum);     
      }
     else 
      if(form.compare("K",Qt::CaseSensitive) == 0) {
         create_Card(lf, sqsl, qslcnt, datum);             
        }
     else
      if(form.compare("KSWL",Qt::CaseSensitive) == 0) {
        create_Card_SWL(lf, datum);                        
      }
}



void PostFile::create_Label( QString fname, int label, int sqsl,int qslcnt,char *datum)
{
  char buf[40];
  float  x = 0;
  float  y = 0;
  float lx = 0;
  float ly = 0;
  QString s;
       switch(label) {  
       case 1: lx  = randx;       ly = randy;
	        x = lx +tposx;     y = ly + hoehe -tposy;
	       break;
       case 2: lx = randx;        ly = randy -zwy -hoehe;
	        x = lx +tposx;     y = ly + hoehe -tposy;
	       break;
       case 3: lx = randx;        ly = randy -(2*zwy) -(2 *hoehe);
	        x = lx +tposx;     y = ly + hoehe -tposy;
	       break;
       case 4: lx = randx;        ly = randy -(3*zwy) -(3 *hoehe);
	        x = lx +tposx;     y = ly + hoehe -tposy;
	       break;
                        
       case 5: lx = randx +zwx +breite; ly = randy;
	        x = lx +tposx;           y = ly + hoehe -tposy;
	       break;
       case 6: lx = randx +zwx +breite; ly = randy -zwy -hoehe;
                x = lx +tposx;           y = ly + hoehe -tposy;
	       break;
       case 7: lx = randx +zwx +breite; ly = randy -(2*zwy) -(2 *hoehe);
               x = lx +tposx;            y = ly + hoehe -tposy;
	       break;
       case 8: lx = randx +zwx +breite; ly = randy -(3*zwy) -(3 *hoehe);
                x = lx +tposx;           y = ly + hoehe -tposy;
	       break;
     }
    
    if(form.compare("LSWL") == 0)
      create_Label_SWL(fname, log, datum, x, y, lx, ly);
    else {
     QFile pfile(fname);                              
     if (!pfile.open(QIODevice::Append | QIODevice::Text)) {
         qDebug() << "ERROR";
        return;
     }
     QTextStream out(&pfile);                         
     out << ".01 setlinewidth\n";
     if(just) {                     
        out << "newpath "+QString::number(lx,0,4)+" "+QString::number(ly,0,4)+" moveto "+QString::number(breite,0,4)+" "+QString::number(0,0,1)+" rlineto "+QString::number(0,0,1)+" "+QString::number(hoehe,0,4)+" rlineto "+QString::number(breite,0,4)+" neg "+QString::number(0,0,1)+" rlineto closepath stroke \n";
     }
     else {
     if(frame) {                                     
       float difx = breite -(2 * frame);             
       float dify = hoehe -(2 * frame);
       out << "newpath "+QString::number(lx+frame,0,4)+" "+QString::number(ly+frame,0,4)+" moveto "+QString::number(difx,0,4)+" "+QString::number(0,0,0)+" rlineto "+QString::number(0,0,0)+" "+QString::number(dify,0,4)+" rlineto "+QString::number(difx,0,4)+" neg "+QString::number(0,0,0)+" rlineto closepath stroke\n";
     }
     out << "/Palatino-Bold findfont [ .3 0 0 .3 0 0] makefont setfont\n";
     ysum = y + 0.06;
     out << QString::number(x,0,4)+" "+QString::number(ysum,0,4)+" moveto ("+userCall+") show\n";

     strcpy(buf,"");
     if(strlen(q->Cept)) {
	sprintf(buf,"%s%s",q->Cept,q->Call);                    
     }
     else {
       sprintf(buf,"%s",q->Call);                               
     }
     
     
     QString str = buf;
     out << QString::number(x+3.8,0,4)+" "+QString::number(y+0.06,0,4)+" moveto (to Radio :) show\n";
     out << "/Palatino-Roman findfont [.3 0 0 .4 0 0] makefont setfont\n";
     out << QString::number(x+5.5,0,4)+" "+QString::number(y+0.06,0,4)+" moveto ("+str+") show\n";
     if( q->Operator ) {                     
      out << "/Palatino-Roman findfont [.22 0 0 .22 0 0] makefont setfont\n";
      out << QString::number(x,0,4)+" "+QString::number(y-0.35,0,4)+" moveto (OP : "+operatorCall+") show\n";
     }
     out << "/Palatino-Bold findfont [.3 0 0 .35 0 0] makefont setfont\n";
     out << QString::number(x+4.5,0,4)+" "+QString::number(y-0.35,0,4)+" moveto (via :      "+q->Qslmgr+") show\n";
     out << QString::number(x,0,4)+" "+QString::number(y-0.45,0,4)+" moveto .03 setlinewidth 8 0 rlineto stroke\n";
     
     if( sqsl ) {
        switch(qslcnt) {
         case 0:
          out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
          out << QString::number(x,0,4)+" "+QString::number(y-0.91)+" moveto (I am glad to confirm the following 2-way QSO's) show\n";
          out << "/Palatino-Bold findfont [.3 0 0 .35 0 0] makefont setfont\n";
          out << QString::number(x,0,4)+" "+QString::number(y-1.55,0,4)+" moveto ( Date               UTC            MHz            Mode          RS(T) )show\n";
          out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
          out << QString::number(x,0,4)+" "+QString::number(y-2.0,0,4)+" moveto ("+q->Day+") show\n";
          out << QString::number(x+1.8,0,4)+" "+QString::number(y-2.0,0,4)+" moveto ("+q->Btim+") show\n";
          out << QString::number(x+3.45,0,4)+" "+QString::number(y-2.0,0,4)+" moveto("+q->Band+") show\n";
          out << QString::number(x+5.1,0,4)+" "+QString::number(y-2.0,0,4)+" moveto ("+q->Mod+") show\n";
          out << QString::number(x+6.7,0,4)+" "+QString::number(y-2.0,0,4)+" moveto ("+q->Rsts+") show\n";
               break;
        case 1:
          out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
          out << QString::number(x,0,4)+" "+QString::number(y-2.4,0,4)+" moveto ("+q->Day+") show\n";
          out << QString::number(x+1.8,0,4)+" "+QString::number(y-2.4,0,4)+" moveto ("+q->Btim+") show\n";
          out << QString::number(x+3.45,0,4)+" "+QString::number(y-2.4,0,4)+" moveto ("+q->Band+") show\n";
          out << QString::number(x+5.1,0,4)+" "+QString::number(y-2.4,0,4)+" moveto ("+q->Mod+") show\n";
          out << QString::number(x+6.7,0,4)+" "+QString::number(y-2.4,0,4)+" moveto ("+q->Rsts+") show\n";
               break;
        case 2:
          out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
          out << QString::number(x,0,4)+" "+QString::number(y-2.8,0,4)+" moveto ("+q->Day+") show\n";
          out << QString::number(x+1.8,0,4)+" "+QString::number(y-2.8,0,4)+" moveto ("+q->Btim+") show\n";
          out << QString::number(x+3.45,0,4)+" "+QString::number(y-2.8,0,4)+" moveto ("+q->Band+") show\n";
          out << QString::number(x+5.1,0,4)+" "+QString::number(y-2.8,0,4)+" moveto ("+q->Mod+") show\n";
          out << QString::number(x+6.7,0,4)+" "+QString::number(y-2.8,0,4)+" moveto ("+q->Rsts+") show\n";
               break;
        case 3:
          out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
          out << QString::number(x,0,4)+" "+QString::number(y-3.2,0,4)+" moveto ("+q->Day+") show\n";
          out << QString::number(x+1.8,0,4)+" "+QString::number(y-3.2,0,4)+" moveto ("+q->Btim+") show\n";
          out << QString::number(x+3.45,0,4)+" "+QString::number(y-3.2,0,4)+" moveto ("+q->Band+") show\n";
          out << QString::number(x+5.1,0,4)+" "+QString::number(y-3.2,0,4)+" moveto ("+q->Mod+") show\n";
          out << QString::number(x+6.7,0,4)+" "+QString::number(y-3.2,0,4)+" moveto ("+q->Rsts+") show\n";
               break;
        }
        out << "/Palatino-Bold findfont [.25 0 0 .25 0 0] makefont setfont\n";
        out << QString::number(x,0,4)+" "+QString::number(y-3.72,0,4)+" moveto ("+q->Ktext+") show\n";
        out << "/Palatino-Roman findfont [.25 0 0 .25 0 0] makefont setfont\n";
        out << QString::number(x,0,4)+" "+QString::number(y-4.1,0,4)+" moveto ("+k->Text1+") show\n";
        out << QString::number(x,0,4)+" "+QString::number(y-4.5,0,4)+" moveto ("+k->Text2+") show\n";
      }
      else {
       out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
       out << QString::number(x,0,4)+" "+QString::number(y-0.91,0,4)+" moveto ("+k->Dear+" "+q->Name+") show\n";
       //-
       out <<  QString::number(x,0,4)+" "+QString::number(y-1.25,0,4)+" moveto ("+k->Well+") show\n";
       out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
       
       
       
       
       out << QString::number(x+0.12,0,4)+" "+QString::number(y-1.71,0,4)+" moveto (Date  ) show\n";
       out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
       out << QString::number(x+1.2+0.12,0,4)+" "+QString::number(y-1.71,0,4)+" moveto ("+q->Day+") show\n";
       out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
       out << QString::number(x+3.0+0.12,0,4)+" "+QString::number(y-1.71,0,4)+" moveto (UTC ) show\n";
       out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
       out << QString::number(x+3.8+0.12,0,4)+" "+QString::number(y-1.71,0,4)+" moveto ("+q->Btim+") show\n";
       out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
       if(strlen(q->Freq))
         out << QString::number(x+5.1+0.12,0,4)+" "+QString::number(y-1.71,0,4)+" moveto ("+q->Band+" - "+q->Freq+" MHz) show\n";
        else
         out << QString::number(x+5.1+0.12,0,4)+" "+QString::number(y-1.71,0,4)+" moveto ("+q->Band+" Band) show\n";
       out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
       
       out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
       out << QString::number(x+0.12,0,4)+" "+QString::number(y-2.06,0,4)+" moveto (2-Way ) show\n";
       out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
       out << QString::number(x+1.2+0.12,0,4)+" "+QString::number(y-2.06,0,4)+" moveto ("+q->Mod+") show\n";
       out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
       out << QString::number(x+3.0+0.12,0,4)+" "+QString::number(y-2.06,0,4)+" moveto (RST  ) show\n";
       out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
       out << QString::number(x+3.8+0.12,0,4)+" "+QString::number(y-2.06,0,4)+" moveto ("+q->Rsts+" ) show\n";
       if(strlen(q->RxPwr)) {                            
         if(strcmp(k->Name,"DL") == 0) {
           out << QString::number(x+4.8,0,4)+" "+QString::number(y-2.06,0,4)+" moveto (Dein ) show\n";
           out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
           out << QString::number(x+5.5+0.12,0,4)+" "+QString::number(y-2.06,0,4)+" moveto (PWR ) show\n";
           out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
           out << QString::number(x+6.4+0.12,0,4)+" "+QString::number(y-2.06,0,4)+" moveto ("+q->RxPwr+" Watt) show\n";
         }
         else {
           out << QString::number(x+4.8,0,4)+" "+QString::number(y-2.06,0,4)+" moveto (your ) show\n";
           out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
           out << QString::number(x+5.5+0.12,0,4)+" "+QString::number(y-2.06,0,4)+" moveto (PWR ) show\n";
           out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
           out << QString::number(x+6.4+0.12,0,4)+" "+QString::number(y-2.06,0,4)+" moveto ("+q->RxPwr+" watts) show\n";
         }
       }
       
       out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
       out << QString::number(x+0.12,0,4)+" "+QString::number(y-2.56,0,4)+" moveto (Trans  ) show\n";
       out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
       out << QString::number(x+1.2+0.12,0,4)+" "+QString::number(y-2.56,0,4)+" moveto ("+r->Trans+" - "+q->TxPwr+" Watt) show\n";
       
       out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
       out << QString::number(x+0.12,0,4)+" "+QString::number(y-2.90,0,4)+" moveto (Ant.    ) show\n";
       out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
       out << QString::number(x+1.2+0.12,0,4)+" "+QString::number(y-2.90,0,4)+" moveto ("+r->Ant+") show\n";
       
       out << QString::number(x,0,4)+" "+QString::number(y-3.4,0,4)+" moveto ("+k->Text1+") show\n";
       
       if( strlen(q->Ktext) != 0 )   
         out << QString::number(x,0,4)+" "+QString::number(y-3.74,0,4)+" moveto ("+q->Ktext+") show\n";
       else
         if( q->QuarterId > 1 )      
              out << QString::number(x,0,4)+" "+QString::number(y-3.74,0,4)+" moveto ("+qth+") show\n";
      
      out << QString::number(x,0,4)+" "+QString::number(y-4.12,0,4)+" moveto ("+k->Text2+") show\n";
      
      out << "/Palatino-Roman findfont [.2 0 0 .2 0 0] makefont setfont\n";
      out << QString::number(x,0,4)+" "+QString::number(y-4.54,0,4)+" moveto ("+k->Text3+") show\n";
     }
    //---
     out << QString::number(x,0,4)+" "+QString::number(y-4.66,0,4)+" moveto .02 setlinewidth 8 0 rlineto stroke\n";
     out << "/Palatino-Roman findfont [.2 0 0 .2 0 0] makefont setfont\n";
     out << QString::number(x,0,4)+" "+QString::number(y-4.96,0,4)+" moveto (Printed "+datum+"  QtLog - QSO-Managment - System : "+r->Op+"    mail - "+mail+") show\n";
    }
    pfile.close();
   }
}



void PostFile::create_Card(QString fname, int sqsl, int qslcnt, char *datum)
{
  char buf[256];
  float lx = 0;
  float ly = 0;
  float x = 0;
  float y = 0;
   lx  = randx;
   ly = randy;
   x = lx +tposx;
   y = ly + hoehe -tposy;
   QFile pfile(fname);
    if (!pfile.open(QIODevice::Append | QIODevice::Text)) {
         qDebug() << "ERROR";
        return;
    }
    QTextStream out(&pfile);
   out << ".01 setlinewidth\n";
   
/*
   out << "newpath "+QString::number(lx,0,4)+" "+QString::number(ly,0,4)+" moveto "+QString::number(breite,0,4)+" "+QString::number(0,0,4)+" rlineto "+QString::number(0,0,4)+" "+QString::number(hoehe,0,4)+" rlineto "+QString::number(breite,0,4)+" neg "+QString::number(0,0,4)+" rlineto closepath stroke\n";
*/
   
   out << "/Palatino-Roman findfont [.4 0 0 .4 0 0] makefont setfont\n";
   out << QString::number(x,0,4)+" "+QString::number(y,0,4)+" moveto ("+userCall+") show\n";
   strcpy(buf,"");
   QString str;
   if(strlen(q->Cept)) {
     str += q->Cept;                                      
     str += q->Call;
   }
   else {
    str = q->Call;                                       
   }
   
   out << QString::number(x+5.8,0,4)+" "+QString::number(y,0,4)+" moveto (to Radio   :) show\n";
   out << "/Palatino-Bold findfont [.4 0 0 .5 0 0] makefont setfont\n";
   out << QString::number(x+8.15,0,4)+" "+QString::number(y,0,4)+" moveto ("+str+") show\n";
    if( q->Operator ) {                    
      out << "/Palatino-Roman findfont [.22 0 0 .22 0 0] makefont setfont\n";
      out << QString::number(x,0,4)+" "+QString::number(y-0.6,0,4)+" moveto (OP : "+operatorCall+") show\n";
    }
   out << "/Palatino-Roman findfont [.4 0 0 .4 0 0] makefont setfont\n";
   out << QString::number(x+6.6,0,4)+" "+QString::number(y-0.6,0,4)+" moveto (via    :    "+q->Qslmgr+") show\n";
   out << QString::number(x,0,4)+" "+QString::number(y-0.8,0,4)+" moveto .03 setlinewidth 12 0 rlineto stroke\n";
   
    if( !sqsl ) {
      out << ".01 setlinewidth\n";
      out << "newpath "+QString::number(x,0,4)+" "+QString::number(y-3.2,0,4)+" moveto "+QString::number(12.0,0,4)+" "+QString::number(0,0,4)+" rlineto "+QString::number(0,0,4)+" "+QString::number(1.0,0,4)+" rlineto   "+QString::number(12.0,0,4)+" neg "+QString::number(0,0,4)+" rlineto closepath stroke\n";
   }
   if( sqsl ) {                             
     switch(qslcnt) {
     case 0:
            out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
            out << QString::number(x,0,4)+" "+QString::number(y-1.6,0,4)+" moveto (I am glad to confirm the following 2-way QSO's) show\n";
            out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
            out << QString::number(x+0.4,0,4)+" "+QString::number(y-2.4,0,4)+" moveto (Date                    UTC                  MHZ             Mode             RS(T)) show\n";
            out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
            out << QString::number(x+0.4,0,4)+" "+QString::number(y-2.9,0,4)+" moveto ("+q->Day+") show\n";
            out << QString::number(x+2.2+0.4)+" "+QString::number(y-2.9,0,4)+" moveto ("+q->Btim+") show\n";
	    out << QString::number(x+4.25+0.4,0,4)+" "+QString::number(y-2.9,0,4)+" moveto ("+q->Band+") show\n";
	    out << QString::number(x+5.95+0.4,0,4)+" "+QString::number(y-2.9,0,4)+" moveto ("+q->Mod+") show\n";
            out << QString::number(x+7.75+0.4,0,4)+" "+QString::number(y-2.9,0,4)+" moveto ("+q->Rsts+") show\n";
            break;
     case 1:
            out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
            out << QString::number(x+0.4,0,4)+" "+QString::number(y-3.4,0,4)+" moveto ("+q->Day+") show\n";
            out << QString::number(x+2.2+0.4,0,4)+" "+QString::number(y-3.4,0,4)+" moveto ("+q->Btim+") show\n";
	    out << QString::number(x+4.25+0.4,0,4)+" "+QString::number(y-3.4,0,4)+" moveto ("+q->Band+") show\n";
	    out << QString::number(x+5.95+0.4,0,4)+" "+QString::number(y-3.4,0,4)+" moveto ("+q->Mod+") show\n";
            out << QString::number(x+7.75+0.4,0,4)+" "+QString::number(y-3.4,0,4)+" moveto ("+q->Rsts+") show\n";
            break;
     case 2:
            out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
            out << QString::number(x+0.4,0,4)+" "+QString::number(y-3.9,0,4)+" moveto ("+q->Day+") show\n";
            out << QString::number(x+2.2+0.4,0,4)+" "+QString::number(y-3.9,0,4)+" moveto ("+q->Btim+") show\n";
	    out << QString::number(x+4.25+0.4,0,4)+" "+QString::number(y-3.9,0,4)+" moveto ("+q->Band+") show\n";
	    out << QString::number(x+5.95+0.4,0,4)+" "+QString::number(y-3.9,0,4)+" moveto ("+q->Mod+") show\n";
            out << QString::number(x+7.75+0.4,0,4)+" "+QString::number(y-3.9,0,4)+" moveto ("+q->Rsts+") show\n";
            break;
     case 3:
            out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
            out << QString::number(x+0.4,0,4)+" "+QString::number(y-4.4,0,4)+" moveto ("+q->Day+") show\n";
            out << QString::number(x+2.2+0.4,0,4)+" "+QString::number(y-4.4,0,4)+" moveto ("+q->Btim+") show\n";
	    out << QString::number(x+4.25+0.4,0,4)+" "+QString::number(y-4.4,0,4)+" moveto ("+q->Band+") show\n";
	    out << QString::number(x+5.95+0.4,0,4)+" "+QString::number(y-4.4,0,4)+" moveto ("+q->Mod+") show\n";
            out << QString::number(x+7.75+0.4,0,4)+" "+QString::number(y-4.4,0,4)+" moveto (%s) show\n";
            break;
     }
    if(sqsl) {
     if(qslcnt == 0) {
     
     out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
     out << QString::number(x+0.4,0,4)+" "+QString::number(y-4.9,0,4)+" moveto ("+q->Ktext+") show\n";
     
     out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
     out << QString::number(x,0,4)+" "+QString::number(y-5.5,0,4)+" moveto ("+k->Text1+") show\n";
     
     out << QString::number(x,0,4)+" "+QString::number(y-5.9,0,4)+" moveto ("+k->Text2+") show\n";
     
     out << QString::number(x,0,4)+" "+QString::number(y-6.6,0,4)+" moveto ("+k->Text3+") show\n";
     }
    }
   }
   else {
      
      out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
      
    
    
    
      out << QString::number(x,0,4)+" "+QString::number(y-1.5,0,4)+" moveto ("+k->Dear+" "+q->Name+",) show\n";
      
      out << QString::number(x,0,4)+" "+QString::number(y-1.9,0,4)+" moveto ("+k->Well+") show\n";
      
      out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
      out << QString::number(x+0.3,0,4)+" "+QString::number(y-2.6,0,4)+" moveto (DATE           : ) show\n";
      
      out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
      out << QString::number(x+2.3,0,4)+" "+QString::number(y-2.6,0,4)+" moveto ("+q->Day+") show\n";
      
      out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
      out << QString::number(x+4+0.4,0,4)+" "+QString::number(y-2.6,0,4)+" moveto (UTC  : ) show\n";
      out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
      out << QString::number(x+5.2+0.4,0,4)+" "+QString::number(y-2.6,0,4)+" moveto ("+q->Btim+") show\n";
      
      out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
      out << QString::number(x+6.7+0.4,0,4)+" "+QString::number(y-2.6,0,4)+" moveto (RST : ) show\n";
      out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
      out << QString::number(x+7.7+0.4,0,4)+" "+QString::number(y-2.6,0,4)+" moveto ("+q->Rsts+") show\n";
      
      out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
      out << QString::number(x+0.3,0,4)+" "+QString::number(y-3.0,0,4)+" moveto (2-Way           : ) show\n";
      out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
      sprintf(buf,"%s  %s    ",q->Mod,q->Band);
      if(strlen(q->Freq)) {
        strcat(buf,q->Freq); strcat(buf," MHz");
      }
      if(strlen(q->RxPwr)) {
        if(strcmp(k->Name,"DL") == 0) {
          strcat(buf,"       Dein PWR : "); strcat(buf,q->RxPwr); strcat(buf," Watt");
        }
        else {
          strcat(buf,"       your PWR : "); strcat(buf,q->RxPwr); strcat(buf," watts");
        }
      }
      out << QString::number(x+2.3,0,4)+" "+QString::number(y-3.0,0,4)+" moveto ("+buf+") show\n";
     
     out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
     out << QString::number(x+0.3,0,4)+" "+QString::number(y-3.6,0,4)+" moveto (Transceiver :) show\n";
     out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
     out << QString::number(x+2.3,0,4)+" "+QString::number(y-3.6,0,4)+" moveto ("+r->Trans+"  /  "+q->TxPwr+" Watt) show\n";
     
     out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
     out << QString::number(x+0.3,0,4)+" "+QString::number(y-4,0,4)+" moveto (Ant.              :) show\n";
     out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
     out << QString::number(x+2.3,0,4)+" "+QString::number(y-4,0,4)+" moveto ("+r->Ant+") show\n";
     
     out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
     out << QString::number(x+0.3,0,4)+" "+QString::number(y-4.4,0,4)+" moveto ("+k->Cqz+") show\n";
     
     if(strlen( q->Ktext ) != 0 )
       out << QString::number(x+0.3,0,4)+" "+QString::number(y-4.9,0,4)+" moveto ("+q->Ktext+") show\n";
     else
       if(q->QuarterId > 1)
         out << QString::number(x,0,4)+" "+QString::number(y-4.9,0,4)+" moveto ("+qth+") show\n";
     
     out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
     if(strcmp(q->Qslr,"R") == 0) {        
        out << QString::number(x,0,4)+" "+QString::number(y-5.5,0,4)+" moveto ("+k->Text1+") show\n";
     }
     else {
       if(strcmp(q->Qslr,"Y") == 0) {
         if(strcmp(k->Name,"DL") == 0)     
           out << QString::number(x,0,4)+" "+QString::number(y-5.4,0,4)+" moveto (Vielen Dank fuer Deine QSL-Karte) show\n";
         else
           out << QString::number(x,0,4)+" "+QString::number(y-5.4,0,4)+" moveto (Thank for received QSL) show\n";
       }
     }
     
     out << QString::number(x,0,4)+" "+QString::number(y-6.2,0,4)+" moveto ("+k->Text2+") show\n";
     
     //fprintf(ft,"/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n");
     out << QString::number(x,0,4)+" "+QString::number(y-6.7,0,4)+" moveto ("+k->Text3+") show\n";
  }
  
  out << QString::number(x,0,4)+" "+QString::number(y-6.8,0,4)+" moveto .02 setlinewidth 12 0 rlineto stroke\n";
  
  out << "/Palatino-Roman findfont [.2 0 0 .2 0 0] makefont setfont\n";
  out << QString::number(x,0,4)+" "+QString::number(y-7.1,0,4)+" moveto (Printed  "+datum+") show\n";
  out << "/Palatino-Bold findfont [.2 0 0 .2 0 0] makefont setfont\n";
  out << QString::number(x+1.96,0,4)+" "+QString::number(y-7.1,0,4)+" moveto (QtLog) show\n";
  out << "/Palatino-Roman findfont [.2 0 0 .2 0 0] makefont setfont\n";
  out << QString::number(x+2.6,0,4)+" "+QString::number(y-7.1,0,4)+" moveto (-  QSO Managmentsystem  :  "+r->Op+" mail - "+mail+"  -  "+www+") show\n";
   pfile.close();
}


void PostFile::create_Label_SWL(QString fname, QString logfile, char * datum, float x, float y, float lx, float ly)
{
  int n, i;
  QString s;
    QFile pfile(fname);
    if(!pfile.open(QIODevice::Append | QIODevice::Text)) {
       qDebug() << "ERROR";
      return;
    }
    QTextStream out(&pfile);
    out << ".01 setlinewidth\n";
    if(just) {                     
        out << "newpath "+QString::number(lx,0,4)+" "+QString::number(ly,0,4)+" moveto "+QString::number(breite,0,4)+" "+QString::number(0,0,1)+" rlineto "+QString::number(0,0,1)+" "+QString::number(hoehe,0,4)+" rlineto "+QString::number(breite,0,4)+" neg "+QString::number(0,0,1)+" rlineto closepath stroke \n";
     }
     else {
     if(frame) {                                     
       float difx = breite -(2 * frame);             
       float dify = hoehe -(2 * frame);
       out << "newpath "+QString::number(lx+frame,0,4)+" "+QString::number(ly+frame,0,4)+" moveto "+QString::number(difx,0,4)+" "+QString::number(0,0,0)+" rlineto "+QString::number(0,0,0)+" "+QString::number(dify,0,4)+" rlineto "+QString::number(difx,0,4)+" neg "+QString::number(0,0,0)+" rlineto closepath stroke\n";
     }
     out << "/Palatino-Bold findfont [ .3 0 0 .3 0 0] makefont setfont\n";
     ysum = y + 0.06;
     out << QString::number(x,0,4)+" "+QString::number(ysum,0,4)+" moveto ("+userCall+") show\n";
     out << QString::number(x+3.8)+" "+QString::number(y+.06)+" moveto (to Radio :) show\n";
     out << "/Palatino-Bold findfont [.3 0 0 .4 0 0] makefont setfont\n";
     out << QString::number(x+5.5)+" "+QString::number(y+.06)+" moveto ("+q->Call+") show\n";
     out << QString::number(x+4.5)+" "+QString::number(y-0.35)+" moveto (via :      "+q->Qslmgr+") show\n";
     out << QString::number(x)+" "+QString::number(y-0.45)+" moveto .03 setlinewidth 8 0 rlineto stroke\n";
     out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
     out << QString::number(x)+" "+QString::number(y-0.91)+" moveto ("+k->Dear+" "+q->Name+",) show\n";
    //-
     out << QString::number(x)+" "+QString::number(y-1.25)+" moveto ("+k->Well+") show\n";
     out << ".01 setlinewidth\n";
     out << "newpath "+QString::number(lx+0.6)+" "+QString::number(y-3.45)+" moveto "+QString::number(8.0)+" "+QString::number(0,0,0)+" rlineto "+QString::number(0,0,0)+" "+QString::number(1.6)+" rlineto "+QString::number(8.0)+" neg "+QString::number(0,0,0)+" rlineto closepath stroke\n";
     n = atoi(q->Swl);
     QSqlQuery query;
     qy = "SELECT rufz,name,dxcc,day,btime,band,mode,freq,qinfo,";
     qy += "rsts,rig,txpwr,rxpwr ";
     qy += "FROM "+logfile+"om LEFT JOIN ("+logfile+"qsl"+","+logfile+") ON (omid=oid AND qsoid=id) ";
     qy += "WHERE id="+s.setNum(n);
       i = 0;
       query.exec(qy);
       query.next();
       strcpy(q->Call,query.value(i++).toString().toAscii());
       strcpy(q->Name,query.value(i++).toString().toAscii());
       strcpy(q->Dxcc,query.value(i++).toString().toAscii());
       strcpy(q->Day,query.value(i++).toString().toAscii());
       strcpy(q->Btim,query.value(i++).toString().toAscii());
       s = query.value(i++).toString();
       strcpy(q->Band,trans.getMyband(s).toAscii());         
       strcpy(q->Mod,query.value(i++).toString().toAscii());
       strcpy(q->Freq,query.value(i++).toString().toAscii());
       strcpy(q->Rsts,query.value(i++).toString().toAscii());
       strcpy(q->Rig,query.value(i++).toString().toAscii());
       strcpy(q->TxPwr,query.value(i++).toString().toAscii());
       strcpy(q->RxPwr,query.value(i++).toString().toAscii());
      out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
      out << QString::number(x)+" "+QString::number(y-1.64-.1)+" moveto (Mein QSO mit "+q->Name+" - "+q->Call+") show\n";
      
      out << QString::number(x+0.12)+" "+QString::number(y-2.3)+" moveto (Date    ) show\n";
      out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
      out << QString::number(x+1.2+0.12)+" "+QString::number(y-2.3)+" moveto ("+q->Day+") show\n";
      out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
      out << QString::number(x+3.0)+" "+QString::number(y-2.3)+" moveto (UTC ) show\n";
      out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
      out << QString::number(x+3.8)+" "+QString::number(y-2.3)+" moveto ("+q->Btim+") show\n";
      out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
      if(strlen(q->Freq))
        out << QString::number(x+5.1+0.12)+" "+QString::number(y-2.3)+" moveto ("+q->Band+" - "+q->Freq+" MHz) show\n";
      else
        out << QString::number(x+5.1+0.12)+" "+QString::number(y-2.3)+" moveto ("+q->Band+" Band) show\n";
      
      out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
      out << QString::number(x+0.12)+" "+QString::number(y-2.65)+" moveto (2-Way ) show\n";
      out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
      out << QString::number(x+1.2+0.12)+" "+QString::number(y-2.65)+" moveto ("+q->Mod+" ) show\n";
    
    out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
    out << QString::number(x+0.12)+" "+QString::number(y-2.98)+" moveto (Trans  ) show\n";
    out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
    out << QString::number(x+1.2+0.12)+" "+QString::number(y-2.98)+" moveto ("+r->Trans+"  -  "+q->TxPwr+" Watt) show\n";
    
    out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
    out << QString::number(x+0.12)+" "+QString::number(y-3.30)+" moveto (Ant.     ) show\n";
    out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
    out << QString::number(x+1.2+0.12)+" "+QString::number(y-3.30)+" moveto ("+r->Ant+") show\n";
    out << QString::number(x)+" "+QString::number(y-3.8)+" moveto ("+k->Cqz+") show\n";
    
    out << QString::number(x)+" "+QString::number(y-4.13)+" moveto ("+k->Text3+") show\n";
    out << "/Palatino-Roman findfont [.2 0 0 .2 0 0] makefont setfont\n";
    //---
    out << QString::number(x)+" "+QString::number(y-4.66)+" moveto .02 setlinewidth 8 0 rlineto stroke\n";
    
    out << "/Palatino-Roman findfont [.2 0 0 .2 0 0] makefont setfont\n";
    out << QString::number(x)+" "+QString::number(y-4.55)+" moveto (eMale  "+mail+"  -  "+www+") show\n";
    out << QString::number(x)+" "+QString::number(y-4.96)+" moveto (Printed "+datum+"  QtLog - QSO-Managment - System : "+r->Op+")  show\n";
  }
  pfile.close();
}


void PostFile::create_Card_SWL(QString fname, char *datum)
{
  float lx = 0;
  float ly = 0;
  float x = 0;
  float y = 0;
  QString s;
  int n, i;
   lx  = randx;
   ly = randy;
   x = lx +tposx;
   y = ly + hoehe -tposy;
    QFile pfile(fname);
    if(!pfile.open(QIODevice::Append | QIODevice::Text)) {
       qDebug() << "ERROR";
      return;
    }
    QTextStream out(&pfile);
   out << ".01 setlinewidth\n";
   
   
   
   out << "/Palatino-Roman findfont [.4 0 0 .4 0 0] makefont setfont\n";
   out << QString::number(x)+" "+QString::number(y)+" moveto ("+userCall+") show\n";
   s = q->Call;
   out << "/Palatino-Roman findfont [.4 0 0 .4 0 0] makefont setfont\n";
   out << QString::number(x+5.8)+" "+QString::number(y)+" moveto (to Radio   :) show\n";
   out << "/Palatino-Bold findfont [.4 0 0 .5 0 0] makefont setfont\n";
   out << QString::number(x+8.15)+" "+QString::number(y)+" moveto ("+s+") show\n";
   out << QString::number(x+6.6)+" "+QString::number(y -.6)+" moveto (via    :     "+q->Qslmgr+") show\n";
   out << QString::number(x)+" "+QString::number(y -.8)+" moveto .03 setlinewidth 12 0 rlineto stroke\n";
   
   out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
   out << QString::number(x)+" "+QString::number(y -1.5)+" moveto ("+k->Dear+"  "+q->Name+",) show\n";
   out << QString::number(x)+" "+QString::number(y -1.9)+" moveto ("+k->Well+") show\n";
   
   out << ".01 setlinewidth\n";
   out << "newpath "+QString::number(x)+" "+QString::number(y -5.4)+" moveto "+QString::number(12.0)+" 0 rlineto 0 "+QString::number(2.8)+" rlineto "+QString::number(12.0)+" neg 0 rlineto closepath stroke\n";
      n = atoi(q->Swl);
      QSqlQuery query;
      qy = "SELECT rufz,name,dxcc,day,btime,band,mode,freq,";
      qy += "rsts,rig,txpwr,rxpwr ";
      qy += "FROM ";
      qy += log;
      qy += "om LEFT JOIN ";
      qy += log;
      qy += " ON (omid=oid) WHERE id="+s.setNum(n);
      i = 0;
      query.exec(qy);
      query.next();
      strcpy(q->Call,query.value(i++).toString().toAscii());
      strcpy(q->Name,query.value(i++).toString().toAscii());
      strcpy(q->Dxcc,query.value(i++).toString().toAscii());
      strcpy(q->Day,query.value(i++).toString().toAscii());
      strcpy(q->Btim,query.value(i++).toString().toAscii());
      s = query.value(i++).toString();
      strcpy(q->Band,trans.getMyband(s).toAscii());         
      strcpy(q->Mod,query.value(i++).toString().toAscii());
      strcpy(q->Freq,query.value(i++).toString().toAscii());
      strcpy(q->Rsts,query.value(i++).toString().toAscii());
      strcpy(q->Rig,query.value(i++).toString().toAscii());
      strcpy(q->TxPwr,query.value(i++).toString().toAscii());
      strcpy(q->RxPwr,query.value(i++).toString().toAscii());
   out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
   out << QString::number(x)+" "+QString::number(y -2.4)+" moveto (Mein QSO mit "+q->Name+" - "+q->Call+") show\n";
   
   out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
   out << QString::number(x+.3)+" "+QString::number(y -3)+" moveto (DATE           : ) show\n";
   out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
   out << QString::number(x +2.3)+" "+QString::number(y -3)+" moveto ("+q->Day+") show\n";
   
   out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
   out << QString::number(x +4 +.4)+" "+QString::number(y -3)+" moveto (UTC  : ) show\n";
   out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
   out << QString::number(x +5.2+.4)+" "+QString::number(y -3)+" moveto ("+q->Btim+") show\n";
   
   out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
   out << QString::number(x +.3)+" "+QString::number(y -3.4)+" moveto (2-Way           : ) show\n";
   out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
   out << QString::number(x +2.3)+" "+QString::number(y -3.4)+" moveto ("+q->Mod+" ) show\n";
   out << QString::number(x+4+0.4)+" "+QString::number(y-3.4)+" moveto ("+q->Band+" "+q->Freq+" MHz) show\n";
  
  out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
  out << QString::number(x+.3)+" "+QString::number(y -4.0)+" moveto (Transceiver :) show\n";
  out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
  out << QString::number(x+2.3)+" "+QString::number(y-4.0)+" moveto ("+r->Trans+"   /   "+q->TxPwr+" Watt) show\n";
  
  out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
  out << QString::number(x+.3)+" "+QString::number(y-4.4)+" moveto (Antenne       :) show\n";
  out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
  out << QString::number(x+2.3)+" "+QString::number(y-4.4)+" moveto ("+r->Ant+") show\n";
  
  out << "/Palatino-Bold findfont [.3 0 0 .3 0 0] makefont setfont\n";
  out << QString::number(x +.3)+" "+QString::number(y -4.9)+" moveto ("+k->Cqz+") show\n";
  
  out << "/Palatino-Roman findfont [.3 0 0 .3 0 0] makefont setfont\n";
  out << QString::number(x)+" "+QString::number(y -6)+" moveto ("+k->Text3+") show\n";
  
  out << "/Palatino-Roman findfont [.2 0 0 .2 0 0] makefont setfont\n";
/*
  if(strlen(strcpy(buf,mail))) {
    strcpy(buf,"eMail - ");
    strcat(buf,mail);
    strcat(buf,"     -      ");
    strcat(buf,www);
    out << QStrng::number(x)+" "+QString::number(y -6.7)+" moveto ("+buf+" ) show\n";
  }
*/
  
  out << QString::number(x)+" "+QString::number(y-6.8)+" moveto .02 setlinewidth 12 0 rlineto stroke\n";
  
  out << QString::number(x)+" "+QString::number(y -7.1)+" moveto (Printed  "+datum+") show\n";
  out << "/Palatino-Bold findfont [.2 0 0 .2 0 0] makefont setfont\n";
  out << QString::number(x+2.6)+" "+QString::number(y -7.1)+" moveto (QtLog) show\n";
  out << "/Palatino-Roman findfont [.2 0 0 .2 0 0] makefont setfont\n";
  out << QString::number(x+3.28)+" "+QString::number(y-7.1)+" moveto (-  QSO Managmentsystem  :  "+r->Op+" ) show\n";
  pfile.close();
}
