/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007, 06.06.09
********************************************************************** */

#include <QApplication>
#include <QtGui>
#include <QtSql>
#include <QStackedWidget>
#include <QtNetwork>
#include "Qtlog.h"
#include "confDiag.h"
#include "reportLayout.h"
#include "confMode.h"
#include "auth.h"
#include "mNote.h"
#include "version.h"
#include "dirmngr.h"
#include "query.h"
#include "rigctl.h"
#define SHOWAWD_TIME 30

QtLog::QtLog(QWidget * parent) : QMainWindow(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
  setupUi(this);
  n = settings.value("FontSize").toString().toInt();
  QFont font;
  font.setPointSize(n);                              
  setFont(font);
  
  
  
   connect(actionENDE, SIGNAL(triggered(bool)), this, SLOT(goExit())); 

   connect(actionDxStatusSSB, SIGNAL(triggered(bool)), this, SLOT(awdDxStatusSsbAc()));
   connect(actionDxStatusCW, SIGNAL(triggered(bool)), this, SLOT(awdDxStatusCwAc()));
   connect(actionJahresMODE, SIGNAL(triggered(bool)), this, SLOT(awdJahresModeAc()));
   connect(actionJahresBAND, SIGNAL(triggered(bool)), this, SLOT(awdJahresBandAc()));
   connect(actionDLDstatus, SIGNAL(triggered(bool)), this, SLOT(awdDLDstatusAc())); 
   connect(actionIOTAstatus, SIGNAL(triggered(bool)), this, SLOT(awdIOTAstatusAc()));

   connect(actionAddQso, SIGNAL(triggered(bool)), this, SLOT(addLog()));
   connect(actionAddClog, SIGNAL(triggered(bool)), this, SLOT(addClog()));
   connect(actionLogChange, SIGNAL(triggered(bool)), this, SLOT(logconfAc()));
   connect(actionFldigi_logging, SIGNAL(triggered(bool)), this, SLOT(addDlog()));
   

   connect(actionQslEingang, SIGNAL(triggered(bool)), this, SLOT(qslEingangAc()));
   connect(actionQslDruck, SIGNAL(triggered(bool)), this, SLOT(qslDruckAc()));
   connect(actionReportLayout, SIGNAL(triggered(bool)), this, SLOT(reportLayoutAc()));
   connect(actionQueryTextOut, SIGNAL(triggered(bool)), this, SLOT(queryTextoutAc()));
   connect(actionADIFexport, SIGNAL(triggered(bool)), this, SLOT(adifExportAc()));
   connect(actionADIFimport, SIGNAL(triggered(bool)), this, SLOT(adifImportAc())); 
   connect(actionDataSave, SIGNAL(triggered(bool)), this, SLOT(dataSaveAc()));

   connect(actionLogconf, SIGNAL(triggered(bool)), this, SLOT(logconfAc())); 
   connect(actionStandort, SIGNAL(triggered(bool)), this, SLOT(standortAc()));
   connect(actionClub, SIGNAL(triggered(bool)), this, SLOT(clubAc())); 
   connect(actionInternet_ON, SIGNAL(triggered(bool)), this, SLOT(InternetOnAc())); 
   connect(actionRigCtl, SIGNAL(triggered(bool)), this, SLOT(rigCtlAc()));

   connect(actionMode, SIGNAL(triggered(bool)), this, SLOT(confmodeAc()));
   connect(actionVerbindungen, SIGNAL(triggered(bool)), this, SLOT(verbindungenAc()));
   connect(actionRig, SIGNAL(triggered(bool)), this, SLOT(rigAc())); 

   connect(actionDBadmin, SIGNAL(triggered(bool)), this, SLOT(admindbAc())); 
   connect(actionDxccTable, SIGNAL(triggered(bool)), this, SLOT(dxTableAc())); 
   connect(actionPrefTable, SIGNAL(triggered(bool)), this, SLOT(prefTableAc())); 
   connect(actionRuDistrictAwdList, SIGNAL(triggered(bool)), this, SLOT(ruDistrictAwdListAc()));
   connect(actionQrzCallbook, SIGNAL(triggered(bool)), this, SLOT(qrzTableAc()));
   connect(actionDokVerwaltung, SIGNAL(triggered(bool)), this, SLOT(dokVerwaltungAc()));
   connect(actionIotaVerwaltung, SIGNAL(triggered(bool)), this, SLOT(iotaVerwaltungAc()));

  connect(actionNotiz, SIGNAL(triggered(bool)), this, SLOT(notizAc())); 

   connect(actionQtlogVersion, SIGNAL(triggered(bool)), this, SLOT(VersionAc()));
   connect(actionRigCtlHamlib, SIGNAL(triggered(bool)), this, SLOT(RigCtlHamlibAc()));
   connect(actionQtLogInfo, SIGNAL(triggered(bool)), this, SLOT(InfoAc()));

   
   connect(checkBoxDateEpoch, SIGNAL(clicked()), this, SLOT(checkBoxDateCb()));
   
   
   
   connect(checkBoxSort, SIGNAL(pressed()), this, SLOT(checkBoxSortCb()));
   
   
   connect (groupEditLine, SIGNAL(returnPressed()), this, SLOT(groupEditLineCb()));
   connect (groupEditLine, SIGNAL(textChanged(QString)), this, SLOT(groupEditLineCb(QString)));
   
   
   connect (BoxAwd, SIGNAL(activated(int)), this, SLOT(boxAwdCb(int)));
   connect (awdEditLine, SIGNAL(returnPressed()), this, SLOT(awdEditLineCb()));
   
   connect (BoxSort, SIGNAL(activated(int)), this, SLOT(boxSortCb(int)));
   
   connect (BoxLayout, SIGNAL(currentIndexChanged(QString)), this, SLOT(boxLayoutCb(QString)));
   
   
   setupAction();
}

