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
   // -- Award.status
   connect(actionDxStatusSSB, SIGNAL(triggered(bool)), this, SLOT(awdDxStatusSsbAc()));
   connect(actionDxStatusCW, SIGNAL(triggered(bool)), this, SLOT(awdDxStatusCwAc()));
   connect(actionJahresMODE, SIGNAL(triggered(bool)), this, SLOT(awdJahresModeAc()));
   connect(actionJahresBAND, SIGNAL(triggered(bool)), this, SLOT(awdJahresBandAc()));
   connect(actionDLDstatus, SIGNAL(triggered(bool)), this, SLOT(awdDLDstatusAc())); 
   connect(actionIOTAstatus, SIGNAL(triggered(bool)), this, SLOT(awdIOTAstatusAc()));
   // -- Logging
   connect(actionAddQso, SIGNAL(triggered(bool)), this, SLOT(addLog()));
   connect(actionAddClog, SIGNAL(triggered(bool)), this, SLOT(addClog()));
   connect(actionLogChange, SIGNAL(triggered(bool)), this, SLOT(logconfAc()));
   connect(actionFldigi_logging, SIGNAL(triggered(bool)), this, SLOT(addDlog()));
   
   // -- Logbook.management
   connect(actionQslEingang, SIGNAL(triggered(bool)), this, SLOT(qslEingangAc()));
   connect(actionQslDruck, SIGNAL(triggered(bool)), this, SLOT(qslDruckAc()));
   connect(actionReportLayout, SIGNAL(triggered(bool)), this, SLOT(reportLayoutAc()));
   connect(actionQueryTextOut, SIGNAL(triggered(bool)), this, SLOT(queryTextoutAc()));
   connect(actionADIFexport, SIGNAL(triggered(bool)), this, SLOT(adifExportAc()));
   connect(actionADIFimport, SIGNAL(triggered(bool)), this, SLOT(adifImportAc())); 
   connect(actionDataSave, SIGNAL(triggered(bool)), this, SLOT(dataSaveAc()));
   // -- config
   connect(actionLogconf, SIGNAL(triggered(bool)), this, SLOT(logconfAc())); 
   connect(actionStandort, SIGNAL(triggered(bool)), this, SLOT(standortAc()));
   connect(actionClub, SIGNAL(triggered(bool)), this, SLOT(clubAc())); 
   connect(actionInternet_ON, SIGNAL(triggered(bool)), this, SLOT(InternetOnAc())); 
   connect(actionRigCtl, SIGNAL(triggered(bool)), this, SLOT(rigCtlAc()));
   // -- operating mode
   connect(actionMode, SIGNAL(triggered(bool)), this, SLOT(confmodeAc()));
   connect(actionVerbindungen, SIGNAL(triggered(bool)), this, SLOT(verbindungenAc()));
   connect(actionRig, SIGNAL(triggered(bool)), this, SLOT(rigAc())); 
   // -- tables
   connect(actionDBadmin, SIGNAL(triggered(bool)), this, SLOT(admindbAc())); 
   connect(actionDxccTable, SIGNAL(triggered(bool)), this, SLOT(dxTableAc())); 
   connect(actionPrefTable, SIGNAL(triggered(bool)), this, SLOT(prefTableAc())); 
   connect(actionRuDistrictAwdList, SIGNAL(triggered(bool)), this, SLOT(ruDistrictAwdListAc()));
   connect(actionQrzCallbook, SIGNAL(triggered(bool)), this, SLOT(qrzTableAc()));
   connect(actionDokVerwaltung, SIGNAL(triggered(bool)), this, SLOT(dokVerwaltungAc()));
   connect(actionIotaVerwaltung, SIGNAL(triggered(bool)), this, SLOT(iotaVerwaltungAc()));
   // -- Qrg_note
   connect(actionNotiz, SIGNAL(triggered(bool)), this, SLOT(notizAc())); 
  
   // -- help
   connect(actionQtlogVersion, SIGNAL(triggered(bool)), this, SLOT(VersionAc()));
   connect(actionRigCtlHamlib, SIGNAL(triggered(bool)), this, SLOT(RigCtlHamlibAc()));
   connect(actionQtLogInfo, SIGNAL(triggered(bool)), this, SLOT(InfoAc()));
   
   // ---------
   // Box db view range
   connect(checkBoxDateEpoch, SIGNAL(clicked()), this, SLOT(checkBoxDateCb()));
   // Box Sort
   connect(checkBoxSort, SIGNAL(pressed()), this, SLOT(checkBoxSortCb()));
   // data groups selektion
   connect (groupEditLine, SIGNAL(returnPressed()), this, SLOT(groupEditLineCb()));
   connect (groupEditLine, SIGNAL(textChanged(QString)), this, SLOT(groupEditLineCb(QString)));
   // AWD selektion
   connect (BoxAwd, SIGNAL(activated(int)), this, SLOT(boxAwdCb(int)));
   connect (awdEditLine, SIGNAL(returnPressed()), this, SLOT(awdEditLineCb()));
   // Box_Sort changed
   connect (BoxSort, SIGNAL(activated(int)), this, SLOT(boxSortCb(int)));
   // Box_layout changed
   connect (BoxLayout, SIGNAL(currentIndexChanged(QString)), this, SLOT(boxLayoutCb(QString)));
   
   setupAction();
}

