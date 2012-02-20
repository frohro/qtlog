/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.01.2009 / 17.08.09
********************************************************************** */

#include <QSettings>
#include <QString>
#include <QTabWidget>
#include <QtGui>
#include <QtSql>
#include "query.h"
#include "../qtlogDiag/translate.h"
#include "dirmngr.h"
#include "quAdif.h"

query::query(QWidget * parent) : QDialog(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
  setupUi(this);
   n = settings.value("FontSize").toString().toInt();
   QFont font;
   font.setPointSize(n);                           
   setFont(font);
   connect(ButtonExit, SIGNAL(clicked()), this, SLOT(goExit()));
   connect(queryList, SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)), this, SLOT(queryListDoubleClicked(QTreeWidgetItem*,int)));
   connect(queryList, SIGNAL(itemClicked(QTreeWidgetItem *,int)), this, SLOT(queryListClicked(QTreeWidgetItem*,int)));
   connect(logList, SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)), this, SLOT(logListClickedCb(QTreeWidgetItem*,int)));
   connect(dataList, SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)), this, SLOT(dataListClickedCb(QTreeWidgetItem*,int)));
   connect(lineEditQname, SIGNAL(textEdited(QString)), this, SLOT(textEditedQnameCb(QString)));
   connect(ButtonSave, SIGNAL(clicked()), this, SLOT(buttonSaveCb()));
   connect(ButtonMkText, SIGNAL(clicked()), this, SLOT(buttonMkTextCb()));
   connect(ButtonMkAdif, SIGNAL(clicked()), this, SLOT(buttonMkAdifCb()));
   connect(ButtonBack, SIGNAL(clicked()), this, SLOT(buttonBackCb()));
   connect(ButtonClear, SIGNAL(clicked()), this, SLOT(buttonClearCb()));
   connect(ButtonCopy, SIGNAL(clicked()), this, SLOT(buttonCopyDataCb()));
   connect(ButtonHilfe, SIGNAL(clicked()), this, SLOT(showHilfeCb()));
   connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabCurrentChangedCb(int)));
   queryList->setColumnWidth(0,80);                  
   queryList->setColumnWidth(1,65);                  
   queryList->setColumnWidth(2,35);                  
   queryList->setColumnWidth(3,1200);                
   queryList->setColumnWidth(4,40);                  
   logList->setColumnWidth(0,30);                    
   logList->setColumnWidth(1,90);                    
   dataList->setColumnWidth(0,80);                   
   logFile = settings.value("logFile").toString();
   transl.installHashTable();                        //-Hash_tabellen zur NamenConvertierung anlegen
   transl.installHashTableBand();                    
   QSqlQuery query; 
   qy = "SELECT refnam FROM refnamen WHERE type='2'  OR type='3' ORDER BY idn";
   query.exec(qy);
   while(query.next()) {                             
      i = 0;
      n = 0;
      s = query.value(n).toString();                 
      if(s.compare("Cept") != 0) {
        QTreeWidgetItem *item = new QTreeWidgetItem(logList);
        item->setText(i++,"");                       
        item->setText(i++,s);                        
      }
   }
   qy = "SELECT * FROM wquery";
   query.exec(qy);
   n = 0;
   while(query.next()) {                                      
      i = 0;
      n = 0;
      QTreeWidgetItem *item = new QTreeWidgetItem(queryList);
      item->setText(i++,query.value(n++).toString());         
      item->setText(i,query.value(n++).toString());           
      item->setTextAlignment(i++,Qt::AlignCenter);
      item->setText(i,query.value(n++).toString());           
      item->setTextAlignment(i++,Qt::AlignCenter);
      item->setText(i++,query.value(n++).toString());         
      item->setText(i++,"");                                  
   }
   
   AWD = settings.value("Val").toString();                    
   showTable = 0;
   upItem = NULL;
   cntdpos = 0;
}

query::~query()
{
}

void query::goExit() 
{
  accept();                              
}


void query::buttonBackCb()
{
   tabWidget->setCurrentIndex(0);
}

void query::showHilfeCb()
{
   settings.setValue("Val","DataExport");
   StartProcess("hilfedb &");
}