void QtLog::setupAction()
{
   readSettings();                             
   labelVersion->setText(VERSION);
   s = "Rel ";
   s += RELEASE;
   labelRelease->setText(s);
   
   mStatLabel = new QLabel;
   statusBar()->addPermanentWidget(mStatLabel);
   stackedWidget->setCurrentIndex (0);         
   dbstat = settings.value("dbconf").toInt();  
   //db = QSqlDatabase::addDatabase(settings.value("qsqlDatabase").toString());
   db = QSqlDatabase::addDatabase("QMYSQL");
   CheckDBconnection();                        
   logFile = settings.value("Logfile").toString(); 
   transl.installHashTable();                  
   transl.installHashTableBand();              
   
   checkBoxSort->hide();
   QDate heute = QDate::currentDate();         
   dateEditVon->setDate(heute);                
   n = heute.month();                          
   n--;
   i = heute.day();                            
   i--;
   heute = heute.addMonths(-n);                
   heute = heute.addDays(-i);
   dateEditBis->setDate(heute);                
   
   timer = new QTimer(this);                   
   connect(timer,SIGNAL(timeout()), this, SLOT(checkReport()));
   timer->start(1000);                         
   checkCount = 1;
   udpSocket = new QUdpSocket(this);               
   udpSocket->bind(QHostAddress::LocalHost,45454); 
   connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
   LogbookName ->setText(logFile);            
   s = "-  Operator : "+ settings.value("Call").toString();  
   labelOperator->setText(s);
   dock->hide();                              
   awdTable->setColumnWidth(0,70);            
   awdTable->setColumnWidth(1,90);            
   QSqlQuery query;                           
   BoxAwd->clear();                           
   qy = "SELECT atype FROM wawdlist WHERE aset !='0' ORDER BY aset";
   query.exec(qy);
   n = 0;
   while(query.next()) {                      
      i = 0;
      BoxAwd->insertItem(n++, query.value(i).toString());                    
   }
   BoxAwd->setCurrentIndex(settings.value("BoxAwd").toString().toInt());     
   BoxLimit->setCurrentIndex(settings.value("BoxLimit").toString().toInt()); 
   connect(BoxLimit, SIGNAL(currentIndexChanged(QString)), this, SLOT(boxLimitCb(QString)));
                                             
   qy = "SELECT awdpos,qstring FROM wlayout WHERE qhname='"+BoxLayout->currentText()+"'";
   query.exec(qy);
   while(query.next()) {
      awdPos = query.value(0).toInt();
      qystr = query.value(1).toString();
   }
   qy = "SELECT logname FROM dblogs WHERE logname='"+logFile+"'";             
   query.exec(qy);
   if(!query.size()) {
       QString f = "\nDas Logbook '"+logFile+"' ist nicht im Logordner.\nPruefe den Logbook-Namen im Config.";
       QMessageBox::information( this,
       tr("Fehler"),f,
       QMessageBox::Ok | QMessageBox::Default,
       QMessageBox::NoButton, QMessageBox::NoButton);
       groupEditLine->setFocus();
       labelQsoBestand->setText("??");
   }
   else {
      buildQuery();                               
      qy = "SELECT COUNT(*) FROM "+logFile;       
      query.exec(qy);
      query.next();
      s = query.value(0).toString()+" QSOs  ";
      labelQsoBestand->setText(s);                
   }
   operatorId = 0;
   qy = "SELECT opid FROM toperator WHERE ocall='"+settings.value("Call").toString()+"'";
   query.exec(qy);
   if(query.size()) {
     query.next();
     operatorId = query.value(0).toInt();
   }
   
   cflg = 180;                                    
   eflg = 0;                                      
   dflg = 0;                                      
                                                  
   connect (reportTable, SIGNAL(itemChanged(QTableWidgetItem *)), this,SLOT(updateReportItem(QTableWidgetItem *)));
   connect (reportTable, SIGNAL(itemClicked(QTableWidgetItem *)), this,SLOT(checkAwdItem(QTableWidgetItem *)));
   connect (awdTable, SIGNAL(itemChanged(QTableWidgetItem*)), this,SLOT(updateAwdItem(QTableWidgetItem * )));
   
   connect(ButtonRemoveQso, SIGNAL(clicked()), this, SLOT(removeQsoCb()));
   
} //ENDE setup
QtLog::~QtLog()
{
  writeColumnWidth();                            
  writeSettings();
}



