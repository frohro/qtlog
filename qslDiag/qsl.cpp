/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5.n : 30.10.2008, 02.05.09
********************************************************************** */

#include <QtGui>
#include <QtSql>
#include <QtNetwork>
#include <QTimer>
#include "qsl.h"
#include "../qtlogDiag/dirmngr.h"
#include "qslConf.h"
#include "adif.h"
#include "mkLquery.h"
#include "adif.h"
extern DirMngr dirMngr;

qsl::qsl(QWidget * parent) : QMainWindow(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
    setupUi(this);
    int n = settings.value("FontSize").toString().toInt();
    QFont font;
    font.setPointSize(n);
    setFont(font);
    
    connect(actionENDE, SIGNAL(triggered(bool)), this, SLOT(goExit()));
    connect(actionQslDrucken, SIGNAL(triggered(bool)), this, SLOT(qslDruckerAc()));
    connect(actionLogVerwaltung, SIGNAL(triggered(bool)), this, SLOT(qtlogAc()));
    connect(actionConfig, SIGNAL(triggered(bool)), this, SLOT(qslConfAc()));
    
    connect(actionLotwUpload, SIGNAL(triggered(bool)), this, SLOT(uploadLotwAc()));
    connect(actionLotwRecords, SIGNAL(triggered(bool)), this, SLOT(adifLotwQslRecordsAc()));
    connect(actionLotwReport, SIGNAL(triggered(bool)), this, SLOT(adifLotwImportAc()));
    
    connect(actionEqslUpload, SIGNAL(triggered(bool)), this, SLOT(uploadEqslAc()));
    connect(actionEqslRecords, SIGNAL(triggered(bool)), this, SLOT(adifEqslRecordsAc())); 
    connect(actionEqslImport, SIGNAL(triggered(bool)), this, SLOT(adifEqslImportAc()));
    
    connect(actionQSLbearbeitung, SIGNAL(triggered(bool)), this, SLOT(qslEingangAc()));
    connect(ButtonEnde, SIGNAL(clicked()), this, SLOT(goExit()));
    connect(ButtonQslCard, SIGNAL(clicked()), this, SLOT(showCardCb()));
    splitqsl = settings.value("SplitQsl").toInt();
    if(splitqsl)
       splitter->restoreState(settings.value("qsl/splitterSizes1").toByteArray());
    else
      splitter->restoreState(settings.value("qsl/splitterSizes0").toByteArray());
    resize(settings.value("qsl/Size",sizeHint()).toSize());
    restoreState(settings.value("qsl/Properties").toByteArray());
    awdTable->setColumnWidth(0,80);       
    awdTable->setColumnWidth(1,80);       

    db = QSqlDatabase::addDatabase(settings.value("qsqlDatabase").toString()); 
    db.setHostName(settings.value("host").toString());   
    db.setDatabaseName(settings.value("dbname").toString());
    db.setUserName(settings.value("dbuser").toString());
    db.setPassword(settings.value("dbpasswd").toString());
    if(!db.open()) {
       qDebug() << db.lastError();                       
    }
     udpSocket = new QUdpSocket(this);                   
     udpSocket->bind(QHostAddress::LocalHost,45504);
     connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
     timer = new QTimer(this);                            
     connect(timer,SIGNAL(timeout()), this, SLOT(timerServiceUpload()));
     mtimer = new QTimer(this);
     connect(mtimer,SIGNAL(timeout()), this, SLOT(timertransferUrl()));
     QSqlQuery query;                                     
     BoxAwd->clear();                                     
     qy = "SELECT atype FROM wawdlist WHERE aset !='0' ORDER BY aset";
     query.exec(qy);
     n = 0;
     while(query.next())
       BoxAwd->insertItem(n++, query.value(0).toString()); 
     BoxAwd->setCurrentIndex(settings.value("Awd").toString().toInt()); 
     logFile = settings.value("logfile").toString();
     s = logFile+"   ";
     lableLogbook->setText(s);
     QDate heute = QDate::currentDate();                
     dateEditHeute->setDate(heute);                     
     s = settings.value("LotLastUp").toString();
     QDate d = QDate::fromString (s,"yyyy-MM-dd");
     dateEditLast->setDate(d);

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
     transl.installHashTableBand();             
     transl.installHashTable();                 
     adi.setBandTable();
     
     work = 0;
     upMgr = 0;
     qslvia = settings.value("QSLvia").toString().toInt(); 
     
     switch(qslvia) {
     case 1 :
     case 2 :
           BoxQslvia->setCurrentIndex( 0 );       
           dateEditHeute->setEnabled(false);
           dateEditLast->setEnabled(false);
           labelUp->setEnabled(FALSE);
           checkBoxZeitraum->setEnabled(FALSE);
           ButtonUnConfirmed->setText("QSL storniert");
           break;
     case 3 :
     case 4 :
           BoxQslvia->setCurrentIndex( 1 );      
           dateEditHeute->setEnabled(false);
           dateEditLast->setEnabled(false);
           labelUp->setEnabled(TRUE); 
           checkBoxZeitraum->setEnabled(TRUE);
           ButtonNotSent->setText(QString::fromUtf8("Nicht Gesendet"));  
           ButtonUnConfirmed->setText(QString::fromUtf8("Gesendet"));    
           work = 1;              
           timer->start(800);     
           break;
     }
     buildHeaderUnreceived();
 
     
     connect (qslTable, SIGNAL(itemClicked(QTableWidgetItem *)), this, SLOT(checkAwdItem(QTableWidgetItem *)));
     connect (awdTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateAwdItem(QTableWidgetItem * )));
     connect (qslTable, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(updateQslItem(QTableWidgetItem *)));
     connect (wLineEdit, SIGNAL(returnPressed()), this,SLOT(sucheCb()));
     connect (wLineEdit, SIGNAL(textEdited(QString)), this,SLOT(wLineEditedTextCb(QString)));
     connect (BoxAwd, SIGNAL(currentIndexChanged(int)), this,SLOT(awdBoxchangedCb(int)));
     connect (BoxQslvia, SIGNAL(currentIndexChanged(int)), this,SLOT(qslviaBoxchangedCb(int)));
     connect (checkBoxZeitraum, SIGNAL(pressed()), this, SLOT(checkBoxZeitraumChanged()));
     connect (ButtonUnReceived, SIGNAL(pressed()), this, SLOT(UnreceivedCb()));         
     connect (ButtonNotSent, SIGNAL(pressed()), this, SLOT(NotSentCb()));               
     connect (ButtonUnConfirmed, SIGNAL(pressed()), this, SLOT(UnConfirmedCb()));       
     connect (ButtonReceived, SIGNAL(pressed()), this, SLOT(ReceivedCb()));             
     tabRow = 0;
     tabLen = 0;
     progressBar->setRange(tabRow,tabLen);
     progressBar->setValue (10);                
     progressBar->hide();
     labelRun->hide();
     
     cardview = 0;
     sflg = 0;
     forUp = 0;
     wLineEdit->setFocus(); 
     awdTable->setEnabled(false);
}