void QtLog::setupAction()
{
   readSettings();                                 // read qtlog size and sizeHint                         
   labelVersion->setText(VERSION);
   s = "Rel ";
   s += RELEASE;
   labelRelease->setText(s);
   
   mStatLabel = new QLabel;
   statusBar()->addPermanentWidget(mStatLabel);
   stackedWidget->setCurrentIndex (0);             // set wellpap
   dbstat = settings.value("dbconf").toInt();  
   db = QSqlDatabase::addDatabase("QMYSQL");       // create DB Instanz
   CheckDBconnection();                            // check open db
   
   logFile = settings.value("Logfile").toString(); // check logfile
   transl.installHashTable();                      // create Hash_table for name translating  
   transl.installHashTableBand();                  //                   for band 
   
   checkBoxSort->hide();
   QDate heute = QDate::currentDate();             // create date and time 
   dateEditVon->setDate(heute);                
   n = heute.month();                          
   n--;
   i = heute.day();                                // Days of month
   heute = heute.addMonths(-n);                    // calculating days to the year          
   heute = heute.addDays(-i);
   dateEditBis->setDate(heute);                
   
   timer = new QTimer(this);                   
   connect(timer,SIGNAL(timeout()), this, SLOT(checkReport()));
   timer->start(1000);                         
   checkCount = 1;
   udpSocket = new QUdpSocket(this);               // datagram_UDP_net 
   udpSocket->bind(QHostAddress::LocalHost,45454); // bind adr
   connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
   
   LogbookName ->setText(logFile);                 // set the name of working logbook
   s = "-  Operator : "+ settings.value("Call").toString();  
   labelOperator->setText(s);
   dock->hide();                                   // AWD Widget                  
   awdTable->setColumnWidth(0,70);            
   awdTable->setColumnWidth(1,90);            
   QSqlQuery query; 
   
   BoxAwd->clear();                           
   qy = "SELECT atype FROM wawdlist WHERE aset !='0' ORDER BY aset";
   query.exec(qy);                               // fill AWD_Box: with names
   n = 0;
   while(query.next()) {                         // AWD table set up  
      i = 0;
      BoxAwd->insertItem(n++, query.value(i).toString());                    
   }
   BoxAwd->setCurrentIndex(settings.value("BoxAwd").toString().toInt());     
   BoxLimit->setCurrentIndex(settings.value("BoxLimit").toString().toInt()); 
   connect(BoxLimit, SIGNAL(currentIndexChanged(QString)), this, SLOT(boxLimitCb(QString)));
   
   // set default AWD                
   qy = "SELECT awdpos,qstring FROM wlayout WHERE qhname='"+BoxLayout->currentText()+"'";
   query.exec(qy);
   while(query.next()) {
      awdPos = query.value(0).toInt();
      qystr = query.value(1).toString();
   }
   
   qy = "SELECT logname FROM dblogs WHERE logname='"+logFile+"'";   // check logfile correctly         
   query.exec(qy);
   if(!query.size()) {
       QMessageBox::information( this,
       tr("Fehler"),tr("\nDas Logbook '%1'ist nicht im Logordner.\nPruefe den Logbook-Namen im Config.").arg(logFile),	 
       QMessageBox::Ok | QMessageBox::Default,
       QMessageBox::NoButton, QMessageBox::NoButton);
       groupEditLine->setFocus();
       labelQsoBestand->setText("??");
   }
   else {                                                           // it is
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
   
   cflg = 180;       // alarm time for work without callsign                        
   eflg = 0;                                      
   dflg = 0;                                      
                                                  
   connect (reportTable, SIGNAL(itemChanged(QTableWidgetItem *)), this,SLOT(updateReportItem(QTableWidgetItem *)));
   connect (reportTable, SIGNAL(itemClicked(QTableWidgetItem *)), this,SLOT(checkAwdItem(QTableWidgetItem *)));
   connect (awdTable, SIGNAL(itemChanged(QTableWidgetItem*)), this,SLOT(updateAwdItem(QTableWidgetItem * )));
   connect(ButtonRemoveQso, SIGNAL(clicked()), this, SLOT(removeQsoCb()));
   
} //ENDE setup

// -------------
QtLog::~QtLog()
{
  writeColumnWidth();                            
  writeSettings();
}

// #####################################

void QtLog::goExit()
{
  qApp->quit();
}

// timer
// ---------------------
void QtLog::checkReport()
{
    if( checkCount ) {
      stackedWidget->setCurrentIndex (1);     // reset Wellpap      
      groupEditLine->setFocus();
      checkCount = 0;
      timer->stop();
    }
    if( cflg ) {                             // remember to callsign
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

// ----------------------
void QtLog::readSettings()
{
  resize(settings.value("QtLog/Size",sizeHint()).toSize());
  restoreState(settings.value("QtLog/Properties").toByteArray());
}

// save table layout
//-----------------------------
void QtLog::writeColumnWidth()
{
  int i, n, x;
  QString a;
     QTableWidgetItem * cItem;
     QSqlQuery query;
     i = reportTable->columnCount();                    // number of columns        
     n = 0;
     while( n != i ) {
         cItem = reportTable->horizontalHeaderItem(n);  // work item
         s = cItem->text();                             
         x = reportTable->columnWidth(n++);             
         qy = "UPDATE refnamen SET breite="+a.setNum(x)+" WHERE refnam='"+s+"'";
         query.exec(qy);                                // save layout
    }
    db.close();                                         
}


// ------------------------------------------
void QtLog::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_Escape :                        
             reportTable->clearSelection();
             rItem = NULL;                        
             reportTable->setCurrentItem(rItem); 
             awdTable->clearSelection();
             dock->hide();                              // dock hide                    
	     groupEditLine->setText("");          
             groupEditLine->setFocus();           
           break;
     case Qt::Key_F10 :                                 // exit                
             goExit();
           break;
   }
}


// Datagram : messages received
// ------------------------------
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
     int com = Left.toInt();                       // fuktion            
     int row = Mid.toInt();                        // parem List
     if(com == 1 || com == 2 || com == 3) {        // this ist ok
       buildQuery();                               
       QSqlQuery query;                            
       qy = "SELECT COUNT(*) FROM "+logFile;       // chech new data set     
       query.exec(qy);
       query.next();
       s = query.value(0).toString()+" QSOs  ";
       labelQsoBestand->setText(s);                // show new data set
     }
     if( com == 3 )
       deleteReportRow(row);                       
   }
}


