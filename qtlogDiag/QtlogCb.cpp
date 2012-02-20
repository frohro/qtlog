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
#include <QSettings>
#include <QtSql>
#include <QMessageBox>
#include "Qtlog.h"
#include "confDiag.h"
#include "version.h"






void QtLog::buildQuery()
{
   i = qystr.indexOf("AWD");             
   qy = qystr.left(i);                   
   if( i != -1 ) {                       
      s = "(SELECT awkenner FROM "+logFile+"awd WHERE awtype='"+BoxAwd->currentText()+"' AND id=qid)";
      qy += s;
      s = qystr.mid(i +3);               
      qy += s;
   }
   qy += " FROM "+logFile+"om LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id)";
   qy += " WHERE ";
   s = transl.getDbField(BoxGroup->currentText());
   if(s.compare("band") == 0) {
     p = "SELECT mband FROM wband WHERE mband='"+groupEditLine->text()+"'"; 
     QSqlQuery query;
     query.exec(p);
     if(!query.size()) {                                                    
        QString f = "\nBandnamen immer kompl. eingeben";
        QMessageBox::information( this,
        tr("Band Fehler"),f,
        QMessageBox::Ok | QMessageBox::Default,
        QMessageBox::NoButton, QMessageBox::NoButton);
        groupEditLine->setFocus(); 
        return;
     }
     query.next();
     qy += "band ='"+transl.getSysband(query.value(0).toString());
     qy += "'";                          
   }
   else
     qy += s+" LIKE '"+groupEditLine->text()+"%'";
     if(checkBoxDateEpoch->isChecked() == true) {        
       qy += " AND day BETWEEN "; 
       QDate d = dateEditBis->date();                    
       qy += "'"+s.setNum(d.year());
       qy += "-"+s.setNum(d.month());
       qy += "-"+s.setNum(d.day());
       qy += "' AND ";
       d = dateEditVon->date();                          
       qy += "'"+s.setNum(d.year());
       qy += "-"+s.setNum(d.month());
       qy += "-"+s.setNum(d.day());
       qy += "'";
     }
     else {                                                
       if(groupEditLine->text().count() == 0) {            
	 n = BoxLimit->currentText().toInt();              
	 if( n != 0) {
           qy += " AND day BETWEEN ";                      
           QDate d = QDate::currentDate();                 
           d = d.addMonths(-n);                            
           qy += "'"+s.setNum(d.year());
           qy += "-"+s.setNum(d.month());
           qy += "-"+s.setNum(d.day());
           qy += "' AND ";
           d = dateEditVon->date();                        
           qy += "'"+s.setNum(d.year());
           qy += "-"+s.setNum(d.month());
           qy += "-"+s.setNum(d.day());
           qy += "'";
	 }
       }
     }
     qy += " ORDER BY ";
     s = transl.getDbField(BoxSort->currentText());
     if(s.indexOf("day") != -1) {
        qy += "day DESC,btime DESC";
     }
     else
       qy += transl.getDbField(BoxSort->currentText())+" DESC";
     qDebug() << "qy is:"<< qy <<"\n";
     readData(qy,awdPos);
     groupEditLine->setFocus();
}


//------------------------------------------------------------------------
void QtLog::awdQuery()
{
  if( awdPos != -1 ) {
    i = qystr.indexOf("AWD");              
    qy = qystr.left(i);                    
    qy += "awkenner";
    qy += qystr.mid(i+3);
    qy += " FROM ("+logFile+"om,"+logFile+"awd)";
    qy += " LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id)";
    qy += " WHERE (awtype='"+BoxAwd->currentText();
    qy += "' AND id=qid) ORDER BY "+transl.getDbField(BoxSort->currentText());
    qy += " DESC";
    readData(qy,awdPos);
    awdEditLine->setFocus();
  }
  else {
    QMessageBox::information( this,
    tr("   INFO    "),tr("\nAWD-Auswertung ist im Layout nicht selektiert"),
    QMessageBox::Ok | QMessageBox::Default,
    QMessageBox::NoButton, QMessageBox::NoButton);
  }
}


