/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#include <QApplication>
#include <QtGui>
#include <QtSql>
#include <QtNetwork>
#include "admin.h"
#include "../qtlogDiag/dirmngr.h"
#include "../qtlogDiag/version.h"
extern DirMngr dirMngr;

admin::admin(QWidget * parent) : QMainWindow(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
   setupUi(this);
   n = settings.value("FontSize").toString().toInt();
   QFont font;
   font.setPointSize(n);                         
   setFont(font);
   connect(actionEnde, SIGNAL(triggered(bool)), this, SLOT(goExit()));
   connect(actionAdif_Export, SIGNAL(triggered(bool)), this, SLOT(adifExpCb()));
   connect(actionAdif_Import, SIGNAL(triggered(bool)), this, SLOT(adifImpCb()));
   connect(actionDump, SIGNAL(triggered(bool)), this, SLOT(dumpCb()));
   connect(actionRefNamen, SIGNAL(triggered(bool)), this, SLOT(refNamenCb()));
   connect(actionHilfe, SIGNAL(triggered(bool)), this, SLOT(getHilfeCb()));
   connect(pushButtonEnde, SIGNAL(clicked()), this, SLOT(goExit()));
   connect(stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(stackedWidgetCb(int)));
   connect(checkBoxLog, SIGNAL(pressed()), this, SLOT(toggelLogBoxCb()));
   connect(checkBoxDb, SIGNAL(pressed()), this, SLOT(toggelDbBoxCb()));
   connect(checkBoxQslMode, SIGNAL(pressed()), this, SLOT(toggelQslModeCb()));
   
   connect(logList, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(logListCb(QTreeWidgetItem*,int)));
   connect(sysList, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(sysListCb(QTreeWidgetItem*,int)));
   connect(RefTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateRefItemCb(QTableWidgetItem*)));
   connect(RefTable, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(saveValCb(QTableWidgetItem*)));
   connect(customsTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateCustomItemCb(QTableWidgetItem*)));
   connect(customsTable, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(saveValCb(QTableWidgetItem*)));
   connect(ButtonDumpDir, SIGNAL(clicked()), this, SLOT(getSaveDirCb()));
   connect(ButtonAdifDir, SIGNAL(clicked()), this, SLOT(getAdifDirCb()));
   connect(ButtonDump, SIGNAL(clicked()), this, SLOT(dbDump()));
   connect(ButtonRestore, SIGNAL(clicked()), this, SLOT(dbRestore()));
   connect(ButtonAdifExport, SIGNAL(clicked()), this, SLOT(adifExport()));
   connect(ButtonAdifImport, SIGNAL(clicked()), this, SLOT(adifImport()));
   s = settings.value("dbname").toString();
   s += " Rel.: ";
   s += RELEASE;
   
   label_db->setText(s);
   label_dbase->setText(s);
   dirsel = 0;
   setupAction();
   lItem = 0;
   sItem = 0;
   udpSocket = new QUdpSocket(this);  
   ltreeWidget = logList;
}
void admin::setupAction()
{
   readSettings();
   logList->setColumnWidth(0,60);     
   logList->setColumnWidth(1,60);     
   logList->setColumnWidth(2,40);     
   sysList->setColumnWidth(0,120);
   
   RefTable->setColumnWidth(0,100);
   RefTable->setColumnWidth(1,180);
   RefTable->setColumnWidth(2,160);
   RefTable->setColumnWidth(3,60);
   RefTable->setColumnWidth(4,60);
   RefTable->setColumnWidth(5,60);
   customsTable->setColumnWidth(0,80);
   customsTable->setColumnWidth(1,190);
   if(settings.value("dbconf").toInt() == 0) {      
     s = tr("\nDas Runtime-Flag 'dbconf' steht auf Null.\nDas Flag ist beschaedigt, ");
     s += tr("oder QtLog ist nicht initialisiert\n");
     s += tr("Das Programm schaltet ab");
     QMessageBox::information( this,
         tr("QtLog INFO"),
         tr(s.toAscii()),
         QMessageBox::Ok | QMessageBox::Default,
         QMessageBox::NoButton, QMessageBox::NoButton);
     exit(0);                                      
   }
   else {
     openDatabase();                               
     showTables(1);                                
   }
   if(settings.value("Val").toInt() == 1) {        
       dirsel = 0;                                 
       adifExpCb();
   }
   else
    if(settings.value("Val").toInt() == 2) {       
       dirsel = 1;                                 
       adifImpCb();
    }
   QSqlQuery query;
   qy = "SELECT rigtype FROM wrig";
   query.exec(qy);
   while(query.next()) {
       n = 0;
       comboBoxRig->addItem(query.value(n++).toString());
   }
   qy = "SELECT lid,logging FROM tlocation WHERE logging != ''";
   query.exec(qy);
   while(query.next()) {
       i = 0;
       n = 0;
       i = query.value(n++).toInt();                          
       comboBoxHome->addItem(query.value(n++).toString());    
   }
   
   comboBoxHome->setCurrentIndex(1);                          
   SavePathEdit->setText(settings.value("dumpPath").toString());
}