//------------------------------------------------------
void query::tabCurrentChangedCb(int index)
{
   QString range, token, t;
   int len, idx, pos;
   if(index == 1) {                                    
     
     s = lineEditQuery->text();
     cnt = s.count();  
     
     if(!cnt) {
       QMessageBox::information(this,tr(" Info"),
         tr("Es ist praktischer  ein vorhandenes query zu modefizieren und unter einem neuen Namen zu speichern."),
       QMessageBox::Ok); 
     }
     
     pos = 0;
     if(cnt != 0) {                                   
       buttonClearCb();
       i = s.indexOf("range");                        
       token = s.left(i);
       token = token.simplified();                    
       range = s.mid(i);                              
       range = range.simplified();
       len = token.count();                           
      
       pos = 1;
       idx = 0;
       while( 1 ) {                                  
        while(len != idx +1) {                       
          t = "";
          while((token[idx] != QChar(',')) && (idx != len)) {  
               t += token[idx]; 
               idx++;                               
          }
          t = t.simplified();
          insertTok(t,pos);                         
          pos++;
          if(idx  == len) break;                    
          idx++;                                    
        }                                           
        if(idx  == len) break;
      }                                             
      cntdpos = pos;
                                                    
      s = lineEditTrenner->text();                  
      s = s.simplified();  
      if(s.compare(";") == 0)
         comboBoxSep->setCurrentIndex(0);
      else
      if(s.compare(",") == 0)
          comboBoxSep->setCurrentIndex(1);
      else
      if(s.compare("+") == 0)
         comboBoxSep->setCurrentIndex(2);
      else
      if(s.compare(":") == 0) 
         comboBoxSep->setCurrentIndex(3);
      
      
      s = lineEditEor->text();                     
      s = s.simplified();  
      if(s.compare("L") == 0)
         comboBoxEor->setCurrentIndex (0);
      else
       if(s.compare("W") == 0)
          comboBoxEor->setCurrentIndex (1);
     
      len = range.count();
      i = range.indexOf("range");                
      range = range.mid(i +5,len);               
      idx = 0;                                   
      while(range[idx] != QChar('<')) range[idx++];         
      idx++;                                                
      whv = "";
      while(range[idx] != QChar('>')) whv += range[idx++];  
      idx++;
      whb = "";
      while(range[idx] != QChar('<')) range[idx++];         
      idx++;
      while(range[idx] != QChar('>')) whb += range[idx++];  
      
      
      QDate d = QDate::fromString (whv,"yyyy-MM-dd");
      dateEditFrom->setDate(d);                           
      d = QDate::fromString (whb,"yyyy-MM-dd");
      dateEditTo->setDate(d);                             
      
      labelQuery->setText(lineEditQname->text());
    }
  }
}


void query::insertTok(QString tok, int pos)
{
  int count;
      count = logList->topLevelItemCount();          
      lItem = logList->currentItem();                
      i = 0;
      while(i != count ) {                           
        lItem = logList->topLevelItem (i);
        if(lItem->text(1).compare(tok) == 0)
           break;                                    
        i++;
      }
      s = s.setNum(pos);                             
      lItem->setText(0,s);
      QTreeWidgetItem *itemN = new QTreeWidgetItem(dataList);
      itemN->setText(0,lItem->text(1));              
}


void query::buttonClearCb()
{
     dataList->clear();                         
     i = logList->topLevelItemCount();          
     lItem = logList->currentItem();            
     n =  logList->indexOfTopLevelItem(lItem);  
     n = 0;
     while( n != i ) {
       lItem = logList->topLevelItem (n);
       lItem->setText(0,"");
       n++;
     }
     cntdpos = 0;
}