void QtLog::goExit()
{
  qApp->quit();
}


void QtLog::checkReport()
{
    if( checkCount ) {
      stackedWidget->setCurrentIndex (1);          
      groupEditLine->setFocus();
      checkCount = 0;
      timer->stop();
    }
    if( cflg ) {                                   
        cflg--;
        if( !cflg ) {
          if(settings.value("Call").toString().count() == 1) {
            QMessageBox::information( this,tr("Info"),tr("Das Stationsrufzeichen ist nicht gesetzt !"),
            QMessageBox::Ok);
          }
        }
     }
}
//--------------------------
void QtLog::writeSettings()
{
  settings.setValue("BoxLimit",BoxLimit->currentIndex());
  settings.setValue("BoxAwd",BoxAwd->currentIndex());
  settings.setValue("QtLog/Size",size());
  settings.setValue("QtLog/Properties",saveState());
}

void QtLog::readSettings()
{
  resize(settings.value("QtLog/Size",sizeHint()).toSize());
  restoreState(settings.value("QtLog/Properties").toByteArray());
}

//------------------------------------------------------
void QtLog::writeColumnWidth()
{
  int i, n, x;
  QString a;
     QTableWidgetItem * cItem;
     QSqlQuery query;
     i = reportTable->columnCount();                    
     n = 0;
     while( n != i ) {
         cItem = reportTable->horizontalHeaderItem(n);  
         s = cItem->text();                             
         x = reportTable->columnWidth(n++);             
         qy = "UPDATE refnamen SET breite="+a.setNum(x)+" WHERE refnam='"+s+"'";
         query.exec(qy);
    }
    db.close();                                         
}


void QtLog::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_Escape :                        
             reportTable->clearSelection();
             rItem = NULL;                        
             reportTable->setCurrentItem(rItem); 
             awdTable->clearSelection();
             dock->hide();                        
	     groupEditLine->setText("");          
             groupEditLine->setFocus();           
           break;
     case Qt::Key_F10 :                           
             goExit();
           break;
   }
}



void QtLog::processPendingDatagram()
{
   while(udpSocket->hasPendingDatagrams()) {
     datagram.resize(udpSocket->pendingDatagramSize());
     QHostAddress sender;
     quint16 senderPort;
     udpSocket->readDatagram(datagram.data(),datagram.size(),&sender, &senderPort);
     i = datagram.indexOf (",",0);
     QByteArray Left = datagram.left(i);           
     QByteArray Mid = datagram.mid(i+1);           
     int com = Left.toInt();                       
     int row = Mid.toInt();                        
     if(com == 1 || com == 2 || com == 3) {        
       buildQuery();                               
       QSqlQuery query;                            
       qy = "SELECT COUNT(*) FROM "+logFile;       
       query.exec(qy);
       query.next();
       s = query.value(0).toString()+" QSOs  ";
       labelQsoBestand->setText(s);                
     }
     if( com == 3 )
       deleteReportRow(row);                       
   }
}



