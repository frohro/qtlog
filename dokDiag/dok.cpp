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

#include "dok.h"
//#include "../qtlogDiag/dirmngr.h"

// -------------------------------------------
dok::dok(QWidget * parent) : QMainWindow(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
    setupUi(this);
    n = settings.value("FontSize").toString().toInt();
    QFont font;
    font.setPointSize(n); 			
    setFont(font);
   
    connect(ButtonEnde, SIGNAL(clicked()), this, SLOT(goExit()));
    connect(dokListe, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(dokListeClickedCb(QTreeWidgetItem *,int)));
    connect(sdokListe, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(sdokListeClickedCb(QTreeWidgetItem *,int)));

    connect(ButtonUpdate, SIGNAL(clicked()), this, SLOT(dokUpdateCb()));
    connect(ButtonNEU, SIGNAL(clicked()), this, SLOT(dokNeuCb()));
    connect(ButtonDEL, SIGNAL(clicked()), this, SLOT(dokDeleteCb()));
    connect(wlineEditDok, SIGNAL(textEdited(QString)), this, SLOT(getDokCb(QString)));
    connect(BoxDOK, SIGNAL(currentIndexChanged(int)), this, SLOT(boxDokChangedCb(int)));
    connect(wEditDok, SIGNAL(textEdited(QString)), this, SLOT(wEditDokCb(QString)));
    connect(wEditDxcc, SIGNAL(textEdited(QString)), this, SLOT(wEditDxccCb(QString)));
    // SDOK
    connect(ButtonSdokUpdate, SIGNAL(clicked()), this, SLOT(sdokUpdateCb()));
    connect(ButtonSdokNeu, SIGNAL(clicked()), this, SLOT(sdokNeuCb()));
    connect(ButtonSdokDel, SIGNAL(clicked()), this, SLOT(sdokDeleteCb()));
    connect(wlineEditSdok, SIGNAL(textEdited(QString)), this, SLOT(getSdokCb(QString)));
    connect(BoxSDOK, SIGNAL(currentIndexChanged(int)), this, SLOT(boxSdokChangedCb(int)));
    connect(wEditSdok, SIGNAL(textEdited(QString)), this, SLOT(wEditSdokCb(QString)));
    connect(wEditCall, SIGNAL(textEdited(QString)), this, SLOT(wEditCallCb(QString)));
    connect(wEditHdok, SIGNAL(textEdited(QString)), this, SLOT(wEditHdokCb(QString)));
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabwidgetCurrentChangedCb(int)));
    connect(ButtonHilfe, SIGNAL(clicked()), this, SLOT(getHilfeCb()));
    
    //db = QSqlDatabase::addDatabase(settings.value("qsqlDatabase").toString());
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(settings.value("host").toString());      
    db.setDatabaseName(settings.value("dbname").toString());
    db.setUserName(settings.value("dbuser").toString());
    db.setPassword(settings.value("dbpasswd").toString());
    
    if(!db.open()) {
       qDebug() << "usrDB_FEHLER - " << db.lastError();     
    }
    
    readSettings();            
    
    sdokListe->clear();
    sdokListe->setColumnWidth(0,80);             // S-DOK
    sdokListe->setColumnWidth(1,90);             // Call
    sdokListe->setColumnWidth(2,150);            // Anlass
    sdokListe->setColumnWidth(3,80);             // DOK
    sdokListe->setColumnWidth(4,100);            // Von
    sdokListe->setColumnWidth(5,100);            // Bis
    dokListe->clear();
    dokListe->setColumnWidth(0,60);              // DOK
    dokListe->setColumnWidth(1,160);             // OV-Name
    dokListe->setColumnWidth(2,50);              // dxcc
    tabWidget->setCurrentIndex(0);
    tabIdx = 0;

    QDate heute = QDate::currentDate();         // erzeuge das Datum von heute
    dateEditVon->setDate(heute);                // setze Datum von heute
    heute = heute.addMonths(12);                // +12 Monate
    dateEditBis->setDate(heute);
    
    qy = "SELECT * FROM tdok WHERE kdxcc = 'DL' ORDER BY dok";
    showDok(qy);
}

dok::~dok()
{
   writeSettings();
}

// ------------------
void dok::goExit()                               
{
    db.close();                                  
    qApp->quit();
}

void dok::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_F10 :                           
             goExit();
           break;
   }
}

// Settings
// ---------------------
void dok::readSettings()
{
    resize(settings.value("dok/Size",sizeHint()).toSize());
    restoreState(settings.value("dok/Properties").toByteArray());
}

// Settings
// ---------------------
void dok::writeSettings()
{
    settings.setValue("dok/Size",size());
    settings.setValue("dok/Properties",saveState());
}