void query:: buttonCopyDataCb()
{
     //Muster: Datum,Start, Ende, Call, Mode, RSTg, RSTe, AWD, Locator range <2007-11-01> to <2008-01-30> ORDER BY Dxcc
     QString q;
     i = dataList->topLevelItemCount();          
     lItem = dataList->currentItem();            
     n =  dataList->indexOfTopLevelItem(lItem);  
     n = 0;
     q = "";
     while( n != i ) {
       lItem = dataList->topLevelItem (n);
       q += lItem->text(0);
       n++;
       if(n != i) 
          q += ",";
     }
     
     QDate d = dateEditFrom->date();               
     whv = t.setNum(d.year());
     whv += "-";
     t.setNum(d.month());
     if(t.count() == 1)
       whv += "0";
     whv += t+"-";  
     t.setNum(d.day());
     if(t.count() == 1)
        whv += "0";
     whv += t;
     
     d = dateEditTo->date();                       
     whb = t.setNum(d.year());
     whb += "-";
     t.setNum(d.month());
     if(t.count() == 1)
       whb += "0";
     whb += t+"-";  
     t.setNum(d.day());
     if(t.count() == 1)
        whb += "0";
     whb += t;
 
     t = comboBoxSep->currentText();
     t = t.simplified(); 
     lineEditTrenner->setText(t);
     
     s = comboBoxEor->currentText();
     s = s.simplified(); 
     t = s.left(1);
     lineEditEor->setText(t);
     
     q += " range <"+whv+"> to <"+whb+"> ORDER BY "+comboBoxOrder->currentText();
     lineEditQuery->setText(q);
     tabWidget->setCurrentIndex (0);
}

void query::queryListDoubleClicked(QTreeWidgetItem *item, int col)
{
    int x;
    upItem = item;
    x = col;
    lineEditQname->setText(item->text(0));
    lineEditTrenner->setText(item->text(1));
    lineEditEor->setText(item->text(2));
    lineEditQuery->setText(item->text(3));
    lineEditOutfile->setText(item->text(0));
    tabWidget->setCurrentIndex(1);
}

void query::queryListClicked(QTreeWidgetItem *item, int col)
{
    int x;
    upItem = item;
    x = col;
    lineEditQname->setText(item->text(0));
    lineEditTrenner->setText(item->text(1));
    lineEditEor->setText(item->text(2));
    lineEditQuery->setText(item->text(3));
    lineEditOutfile->setText(item->text(0));
}

void query::textEditedQnameCb(QString str)
{
   lineEditQname->setText(str.toUpper());
}


void query::logListClickedCb(QTreeWidgetItem *item,int col)
{
    int x;
    x = col;
    if(item->text(0).count() == 0) {
      item->setText(0,s.setNum(cntdpos));
      QTreeWidgetItem *itemnew = new QTreeWidgetItem(dataList);
      itemnew->setText(0,item->text(1));
      cntdpos++;
    }
    else {                                         
       item->setText(0,"");                        
       i = dataList->topLevelItemCount();          
       lItem = dataList->currentItem();            
       n =  dataList->indexOfTopLevelItem(lItem);  
       n = 0;
       while( n != i ) {
         lItem = dataList->topLevelItem (n);
         s = lItem->text(0);
         x = s.compare(item->text(1));             
         if( !x ) {
           dataList->takeTopLevelItem(n);          
           break;
         }
         n++;
       }
       if( x ) {
         qDebug() << "Grober FEHLER";
        return;
       }
    }
}


void query::dataListClickedCb(QTreeWidgetItem *item, int col)
{
     int x;
     x = col;
     i = logList->topLevelItemCount();          
     lItem = logList->currentItem();
     n = 0;
     while( n != i ) {
       lItem = logList->topLevelItem (n);
       s = lItem->text(1);
       x = s.compare(item->text(0));            
         if( !x ) {
           lItem->setText(0,"");
           break;
         }
         n++;
       }
       n =  dataList->indexOfTopLevelItem(item);  
       dataList->takeTopLevelItem(n);             
}
//-------------------------------------------------------------------------------