qsl::~qsl()
{
   if(splitqsl)
     settings.setValue("qsl/splitterSizes1", splitter->saveState());  
   else
     settings.setValue("qsl/splitterSizes0", splitter->saveState());  
   settings.setValue("SplitQsl",splitqsl);
   settings.setValue("qsl/Size",size());
   settings.setValue("qsl/Properties",saveState());
   writeColumnWidth();                                               
   if(upMgr)
     processWriteDatagram(-1, "-1", 45506);
   if(cardview)
     processWriteDatagram(-1, "-1", 45508);
}

void qsl::goExit()
{
  qApp->quit();
}

void qsl::qslConfAc()
{
   qslConf *confL = new qslConf();    
   confL->exec();
   s = confL->s;
   QDate d = QDate::fromString (s,"yyyy-MM-dd");
   dateEditLast->setDate(d);
}

void qsl::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_F10 :                           
             goExit();
           break;
   }
   qslTable->clearSelection(); 
}

void qsl::qslEingangAc()
{
   settings.setValue("Val","QslEingang");
   StartProcess("hilfedb &");
}

void qsl::qtlogAc()
{
   StartProcess("qtlog &");                    
}

void qsl::qslDruckerAc()
{
   StartProcess("qslprintdb &");               
}


void qsl::writeColumnWidth()
{
  int i, n, x;
  QString a;
     QTableWidgetItem * cItem;
     QSqlQuery query;
     i = qslTable->columnCount();                    
     n = 0;
     while( n != i ) {
         cItem = qslTable->horizontalHeaderItem(n);  
         s = cItem->text();                          
         x = qslTable->columnWidth(n++);             
         qy = "UPDATE refnamen SET br2="+a.setNum(x)+" WHERE refnam='"+s+"'";
         query.exec(qy);
    }
    db.close();                                      
}