// send datagram dataset changed
// ----------------------------------------------------
void QtLog::processWriteDatagram(int dNr, QString dStr)
{
     datagram = QByteArray::number(dNr);  
     datagram.append(",");
     datagram.append(dStr);               
     udpSocket->writeDatagram(datagram.data(),datagram.size(),
                       QHostAddress::LocalHost, 45450);
}

// check_Box: determine data range
// -------------------------------
void QtLog::checkBoxDateCb()
{
    if(checkBoxDateEpoch->isChecked() == true) {
        checkBoxDateEpoch->setText(tr("Laufendes Jahr"));
        dateEditBis->setEnabled(true);
        dateEditVon->setEnabled(true);
    }
    else {
       checkBoxDateEpoch->setText(tr("Ohne Kalender"));
       dateEditBis->setEnabled(false);
       dateEditVon->setEnabled(false);
    }
    buildQuery();                                   
    groupEditLine->setFocus();                      
}

// sort dataset up or down
// -------------------------
void QtLog::checkBoxSortCb()
{
    if(checkBoxSort->isChecked() == false)
        checkBoxSort->setText(tr("Abw.Sort"));
    else
       checkBoxSort->setText(tr("Aufw.Sort"));
    buildQuery();                            
    groupEditLine->setFocus();               
}

// ==========================================

// dataset limit changed
// ----------------------
void QtLog::boxLimitCb(QString text )
{
     s = text;                                   
     buildQuery();                 // make new query and display                  
}


// looking for data group name
// --------------------------
void QtLog::groupEditLineCb()
{
     dflg = 1;                                    
     buildQuery();                 // make query and display            
}

// other group
// --------------------------
void QtLog::groupEditLineCb(QString str)
{
   if(str.count() >= 3) {
     dflg = 1;                                    
     buildQuery();                          
   }
}

