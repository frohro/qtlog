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

#include "laDiag.h"

// --------------------------------------------------
laDiag::laDiag(QWidget * parent) : QMainWindow(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
   setupUi(this);

   int n = settings.value("FontSize").toString().toInt();
   QFont font;
   font.setPointSize(n); 			
   setFont(font);
   
  //-- File ---
   //connect(actionENDE, SIGNAL(triggered(bool)), this, SLOT(goExit()));
   
   connect(actionInstall_Neue_Ctydat, SIGNAL(triggered(bool)), this, SLOT(readFileCtyWtDat()));
   
   connect(actionSubPrefixdb, SIGNAL(triggered(bool)), this, SLOT(subPrefixdb()));
   connect(actionDoc_DX_CtyListe, SIGNAL(triggered(bool)), this, SLOT(getHilfeDxListCb()));
   
   connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxChangedCb(int)));
   connect(ButtonENDE, SIGNAL(clicked()), this, SLOT(goExit()));
   connect(WlineEditPref, SIGNAL(textEdited(QString)), this, SLOT(getPraefixList(QString)));
   
   readSettings();

   //db = QSqlDatabase::addDatabase(settings.value("qsqlDatabase").toString());
   db = QSqlDatabase::addDatabase("QMYSQL");
   db.setHostName(settings.value("host").toString()); 
   db.setDatabaseName(settings.value("dbname").toString());
   db.setUserName(settings.value("dbuser").toString());
   db.setPassword(settings.value("dbpasswd").toString());
   if(!db.open()) {
       qDebug() << db.lastError();                         
   }
   
   qy = "SELECT * FROM tla";
   getLaTable(qy);                                        
   
   connect (laTabelle, SIGNAL(itemChanged(QTableWidgetItem *)), this,SLOT(updateLaItem(QTableWidgetItem *)));

   dbflg = 0;
}

// -----------------------------------------------------------------
laDiag::~laDiag()
{
  writeSettings();
}

// -----------------
void laDiag::goExit()
{
  db.close();
  qApp->quit();
}

// -------------------------------------------
void laDiag::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_F10 :                           
             goExit();
           break;
     case Qt::Key_Escape :
           WlineEditPref->setText("");
           WlineEditPref->setFocus();
           break;
   }
}

// ------------------------------------------
void laDiag::writeSettings()
{
  settings.setValue("laDiag/Size",size());
  settings.setValue("laDiag/Properties",saveState());
}

// ------------------------------------------
void laDiag::readSettings()
{
  resize(settings.value("laDiag/Size",sizeHint()).toSize());
  restoreState(settings.value("laDiag/Properties").toByteArray());
}

// -----------------------------------------
void laDiag::subPrefixdb()
{
   i = system("prefdb &");
}

// Hilfe
// ------------------------------------------
void laDiag::getHilfeDxListCb()
{
   settings.setValue("Val","DX-LaenderListe");
   i = system("hilfedb &");
}

// -----------------------------------------
void laDiag::comboBoxChangedCb(int id)
{
    i = id;
    WlineEditPref->setFocus();
}

// ================================================================
// Prefix_Liste laden
// --------------------------------------
void laDiag::getPraefixList(QString str)
{
     dbflg = 1;
     y = str.toUpper();
     WlineEditPref->setText(y);
 
     if(comboBox->currentIndex() == 0) {
       qy = "SELECT * FROM tla WHERE la LIKE '" +y+"%'";      
     }
     else
     if(comboBox->currentIndex() == 1) {
        qy = "SELECT * FROM tla WHERE lname LIKE '" +y+"%'";  
     }
     else
     if(comboBox->currentIndex() == 2) {
        qy = "SELECT * FROM tla WHERE adif LIKE '" +y+"%'";   
     }
     else
     if(comboBox->currentIndex() == 3) {
        qy = "SELECT * FROM tla WHERE lwae LIKE '" +y+"%'";   
     }
     else
     if(comboBox->currentIndex() == 4) {
        //qy = "SELECT * FROM tla WHERE lwae != '' And LIKE '" +y+"%'";   
        qy = "SELECT * FROM tla WHERE lwae != ''";   
     }
     else
     if(comboBox->currentIndex() == 5) {
        qy = "SELECT * FROM tla WHERE lcn LIKE '" +y+"%'";    
     }
     
     getLaTable(qy);
     dbflg = 0;
}



// ********************************************************************************************************
// LaTabelle: Daten in den Spalten 10 oder 11 wurden editiert und mit 'cr' quittiert - jetzt in der DB updaten
// SIGNAL ( itemChanged(QTableWidgetItem *));
// -----------------------------------------------------------------------------------------------------------
void laDiag::updateLaItem( QTableWidgetItem *item )
{
  int row, col;
  QString s;
  
    if(item != laTabelle->currentItem())                // fremde items nicht bearbeiten
        return;
    
    QSqlField field("feld",QVariant::String);
    if(item) {                                          // gibt es ein Item ?  ja: ok
      col = laTabelle->column ( item );                 // spalte
      row = laTabelle->row( item );                     // zeile
      QTableWidgetItem * v = laTabelle->verticalHeaderItem ( row );  // arbeits_item
   
      if((col == 8) || (col == 10) || (col == 11)) {
	if(col == 8)
	  s = "lwae";
	if(col == 10)
	  s = "adif";                                   // ADIF_No verändert
	if(col == 11) 
	  s = "notiz";                                  // Notiz verändert
	
	qy = "UPDATE tla SET "+s+"=";
	field.setValue(item->data(Qt::EditRole).toString());
	qy += db.driver()->formatValue(field,false);
        qy += " WHERE la='"+v->text()+"'";
	QSqlQuery query(qy);
      }
    }
    WlineEditPref->setFocus();
}