// ----------------------------------------
void dok::tabwidgetCurrentChangedCb(int i)
{
   switch(i) {
      case 0: break;               // Dok_Liste zeigen
      case 1: break;               
      case 2 : if( tabIdx != 3 )
                 getSdokList();    // SDOK _Liste zeigen
               tabIdx = 2;
              break;
      case 3 :                     // SDOK Edit
              break;
   }
}

// ---------------------
void dok::getHilfeCb()
{
   settings.setValue("Val","DOK Verwaltung");
   system("hilfedb &");
}

// ---------------------
void dok::wEditDokCb(QString st)
{
    QString f;
    f = st.toUpper();
    wEditDok->setText(f);
}

// ----------------------
void dok::wEditDxccCb(QString st)
{
    QString f;
    f = st.toUpper();
    wEditDxcc->setText(f);
}


// -------------------------------------------------------
// showDOK
// -------------------------------------------------------
void dok::showDok(QString q)
{
    dokListe->clear();
    QSqlQuery query;
    query.exec(q);
    n = query.size();
    while(query.next()) {
      i = 0;
      r = 0;
      QTreeWidgetItem *item = new QTreeWidgetItem(dokListe);
      item->setText(r++,query.value(i).toString());        // DOK
      item->setTextAlignment(i++,Qt::AlignCenter);
      item->setText(r++,query.value(i++).toString());      // OV-Name
      item->setText(r++,query.value(i).toString());        // Dxcc
      item->setTextAlignment(i++,Qt::AlignCenter);
      item->setText(r++,query.value(i++).toString());      // DistriktName
      item->setText(r++,query.value(i++).toString());      // Bemerkungen
    }
    t = BoxDOK->currentText();
    t += " : "+s.setNum(n);
    t += tr("   Eintraege");
    DokCntLabel->setText(t);
    wlineEditDok->setFocus();                   
}


// -------------------------------------------------------
// In DOK_Liste suchen
// -------------------------------------------------------
void dok::getDokCb(QString str)
{
    QString f;
    f = str.toUpper();
    wlineEditDok->setText(f);
    qy = "SELECT * FROM tdok WHERE kdxcc ='"+BoxDOK->currentText()+"' AND dok LIKE '"+f+"%' ORDER BY dok";
    showDok(qy);
}

// BoxDok verändert
// ------------------------------
void dok::boxDokChangedCb(int id)
{
  id = 0;
  wlineEditDok->setFocus();
}


// --------------------------------------------------------
// dokListe clicked
// --------------------------------------------------------
void dok::dokListeClickedCb(QTreeWidgetItem * item, int i)
{
     int d = i;
     d = 0;
     wEditDok->setText(item->text(0));
     wEditOVname->setText(item->text(1));
     wEditDxcc->setText(item->text(2));
     wEditDistrikt->setText(item->text(3));
     wEditBemerkung->setText(item->text(4));
     tabWidget->setCurrentIndex(1);
     uItem = item;
}

// ----------------------------------------------------------
// Button_Update
//----------------------------------------------------------
void dok::dokUpdateCb()
{
   if(wEditDok->text().count() == 0)
     return;
   QSqlQuery query;
   qy = "UPDATE tdok SET ovnam='"+wEditOVname->text();
   qy += "',kdxcc='"+wEditDxcc->text();
   qy += "',disnam='"+wEditDistrikt->text();
   qy += "',bemerk='"+wEditBemerkung->text();
   qy += "' WHERE dok = '"+wEditDok->text()+"'";
   query.exec(qy);
   uItem->setText(1,wEditOVname->text());
   uItem->setText(2,wEditDxcc->text());
   uItem->setText(3,wEditDistrikt->text());
   uItem->setText(4,wEditBemerkung->text());
   tabWidget->setCurrentIndex(0);
}


// ----------------------------------------------------------
// Button_NEU
// ----------------------------------------------------------
void dok::dokNeuCb()
{
   if(wEditDok->text().count() == 0)
     return;
   QSqlQuery query;
   qy = "SELECT dok FROM tdok WHERE dok='"+wEditDok->text()+"'";
   query.exec(qy);
   if(query.size() != 0) {
     
     return;
   }
   qy = "INSERT INTO tdok VALUES ('";
   qy += wEditDok->text();
   qy += "','"+wEditOVname->text();
   qy += "','"+wEditDxcc->text();
   qy += "','"+wEditDistrikt->text();
   qy += "','"+wEditBemerkung->text();
   qy += "')";
   query.exec(qy);
   r = 0;
   QTreeWidgetItem *item = new QTreeWidgetItem(dokListe);
   item->setText(r,wEditDok->text());              // DOK
   item->setTextAlignment(r++,Qt::AlignCenter);
   item->setText(r++,wEditOVname->text());         // OV-Name
   item->setText(r,wEditDxcc->text());             // Dxcc
   item->setTextAlignment(r++,Qt::AlignCenter);
   item->setText(r++,wEditDistrikt->text());       // DistriktName
   item->setText(r++,wEditBemerkung->text());      // Bemerkungen
   tabWidget->setCurrentIndex(0);
}