// AWD_box finishing work
// ------------------------------
void QtLog::boxAwdCb(int i)
{
    dflg = i;                                   
    dock->hide();                               
    awdEditLine->setText("");
    awdQuery();                                 
}

// BoxAwd: new name inserted
// -----------------------------
void QtLog::awdEditLineCb()
{
    if(awdEditLine->text().count() == 0)
       awdQuery();                             
    else
      awdQuerykenner();                        
}

// sort value changed
// ----------------------------
void QtLog::boxSortCb(int i)
{
    dflg = i;                                   
    buildQuery();                               
}

// box_layout changed
// -----------------------------
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


// -- SLOTS -

// -------------------------
// AWD menue
// --------------------------
void QtLog::awdDxStatusSsbAc()          // awd_DxStatus_SSB
{
   settings.setValue("Val","0");
   StartProcess("awdstdb &");
}

// -----------------------
void QtLog::awdDxStatusCwAc()           // awd_DxStatus_CW     
{
   settings.setValue("Val","1");
   StartProcess("awdstdb &");
}

// ------------------------
void QtLog::awdJahresModeAc()           // awd_Jahres_Aktivität_MODE
{
   settings.setValue("Val","2");
   StartProcess("awdstdb &");
}

// -----------------------
void QtLog::awdJahresBandAc()           // awd_Jahres_Aktivität_Band
{
   settings.setValue("Val","3");
   StartProcess("awdstdb &");
}

// ------------------------
void QtLog::awdDLDstatusAc()            // awd DLD-Status
{
   settings.setValue("Val","4");
   StartProcess("awdstdb &");
}

// --------
void QtLog::awdIOTAstatusAc()           // awd IOTA-Status
{
   settings.setValue("Val","5");
   StartProcess("awdstdb &");
}

// ---------------------------------
// logging menue
// ---------------------------------
void QtLog::addLog()                   // call; add_qso dialog
{
    if(settings.value("Call").toString().count() == 1) {
      QMessageBox::information( this,tr("Info"),tr("Das Stationsrufzeichen ist nicht gesetzt !"),
      QMessageBox::Ok);
    }
    StartProcess("addqsodb &");                  
}

// -----------------
void QtLog::addClog()                  // call; add_contest_qso dialog    
{
    StartProcess("addClogdb &"); 
}

// -----------------
void QtLog::addDlog()                  // call; add_digital_qso dialog
{
  StartProcess("addDqsodb &");
  StartProcess("fldigi &");
}

// --------------
// menue logbook management
// ---------------------
void QtLog::reportLayoutAc()            // field name changed 
{
   settings.setValue("Val",BoxLayout->currentText());
   reportLayout *reportL = new reportLayout(); 
   reportL->exec();
   qystr = reportL->s;
   awdPos = reportL->awdPos;
   if(reportL->changeflg)
      buildQuery();                            
}

// ----------------------
void QtLog::qslEingangAc()              // call;  qsl managmet     
{
   StartProcess("qsldb &");                    
}

// ----------------------
void QtLog::qslDruckAc()                // call; qsl printing
{
  StartProcess("qslprintdb &");                
}

// ------------------------
void QtLog::queryTextoutAc()            // set new table layout
{
  settings.setValue("Val",BoxAwd->currentText());
  query *qDiag = new query();
  qDiag->exec();
  if(qDiag->showTable) {
    qy = qDiag->qy;
    textExportData(qy, qDiag->awdPos);
  }
}

// ----------------------------
// menue data security
// ----------------------------
void QtLog::dataSaveAc()                       
{
    settings.setValue("Val",0);          // DUMP - RESTORE database
    StartProcess("admindb &"); 
}

void QtLog::adifExportAc()               // ADIF export
{
   settings.setValue("Val",1);                  
   StartProcess("admindb &");
}

void QtLog::adifImportAc()                // ADIF import
{
   settings.setValue("Val",2);                  
   StartProcess("admindb &");
}

// --------------------
// nenue operating mode
// -----------------------------------------
void QtLog::confmodeAc()                   // changed; band, mode, pwr
{                                          
   settings.setValue("Val",0);
   confMode *conFm = new confMode();
   conFm->exec();
}