void qsl::awdBoxchangedCb(int idx)
{
   int x;
   x = idx;
   if(sflg)
     return; 
   sucheCb();
}


void qsl::checkBoxZeitraumChanged()
{
   if(BoxQslvia->currentIndex() > 0) {              
    if(checkBoxZeitraum->isChecked() == FALSE) {
      if(BoxQslvia->currentIndex() == 1) {          
        s = settings.value("EqslLastUp").toString();
        QDate d = QDate::fromString (s,"yyyy-MM-dd");
        dateEditLast->setDate(d);
      }
      checkBoxZeitraum->setText("Upload  mit Zeitraum");
      dateEditHeute->setEnabled(TRUE);
      dateEditLast->setEnabled(TRUE);
    }
    else {
     checkBoxZeitraum->setText("Upload ohne Zeitraum");
     dateEditHeute->setEnabled(FALSE);
     dateEditLast->setEnabled(FALSE);
    }
  }
}


void qsl::qslviaBoxchangedCb(int i)
{
  switch( i ) {
    case 0 :                                    
        if( upMgr ) {                           
            processWriteDatagram(-1, "-1", 45506);
            upMgr = 0;
        }
       dateEditHeute->setEnabled(false);
       dateEditLast->setEnabled(false);
       labelUp->setEnabled(FALSE);
       checkBoxZeitraum->setEnabled(FALSE);
       checkBoxZeitraum->setChecked(FALSE);
       checkBoxZeitraum->setText("Upload  ohne Zeitraum");
       ButtonUnReceived->setText(QString::fromUtf8("Nicht erhalten"));     
       ButtonNotSent->setText(QString::fromUtf8("Nicht gedruckt"));        
       ButtonUnConfirmed->setText(QString::fromUtf8("Qsl storniert"));     
       buildHeaderUnreceived();
      break;
    case 1 :                                     
        work = 1;                                
        timer->start(800);                       
        dateEditHeute->setEnabled(false);
        dateEditLast->setEnabled(false);
        labelUp->setEnabled(TRUE);
        checkBoxZeitraum->setEnabled(TRUE);
        checkBoxZeitraum->setChecked(FALSE);
        checkBoxZeitraum->setText("Upload  ohne Zeitraum");
        ButtonUnReceived->setText(QString::fromUtf8("Nicht bestätigt"));    
        ButtonNotSent->setText(QString::fromUtf8("Nicht Gesendet"));        
        ButtonUnConfirmed->setText(QString::fromUtf8("Gesendet"));          
        buildHeaderUnreceived();
      break;
    case 2 :                                    
        if( upMgr ) {                           
            processWriteDatagram(-1, "-1", 45506);
            upMgr = 0;
        }
        dateEditHeute->setEnabled(true);
        dateEditLast->setEnabled(true);
        labelUp->setEnabled(TRUE);
        s = settings.value("LotLastUp").toString();
        QDate d = QDate::fromString (s,"yyyy-MM-dd");
        dateEditLast->setDate(d);
        checkBoxZeitraum->setEnabled(TRUE);
        checkBoxZeitraum->setChecked(TRUE);
        checkBoxZeitraum->setText("Upload  mit Zeitraum");
        ButtonUnReceived->setText(QString::fromUtf8("Nicht bestätigt"));    
        ButtonNotSent->setText(QString::fromUtf8("Nicht Gesendet"));        
        ButtonUnConfirmed->setText(QString::fromUtf8("Gesendet"));          
        buildHeaderUnreceived();
      break;
  }
}


void qsl::UnreceivedCb()
{
    if(BoxQslvia->currentIndex() == 0)          
       buildHeaderUnreceived();                 
    else 
    if(BoxQslvia->currentIndex() == 1)          
       buildHeaderUnreceived();                 
    else 
    if(BoxQslvia->currentIndex() == 2)          
      buildHeaderUnreceived();                  
}


void qsl::NotSentCb()
{
    if(BoxQslvia->currentIndex() == 0)          
      buildHeaderNotSend();                     
    else
    if(BoxQslvia->currentIndex() == 1) {        
      buildHeaderNotSend();                     
      ButtonNotSent->setText(QString::fromUtf8("Nicht Gesendet"));    
    }
    else
    if(BoxQslvia->currentIndex() == 2) {        
      buildHeaderNotSend();                     
      ButtonNotSent->setText(QString::fromUtf8("Nicht Gesendet")); 
    }
}