void query::buttonSaveCb()
{
     QSqlQuery query;              
     qy = "SELECT name FROM wquery WHERE name='"+lineEditQname->text()+"'";
     query.exec(qy);
     if(query.size() == 1) {                               
       qy = "UPDATE wquery SET sep='"+lineEditTrenner->text();
       qy += "',eor='"+lineEditEor->text();
       qy += "',query='"+lineEditQuery->text();  
       qy += "' WHERE name='"+lineEditQname->text()+"'";
       query.exec(qy);
       i = 1;
       upItem->setText(i++,lineEditTrenner->text());          
       upItem->setText(i++,lineEditEor->text());              
       upItem->setText(i++,lineEditQuery->text());            
     }
     else {                                                   
       if(lineEditQuery->text().count() == 0) {               
          QMessageBox::information(this,tr(" Info"),tr("Kein Query konfiguriert"),
          QMessageBox::Ok); 
         return;
       }
       
       if(lineEditQname->text().count() == 0) {
          QMessageBox::information(this,tr(" Info"),tr("Query Name fehlt"),
          QMessageBox::Ok); 
         return;
       }
       
       qy = "INSERT INTO wquery VALUES ('"+lineEditQname->text();
       qy += "','"+lineEditTrenner->text();
       qy += "','"+lineEditEor->text();
       qy += "','"+lineEditQuery->text();
       qy += "')";
       query.exec(qy);
       i = 0;
       QTreeWidgetItem *item = new QTreeWidgetItem(queryList);
       item->setText(i++,lineEditQname->text());         
       item->setText(i,lineEditTrenner->text());         
       item->setTextAlignment(i++,Qt::AlignCenter);
       item->setText(i,lineEditEor->text());             
       item->setTextAlignment(i++,Qt::AlignCenter);
       item->setText(i++,lineEditQuery->text());         
       item->setText(i++,"");                            
     }
    
    
    
    
}



void query::buttonMkTextCb()
{
     err = 0;
     createQuery();                           
     if(err) 
        return;                               
     err = 0;
     if(lineEditOutfile->text().count() == 0) {
        int status = QMessageBox::information(this,tr("Parameter Check"),
              tr("Dateiname fehlt.\nAbfrage Ergebnis im Report-Dialog anzeigen ?"),
        QMessageBox::Yes,QMessageBox::No); 
       if(status != QMessageBox::Yes)
        return;
       else {
          showTable = 1;                       
          goExit();
       }
     }
     else {                                    
       QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
       mkText();                               
       QApplication::restoreOverrideCursor();
       s = "\n"+t.setNum(cnt)+"  Datensaetze - erzeugt\nDatei:  ";
       s +=  QDir::homePath();                     
       s += "/";
       s += lineEditOutfile->text()+".csv  \n\n";
       s += "Zurueck zum ReportDialog";
       int status = QMessageBox::information(this,tr("Ergebnis Check"),(s),
                    QMessageBox::Yes,QMessageBox::No); 
       if(status == QMessageBox::Yes)
         goExit();
     }
}



void query::buttonMkAdifCb()
{
     err = 0;
     createQuery();                           
     if(err) 
        return;                               
     err = 0;
     if(lineEditOutfile->text().count() == 0) {
        int status = QMessageBox::information(this,tr("Parameter Check"),
              tr("      Dateiname fehlt.\n\nAbfrage Ergebnis im ReportDialog anzeigen ?"),
        QMessageBox::Yes,QMessageBox::No); 
       if(status != QMessageBox::Yes)
        return;
       else {
          showTable = 1;                      
          goExit();
       }
     }
     else {                                   
       QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
       mkAdif();                              
       QApplication::restoreOverrideCursor();
       s = "\n"+t.setNum(cnt)+"   Datensaetze erzeugt\nDatei:  ";
       s +=  QDir::homePath();                     
       s += "/";
       s += lineEditOutfile->text()+".adi  \n\n";
       s += "Zurueck zum ReportDialog";
       int status = QMessageBox::information(this,tr("Ergebnis Check"),(s),
                    QMessageBox::Yes,QMessageBox::No); 
       if(status == QMessageBox::Yes)
         goExit();
     }
}