void QtLog::processWriteDatagram(int dNr, QString dStr)
{
     
     
     
     
     datagram = QByteArray::number(dNr);  
     datagram.append(",");
     datagram.append(dStr);               
     udpSocket->writeDatagram(datagram.data(),datagram.size(),
                       QHostAddress::LocalHost, 45450);
}


void QtLog::checkBoxDateCb()
{
    if(checkBoxDateEpoch->isChecked() == true) {
        checkBoxDateEpoch->setText("Laufendes Jahr");
        dateEditBis->setEnabled(true);
        dateEditVon->setEnabled(true);
    }
    else {
       checkBoxDateEpoch->setText("Ohne Kalender");
       dateEditBis->setEnabled(false);
       dateEditVon->setEnabled(false);
    }
    buildQuery();                                   
    groupEditLine->setFocus();                      
}


void QtLog::checkBoxSortCb()
{
    if(checkBoxSort->isChecked() == false)
        checkBoxSort->setText("Abw.Sort");
    else
       checkBoxSort->setText("Aufw.Sort");
    buildQuery();                            
    groupEditLine->setFocus();               
}



void QtLog::boxLimitCb(QString text )
{
     s = text;                                   
     buildQuery();                               
}


void QtLog::groupEditLineCb()
{
     dflg = 1;                                    
     buildQuery();                                
}



void QtLog::groupEditLineCb(QString str)
{
   if(str.count() >= 3) {
     dflg = 1;                                    
     buildQuery();                                
   }
}



void QtLog::boxAwdCb(int i)
{
    dflg = i;                                   
    dock->hide();                               
    awdEditLine->setText("");
    awdQuery();                                 
}



void QtLog::awdEditLineCb()
{
    if(awdEditLine->text().count() == 0)
       awdQuery();                             
    else
      awdQuerykenner();                        
}



void QtLog::boxSortCb(int i)
{
    dflg = i;                                   
    buildQuery();                               
}



void QtLog::boxLayoutCb(QString t)
{
   QSqlQuery query;                      
   qy = "SELECT awdpos,qstring FROM wlayout WHERE qhname='"+t+"'";
   query.exec(qy);
   while(query.next()) {
      awdPos = query.value(0).toInt();   
      qystr = query.value(1).toString(); 
   }
   buildQuery();                         
}




void QtLog::awdDxStatusSsbAc()         
{
   settings.setValue("Val","0");
   StartProcess("awdstdb &");
}

void QtLog::awdDxStatusCwAc()          
{
   settings.setValue("Val","1");
   StartProcess("awdstdb &");
}

void QtLog::awdJahresModeAc()          
{
   settings.setValue("Val","2");
   StartProcess("awdstdb &");
}

void QtLog::awdJahresBandAc()          
{
   settings.setValue("Val","3");
   StartProcess("awdstdb &");
}

void QtLog::awdDLDstatusAc()           
{
   settings.setValue("Val","4");
   StartProcess("awdstdb &");
}

void QtLog::awdIOTAstatusAc()          
{
   settings.setValue("Val","5");
   StartProcess("awdstdb &");
}



void QtLog::addLog()
{
    if(settings.value("Call").toString().count() == 1) {
      QMessageBox::information( this,tr("Info"),tr("Das Stationsrufzeichen ist nicht gesetzt !"),
      QMessageBox::Ok);
    }
    StartProcess("addqsodb &");                  
}


void QtLog::addClog()
{
    StartProcess("addClogdb &"); 
}


void QtLog::addDlog()
{
  StartProcess("addDqsodb &");
  StartProcess("fldigi &");
}



void QtLog::reportLayoutAc()                   //FeldNamen einbinden bzw verändern
{
   settings.setValue("Val",BoxLayout->currentText());
   reportLayout *reportL = new reportLayout(); 
   reportL->exec();
   qystr = reportL->s;
   awdPos = reportL->awdPos;
   if(reportL->changeflg)
      buildQuery();                            
}

void QtLog::qslEingangAc()                     
{
   StartProcess("qsldb &");                    
}

void QtLog::qslDruckAc()
{
  StartProcess("qslprintdb &");                
}