void qsl::UnConfirmedCb()
{
    if(BoxQslvia->currentIndex() == 0)          
       buildHeaderNotConfirmed();               
    else 
    if(BoxQslvia->currentIndex() == 1)          
       buildHeaderNotConfirmed();               
    else
    if(BoxQslvia->currentIndex() == 2)          
       buildHeaderNotConfirmed();               
}


void qsl::ReceivedCb()
{
    if(BoxQslvia->currentIndex() == 0)          
       buildHeaderReceived();                   
    else 
    if(BoxQslvia->currentIndex() == 1)          
       buildHeaderReceived();                   
    else
    if(BoxQslvia->currentIndex() == 2)          
       buildHeaderReceived();                   
}



void qsl::wLineEditedTextCb(QString str)
{
  i = str.count();
     if( i >= 2 ) {
       if(i > strlen.count()) {
          strlen = str; 
          sucheCb();
       }
       else
         strlen = str;
     }
}

void qsl::setpalettgruen(QCheckBox *cBox)
{
    QPalette palette2;
    QBrush brush4(QColor(152, 232, 123, 255));
    brush4.setStyle(Qt::SolidPattern);
    palette2.setBrush(QPalette::Active, QPalette::Button, brush4);
    palette2.setBrush(QPalette::Inactive, QPalette::Button, brush4);
    palette2.setBrush(QPalette::Disabled, QPalette::Button, brush4);
    cBox->setPalette(palette2);
}

void qsl::setpalettgrau(QCheckBox *cBox)
{
    QPalette palette3;
    QBrush brush5(QColor(211, 210, 210, 255));
    brush5.setStyle(Qt::SolidPattern);
    palette3.setBrush(QPalette::Active, QPalette::Button, brush5);
    palette3.setBrush(QPalette::Inactive, QPalette::Button, brush5);
    palette3.setBrush(QPalette::Disabled, QPalette::Button, brush5);
    cBox->setPalette(palette3);
}



void qsl::readData(QString qy, int wPos, int iflg)
{
  QString b, s, cept;
  int i, col,row,z,r,c;
      qslTable->clearSelection();                           
      QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
      QSqlQuery query;
      query.exec(qy);                                        
      i = query.size();                                      
      tabLen = i;
      col = query.record().count();                          
      qslTable->setRowCount(i);                              
      qslTable->setColumnCount(col -2);                      
                                            //Header aufbauen - ist jetzt statisch und nicht änderbar
      awdPos = wPos;                                         
      n = 0;
      s = query.record().fieldName(n++);                     
      qslTable->setColumnWidth(0,80);                        
      s = query.record().fieldName(n++);                     
      i = 2;
      while(i != col ) {                                     
        s = query.record().fieldName(i);                     
        QTableWidgetItem * cItem = new QTableWidgetItem(tr("%1").arg((i)));
        if(!(QString::compare(s,"band"))) blfg = i -1;       
        if(i  == awdPos) {                                   
           b = "AWD";
        }
        else {
          b = transl.getUsrField(s);                         
          cItem->setText(b);
        }
        qslTable->setHorizontalHeaderItem(i -2,cItem);
        qslTable->setColumnWidth(i -2,transl.getFieldBr2(b));
        i++;
      }
      QTableWidgetItem * h = qslTable->horizontalHeaderItem(awdPos -2); 
      h->setText(BoxAwd->currentText());
      qslTable->setHorizontalHeaderItem(awdPos -2,h);                   
                                                                 
      row = 0;                                                   
      while(query.next()) {                                      
        z = 0;                                                   
        r = 0;                                                   
        i = 0;                                                   
        s = query.value(i++).toString();                         
        QTableWidgetItem *rowItem = new QTableWidgetItem(tr("%1").arg((r++)*(z++))); //Id_item erzeugen
        rowItem->setText(s); 
        qslTable->setVerticalHeaderItem(row,rowItem);            
        c = 0;
        while( c != col -2 ) {                                   
          if(i == 1)                                             
            cept = query.value(i++).toString();
          else {
           QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg((row)*(c))); 
           if(c == 0) {
               cept += query.value(i++).toString();
               newItem->setText(cept);                          
           }
          else 
           if( c == 1)                                          
              newItem->setText(transl.getMyband(query.value(i++).toString()));  
          else
            newItem->setText(query.value(i++).toString());      
          if( c == 7 || c == 8 || c == 9 || c == 10 || c == 11)
            newItem->setTextAlignment(Qt::AlignCenter);         
          qslTable->setItem(row,c++,newItem);                   
        }
       }
       row++;                                                   
     }
     rcount = row;
     labelCnt->setText(s.setNum(row)+"   QSO");
     QApplication::restoreOverrideCursor();
     wLineEdit->setFocus(); 
     sflg = 0;
     if(iflg == 1) {
       s = "\n"+settings.value("Val").toString()+"  Neue QSL erhalten.";
       QMessageBox::information( this,tr("Info Download"),( s ),
       QMessageBox::Ok);
       settings.setValue("Val",0);                                
     }
     else 
     if(iflg == 2) {
       QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));  
       
       
       
       
     }
}