void admin::openDatabase()
{
    //db = QSqlDatabase::addDatabase(settings.value("qsqlDatabase").toString());
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(settings.value("host").toString());                         
    db.setDatabaseName(settings.value("dbname").toString());                   
    db.setUserName(settings.value("dbuser").toString());                       
    db.setPassword(settings.value("dbpasswd").toString());                     
    if(!db.open()) {
       qDebug() << "usrDB_FEHLER - " << db.lastError();     
    }
}



void admin::showTables(int f)
{
    i = f;                                             
    QSqlQuery query;
    qy = "SELECT * FROM dblogs";                       
    query.exec(qy);
    while(query.next()) {
       i = 0;
       n = 0;
       QTreeWidgetItem *item = new QTreeWidgetItem(logList);
       item->setText(i++,query.value(n++).toString()); 
       item->setText(i++,query.value(n++).toString()); 
       item->setText(i++,query.value(n++).toString()); 
       item->setText(i++,query.value(n++).toString()); 
    }

    int row = logList->topLevelItemCount();            
    QTreeWidgetItem *Item;                             
    qy = "SHOW TABLES FROM "+settings.value("dbname").toString();
    query.exec(qy);                                    
    while(query.next()) {
       n = 0;
       s = query.value(n++).toString();                
       i = 0;
       while(row != i ) {                              
         Item =  logList->topLevelItem (i++);          
         b = Item->text(0);                            
         x = b.size();                                 
         b = s.left(x);                                //
         if((b.compare(Item->text(0)) == 0)) {         
             x = 0;                                    
           break;                                      
         }
       }
       if( x == 0 )                                    
        ;                                              
       else {                                          
        QTreeWidgetItem *item = new QTreeWidgetItem(sysList); 
        item->setText(0,s);                            
       }
    }
    
    row = sysList->topLevelItemCount();              
    QTreeWidgetItem *sItem;                          
    i = 0;
    while( row != i ) {
       sItem =  sysList->topLevelItem (i);           
       s = sItem->text(0);                           
       qy = "SELECT COUNT(*) FROM "+s;
       query.exec(qy);                               
       query.next();
       sItem->setText(1,query.value(0).toString());  
       i++;
    }
}

admin::~admin()
{
    writeSettings();
}

void admin::writeSettings()
{
  settings.setValue("admin/Size",size());
  settings.setValue("admin/Properties",saveState());
  settings.setValue("dumpPath",SavePathEdit->text());
}

void admin::readSettings()
{
  resize(settings.value("admin/Size",sizeHint()).toSize());
  restoreState(settings.value("admin/Properties").toByteArray());
}

void admin::goExit()                           
{
    db.close();                                
    qApp->quit();
}

void admin::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_F10 :                       
             goExit();
         break;
   }
}


void admin::getHilfeCb()
{
   settings.setValue("Val","DB-Administration");
   StartProcess("hilfedb &");
}

void admin::stackedWidgetCb( int page )
{
     switch(page) {
     case 0:                                             
              logList->setEnabled(true);                 
              sysList->setEnabled(true);                 
             break;
     case 1:                                             
              logList->setEnabled(true);                 
              sysList->setEnabled(false);                
              getCustomsfields();                        
             break;
     case 2:                                  
              sysList->setEnabled(false);               
              logList->setEnabled(true);                
             break;
     case 3 :                                           
              logList->setEnabled(false);               
              sysList->setEnabled(false);               
              getRefnamen();                            
             break;
     }
}

void admin::adifExpCb()
{
   ButtonAdifExport->setEnabled(true);                              
   ButtonAdifExport->setPalette( QPalette(QColor(180, 210, 200)));  
   ButtonAdifImport->setEnabled(false);                             
   ButtonAdifImport->setPalette( QPalette(QColor(238, 238, 238)));  
   checkBoxQslMode->setEnabled(false);                              
   checkBoxQslMode->setPalette( QPalette(QColor(238,238,238)));     
   comboBoxRig->setEnabled(false);                                  
   comboBoxRig->setPalette( QPalette(QColor(238,238,238)));         
   comboBoxHome->setEnabled(false);                                 
   comboBoxHome->setPalette( QPalette(QColor(238,238,238)));        
   SavePathAdif->setText("");
   dirsel = 0;
   ButtonAdifDir->setText(tr("Waehle ADIF Pfad"));
   stackedWidget->setCurrentIndex(1);
}