void QtLog::queryTextoutAc()       
{
  settings.setValue("Val",BoxAwd->currentText());
  query *qDiag = new query();
  qDiag->exec();
  if(qDiag->showTable) {
    qy = qDiag->qy;
    textExportData(qy, qDiag->awdPos);
  }
}

void QtLog::dataSaveAc()                       
{
    settings.setValue("Val",0);
    StartProcess("admindb &");
}

void QtLog::adifExportAc()
{
   settings.setValue("Val",1);                  
   StartProcess("admindb &");
}

void QtLog::adifImportAc()
{
   settings.setValue("Val",2);                  
   StartProcess("admindb &");
}



void QtLog::confmodeAc()                   
{                                          
   settings.setValue("Val",0);
   confMode *conFm = new confMode();
   conFm->exec();
}

void QtLog::verbindungenAc()                
{                                           
   settings.setValue("Val",1);
   confMode *conFm = new confMode();
   conFm->exec();
   
   if( conFm->rIndex == 5 ) {               
     QSqlQuery query;
     BoxAwd->clear();
     qy = "SELECT atype FROM wawdlist WHERE aset !='0' ORDER BY aset";
     query.exec(qy);
     n = 0;
     while(query.next()) {                   
       i = 0;
       BoxAwd->insertItem(n++, query.value(i).toString());                
     }
     BoxAwd->setCurrentIndex(settings.value("BoxAwd").toString().toInt()); 
     processWriteDatagram(6, "wawdlist");    
   }
   if(conFm->customId1 == 1)                 
     transl.setUsrField(conFm->custom1,conFm->val);
   if(conFm->customId2 == 1)                 
     transl.setUsrField(conFm->custom2,conFm->val2);
}

void QtLog::rigAc()                         
{
   settings.setValue("Val",2);
   confMode * conFm = new confMode();
   conFm->exec();
}



void QtLog::dxTableAc()
{
   StartProcess("ladb &");                        
}

void QtLog::prefTableAc()
{
   StartProcess("prefdb &");                      
}

void QtLog::dokVerwaltungAc()
{
   StartProcess("dokdb &");                       
}

void QtLog::iotaVerwaltungAc()
{
   StartProcess("iotadb &");                      
}

void QtLog::qrzTableAc()
{
  StartProcess("qrzdb &");                         
}

void QtLog::ruDistrictAwdListAc()
{
  StartProcess("uawdb &");                         
}

void QtLog::admindbAc()
{
    settings.setValue("Val",0);
    StartProcess("admindb &");                     
}



void QtLog::notizAc()
{
   mNote *note = new mNote();
   note->exec();
}



void QtLog::VersionAc()
{
   auth *au = new auth();
   au->exec();
}
void QtLog::InfoAc()
{
    settings.setValue("Val","Qso Verwaltung");
    StartProcess("hilfedb &");
}
void QtLog::RigCtlHamlibAc()
{
    settings.setValue("Val","Rig Control-Hamlib");
    StartProcess("hilfedb &");
}



void QtLog::logconfAc()                      
{                                            
    settings.setValue("Val",0);              
    configCb();
}

void QtLog::standortAc()                     
{                                            
    settings.setValue("Val",1);              
    configCb();
}

void QtLog::clubAc()                         
{                                            
    settings.setValue("Val",2);              
    configCb();
}

void QtLog::InternetOnAc()                  
{                                           
    settings.setValue("Val",3);
    configCb();                             
}

void QtLog::rigCtlAc()                      
{                                           
    rigctl *RigCtl = new rigctl();
    RigCtl->exec();
    
}


void QtLog::configCb()
{
  confDialog *conF = new confDialog();
  conF->exec();
  if(conF->state == 2) {                     
   if(logFile.compare(conF->s) != -1) {      
      logFile = conF->s;                     
      LogbookName->setText(logFile);         
      settings.setValue("Logfile",logFile);  
      QSqlQuery query;                       
      qy = "SELECT COUNT(*) FROM "+logFile;  
      query.exec(qy);
      query.next();
      s = query.value(0).toString()+" QSOs  ";
      labelQsoBestand->setText(s);           
      buildQuery();                          
      processWriteDatagram(5, logFile);      
   }
  }
  if(conF->work)
    labelOperator->setText(settings.value("Call").toString());  
}