void qsl::checkAwdItem(QTableWidgetItem *eitem)
{
    if (eitem != qslTable->currentItem())               
       return;
    awdTable->clearSelection();                         
    col = qslTable->column ( eitem );                   
    celltext = eitem->text();                           
    if(col >= 7 && BoxQslvia->currentIndex() >= 1 ) {
        s = QString::fromUtf8("\nLotw oder eQSL  * Items *\nWerden nur durch Upload oder Download gesetzt.\n\nUpload eQSL   -  klicke CAll\nUpload LotW    - waehle im Headermenue Upload");
       QMessageBox::information( this,tr("Check report"),s, 
       QMessageBox::Ok);
       qslTable->clearSelection();    
      return;
    }
    row = qslTable->row( eitem );                       
    if( col == awdPos -2 ) {                            
       sflg = 1;                                        
       if( !splitqsl ) {                                
          splitqsl = 1;
          splitter->restoreState(settings.value("qsl/splitterSizes1").toByteArray()); 
       }
       else {
        splitqsl = 0;                                                                
        splitter->restoreState(settings.value("qsl/splitterSizes0").toByteArray());  
        return;
       }
       awdTable->setEnabled(true);                      
       row = qslTable->row(eitem);
       QTableWidgetItem * v = qslTable->verticalHeaderItem(row);
       awdqId = v->text().toInt();                      
       
       awdTable->clear();                               
       QTableWidgetItem *hItem0 = new QTableWidgetItem();
       hItem0->setText(QApplication::translate("QtLog","Type", 0, QApplication::UnicodeUTF8));
       awdTable->setHorizontalHeaderItem(0, hItem0);
       QTableWidgetItem *hItem1 = new QTableWidgetItem();
       hItem1->setText(QApplication::translate("QtLog","Kenner", 0, QApplication::UnicodeUTF8));
       awdTable->setHorizontalHeaderItem(1,hItem1);
                                           
       r = BoxAwd->currentIndex();                     
       n = BoxAwd->count();                            
       awdTable->setRowCount(n);                       
       row = 0;
       BoxAwd->setCurrentIndex(row);                   
       while( row != n ) {                             
          c = 0;
          QTableWidgetItem *newItem0 = new QTableWidgetItem(tr("%1").arg((row)*(c))); 
          newItem0->setText(BoxAwd->currentText());                         
          awdTable->setItem(row,c++,newItem0);                              
          QTableWidgetItem *newItem1 = new QTableWidgetItem(tr("%1").arg((row)*(c))); 
          newItem1->setText("");                                            
          awdTable->setItem(row++,c,newItem1);                              
          BoxAwd->setCurrentIndex(row);                                     
       }
       BoxAwd->setCurrentIndex(r);                                          
       QTableWidgetItem * aItem;                
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
            aItem = awdTable->item(i,0);             
            f = aItem->text();
            while( i != n ) {                        
              if((s.compare(f)) == 0 ) {             
                 c = 1;
                 QTableWidgetItem *kItem = new QTableWidgetItem(tr("%1").arg((row)*(c))); 
                 kItem->setText(p);                  
                 awdTable->setItem(i, 1, kItem);     
                 break;                              
              }
             if( i == n -1 ) {                       
                f = "\nDer AwdType ' "+s+" ' ist unbekannt.\nBitte anlegen.";
                QMessageBox::information( this,
                tr("AWD Fehler"),f,
                QMessageBox::Ok | QMessageBox::Default,
                QMessageBox::NoButton, QMessageBox::NoButton);
               return;                               
             }
             i++;
             aItem = awdTable->item(i,0);
             f = aItem->text();                      
          }
        }
      }
    }
    else {
     if( splitqsl ) {
        splitqsl = 0;
        splitter->restoreState(settings.value("qsl/splitterSizes0").toByteArray()); 
     }
     if( col == 8 ) {                               
        QSqlQuery query;
        awdTable->setEnabled(false);
        QStringList items;
        s = QString::fromUtf8("R   gewünscht");
        items << tr("Y   erhalten") << tr("I   Ignore") << s << tr("-     keine.");
        bool ok;
        QString litem = QInputDialog::getItem(this, tr("QSL"),tr("  QSL Karte :"), items, 0, false, &ok);
        if (ok && !litem.isEmpty()) {
          eitem->setText(litem.left(1));            
        }
     }
    else 
     if( col == 9 ) {                               
        awdTable->setEnabled(false);
        QStringList items;
        items << tr("R   angefordert") <<("I   Ignore") << tr("-    keine");
        bool ok;
        QString litem = QInputDialog::getItem(this, tr("QSL"),tr("  QSL Karte :"), items, 0, false, &ok);
        if (ok && !litem.isEmpty()) {
          eitem->setText(litem.left(1));
        }
     }
    else
     if( col == 10 ) {                               
        awdTable->setEnabled(false);
        QStringList items;
        s = QString::fromUtf8("B  Büro");
        items << s << tr("     nicht Drucken");
        bool ok;
        QString litem = QInputDialog::getItem(this, tr("QSL"),tr("  Drucken via :"), items, 0, false, &ok);
        if (ok && !litem.isEmpty()) {
          eitem->setText(litem.left(1));
        }
     }
    else
    if(col == 0 && BoxQslvia->currentIndex() == 1 ) { 
        awdTable->setEnabled(false);
        QStringList items; 
        s = QString::fromUtf8("U   - upload");
        items << s << tr("-    storno");
        bool ok;
        QString litem = QInputDialog::getItem(this, tr("QSL"),tr("  Send eQsl.cc"), items, 0, false, &ok);
        if (ok && !litem.isEmpty())
           eitem->setText(litem.left(1));
     }
    else
     awdTable->setEnabled(false);
   wLineEdit->setFocus(); 
  }
  sflg = 0;                                        
}