//------------------------------------------------------------------------
void QtLog::awdQuerykenner()
{
  if( awdPos != -1 ) {
    i = qystr.indexOf("AWD");              
    qy = qystr.left(i);                    
    qy += "awkenner";
    qy += qystr.mid(i+3);
    qy += " FROM ("+logFile+"om,"+logFile+"awd)";
    qy += " LEFT JOIN ("+logFile+"qsl,"+logFile+")";
    qy += " ON (omid=oid AND qsoid=id)";
    qy += " WHERE awtype='"+BoxAwd->currentText();
    qy += "' AND awkenner='"+awdEditLine->text();
    qy += "' AND id=qid ORDER BY "+transl.getDbField(BoxSort->currentText());
    qy += " DESC";
    readData(qy,awdPos);
    awdEditLine->setFocus();
  }
  else {
     QMessageBox::information( this,
     tr("   INFO    "),tr("\nAWD-Auswertung ist nicht selektiert"),
     QMessageBox::Ok | QMessageBox::Default,
     QMessageBox::NoButton, QMessageBox::NoButton);
  }
}



void QtLog::textExportData(QString q, int awdp)
{
   QString h;
   s = q;
   i = s.indexOf("SELECT");
   h = "SELECT id,"+s.mid(i +6);
   readData(h,awdp);
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void QtLog::readData(QString q, int awdpos)
{
      QString b;
      bflg = -1;
      QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
      reportTable->clear();                                
      QSqlQuery query; 
      query.exec(q);                                       
      //qDebug() << query.lastError();
      i = query.size();                                    
      col = query.record().count();                        
      reportTable->setColumnCount(col -1);                 
      reportTable->setRowCount(i);                         
      n = 0;                                               
      s = query.record().fieldName(n++);                   
      //Pos '0' im Horizontal_header ist das Datum ( nicht die linke obere Ecke ! )
      reportTable->setColumnWidth(0,80);                   
      while(n  != col) {                                   
         s = query.record().fieldName(n);                  
         QTableWidgetItem * cItem = new QTableWidgetItem(tr("%1").arg((n)));
         if( !(QString::compare(s,"band"))) bflg = n -1;   
         if(n == awdpos) {                                 
             cItem->setText(BoxAwd->currentText());        
             b = "AWD";
         }
         else {
           b = transl.getUsrField(s);                            
           cItem->setText(b);                                    
         }
         
         reportTable->setHorizontalHeaderItem(n -1,cItem);       
         reportTable->setColumnWidth(n -1,transl.getFieldBr(b)); 
         n++;                                                    
      }
    
      row = 0;                                                   
      while(query.next()) {                                      
        z = 0;                                                   
        r = 0;                                                   
        i = 0;                                                   
        s = query.value(i++).toString();                         
        QTableWidgetItem *rowItem = new QTableWidgetItem(tr("%1").arg((r++)*(z++))); //Id_item erzeugen
        rowItem->setText(s);                                     
        reportTable->setVerticalHeaderItem(row,rowItem);         
        c = 0;
        while( c != col -1 ) {                                   
          QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg((row)*(c))); 
          if( c == bflg ) {                                      
             newItem->setText(transl.getMyband(query.value(i++).toString()));  
          }
          else
            newItem->setText(query.value(i++).toString());       
          reportTable->setItem(row,c++,newItem);                 
        }
        row++;                                                   
     }
     statusBar()->showMessage(tr("Fertig"));
     s = "Anzeige : "+s.setNum(row)+" QSO's   -  "+p.setNum(col)+"  Spalten    ";
     mStatLabel->setText(s); 
     groupEditLine->setFocus();
     QApplication::restoreOverrideCursor();
}