// ------------------------
void QtLog::verbindungenAc()                // config AWD       
{                                           
   settings.setValue("Val",1);
   confMode *conFm = new confMode();
   conFm->exec();
   
   if( conFm->rIndex == 5 ) {               // AWD working list changed       
     QSqlQuery query;
     BoxAwd->clear();
     qy = "SELECT atype FROM wawdlist WHERE aset !='0' ORDER BY aset";
     query.exec(qy);
     n = 0;
     while(query.next()) {                  // insert new AWD List           
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

// ---------------
void QtLog::rigAc()                          // rig                
{
   settings.setValue("Val",2);
   confMode * conFm = new confMode();
   conFm->exec();
}

// -------------
// menue tables
// ------------------------
void QtLog::dxTableAc()                      // country table
{
   StartProcess("ladb &");                        
}
// ---------------------
void QtLog::prefTableAc()                    // SubPrefix_table
{
   StartProcess("prefdb &");                      
}
// ------------------------
void QtLog::dokVerwaltungAc()                // DOK_table
{
   StartProcess("dokdb &");                       
}
// -----------------------
void QtLog::iotaVerwaltungAc()               // IOTA_table
{
   StartProcess("iotadb &");                      
}
// -----------------------
void QtLog::qrzTableAc()                     // QRZ_table
{
  StartProcess("qrzdb &");                         
}
// ----------------------
void QtLog::ruDistrictAwdListAc()            // RU-District_AWD_List
{
  StartProcess("uawdb &");                         
}
// ---------------------
void QtLog::admindbAc()                      //Administration of DB
{
    settings.setValue("Val",0);
    StartProcess("admindb &");                     
}

// ----------
// Menue note
// -------------
void QtLog::notizAc()                        // create new not_table
{
   mNote *note = new mNote();
   note->exec();
}

// --------------
// menue help
// --------------
void QtLog::VersionAc()
{
   auth *au = new auth();
   au->exec();
}
// ------------------
void QtLog::InfoAc()
{
    settings.setValue("Val","Qso Verwaltung");
    StartProcess("hilfedb &");
}
// ------------------------
void QtLog::RigCtlHamlibAc()
{
    settings.setValue("Val","Rig Control-Hamlib");
    StartProcess("hilfedb &");
}

// ++++++++++++++++++++
// menue config
// ++++++++++++++++++++
void QtLog::logconfAc()                      
{                                            
    settings.setValue("Val",0);     // Logbook         
    configCb();
}
// ---------------------
void QtLog::standortAc()                     
{                                            
    settings.setValue("Val",1);     // Qth           
    configCb();
}
// --------------------
void QtLog::clubAc()                         
{                                            
    settings.setValue("Val",2);     // club          
    configCb();
}
// ----------------------
void QtLog::InternetOnAc()                  
{                                           
    settings.setValue("Val",3);     // Internet
    configCb();                             
}
// -----------------------
void QtLog::rigCtlAc()                      
{                                           
    rigctl *RigCtl = new rigctl();   // Rig
    RigCtl->exec();
    
}


// ConfigDialog Call
// -------------------------
void QtLog::configCb()
{
  confDialog *conF = new confDialog();
  conF->exec();
  if(conF->state == 2) {                     
   if(logFile.compare(conF->s) != -1) {      
      logFile = conF->s;                     
      LogbookName->setText(logFile);         
      settings.setValue("Logfile",logFile);     // save
      
      QSqlQuery query;                       
      qy = "SELECT COUNT(*) FROM "+logFile;  
      query.exec(qy);
      query.next();
      s = query.value(0).toString()+" QSOs  ";
      labelQsoBestand->setText(s);           
      buildQuery();                          
      processWriteDatagram(5, logFile);         // logbook changed. datagram to -> addqso
   }
  }
  if(conF->work)
    labelOperator->setText(settings.value("Call").toString());  // save
}

// ---------------------------
// remove QSO
// ---------------------------
void QtLog::removeQsoCb()
{
     int row, col;
     rItem = reportTable->currentItem();
     if(rItem == NULL) {
       QMessageBox::information( this,
       tr("ReportTable"),tr("\nKein QSO selectiert !"),
       QMessageBox::Ok); 
       return;
     }
    
     row = reportTable->row(rItem);  
     QTableWidgetItem * h; 
     QTableWidgetItem * v = reportTable->verticalHeaderItem(row);   
     col = reportTable->columnCount();                               
     QTableWidgetItem * item;
     n = 0;
     while(n != col) {                                      // get callsign                  
         h = reportTable->horizontalHeaderItem( n );  
         if(h->text().compare("Call") == 0) break;
         n++;
     }
 
     item = reportTable->item(row,n);
     int status = QMessageBox::question(this,
                 tr("Qso entfernen ?"),
                 tr("\nDas QSO mit  '")+item->text()+tr(" '  - Id: ")+v->text()+tr("\nentfernen ?"),
                 QMessageBox::Yes , QMessageBox::No); 
     if(status != QMessageBox::Yes)
        return;
     QSqlQuery query; 
     qy = "SELECT oid FROM "+logFile+" WHERE oid = "+v->text(); // record number
     query.exec(qy);
     
     if(query.size() == 1) {
       query.next();
       s = query.value(0).toString();
       qy = "DELETE FROM "+logFile+"om WHERE omid="+s;          // logFile_OM
       query.exec(qy);    
     }          
     
     qy = "SELECT aid FROM "+logFile+"awd WHERE qid="+v->text(); 
     query.exec(qy);
     if(query.size() == 1) {
       query.next();
       s = query.value(0).toString();
       qy = "DELETE FROM "+logFile+"awd WHERE qid="+s;          // logFile_AWD    
       query.exec(qy);
     }          
      
     qy = "DELETE FROM "+logFile+"qsl WHERE qsoid="+v->text();  
     query.exec(qy);
                                                               
							       
     qy = "DELETE FROM "+logFile+" WHERE id="+v->text();        // logFile_QSO  
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
  

// Report_table : data edited
// -----------------------------------------
void QtLog::updateReportItem( QTableWidgetItem *item )
{
    if (item != reportTable->currentItem())                
        return;
    if (item) {                                            
         QSqlQuery query;
         row = reportTable->row( item );                   
         col = reportTable->column ( item );               
         QTableWidgetItem * h = reportTable->horizontalHeaderItem( col );       // workitem   
         QTableWidgetItem * v = reportTable->verticalHeaderItem ( row );        
         if(h->text().compare(BoxAwd->itemText(BoxAwd->currentIndex())) == 0) { // AWD_col ?
            item->setText(celltext);                                            
            return;                                                             
         }
         QSqlField field("feld",QVariant::String);
         if(transl.getDbField(h->text()).compare("name") == 0) {                // name ?
             s = "SELECT oid FROM "+logFile+" WHERE id="+v->text();             // yes
             query.exec(s);                                                     // get OM_id
             query.next();
             qy = "UPDATE "+logFile+"om SET "+transl.getDbField( h->text())+"=";
             field.setValue(item->data(Qt::EditRole).toString());   
             qy += db.driver()->formatValue(field,false)+" WHERE omid="+query.value(0).toString();
         }
          else
            if(transl.getDbField(h->text()).compare("rufz") == 0) {             // callsign ?
              qy = "SELECT id,oid FROM "+logFile+" WHERE oid = ";               // yes
              qy += "(SELECT oid FROM "+logFile+" WHERE id="+v->text()+")";
              query.exec(qy);
              n = query.size();
              if(n == 1) {                                                      // if only 1 qso, update                   
                 query.next();
                 qy = "UPDATE "+logFile+"om SET "+transl.getDbField( h->text())+"=";
                 field.setValue(item->data(Qt::EditRole).toString());
                 qy += db.driver()->formatValue(field,false)+" WHERE omid="+query.value(1).toString();
              }
              else {                                                            // there are several qso                          
                qy = "INSERT INTO "+logFile+"om VALUES (0,'"+item->data(Qt::EditRole).toString();
                qy += "','')";                                                  // name missing ?
                query.exec(qy);                                                 // INSERT                      
                n = query.lastInsertId().toInt();
                                                                                // Update                                                          
                if(transl.getLogTable(transl.getDbField(h->text())) == 2) {     // qso_table
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
            if(transl.getDbField(h->text()).compare("band") == 0) {              // band ?
               field.setValue(item->data(Qt::EditRole).toString());              // yes
               s = db.driver()->formatValue(field,false);
               qy = "SELECT mband FROM wband WHERE mband="+s;                    // check band format     
               query.exec(qy);                                                   // if ERROR
	       //qDebug() << "Band:" << s;
               if( !query.size()) {                                   
                 QMessageBox::information( this, 
                 tr("Band Fehler"),tr("\nDer Bandname %1 ist unbekannt").arg(s), // format ERROR
                 QMessageBox::Ok | QMessageBox::Default,
                 QMessageBox::NoButton, QMessageBox::NoButton);
                 groupEditLine->setFocus();
                 return;
               }
               n = s.indexOf("'",0);                                             // band ok                               
               s.remove(n,1);
               n = s.indexOf("'",0);
               s.remove(n,1);
               if(transl.getLogTable(transl.getDbField(h->text())) == 2) {       // UPDATE qso_table
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
            if(transl.getLogTable(transl.getDbField(h->text())) == 2) {          // UPDATE qso
              qy = "UPDATE "+logFile+" SET "+transl.getDbField( h->text())+"=";
              field.setValue(item->data(Qt::EditRole).toString());
              qy += db.driver()->formatValue(field,false)+" WHERE id="+v->text();
            }
            else {                                                              // qsl          
              qy = "UPDATE "+logFile+"qsl SET "+transl.getDbField( h->text())+"=";
              field.setValue(item->data(Qt::EditRole).toString());
              qy += db.driver()->formatValue(field,false)+" WHERE qsoid="+v->text();
           }
          }
          query.exec(qy);                                                      // cell_update                                    
    }        
    groupEditLine->setFocus();
}

// -------------------------------------------------
// Report AWD row/col clicked
// -------------------------------------------------
void QtLog::checkAwdItem(QTableWidgetItem *ritem)
{
    if (ritem != reportTable->currentItem())             
       return;                                                // fremd ?
    awdTable->clearSelection();                               // nein; cleart vorsorglich selection
    col = reportTable->column ( ritem );                               // Spalte ermitteln
    QTableWidgetItem * h = reportTable->horizontalHeaderItem( col );   // arbeitsItem f.Spalte
    celltext = ritem->text();                                          // cell_text sichern
    
    if(h->text().compare(BoxAwd->itemText(BoxAwd->currentIndex())) == 0) { // ist es AWD_col ?
        //dock->setFloating(true);                           // wird leider mittig positioniert
        dock->show();                                        // ja: dockWidget
        row = reportTable->row(ritem);
        QTableWidgetItem *v = reportTable->verticalHeaderItem(row);
        if(eflg) {                                           // check if something has changed in table
          if(awdqId == v->text().toInt()) {                  // und row nicht verändert wurde
             eflg = 0;                                       // dann:
             awdqId = v->text().toInt();
             dock->hide();                                   // dockWidget abschalten       
             return;                                         // no                                
          }
        }
        eflg = SHOWAWD_TIME;                     // dockWidget wird weiter angezeigt -erneuere showtime
        awdqId = v->text().toInt();                          // save rowpos ( is qso_id )             
        awdTable->clear();                                   // awd_Tabelle kompl.löschen
	
        QTableWidgetItem *hItem0 = new QTableWidgetItem();   // header wieder aufbauen
        hItem0->setText(QApplication::translate("QtLog", "Type", 0, QApplication::UnicodeUTF8));
        awdTable->setHorizontalHeaderItem(0, hItem0);
        QTableWidgetItem *hItem1 = new QTableWidgetItem();
        hItem1->setText(QApplication::translate("QtLog", "Kenner", 0, QApplication::UnicodeUTF8));
        awdTable->setHorizontalHeaderItem(1,hItem1);
	
        // awd-table set up ( without value ) Box_AWD is conductive file    !!! ändern !!
        r = BoxAwd->currentIndex();                          // aktuelle Pos. der BoxAwd sichern
        n = BoxAwd->count();                                 // Anzahl vom anwender definierte AWD_typen
        awdTable->setRowCount(n);                            // awdTabellnelänge setzen
        row = 0;
        BoxAwd->setCurrentIndex(row);                        // auf 1.Pos BoxAwd einstellen
        while( row != n ) {                                  // und los
           c = 0;
           QTableWidgetItem *newItem0 = new QTableWidgetItem(tr("%1").arg((row)*(c))); //Type
           newItem0->setText(BoxAwd->currentText());         // Type
           awdTable->setItem(row,c++,newItem0);              // eintagen
           QTableWidgetItem *newItem1 = new QTableWidgetItem(tr("%1").arg((row)*(c))); //Kenner
           newItem1->setText("");                                                      // leerzeile
           awdTable->setItem(row++,c,newItem1);                                        // eintagen
           BoxAwd->setCurrentIndex(row);                                               // next          
        }
        BoxAwd->setCurrentIndex(r);                                  // put back the old index
	
        QTableWidgetItem *awdItem;                                   // new work item                   
        QString f;             // jetzt: awd_key und kenner lesen -kenner in awdTabelle einfügen
        QSqlQuery query;
        qy = "SELECT awtype,awkenner FROM "+logFile+"awd WHERE qid="+v->text();
        query.exec(qy);
        r = query.size();                                            // Anzahl Awards zum QSO
        if( r ) {                     // falls AWD's f.dieses Qso vorhanden, alle in awdTable eintragen
           while(query.next()) {
             r = 0;
             i = 0;
             s = query.value(r++).toString();            // key
             p = query.value(r).toString();              // kenner
	     
             awdItem = awdTable->item(i,0);
             f = awdItem->text();                        // key_name row, col
             while( i != n ) {                           // vergleiche ( n  mal mit key )
               if((s.compare(f)) == 0 ) {                // suche passenden key
                    c = 1;
                    QTableWidgetItem *kItem = new QTableWidgetItem(tr("%1").arg((i)*(c))); //Kenner
                    //kItem = awdTable->item(i,1);      // falls gefunden
                    kItem->setText(p);                  // kenner übernehmen
                    awdTable->setItem(i,1,kItem);       // kenner in TabellenSpalte 1 eintragen
                   break;                               // gefunden: ende
               }
               if( i == n -1) {                         // ERROR  type not found                
                    QMessageBox::information( this,
                    tr("AWD Fehler"),tr("\nDer AwdType ' %1 ' ist unbekannt.\nBitte anlegen.!").arg(s),
                    QMessageBox::Ok | QMessageBox::Default,
                    QMessageBox::NoButton, QMessageBox::NoButton);
                   return;                              // kein Vergleichs_type gefunden : ergebnis -1
                }
               i++;
               awdItem = awdTable->item(i,0);
               f = awdItem->text();                     // next key ( row, col )            
             }
           }
        }
    }
    else {                                              // AWD_modus wurde verlassen
      dock->hide();                                     // dockWidget abschalten
      eflg = 0;                            // da kein Zusammenhang zwischen AWD und QSO besteht
    }
    groupEditLine->setFocus();
}


// AWD_table col clicked
// AWD_name edited
// -----------------------------------------------
void QtLog::updateAwdItem(QTableWidgetItem *item)
{
     if (item != awdTable->currentItem())      // fremdes item - nicht bearbeiten
        return;
     if(awdTable->column( item ) == 0)         // col zero - do not edit
        return;         // col wird in der DB nicht verändert. In der Tabelle aber scheinbar ja:
                        
      row = awdTable->row( item );
      QTableWidgetItem * eItem = awdTable->item( row, 0); // eItem erzeugen und auf row,col positionieren
      QSqlQuery query;
      qy = "SELECT awkenner FROM "+logFile+"awd WHERE qid="+s.setNum(awdqId)+" AND awtype='";
      qy += eItem->text()+"'";
      query.exec(qy);
      if( query.size() == 1 ) {                           // UPDATE                     
         qy = "UPDATE "+logFile+"awd SET awkenner='"+item->data(Qt::EditRole).toString();
         qy += "' WHERE qid="+s.setNum(awdqId)+" AND awtype='"+eItem->text()+"'";
         query.exec(qy);
      }
      else {                                              // new AWD for this qso               
         qy = "SELECT rufz FROM "+logFile+","+logFile+"om WHERE id="+s.setNum(awdqId);
         qy += " AND omid=oid";
         query.exec(qy);
         query.next();                                    // hole qso_Id
         qy = "INSERT INTO "+logFile+"awd VALUES (0,"+s.setNum(awdqId);  // INSERT qso_Id
         qy += ",'"+query.value(0).toString();                           // CALLsign
         qy += "','"+eItem->text();                                      // awtype
         qy += "','"+item->data(Qt::EditRole).toString();                // awkenner
         qy += "')";
         query.exec(qy);
     }
     groupEditLine->setFocus();                    // besser wäre clear currentItem ???
     eflg = SHOWAWD_TIME;                          // show_zeit erneuern
}

// -------------------------------
// Message received : delete qso
// -------------------------------
void QtLog::deleteReportRow(int idNr)              // idNr ist Datensatz_Nr im VerticalHaeder
{
 int count;
     s = s.setNum(idNr);
     count = reportTable->rowCount();                             
     row = 0;
     QTableWidgetItem * v = reportTable->verticalHeaderItem(row); 
     while(row != count) {
         if(v->text().compare(s) == 0)
            break;                                 // gefunden     
         row++;
         v = reportTable->verticalHeaderItem(row);                
     }
     reportTable->removeRow(row);                  // entfern die Zeile
}


// only for debug

// Aufruf von : processPendingDatagram()
// Datagram_message von 'addqso' eingegangen : Datensatz wurde um 1 erweitert
// eine Zeile in der SELCTION hinzufügen
// Diese Funktion wird z.Z NICHT aufgerufen. !
// ---------------------------------
void QtLog::addReportRow(int idNr)
{ 
   qDebug() << "neuen Datensatz anhängen" << idNr;
   qDebug() << "das geht so nicht, da die Spaltenanzahl, -Namen, und -Sortierung nicht bekannt sind";
   qDebug() << "neue Idee: - Satz aus der DB lesen - aber mit welchen Querry ? MERKER f.last_QUERY ?";
}

// eine Zeile in der Report-Tabelle durch UPDATE bei 'addqso' ersetzen
// ??
// --------------------------------------------------------------------