void QtLog::removeQsoCb()
{
     int row, col;
     rItem = reportTable->currentItem();
     if(rItem == NULL) {
       QMessageBox::information( this,
       tr("ReportTable"),"\nKein QSO selectiert !",
       QMessageBox::Ok); 
       return;
     }
    
     row = reportTable->row(rItem);  
     QTableWidgetItem * h; 
     QTableWidgetItem * v = reportTable->verticalHeaderItem(row);   
     col = reportTable->columnCount();                               
     QTableWidgetItem * item;
     n = 0;
     while(n != col) {                               
         h = reportTable->horizontalHeaderItem( n );  
         if(h->text().compare("Call") == 0) break;
         n++;
     }
 
     item = reportTable->item(row,n);
     int status = QMessageBox::question(this,
                 tr("Qso entfernen ?"),
                 tr("\nDas QSO mit  ' %1 '  - Id: %2\nentfernen ?")
                 .arg(item->text(),v->text()),
                 QMessageBox::Yes , QMessageBox::No); 
     if(status != QMessageBox::Yes)
        return;
     QSqlQuery query; 
     qy = "SELECT oid FROM fun WHERE oid = "+v->text();
     query.exec(qy);
     
     if(query.size() == 1) {
       query.next();
       s = query.value(0).toString();
       qy = "DELETE FROM "+logFile+"om WHERE omid="+s;         
       query.exec(qy);    
     }          
     
     qy = "SELECT aid FROM funawd WHERE qid="+v->text();
     query.exec(qy);
     if(query.size() == 1) {
       query.next();
       s = query.value(0).toString();
       qy = "DELETE FROM "+logFile+"awd WHERE qid="+s;         
       query.exec(qy);
     }          
      
     qy = "DELETE FROM "+logFile+"qsl WHERE qsoid="+v->text(); 
     query.exec(qy);
                                                               
							       
     qy = "DELETE FROM "+logFile+" WHERE id="+v->text();       
     query.exec(qy);
     
     reportTable->removeRow (row);
     reportTable->clearSelection();                            
     rItem = NULL;                                             
     reportTable->setCurrentItem(rItem); 
     
     qy = "SELECT COUNT(*) FROM "+logFile;       
     query.exec(qy);
     query.next();
     s = query.value(0).toString()+" QSOs  ";
     labelQsoBestand->setText(s); 
}
  