void query::createQuery()
{
    QString qtext, sep, s, q, qday;
    int err, cnt, len;
    qtext = lineEditQuery->text();
    if(!qtext.count()) {
       QMessageBox::information(this,tr("Parameter Check"),
         tr("Die Queryzeile enthaelt einen Fehler"),
       QMessageBox::Ok); 
       err = 1;
       return;
    }
    if((lineEditTrenner->text().count() == 0) || (lineEditEor->text().count() == 0)) {
      QMessageBox::information(this,tr("Parameter Check"),
         tr("Es gibt einen Fehler bei der Trennerangabe"),
      QMessageBox::Ok); 
      err = 1;
      return;
    }
    sep = lineEditTrenner->text();
    sep = sep.simplified();                   
    idx = 0;
    cnt = 0;
    len = qtext.count();
    
    
    
    while(idx !=  len)                        
       if(qtext[idx++] == QChar('<') ) cnt++;
    idx = 0;
    while(idx !=  len)                        
       if(qtext[idx++] == QChar('>') ) cnt++;
    if(cnt != 4) {
       QMessageBox::information(this,tr("Syntax Fehler"),tr("Im Datum Format fehlen '<' oder '>'"),
       QMessageBox::Ok); 
       err = 1;
     return;
    }
    idx = 0;
    cnt = 0;
    while(idx !=  len) 
       if(qtext[idx++] == QChar('-') ) cnt++;
    if(cnt != 4) {                             
      QMessageBox::information(this,tr("Syntax Fehler"),tr("Im Datum Format fehlen  '-'"),
       QMessageBox::Ok); 
       err = 1;
     return;
    }
    
    idx = qtext.indexOf("range");             
    if(idx == -1) {                           
       QMessageBox::information(this,tr("Syntax Fehler"),tr("' range '  FEHLT !"),
       QMessageBox::Ok); 
       err = 1;
     return;
    }
    q = qtext.left(idx);                      
    qday = qtext.mid(idx +5,len);             
    qday = qday.simplified();                 
    q = q.simplified();                       
    
    qy = "SELECT cept,";                      
    t = "";
    n = 0;
    err = 0;
    len = q.count();
    idx = 0;
    while( 1 ) {                              
         while(len != idx +1) {               
           t = "";
           while((q[idx] != QChar(',')) && (idx != len)) {  
               t += q[idx]; 
               if(t.count() > 10) {
                 f = tr("Hier fehlt ein Komma ! ")+t;
                 QMessageBox::information(this,tr("Syntax Error"),(f),
                 QMessageBox::Ok);
                 err = 1;
                 return;
               }
               idx++;                                
           }
       
           t = t.simplified();                       
           f = t;                                    
           if(idx  == len) break;                    
           t = transl.getDbField(t);                 
           if(t.compare("?") == 0) {
              t = f+" ?";                            
              err = 1;
             break;                                  
           }
           idx++;                                    
           if(idx  == len) break;                    
           qy += t+",";                              
         }
         t = transl.getDbField(t);                   
         if(t.compare("?") == 0) {
            t = f+" ?";                              
            err = 2;
           break;                                    
         }
         qy += t;                                    
         if(idx  == len) break;
         if(err) break;
    }                                               
    if(err) {
      f = tr("Feldnamen Error : ")+t;
      QMessageBox::information(this,tr("Syntax Fehler"),(f),
        QMessageBox::Ok);
        err = 1;
      return;
    }

    f = qy;
    idx = f.indexOf("AWD");               
    n = idx;
    awdPos = idx;                         
    if( idx != -1 ) {                     
      i = 0;                              
      cnt = 0;
      while(i !=  n)
       if(f[i++] == QChar(',') ) cnt++;   
      awdPos = cnt +1;                    
      qy = f.left(idx);                   
      if( idx != -1 ) {                   
        s = "(SELECT awkenner FROM "+logFile+"awd WHERE awtype='";
        s += settings.value("Val").toString();
        s += "' AND id=qid)";
        qy += s;
        s = f.mid(idx +3);                
        qy += s;
      }
    }
    
    qy += " FROM "+logFile+"om LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id)";
    qy += " WHERE ";                      
    idx = 0;                              
    while(qday[idx] != QChar('<')) qday[idx++];         
    idx++;                                              
    whv = "";
    while(qday[idx] != QChar('>')) whv += qday[idx++];  
    idx++;
    whb = "";
    while(qday[idx] != QChar('<')) qday[idx++];         
    idx++;
    while(qday[idx] != QChar('>')) whb += qday[idx++];  
    qy += " day BETWEEN '"; 
    qy += whv;
    qy += "' AND '";
    qy += whb;
    qy += "'";
    idx = qday.indexOf("ORDER BY");                    
    if(idx != -1) {                                    
      qy += " ORDER BY ";
      s = qday.mid(idx +8);  
      s = s.simplified(); 
      f = s;
      s = transl.getDbField(s);                        //
         if(s.compare("?") == 0) {
            t = f+" ?";                                
            f = tr("Feldnamen Error : ")+t;
            QMessageBox::information(this,tr("Syntax Fehler"),(f),
            QMessageBox::Ok);
            err = 1;
          return;
         }
      qy += s;
    }
}