// ---------------------------------------------------------------------
// Button_Delete
// ---------------------------------------------------------------------
void dok::dokDeleteCb()
{
    QSqlQuery query;
    qy = " DELETE FROM tdok WHERE dok='"+wEditDok->text()+"'";
    query.exec(qy);
    n = dokListe->indexOfTopLevelItem(uItem);
    dokListe->takeTopLevelItem( n );
    tabWidget->setCurrentIndex(0);
}


// ======================================================================
// SDOK
// ======================================================================
void dok::wEditSdokCb(QString st)
{
    QString f;
    f = st.toUpper();
    wEditSdok->setText(f);
}

// -----------------------------
void dok::wEditCallCb(QString st)
{
    QString f;
    f = st.toUpper();
    wEditCall->setText(f);
}

//-------------------------------
void dok::wEditHdokCb(QString st)
{
    QString f;
    f = st.toUpper();
    wEditHdok->setText(f);
}


// -------------------------------------------------------------
// SDOK selectieren
// ------------------------------------------------------------
void dok::getSdokList()
{
    qy = "SELECT * FROM tsdok";
    getSdokData(qy);
}


// ------------------------------------------------------------
// S_DOKs_data_tabelle zeigen
// ------------------------------------------------------------
void dok::getSdokData(QString q)
{
    sdokListe->clear();
    QSqlQuery query;
    query.exec(q);
    n = query.size();
    while(query.next()) {
      i = 0;
      r = 0;
      QTreeWidgetItem *item = new QTreeWidgetItem(sdokListe);
      item->setText(r++,query.value(i++).toString());        // SDOK
      item->setText(r++,query.value(i++).toString());        // SDOK
      item->setText(r++,query.value(i++).toString());        // Anlass
      item->setText(r++,query.value(i).toString());          // DOK
      item->setTextAlignment(i++,Qt::AlignCenter);
      item->setText(r++,query.value(i++).toString());        // Von
      item->setText(r++,query.value(i++).toString());        // Bis
      item->setText(r++,query.value(i++).toString());        // ID
    }
    t = tr("-  S-DOK  ");
    t += s.setNum(n);
    SdokCntLabel->setText(t);
    wlineEditSdok->setFocus();                   
}

// BoxSDOK verändert
// -------------------------------------------------------
void dok::boxSdokChangedCb(int id)
{
  id = 0;
  wlineEditSdok->setFocus();
}


// -------------------------------------------------------
// In S-DOK_Liste suchen
// -------------------------------------------------------
void dok::getSdokCb(QString str)
{
    QString f;
    f = str.toUpper();
    wlineEditDok->setText(f);
    if(BoxSDOK->currentText().compare("DOK") == 0)
       s = "H"+BoxSDOK->currentText();
    else
      s = BoxSDOK->currentText();
    qy = "SELECT * FROM tsdok WHERE ";
    qy += s+" LIKE '"+f+"%' ORDER BY sdok";
    getSdokData(qy);
}


// --------------------------------------------------------
// SDOK_Liste clicked
// --------------------------------------------------------
void dok::sdokListeClickedCb(QTreeWidgetItem * item, int i)
{
     int d = i;
     d = 0;
     wEditSdok->setText(item->text(0));                  // S-Dok
     wEditCall->setText(item->text(1));                  // Call
     wEditAnlass->setText(item->text(2));                // Anlass
     wEditHdok->setText(item->text(3));                  // DOK
     s = item->text(4);                                  // Datum von
     QDate da = QDate::fromString (s,"dd-MM-yyyy");
     dateEditVon->setDate(da);
     s = item->text(5);                                  // Datum bis
     da = QDate::fromString (s,"dd-MM-yyyy");
     dateEditBis->setDate(da);
     wEditId->setText(item->text(6));                    // Id
     tabWidget->setCurrentIndex(3);
     uItem = item;
}