void QtLog::updateReportItem( QTableWidgetItem *item )
{
    if (item != reportTable->currentItem())                
        return;
    if (item) {                                            
         QSqlQuery query;
         row = reportTable->row( item );                   
         col = reportTable->column ( item );               
         QTableWidgetItem * h = reportTable->horizontalHeaderItem( col );       
         QTableWidgetItem * v = reportTable->verticalHeaderItem ( row );        
         if(h->text().compare(BoxAwd->itemText(BoxAwd->currentIndex())) == 0) { 
            item->setText(celltext);                                            
            return;                                                             
         }
         QSqlField field("feld",QVariant::String);
         if(transl.getDbField(h->text()).compare("name") == 0) {    
             s = "SELECT oid FROM "+logFile+" WHERE id="+v->text(); 
             query.exec(s);
             query.next();
             qy = "UPDATE "+logFile+"om SET "+transl.getDbField( h->text())+"=";
             field.setValue(item->data(Qt::EditRole).toString());   
             qy += db.driver()->formatValue(field,false)+" WHERE omid="+query.value(0).toString();
         }
          else
            if(transl.getDbField(h->text()).compare("rufz") == 0) { 
              qy = "SELECT id,oid FROM "+logFile+" WHERE oid = ";   
              qy += "(SELECT oid FROM "+logFile+" WHERE id="+v->text()+")";
              query.exec(qy);
              n = query.size();
              if(n == 1) {                               
                 query.next();
                 qy = "UPDATE "+logFile+"om SET "+transl.getDbField( h->text())+"=";
                 field.setValue(item->data(Qt::EditRole).toString());
                 qy += db.driver()->formatValue(field,false)+" WHERE omid="+query.value(1).toString();
              }
              else {                                                           
                qy = "INSERT INTO "+logFile+"om VALUES (0,'"+item->data(Qt::EditRole).toString();
                qy += "','')";                                                 
                query.exec(qy);                                                
                n = query.lastInsertId().toInt();
                                                                               
                if(transl.getLogTable(transl.getDbField(h->text())) == 2) {    
                  qy = "UPDATE "+logFile+" SET "+transl.getDbField( h->text())+"=";
                  field.setValue(item->data(Qt::EditRole).toString());
                  qy += db.driver()->formatValue(field,false)+" WHERE id="+v->text();
                }
                else {                                                         
                 qy = "UPDATE "+logFile+"qsl SET "+transl.getDbField( h->text())+"=";
                 field.setValue(item->data(Qt::EditRole).toString());
                 qy += db.driver()->formatValue(field,false)+" WHERE qsoid="+v->text();
                }
              }
            }
          else
            if(transl.getDbField(h->text()).compare("band") == 0) {   
               field.setValue(item->data(Qt::EditRole).toString());   
               s = db.driver()->formatValue(field,false);
               qy = "SELECT mband FROM wband WHERE mband="+s;         
               query.exec(qy);
               if( !query.size()) {                                   
                 QString f = "\nDer Bandname  "+s+"  ist unbekannt";  
                 QMessageBox::information( this,
                 tr("Band Fehler"),f,
                 QMessageBox::Ok | QMessageBox::Default,
                 QMessageBox::NoButton, QMessageBox::NoButton);
                 groupEditLine->setFocus();
                 return;
               }
               n = s.indexOf("'",0);                                        
               s.remove(n,1);
               n = s.indexOf("'",0);
               s.remove(n,1);
               if(transl.getLogTable(transl.getDbField(h->text())) == 2) {  
                 qy = "UPDATE "+logFile+" SET "+transl.getDbField( h->text())+"=";
                 qy += "'"+transl.getSysband(s)+"' WHERE id="+v->text();    
               }
               else {                                                       
                 qy = "UPDATE "+logFile+"qsl SET "+transl.getDbField( h->text())+"=";
                 field.setValue(item->data(Qt::EditRole).toString());
                 qy += db.driver()->formatValue(field,false)+" WHERE qsoid="+v->text();
              }
            }
          else {
            if(transl.getLogTable(transl.getDbField(h->text())) == 2) {      
              qy = "UPDATE "+logFile+" SET "+transl.getDbField( h->text())+"=";
              field.setValue(item->data(Qt::EditRole).toString());
              qy += db.driver()->formatValue(field,false)+" WHERE id="+v->text();
            }
            else {                                                         
              qy = "UPDATE "+logFile+"qsl SET "+transl.getDbField( h->text())+"=";
              field.setValue(item->data(Qt::EditRole).toString());
              qy += db.driver()->formatValue(field,false)+" WHERE qsoid="+v->text();
           }
          }
          query.exec(qy);                                              
    }
    groupEditLine->setFocus();
}