void query::mkText()
{
    QString p, hfeld,rip;
    QString Cept, Call, val, id;
    int col, idx;
    cnt = 0;
    path =  QDir::homePath();                     
    path += "/";                                      
    p = path+lineEditOutfile->text()+".csv";          

    QFile file(p);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Dein Linux ist kaputt";
       return;
    }
    QTextStream out(&file);
    rip = lineEditTrenner->text();
    rip = rip.simplified();
    rip += " ";
    
    QSqlQuery query; 
    query.exec(qy);                                  
    n = query.size();                                
    col = query.record().count();                    
    while(query.next() ) {                           
      idx = 0;
      p = "";
      Call = "";
      Cept = "";
      while( idx != col ) {                          
        n = 0;                                       
        hfeld = query.record().fieldName(idx);       
        val = query.value(idx).toString();           
        if(hfeld.compare("id") == 0) {               
           id = val;
        }
       else 
        if(hfeld.compare("cept") == 0) { 
           Cept = val;                               
        }
       else
        if(hfeld.compare("rufz") == 0) {             
          Call = val;
          if(Cept.count() != 0) {                    
              val = Cept+Call;                       
              p += val+rip;
           }
           else 
             p += Call+rip;
        }
       else
        if(hfeld.compare("band") == 0) {
           val = transl.getMyband(val);              
           p += val+rip;
        }
       else
        if(hfeld.compare("day") == 0) {              
          if(comboBoxDform->currentIndex() == 0)     
            p += val+rip;
          else {                                     
             val = val.remove(QChar('-'), Qt::CaseInsensitive);
             p += val+rip;
          }
       }
       else
        if(hfeld.compare("btime") == 0) {            
          if(comboBoxTform->currentIndex() == 1) {   
             val = val.remove(QChar(':'), Qt::CaseInsensitive);
          }
          else
           if(comboBoxTform->currentIndex() == 2) {  
             val = val.remove(QChar(':'), Qt::CaseInsensitive);
             val = val.left(4);
           }
          p += val+=rip;
        }
       else 
        if(hfeld.compare("etime") == 0) {            
          if(comboBoxTform->currentIndex() == 1) {   
             val = val.remove(QChar(':'), Qt::CaseInsensitive);
          }
          else
           if(comboBoxTform->currentIndex() == 2) {  
             val = val.remove(QChar(':'), Qt::CaseInsensitive);
             val = val.left(4);
           }
           p += val+rip;
        }
       else {                                       
          p += val;
        if( idx +1  == col )
            ;
        else
          p += rip;
       }
       idx++;                                      
      }
      out << p << "\n";                            
      cnt++;
    }
    file.close();
}



void query::mkAdif()
{
    QString fp;
    path =  QDir::homePath();                     
    path += "/";
    fp = path+lineEditOutfile->text()+".adi";         
   
    quadi adi;
    adi.createAdifTableHeader();
    adi.setBandTable();
   
    QSqlQuery query; 
    s = "SELECT atype,adiftype FROM wawdlist WHERE aset != ''";
    query.exec(s);
    while(query.next()) { 
       n = 0;
       t = query.value(n++).toString();         
       s = query.value(n++).toString();         
       adi.loadAdifAwdTable(t,s);               
    }
    
    adi.setDayForm(comboBoxDform->currentIndex());
    adi.setTimeForm(comboBoxTform->currentIndex());
    
    s = qy;
    i = s.indexOf("SELECT");
    qy = "SELECT id,"+s.mid(i +6);
    adi.doExport( logFile, qy, fp,AWD, 0, 0 );
   
    cnt = adi.getCount();
    adi.clearExpHash();
}