// -------------------------------------------------------
// Button_SDOK_update
//--------------------------------------------------------
void dok::sdokUpdateCb()
{
   if(wEditSdok->text().count() == 0)
     return;
   QSqlQuery query;
   qy = "UPDATE tsdok SET sdok='"+wEditSdok->text();
   qy += "',acall='"+wEditCall->text();
   qy += "',anlass='"+wEditAnlass->text();
   qy += "',hdok='"+wEditHdok->text();
   
   qy += "',von='";                           // Von Tag
   QDate d = dateEditVon->date();             // Datum
   s = "";
   t.setNum(d.day());                         // Tag
   if(t.count() == 1)
      s = "0";
   qy += s+t+"-";
   t.setNum(d.month());                       // Monat
   s = "";
   if(t.count() == 1)
      s = "0";
   qy += s+t;
   qy += "-"+t.setNum(d.year());              // Jahr
   
   qy += "',bis='";                           // Bis Tag
   d = dateEditBis->date();                   // Datum
   s = "";
   t.setNum(d.day());
   if(t.count() == 1)
      s = "0";
   qy += s+t+"-";                             // Tag
   t.setNum(d.month());
   if(t.count() == 1)
        s = "0";
   qy += s+t;                                 // Monat
   qy += "-"+t.setNum(d.year());              // Jahr
   qy += "' WHERE id="+wEditId->text();
   query.exec(qy);
   uItem->setText(0,wEditSdok->text());
   uItem->setText(1,wEditCall->text());
   uItem->setText(2,wEditAnlass->text());
   uItem->setText(3,wEditHdok->text());
// --
   d = dateEditVon->date();                   // hole Datum Von
   s = "";
   t.setNum(d.day());                         // Tag
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";
//--
   t.setNum(d.month());                       // Monat
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";

   s += t.setNum(d.year());                   // Jahr
   uItem->setText(4,s);                       // setze Datum bis

   // --
   d = dateEditBis->date();                   // hole Datum Bis
   s = "";
   t.setNum(d.day());                         // Tag
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";
//--
   t.setNum(d.month());                       // Monat
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";

   s += t.setNum(d.year());                   // Jahr
   uItem->setText(5,s);                       // setze Datum bis
   
   uItem->setText(6,s.setNum(n));             // Last Insert
   tabIdx = 3;
   tabWidget->setCurrentIndex(2);
}


// -----------------------------------------------------------
// Button_SDOK_NEU
// -----------------------------------------------------------
void dok::sdokNeuCb()
{
   if(wEditSdok->text().count() == 0)
     return;
   QSqlQuery query;
   qy = "INSERT INTO tsdok VALUES ('";
   qy += wEditSdok->text();                    // SDOK
   qy += "','"+wEditCall->text();              // Call
   qy += "','"+wEditAnlass->text();            // Anlass
   qy += "','"+wEditHdok->text();              // DOK
   qy += "','";
   QDate d = dateEditVon->date();              // Datum Von
   s = "";
   t.setNum(d.day());                          // Tag
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";
// --
   t.setNum(d.month());                        // Monat
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";
   qy += s+t.setNum(d.year());                 // Jahr
   qy += "','";
// --
   d = dateEditBis->date();                   // Datum Bis
   s = "";
   t.setNum(d.day());                         // Tag
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";
// --
   t.setNum(d.month());                        // Monat
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";
   qy += s+t.setNum(d.year());                // Jahr
   qy += "',0)";                               // ID
   query.exec(qy);
   n = query.lastInsertId().toInt();
   r = 0;
   QTreeWidgetItem *item = new QTreeWidgetItem(sdokListe);
   item->setText(r++,wEditSdok->text());      // SDOK
   item->setText(r++,wEditCall->text());      // Call
   item->setText(r++,wEditAnlass->text());    // Anlass
   
   item->setText(r,wEditHdok->text());        // DOK
   item->setTextAlignment(r++,Qt::AlignCenter);
// --
   d = dateEditVon->date();                   // hole Datum Von
   s = "";
   t.setNum(d.day());                          // Tag
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";
// --
   t.setNum(d.month());                       // Monat
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";
// --
   s += t.setNum(d.year());                   // Jahr
   item->setText(r++,s);                      // setze Datum bis
// --
   d = dateEditBis->date();                   // hole Datum Bis
   s = "";
   t.setNum(d.day());                         // Tag
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";

   t.setNum(d.month());                       // Monat
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";

   s += t.setNum(d.year());                   // Jahr
   item->setText(r++,s);                      // setze Datum bis
   item->setText(r++,s.setNum(n));            // Last Insert
   tabIdx = 3;
   tabWidget->setCurrentIndex(2);
   // Count +1 setzen
}


// ---------------------------------------------------------------------
// Button_SDOK_Delete
// ---------------------------------------------------------------------
void dok::sdokDeleteCb()
{
    QSqlQuery query;
    qy = " DELETE FROM tsdok WHERE id="+wEditId->text();
    query.exec(qy);
    n = sdokListe->indexOfTopLevelItem(uItem);
    sdokListe->takeTopLevelItem( n );
    tabIdx = 3;
    tabWidget->setCurrentIndex(2);
     // count -1 setzen
}