void QtLog::checkAwdItem(QTableWidgetItem *ritem)
{
    if (ritem != reportTable->currentItem())             
       return;
    awdTable->clearSelection();                          
    col = reportTable->column ( ritem );                               
    QTableWidgetItem * h = reportTable->horizontalHeaderItem( col );   
    celltext = ritem->text();                                          
    if(h->text().compare(BoxAwd->itemText(BoxAwd->currentIndex())) == 0) { 
        //dock->setFloating(true);                      
        dock->show();                                   
        row = reportTable->row(ritem);
        QTableWidgetItem *v = reportTable->verticalHeaderItem(row);
        if(eflg) {                
          if(awdqId == v->text().toInt()) {              
             eflg = 0;                                   
             awdqId = v->text().toInt();
             dock->hide();                               
             return;                                     
          }
        }
        eflg = SHOWAWD_TIME;           
        awdqId = v->text().toInt();                      
        awdTable->clear();                               
        QTableWidgetItem *hItem0 = new QTableWidgetItem();       
        hItem0->setText(QApplication::translate("QtLog", "Type", 0, QApplication::UnicodeUTF8));
        awdTable->setHorizontalHeaderItem(0, hItem0);
        QTableWidgetItem *hItem1 = new QTableWidgetItem();
        hItem1->setText(QApplication::translate("QtLog", "Kenner", 0, QApplication::UnicodeUTF8));
        awdTable->setHorizontalHeaderItem(1,hItem1);
       
        r = BoxAwd->currentIndex();                      
        n = BoxAwd->count();                             
        awdTable->setRowCount(n);                        
        row = 0;
        BoxAwd->setCurrentIndex(row);                    
        while( row != n ) {                              
           c = 0;
           QTableWidgetItem *newItem0 = new QTableWidgetItem(tr("%1").arg((row)*(c))); //Type
           newItem0->setText(BoxAwd->currentText());     
           awdTable->setItem(row,c++,newItem0);          
           QTableWidgetItem *newItem1 = new QTableWidgetItem(tr("%1").arg((row)*(c))); //Kenner
           newItem1->setText("");                        
           awdTable->setItem(row++,c,newItem1);          
           BoxAwd->setCurrentIndex(row);                 
        }
        BoxAwd->setCurrentIndex(r);                      
        QTableWidgetItem *awdItem;                       
        QString f;               
        QSqlQuery query;
        qy = "SELECT awtype,awkenner FROM "+logFile+"awd WHERE qid="+v->text();
        query.exec(qy);
        r = query.size();        
        if( r ) {                
           while(query.next()) {
             r = 0;
             i = 0;
             s = query.value(r++).toString();           
             p = query.value(r).toString();             
             awdItem = awdTable->item(i,0);
             f = awdItem->text();                       
             while( i != n ) {                          
               if((s.compare(f)) == 0 ) {               
                    c = 1;
                    QTableWidgetItem *kItem = new QTableWidgetItem(tr("%1").arg((i)*(c))); //Kenner
                    //kItem = awdTable->item(i,1);      
                    kItem->setText(p);                  
                    awdTable->setItem(i,1,kItem);       
                   break;                               
               }
               if( i == n -1) {                         
                    f = "\nDer AwdType ' "+s+" ' ist unbekannt.\nBitte anlegen.";
                    QMessageBox::information( this,
                    tr("AWD Fehler"),f,
                    QMessageBox::Ok | QMessageBox::Default,
                    QMessageBox::NoButton, QMessageBox::NoButton);
                   return;                              
                }
               i++;
               awdItem = awdTable->item(i,0);
               f = awdItem->text();                     
             }
           }
        }
    }
    else {                                              
      dock->hide();                                     
      eflg = 0;                            
    }
    groupEditLine->setFocus();
}






void QtLog::updateAwdItem(QTableWidgetItem *item)
{
     if (item != awdTable->currentItem())                 
        return;
     if(awdTable->column( item ) == 0)                    
        return;         
                        
      row = awdTable->row( item );
      QTableWidgetItem * eItem = awdTable->item( row, 0); 
      QSqlQuery query;
      qy = "SELECT awkenner FROM "+logFile+"awd WHERE qid="+s.setNum(awdqId)+" AND awtype='";
      qy += eItem->text()+"'";
      query.exec(qy);
      if( query.size() == 1 ) {                           
         qy = "UPDATE "+logFile+"awd SET awkenner='"+item->data(Qt::EditRole).toString();
         qy += "' WHERE qid="+s.setNum(awdqId)+" AND awtype='"+eItem->text()+"'";
         query.exec(qy);
      }
      else {                                              
         qy = "SELECT rufz FROM "+logFile+","+logFile+"om WHERE id="+s.setNum(awdqId);
         qy += " AND omid=oid";
         query.exec(qy);
         query.next();                                    
         qy = "INSERT INTO "+logFile+"awd VALUES (0,"+s.setNum(awdqId);  
         qy += ",'"+query.value(0).toString();                           
         qy += "','"+eItem->text();                                      
         qy += "','"+item->data(Qt::EditRole).toString();                
         qy += "')";
         query.exec(qy);
     }
     groupEditLine->setFocus();                   
     eflg = SHOWAWD_TIME;                         
}








void QtLog::deleteReportRow(int idNr)         
{
 int count;
     s = s.setNum(idNr);
     count = reportTable->rowCount();                             
     row = 0;
     QTableWidgetItem * v = reportTable->verticalHeaderItem(row); 
     while(row != count) {
         if(v->text().compare(s) == 0)
            break;                                                
         row++;
         v = reportTable->verticalHeaderItem(row);                
     }
     reportTable->removeRow(row);                                 
}






void QtLog::addReportRow(int idNr)
{
   int id;
   id = idNr;  
   qDebug() << "neuen Datensatz anhängen";
   qDebug() << "das geht so nicht, da die Spaltenanzahl, -Namen, und -Sortierung nicht bekannt sind";
   qDebug() << "neue Idee: - Satz aus der DB lesen - aber mit welchen Querry ? MERKER f.last_QUERY ?";
}