void QtLog::CheckDBconnection()
{
   s = getenv("HOME");                         
   s += "/.qtlog/qlogfile";
   QFile lfile(s);
   if(! lfile.open(QIODevice::Append | QIODevice::Text)) {
         qDebug() << "FEHLER - qlogfile nicht vorhanden !! hier stimmt nichts";
       return;
   }
   QTextStream out(&lfile);
   if(! dbstat ) {                             
       QDateTime heute = QDateTime::currentDateTime();
       out << "\nqtlog - DB Info - "+heute.toString (Qt::TextDate)+"\n";
       out << "settings: \n";
       out << "dbname = "+settings.value("dbname").toString()+"\n";
       out << "dbpasswd = "+settings.value("dbpasswd").toString()+"\n";
       out << "dbuser = "+settings.value("dbuser").toString()+"\n";
       out << "host = "+settings.value("host").toString()+"\n";
       out << "port = "+settings.value("port").toString()+"\n";
       out << "mySQLPath = "+settings.value("mySQLPath").toString()+"\n";
       out << "Usermeldung: ";
       s = tr("QtLog System Info ");
       p = tr("\nDas Datenbank runtime_Flag steht auf: 0 \nQtLog wurde noch nicht Initialisiert !!\nOder dieses Flag wurde beschaedigt.\n\nIn diesem Fall ist ein groesserer Schaden entstanden.\nSchaue in '.config/Qtlog/qtlog.ini'.\nLese mit einem Browser im doc_Ordner INSTALL.\n\nWird keine Abweichung gefunden, sende mir eine eMail mit dem '.qtlog/qlogfile' an diek@dl1hbd.de.\n");
       out << p;
       QMessageBox::information( this,s,p,
             QMessageBox::Ok | QMessageBox::Default,
             QMessageBox::NoButton, QMessageBox::NoButton);
       lfile.close();
      exit(0);
  }
  else {
    db.setHostName(settings.value("host").toString());           
    db.setDatabaseName(settings.value("dbname").toString());
    db.setUserName(settings.value("dbuser").toString());
    db.setPassword(settings.value("dbpasswd").toString());
  }
  if( !db.open() ) {
     err = db.lastError ();
     QDateTime heute = QDateTime::currentDateTime();
     out << "\n\nqtlog - DB Info - "+heute.toString (Qt::TextDate)+"\n";
     out << "Server: "+ err.databaseText()+"\n";
     out << "settings :\n";
     out << "dbpasswd = "+settings.value("dbpasswd").toString()+"\n";
     out << "dbuser = "+settings.value("dbuser").toString()+"\n";
     out << "host = "+settings.value("host").toString()+"\n";
     out << tr("Meldung an User:");
     s = tr("\nFehlermeldung vom MySQL-Server !\nDie Zugangsberechtigung ist beschaedigt\nBetrieb ist nicht moeglich. Pruefe:\n");
     s += tr("UserName = ")+ settings.value("dbuser").toString();
     s += tr("\nPasswort = ") + settings.value("dbpasswd").toString();
     s += tr("\ndbname = ")+settings.value("dbname").toString();
     s += "\n";
     out << s;
     QMessageBox::information( this,
         tr("Server-Datenbank INFO"),
         tr(s.toAscii()),
         QMessageBox::Ok | QMessageBox::Default,
         QMessageBox::NoButton, QMessageBox::NoButton);
     lfile.close();
     exit(0);                                 
  }
   
   QDateTime heute = QDateTime::currentDateTime();
   QSqlQuery query;                           
   qy = "SELECT relnr FROM dblogs";
   query.exec(qy);
   query.next();
   s = query.value(0).toString();
   if(s.compare(RELEASE) != 0) {              
     s = RELEASE;
     out << tr("Rel.Nr : ")+s+"\n";
     out << tr("\nqtlog - Versions Info - ")+heute.toString (Qt::TextDate)+"\n";
     s = tr("\nDie DB_Release_Nr ist ungueltig.\n\nEs muss mit 'setupqtlog' eine Datenbank-Korrektur durchgefuehrt werden.\nInformiere dich auf www.dl1hbd.de, oder sende eine Anfrage. Betrieb ist nicht moeglich !");
     out << s;
     QMessageBox::information( this,
         tr("\nDatenbank - Info\n"),
         tr(s.toAscii()),
         QMessageBox::Ok | QMessageBox::Default,
         QMessageBox::NoButton, QMessageBox::NoButton);
     lfile.close();
     exit(0);                                 
   }
   out << "Start : qtlog - "+heute.toString (Qt::TextDate)+"\n";
   out << VERSION << "Rel" << RELEASE"\n";
   s = "  "+settings.value("dbname").toString()+" ]";
   labelDatabase->setText(s);
   lfile.close();
}