void qsl::updateAwdItem(QTableWidgetItem *item)
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
     awdTable->clearSelection(); 
     wLineEdit->setFocus(); 
}




void qsl::updateQslItem( QTableWidgetItem * item )
{
    if (item != qslTable->currentItem())                    
         return;
    QString datum;
    int cntb = 0;
    if ( !sflg ) {                                          
         QString La, mod, band, Id, qsl;
         row = qslTable->row( item );                       
         col = qslTable->column ( item );                   
         QTableWidgetItem * h = qslTable->horizontalHeaderItem( col ); 
         QTableWidgetItem * v = qslTable->verticalHeaderItem ( row );
         if(h->text().compare(BoxAwd->itemText(BoxAwd->currentIndex())) == 0) 
            return;                                                           
         
         qsl = item->data(Qt::EditRole).toString();        
         if( col == 8 ) {                                  
            QSqlQuery query; 
            item = qslTable->item(row,13);                 
            La = item->text();
            item = qslTable->item(row,2);                  
            mod = item->text();
            item = qslTable->item(row,1);                  
            band = item->text();
            item = qslTable->item(row,11);                 
            QDate heute = QDate::currentDate();            
            datum = heute.toString("yyyy-MM-dd");
            qy = "SELECT cntb FROM "+logFile+"dx ";
            qy += "WHERE dxpref='"+La+"' AND mode='"+mod+"'"; 
            query.exec(qy);
            n = query.size();                                 
            if( n ) {
               query.next();
               cntb = query.value(0).toInt();                 
            }
          
            if(qsl.compare("Y") == 0) {                      
               qy = "SELECT qslRvia FROM "+logFile+" WHERE id="+v->text();
               QSqlQuery query(qy);
               query.next();
               s = query.value(0).toString();                
               if(s.indexOf("B") == -1)                      
                    s += "B";                                
              item->setText(s);
              qy = "UPDATE "+logFile+" SET qslr='Y',qslrd='"+datum+"',qslRvia='"+s+"' WHERE id="+v->text();
              query.exec(qy);
              if( n ) {                                      
                qy = "UPDATE "+logFile+"dx SET cntb=";       
                qy += s.setNum(++cntb);
                qy += ", b"+transl.getSysband(band);         
                qy += "='x' WHERE dxpref='"+La+"' AND mode='"+mod+"'";
                QSqlQuery query(qy);
              }
              else {                                         
                qy = "INSERT INTO "+logFile+"dx VALUES ('";
                qy += La;                                    
                qy += "','"+mod;                             
                qy += "',1,0,";                              
                qy += "'','','','','','','','','','','','','','','','','','','')";
                query.exec(qy);
                qy = "UPDATE "+logFile+"dx SET b"+transl.getSysband(band);
                qy += "='x' WHERE dxpref='"+La+"' AND mode='"+mod+"'";
                query.exec(qy);
              }
            }
            else                                            
             if(qsl.compare("R") == 0) {                    
                qy = "UPDATE "+logFile+"dx SET cntb=";      
                qy += s.setNum(n);
                qy += ", b"+transl.getSysband(band);
                qy += "='x' WHERE dxpref='"+La+"' AND mode='"+mod+"'";
                query.exec(qy);                             
                qy = "SELECT qslRvia FROM "+logFile+" WHERE id="+v->text();
                query.exec(qy); 
                query.next();
                p = query.value(0).toString(); 
                p.remove(QChar('B'), Qt::CaseInsensitive);  
                item->setText(p);
                s = "0000-00-00";
                qy = "UPDATE "+logFile+" SET qslr='R',qslrd='"+s+"',qslRvia='"+p+"' WHERE id="+v->text();
                QSqlQuery query(qy);
                cntb--;
                qy = "UPDATE "+logFile+"dx SET cntb="+s.setNum(cntb);
                if(cntb == 0)
                  qy += ",b"+transl.getSysband(band)+"=''";    
                qy += " WHERE dxpref='"+La+"' AND mode='"+mod+"'";
                query.exec(qy);
             }
             else {                                         
               qy = "UPDATE "+logFile+" SET "+transl.getDbField( h->text())+"='";
               qy += qsl;
               qy += "',qslrd='0000-00-00' WHERE id="+v->text();
               query.exec(qy);
             }
        }
       else
        if(col == 0 && BoxQslvia->currentIndex() == 1 ) {  
           if(qsl.compare("U") == 0) {                     
             item->setText(celltext); 
             
             processWriteDatagram(2, v->text(), 45506);    
           }
           else
             if(qsl.compare("-") == 0) {                   
               item->setText(celltext);
               qy = "UPDATE "+logFile+"qsl SET eqslr='-',eqsls='-' WHERE qsoid="+v->text();
               QSqlQuery query(qy);
             }
        }
       else 
        if( col == 9 && BoxQslvia->currentIndex() == 0 ) { 
          qsl = item->data(Qt::EditRole).toString();       
	  if((qsl.compare("-")==0) || (qsl.compare("I")==0)) {  
            qy = "UPDATE "+logFile+" SET qsls='"+qsl+"',qslsd='0000-00-00',prflg='' WHERE id="+v->text();
            QSqlQuery query(qy);
            QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(pow(row,++col)));
            newItem->setText("");                         
            newItem->setTextAlignment(Qt::AlignCenter);
            qslTable->setItem(row,col,newItem); 
          }
          else 
            if(qsl.compare("R") == 0) {                   
               qy = "UPDATE "+logFile+" SET qsls='R',qslsd='0000-00-00',prflg='B' WHERE id="+v->text();
               QSqlQuery query(qy);
             
               QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(pow(row,++col)));
               newItem->setText("B");                     
               newItem->setTextAlignment(Qt::AlignCenter);
               qslTable->setItem(row,col,newItem);
            }
        }
        else {                                                       
          QSqlField field("feld",QVariant::String);
          if(transl.getDbField(h->text()).compare("name") == 0) {    
            s = "SELECT oid FROM "+logFile+" WHERE id="+v->text();   
            QSqlQuery query(s);                                      
            query.next();
            qy = "UPDATE "+logFile+"om SET "+transl.getDbField( h->text())+"=";
            field.setValue(item->data(Qt::EditRole).toString());
            qy += db.driver()->formatValue(field,false)+" WHERE omid="+query.value(0).toString();
          }
          else {                                                           
            if(transl.getDbField(h->text()).compare("contactdOp") == 0) {  
              qy = "UPDATE "+logFile+"qsl SET ";
              qy +=transl.getDbField( h->text())+"=";
              field.setValue(item->data(Qt::EditRole).toString());
              qy += db.driver()->formatValue(field,false)+" WHERE qsoid="+v->text();
            }
           else
            if(transl.getDbField(h->text()).compare("band") == 0) {   
              field.setValue(item->data(Qt::EditRole).toString());    
              s = db.driver()->formatValue(field,false);
              qy = "SELECT mband FROM wband WHERE mband="+s;          
              QSqlQuery query(qy);
              if( !query.size()) {                                    
                 QString f = "\nDer Bandname  "+s+"  ist unbekannt";  
                 QMessageBox::information( this,
                 tr("Band Fehler"),f,
                 QMessageBox::Ok | QMessageBox::Default,
                 QMessageBox::NoButton, QMessageBox::NoButton);
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
            }
            else {                                                         
             qy = "UPDATE "+logFile+" SET ";
             qy +=transl.getDbField( h->text())+"=";
             field.setValue(item->data(Qt::EditRole).toString());
             qy += db.driver()->formatValue(field,false)+" WHERE id="+v->text();
            }
          }
          QSqlQuery query(qy);                                              
      }
    }
    qslTable->clearSelection(); 
    wLineEdit->setFocus(); 
}