void admin::adifImpCb()
{
   ButtonAdifExport->setEnabled(false);                             
   ButtonAdifExport->setPalette( QPalette(QColor(238, 238, 238)));  
   
   ButtonAdifImport->setEnabled(true);                              
   ButtonAdifImport->setPalette( QPalette(QColor(180, 210, 200)));  
   checkBoxQslMode->setEnabled(true);
   checkBoxQslMode->setPalette( QPalette(QColor(180, 210, 200)));   
   
   comboBoxRig->setEnabled(true);                                   
   comboBoxRig->setPalette( QPalette(QColor(180, 210, 200)));       
   comboBoxHome->setEnabled(true);                                  
   comboBoxHome->setPalette( QPalette(QColor(180, 210, 200)));      
   SavePathAdif->setText("");
   dirsel = 1;
   ButtonAdifDir->setText(tr("Waehle ADIFdatei"));
   stackedWidget->setCurrentIndex(1);
}


void admin::dumpCb()
{
   stackedWidget->setCurrentIndex(0);
}


void admin::refNamenCb()
{
   stackedWidget->setCurrentIndex(3);
}



void admin::toggelLogBoxCb()
{
    if(checkBoxLog->isChecked() == FALSE)
       checkBoxDb->setChecked(FALSE);
    else
       checkBoxDb->setChecked(TRUE);
}


void admin::toggelDbBoxCb()
{
    if(checkBoxDb->isChecked() == FALSE)
       checkBoxLog->setChecked(FALSE);
    else
       checkBoxLog->setChecked(TRUE);
}


void admin::toggelQslModeCb()
{
    if(checkBoxQslMode->isChecked() == FALSE) {
        checkBoxQslMode->setChecked(FALSE);
        checkBoxQslMode->setText("Default Import");
    }
    else {
      checkBoxQslMode->setChecked(TRUE);
      checkBoxQslMode->setText("Nicht setzen - Contest Import");
    }
}



void admin::logListCb(QTreeWidgetItem *item,int)
{
   if( logList != ltreeWidget ) {
     ltreeWidget->clearSelection ();              
   }
   ltreeWidget = logList;                         
   sItem = 0;
   lItem = item;
   val = lItem->text(0);                          
}


void admin::sysListCb(QTreeWidgetItem *item,int)
{
   if( sysList != ltreeWidget ) {
     ltreeWidget->clearSelection ();         
   }
   lItem = 0;
   ltreeWidget = sysList;                    
   sItem = item;
   val = item->text(0);                      
}

QString admin::DirSelector()
{
   if( !dirsel ) {
     QString directory = QFileDialog::getExistingDirectory(this,
             tr("Waehle einen Ordner"), QDir::homePath());
	     directory+="/";
     return directory;
   }
   else {
     QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                        "/home", tr(" *.adi *.adif *.ADI *.ADIF"));
     return fileName;
   }
}



void admin::getSaveDirCb()
{
   dirsel = 0;
   SavePathEdit->setText(DirSelector());
}



void admin::dbDump()
{
    if(SavePathEdit->text().count() == 0) {       
      QMessageBox::information( this,
        tr("INFO"),
        tr("\nBitte DUMP-Ordner waehlen !!"),
        QMessageBox::Ok);
      return;
    }
        QString pathToMysql="";
#ifdef Q_WS_WIN
    pathToMysql=settings.value("mySQLPath","").toString();
#endif
    if(checkBoxDb->isChecked() == TRUE ) {    
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        s = pathToMysql+"mysqldump -u"+settings.value("dbuser").toString()+" -p";
        s += settings.value("dbpasswd").toString();
        s += " --opt "+settings.value("dbname").toString()+" > "+SavePathEdit->text()+"backup-db.sql"; 
        StartProcess(s.toAscii());
        QApplication::restoreOverrideCursor();
    }
    else {                                   
       QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
       if(lItem) {
         s = pathToMysql+"mysqldump -u"+settings.value("dbuser").toString()+" -p"+settings.value("dbpasswd").toString()+" --opt "+settings.value("dbname").toString()+" ";
          s += lItem->text(0)+" ";          
          s += lItem->text(0)+"qsl ";       
          s += lItem->text(0)+"om ";        
          s += lItem->text(0)+"awd ";       
          s += lItem->text(0)+"card ";      
          s += lItem->text(0)+"dx ";        
          s += "wawdlist > ";               
          s+= SavePathEdit->text()+"backup-"+lItem->text(0)+".sql";
          StartProcess(s.toAscii());        
          QString tag, monat, jahr, dform;
          QDate d = QDate::currentDate();   
          tag = s.setNum(d.day());
          monat = s.setNum(d.month());
          jahr = s.setNum(d.year());
          if(tag.count() < 2) tag = "0"+tag;
          if(monat.count() < 2) monat = "0"+monat;
          
          qy = "UPDATE dblogs SET saved='"+jahr+"-"+monat+"-"+tag;
          qy += "' WHERE logname='"+lItem->text(0)+"'";
          QSqlQuery query;
          query.exec(qy);
          s = jahr+"-"+monat+"-"+tag;
          lItem->setText(3,s);
       }
       else
         if(sItem) {                         
            s = pathToMysql + "mysqldump -u"+settings.value("dbuser").toString()+" -p"+settings.value("dbpasswd").toString()+" --opt "+settings.value("dbname").toString();
            s += " "+sItem->text(0)+" > ";
            s+= SavePathEdit->text()+"backup-"+sItem->text(0)+".sql";
            StartProcess(s.toAscii());       
         }
       else {
          QApplication::restoreOverrideCursor();
          QMessageBox::information( this,
          tr("INFO"),
          tr("\nErst eine Tabelle im Logbook- oder QtLog-Ordner waehlen"),
          QMessageBox::Ok);
          return;
       }
    }
    
    s = "cp "+QDir::homePath();             
    s += "/.config/QtLog/qtlog.ini "+SavePathEdit->text();
    StartProcess(s.toAscii());
    
    QApplication::restoreOverrideCursor();
}



void admin::dbRestore()
{
  int i;
  QString p;
  QFile file;
     i = 0;
     QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
     QString pathToMysql="";
#ifdef Q_WS_WIN
    pathToMysql=settings.value("mySQLPath","").toString();
#endif
     if(checkBoxDb->isChecked() == TRUE ) {         
        p += SavePathEdit->text()+"backup-db.sql";
        if(!file.exists(p)) {
           QApplication::restoreOverrideCursor();
           QMessageBox::information( this,
           tr("INFO"),
           tr("\nRESTORE nicht moeglich - Datei nicht vorhanden !"),
           QMessageBox::Ok);
          return;
        }
        QSqlQuery query;
        
        qy = "DROP DATABASE IF EXISTS "+settings.value("dbname").toString(); 
        query.exec(qy);
        
        qy = "CREATE DATABASE logdb";                                        
        query.exec(qy);
        qy = "USE logdb";
        query.exec(qy);
 
        
        s = pathToMysql + "mysql -u"+settings.value("dbuser").toString()+" -p"+settings.value("dbpasswd").toString();
        s += " "+ settings.value("dbname").toString()+" < ";
        s += p;
        StartProcess(s.toAscii());
        i = 1;
     }
     else {                                              
        if( lItem) {
          p = SavePathEdit->text()+"backup-"+lItem->text(0)+".sql";
          if(!file.exists(p)) {
             QApplication::restoreOverrideCursor();
             QMessageBox::information( this,
             tr("INFO"),
             tr("\nRESTORE nicht moeglich - Datei nicht vorhanden !"),
             QMessageBox::Ok);
            return;
          }
         
          s = pathToMysql + "mysql -u"+settings.value("dbuser").toString()+" -p"+settings.value("dbpasswd").toString()+" "+settings.value("dbname").toString()+" < ";
          s+= p;
          StartProcess(s.toAscii());
          QSqlQuery query;
          qy = "SELECT COUNT(*) FROM "+lItem->text(0);
          query.exec(qy);
          query.next();
          s = query.value(0).toString();
          qy = "UPDATE dblogs SET qsocnt='"+s+"' WHERE logname='"+lItem->text(0)+"'";
          query.exec(qy);
          i = 0;
       }
       else
          if( sItem ) {                                  
             p = SavePathEdit->text()+"backup-"+sItem->text(0)+".sql";
             if(!file.exists(p)) {
                QApplication::restoreOverrideCursor();
                QMessageBox::information( this,
                tr("INFO"),
                tr("\nRESTORE nicht moeglich - Datei nicht vorhanden !"),
                QMessageBox::Ok);
               return;
             }
             s = pathToMysql + "mysql -u"+settings.value("dbuser").toString()+" -p"+settings.value("dbpasswd").toString()+" "+settings.value("dbname").toString()+" < ";
             s += p;
             StartProcess(s.toAscii());
             i = 0;
          }
        else {
          QApplication::restoreOverrideCursor();
          QMessageBox::information( this,
          tr("INFO"),
          tr("\nErst eine Tabelle im Logbook- oder QtLog-Ordner waehlen"),
          QMessageBox::Ok);
          return;
       }
     }
     logList->clear();
     sysList->clear();
     showTables(i);                              
     QApplication::restoreOverrideCursor();
}

void admin::message()
{
     qDebug() << "FEHLER";
}