void qsl::sucheCb()
{
     sflg = 1;                                                         
     ButtonUnReceived->setPalette( QPalette(QColor(224, 224, 224)));   
     ButtonNotSent->setPalette( QPalette(QColor(224, 224, 224)));      
     ButtonUnConfirmed->setPalette( QPalette(QColor(224, 224, 224)));  
     selectedbut = 5;
     qy = "SELECT id,cept,rufz,band,mode,name,qth,day,btime,";
     qy += "(SELECT awkenner FROM "+logFile+"awd WHERE awtype ='";
     qy += BoxAwd->currentText()+"' AND id=qid),";
     if(BoxQslvia->currentIndex() == 0)                                
        qy += "qslr,qsls,prflg,qslRvia,qslmgr,dxcc,loc,contactdOp";
     else 
      if(BoxQslvia->currentIndex() == 1)                               
        qy += "eqslr,eqsls,qslRvia,ag,eqslrd,eqslsd,dxcc,loc,contactdOp,qslmsg";
     else
      if(BoxQslvia->currentIndex() == 2)                               
        qy += "lotqslr,lotqsls,qslRvia,lotqslrd,lotqslsd,dxcc,loc,contactdOp";
     qy += " FROM "+logFile+"om LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id)";
     qy += "WHERE ";
     qy += transl.getDbField(wBox->currentText());                
     qy += " LIKE '";
     qy += wLineEdit->text()+"%' ORDER BY rufz";                  
     awdPos = 9;
     readData(qy,awdPos,0);                                        
     wLineEdit->setFocus(); 
     sflg = 0;
     awdTable->setEnabled(false);
}



void qsl::showCardCb()
{
    if( !cardview ) {                             
      StartProcess("qslcarddb &");
      timer->start(800);                          
      work = 6;
      cardview = 1;
      return;
    }
    int row = qslTable->currentRow();
    if(row == -1) return;
    QTableWidgetItem * v = qslTable->verticalHeaderItem ( row );
    processWriteDatagram(1, v->text(), 45508);    
}
//-----------------------------------------------
void qsl::timerServiceUpload()
{
    timer->stop();
    QString s;
    switch(work) {
     case 1 : 
             if(settings.value("EqslRealtime").toInt()) {   
               if(settings.value("EqslPasswd").toString().count() != 0) {
                  s = "uploadEqsldb 45506 &";
                  StartProcess( s );                        
                  upMgr = 1;
                }
              }
              else {
                s ="\nDer Realtime UploadMgr konnte nicht gestartet werden,\nDer Internet Zugang ist nicht konfiguriert.";
                QMessageBox::information( this,tr("Message"),s, 
                QMessageBox::Ok);
             }
             break;
     case 6 : 
               int row = qslTable->currentRow();
               if(row == -1) 
                 break;
               QTableWidgetItem * v = qslTable->verticalHeaderItem ( row );
               processWriteDatagram(1, v->text(), 45508);   
            break;
     }
}
