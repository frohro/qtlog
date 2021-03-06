/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#include <QSettings>
#include <QString>
#include <QTabWidget>
#include <QtGui>
#include <QtSql>
#include "confMode.h"
#include "dirmngr.h"

confMode::confMode(QWidget * parent) : QDialog(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
   setupUi(this);
   n = settings.value("FontSize").toString().toInt();
   QFont font;
   font.setPointSize(n); 			
   setFont(font);
   
   connect(pushButtonExit, SIGNAL(clicked()), this, SLOT(goExit()));
   connect(bandList, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(bandListCb(QTreeWidgetItem*,int)));
   connect(modeList, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(modeListCb(QTreeWidgetItem*,int)));
   connect(pwrList, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(pwrListCb(QTreeWidgetItem*,int)));
   connect(lineList, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(lineListCb(QTreeWidgetItem*,int)));
   connect(awdList, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(awdListCb(QTreeWidgetItem*,int)));
   connect(customsList, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(customsListCb(QTreeWidgetItem*,int)));
   connect(rigList, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(rigListCb(QTreeWidgetItem*,int)));
   connect(ButtonHilfe, SIGNAL(clicked()), this, SLOT(getHilfeModeCb()));
   connect(SavePushButton, SIGNAL(clicked()), this, SLOT(SaveConfValues()));
   connect(NewPushButton, SIGNAL(clicked()), this, SLOT(NewAwdValues()));
   connect(RigSavePushButton, SIGNAL(clicked()), this, SLOT(RigSaveButton()));
   connect(RigClearPushButton, SIGNAL(clicked()), this, SLOT(RigClearButton()));
   
    // Band
   bandList->setColumnWidth(0,70);  // Band
   bandList->setColumnWidth(1,40);  // Set
   bandList->setColumnWidth(2,80);  // myBand
   bandList->setColumnWidth(3,70);  // Frequenz
   bandList->setColumnWidth(4,60);  // Band>Rig
   // Mode
   modeList->setColumnWidth(0,30);  // Set
   modeList->setColumnWidth(1,40);  // Mode
   // PWR
   pwrList->setColumnWidth(0,30);   // Set
   pwrList->setColumnWidth(1,40);   // Wort
   pwrList->setColumnWidth(2,60);   // PWR
   // Verbindungs_Arten
   lineList->setColumnWidth(0,30);  // set
   lineList->setColumnWidth(1,80);  // Mode
   // AWD_Typen
   awdList->setColumnWidth(0,40);   // aset
   awdList->setColumnWidth(1,80);   // atyp
   // customs_Felder
   customsList->setColumnWidth(0,80);   // custom_db_feld
   customsList->setColumnWidth(1,140);  // user_feld_name
   // Rig_List
   rigList->setColumnWidth(0,80);    // trans
   rigList->setColumnWidth(1,250);   // trans
   rigList->setColumnWidth(2,250);   // ant
   rigList->setColumnWidth(3,200);   // Linux
   QSqlQuery query;
   // Band
   qy = "SELECT band,work,mband,freq,brig FROM wband";
   query.exec(qy);
   while(query.next()) {
       n = 0;
       i = 0;
       QTreeWidgetItem *item = new QTreeWidgetItem(bandList);
       item->setText(i++,query.value(n++).toString());    // band
       item->setText(i++,query.value(n++).toString());    // work
       item->setText(i++,query.value(n++).toString());    // myband
       item->setText(i++,query.value(n++).toString());    // freq
       item->setText(i++,query.value(n++).toString());    // Band -> Rig
   }
   // Mode
   qy = "SELECT * FROM wmode ORDER BY work DESC";
   query.exec(qy);
   while(query.next()) {
       n = 0;
       i = 0;
       QTreeWidgetItem *item = new QTreeWidgetItem(modeList);
       item->setText(i++,query.value(n++).toString());     // work
       item->setText(i++,query.value(n++).toString());     // mode
   }
    // PWR
   qy = "SELECT * FROM wpwr";
   query.exec(qy);
   while(query.next()) {
       n = 0;
       i = 0;
       QTreeWidgetItem *item = new QTreeWidgetItem(pwrList);
       item->setText(i++,query.value(n++).toString());    // pwr
       item->setText(i++,query.value(n++).toString());    // work
       item->setText(i++,query.value(n++).toString());    // watt
   }
   //
   qy = "SELECT * FROM wline ";
   query.exec(qy);
   while(query.next()) {
       n = 0;
       i = 0;
       QTreeWidgetItem *item = new QTreeWidgetItem(lineList);
       item->setText(i++,query.value(n++).toString());    // id
       item->setText(i++,query.value(n++).toString());    // line
   }
   // AWD
   qy = "SELECT aset,atype FROM wawdlist ORDER BY id";
   query.exec(qy);
   while(query.next()) {
       n = 0;
       i = 0;
       QTreeWidgetItem *item = new QTreeWidgetItem(awdList);
       item->setText(i++,query.value(n++).toString());    // aset
       item->setText(i++,query.value(n++).toString());    // atype
   }
    // customs
   qy = "SELECT dbfield,refnam FROM refnamen WHERE dbfield LIKE 'custom%'";
   query.exec(qy);
   while(query.next()) {
       n = 0;
       i = 0;
       QTreeWidgetItem *item = new QTreeWidgetItem(customsList);
       item->setText(i++,query.value(n++).toString());    // db_feld
       item->setText(i++,query.value(n++).toString());    // user_name
   }
    // Rig
   qy = "SELECT * FROM wrig";
   query.exec(qy);
   while(query.next()) {
       n = 0;
       i = 0;
       QTreeWidgetItem *item = new QTreeWidgetItem(rigList);
       item->setText(i++,query.value(n++).toString());     // rigtype
       item->setText(i++,query.value(n++).toString());     // trans
       item->setText(i++,query.value(n++).toString());     // ant
       item->setText(i++,query.value(n++).toString());     // op
       item->setText(i++,query.value(n++).toString());     // spez
   }
   NewPushButton->hide();
   rtreeWidget = bandList;
   customId1 = 0;
   customId2 = 0;
   BandSetLineEdit->setFocus();
   
   LineNameEdit->setEnabled(FALSE);                // sperren 
   AwdSetLineEdit->setEnabled(FALSE);              // sperren
   NameTypeLineEdit->setEnabled(FALSE);            // sperren
   tabWidget->setCurrentIndex(settings.value("Val").toInt());
   if(settings.value("Val").toInt() == 1) {
       QPalette palette7;
       QBrush brush8(QColor(237, 255, 183, 255));  // GELB/grün
       palette7.setBrush(QPalette::Active, QPalette::Base, brush8);
       awdList->setPalette(palette7);
   }
   
   connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(currentIndexchangedCb(int)));
}

// ------------------
confMode::~confMode()
{}

// ================================================
void confMode::goExit()               
{
  accept();                           
}

// ---------------------------------------------
void confMode::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_F10 :                           
             goExit();
           break;
   }
}

// --------------------------------------------
void confMode::currentIndexchangedCb(int idx)
{
  qDebug() << idx;
   if(idx <= 2 ) {
    SavePushButton->show();
    
   }
   else
   if(idx >= 2) {
    SavePushButton->hide();
    
   }
   else
   if( idx != 1) {
    NewPushButton->hide();
    
   }
}

// Hilfe
// ---------------------------
void confMode::getHilfeModeCb()
{
   settings.setValue("Val","Betriebsarten");
   StartProcess("hilfedb &");
}

//  BandList Clicked
// ---------------------------
void confMode::bandListCb(QTreeWidgetItem *item,int)
{
   if( bandList != rtreeWidget ) {
       rtreeWidget->clearSelection ();  
       clearLineEdit(rIndex);
   }
   rIndex = 1;
   rItem = item;
   rtreeWidget = bandList;
   val = item->text(0);
   NewPushButton->hide();
   BandSetLineEdit->setText(item->text(1));
   BandMyBandLineEdit->setText(item->text(2));
   BandFreqLineEdit->setText(item->text(3));
   BandRigLineEdit->setText(item->text(4));
   BandSetLineEdit->setFocus();
}

// ModeList Clicked
// ---------------------------------------------
void confMode::modeListCb(QTreeWidgetItem*item,int)
{
   if( modeList != rtreeWidget) {
      rtreeWidget->clearSelection ();
      clearLineEdit(rIndex);
   }
   rIndex = 2;
   rItem = item;
   rtreeWidget = modeList;
   val = item->text(1);
   NewPushButton->hide();
   ModeSetLineEdit->setText(item->text(0));
   ModeSetLineEdit->setFocus();
}

// pwrList Clicked
void confMode::pwrListCb(QTreeWidgetItem *item,int)
{
   if(pwrList != rtreeWidget) {
      rtreeWidget->clearSelection ();
      clearLineEdit(rIndex);
   }
   rIndex = 3;
   rItem = item;
   rtreeWidget = pwrList;
   val = item->text(0);
   NewPushButton->hide();
   PwrWorkLineEdit->setText(item->text(1));
   PwrWattLineEdit->setText(item->text(2));
   PwrWorkLineEdit->setFocus();
}

// ******************************************************
// LineList Clicked
void confMode::lineListCb(QTreeWidgetItem *item,int)
{
  if(lineList != rtreeWidget) {
      rtreeWidget->clearSelection ();
      clearLineEdit(rIndex);
   }
   rIndex = 4;
   rItem = item;
   rtreeWidget = lineList;
   
   LineNameEdit->setEnabled(TRUE);               // frei
   QPalette palette7;
   QBrush brush8(QColor(237, 255, 183, 255));   // GELB/grün
   palette7.setBrush(QPalette::Active, QPalette::Base, brush8);
   lineList->setPalette(palette7);
   LineNameEdit->setPalette(palette7);
   
   AwdSetLineEdit->setEnabled(FALSE);            // sperren
   NameTypeLineEdit->setEnabled(FALSE);          // sperren
   QPalette palette8;
   QBrush brush7(QColor(248, 248,248, 255));     // weis/grau
   palette8.setBrush(QPalette::Active, QPalette::Base, brush7);
   
   awdList->setPalette(palette8);                // weiss/grau
   AwdSetLineEdit->setPalette(palette8);         // weiss/grau
   customsList->setPalette(palette8);            // weiss/grau
   NameTypeLineEdit->setPalette(palette8);       // weiss/grau
   
   val = item->text(0);
   NewPushButton->hide();
   LineNameEdit->setText(item->text(1));
   LineNameEdit->setFocus();
}

// AWDList clicked
void confMode::awdListCb(QTreeWidgetItem *item,int)
{
  if(awdList != rtreeWidget) {
      rtreeWidget->clearSelection ();
      clearLineEdit(rIndex);
   }
   rIndex = 5;
   rItem = item;
   rtreeWidget = awdList;
   
   AwdSetLineEdit->setEnabled(TRUE);            // frei
   NameTypeLineEdit->setEnabled(TRUE);          // frei
   QPalette palette7;
   QBrush brush8(QColor(237, 255, 183, 255));   // GELB/grün
   palette7.setBrush(QPalette::Active, QPalette::Base, brush8);
   awdList->setPalette(palette7);
   AwdSetLineEdit->setPalette(palette7);
   NameTypeLineEdit->setPalette(palette7);
   
   LineNameEdit->setEnabled(FALSE);             // sperren
   QPalette palette8;
   QBrush brush7(QColor(248, 248,248, 255));    // weiss/grau
   palette8.setBrush(QPalette::Active, QPalette::Base, brush7);
   lineList->setPalette(palette8);              // weiss/grau
   customsList->setPalette(palette8);           // weiss/grau
   
   NewPushButton->show();
   AwdSetLineEdit->setText(item->text(0));
   NameTypeLineEdit->setText(item->text(1));
   labelUsrName->setText(QString::fromUtf8("Award Name"));
   AwdSetLineEdit->setFocus();
}

// customsList clicked
// ----------------------
void confMode::customsListCb(QTreeWidgetItem *item,int)
{
  if(customsList != rtreeWidget) {
      rtreeWidget->clearSelection ();
      clearLineEdit(rIndex);
   }
   rIndex = 6;
   rItem = item;
   rtreeWidget = customsList;
   
   NameTypeLineEdit->setEnabled(TRUE);          // freigeben   
   QPalette palette7;
   QBrush brush8(QColor(237, 255, 183, 255));  // GELB/grün
   palette7.setBrush(QPalette::Active, QPalette::Base, brush8);
   customsList->setPalette(palette7);
   NameTypeLineEdit->setPalette(palette7);;    // gelb/grün
     
   LineNameEdit->setEnabled(FALSE);            // sperren
   AwdSetLineEdit->setEnabled(FALSE);          // sperren
   QPalette palette8;
   QBrush brush7(QColor(248, 248,248, 255));   // weiss/grau
   palette8.setBrush(QPalette::Active, QPalette::Base, brush7);
   lineList->setPalette(palette8);
   LineNameEdit->setPalette(palette8);
   awdList->setPalette(palette8);
   AwdSetLineEdit->setPalette(palette8);
   //--
   NewPushButton->hide();
   NameTypeLineEdit->setText(item->text(1));
   labelUsrName->setText(tr("Waehle user Feldname"));
   NameTypeLineEdit->setFocus();
}

// *******************************************************

// RIG_List
// ------------------------------------
void confMode::rigListCb(QTreeWidgetItem *item,int)
{
   rItem = item;
   NewPushButton->hide();
   RigNameLineEdit->setText(item->text(0));
   RigTransLineEdit->setText(item->text(1));
   RigAntLineEdit->setText(item->text(2));
   RigOpLineEdit->setText(item->text(3));
   RigSpezLineEdit->setText(item->text(4));
   tabWidget->setCurrentIndex (3);
   RigNameLineEdit->setFocus();
}

// Rig speichern
// -----------------------
void confMode::RigSaveButton()
{
   QSqlQuery query;
   qy = "SELECT rigtype FROM wrig WHERE rigtype='"+RigNameLineEdit->text()+"'";
   query.exec(qy);
   i = query.numRowsAffected();
   if( i ) {
       qy = "UPDATE wrig SET trans='"+RigTransLineEdit->text();
       qy += "',ant='"+RigAntLineEdit->text();
       qy += "',op='"+RigOpLineEdit->text();
       qy += "',spez='"+RigSpezLineEdit->text();
       qy += "' WHERE rigtype='"+RigNameLineEdit->text();
       qy += "'";
       query.exec(qy);
       rItem->setText(1,RigTransLineEdit->text());
       rItem->setText(2,RigAntLineEdit->text());
       rItem->setText(3,RigOpLineEdit->text());
       rItem->setText(4,RigSpezLineEdit->text());
       tabWidget->setCurrentIndex (2);
   }
   else {                                         // NEU anlegen
    qy = "INSERT INTO wrig VALUES ('";
    qy += RigNameLineEdit->text();
    qy += "','"+RigTransLineEdit->text();
    qy += "','"+RigAntLineEdit->text();
    qy += "','"+RigOpLineEdit->text();
    qy += "','"+RigSpezLineEdit->text();
    qy += "')";
    query.exec(qy);
    QTreeWidgetItem *item = new QTreeWidgetItem(rigList);
    item->setText(i++,RigNameLineEdit->text());    // rigtype
    item->setText(i++,RigTransLineEdit->text());   // trans
    item->setText(i++,RigAntLineEdit->text());     // ant
    item->setText(i++,RigOpLineEdit->text());      // op
    item->setText(i++,RigSpezLineEdit->text());    // spez
    tabWidget->setCurrentIndex (2);
   }
}

// Rig löschen
// ------------------------------
void confMode::RigClearButton()
{
   QSqlQuery query;
   qy = "DELETE FROM wrig WHERE rigtype='"+RigNameLineEdit->text()+"'";
   query.exec(qy);
   i = rigList->indexOfTopLevelItem ( rItem );
   rigList->takeTopLevelItem ( i );                
   SavePushButton->show();
   tabWidget->setCurrentIndex (2);
}
// --------
void confMode::clearLineEdit(int cl)
{
   switch(cl) {
     case 1:  BandSetLineEdit->setText("");
              BandMyBandLineEdit->setText("");
              BandFreqLineEdit->setText("");
              BandRigLineEdit->setText("");
              break;
     case 2:  ModeSetLineEdit->setText("");
              break;
     case 3:  PwrWorkLineEdit->setText("");
              PwrWattLineEdit->setText("");
              break;
     case 4:  LineNameEdit->setText("");
              break;
     case 5:  AwdSetLineEdit->setText("");
              NameTypeLineEdit->setText("");
              break;
     case 6:  
              AwdSetLineEdit->setText("");
              NameTypeLineEdit->setText("");
              break;
   }
}

// -------------------------------------------------
// Save edited ... UPDATE
// -------------------------------------------------
void confMode::SaveConfValues()
{
   QString s;
   int id;
   QSqlQuery query;
   switch (rIndex) {
    case 1: // BAND
             qy = "UPDATE wband SET work="+BandSetLineEdit->text();
             qy += ",mband='"+BandMyBandLineEdit->text();
             qy += "',freq='"+BandFreqLineEdit->text();
             qy += "',brig='"+BandRigLineEdit->text();
             qy += "' WHERE band='"+val+"'";
             query.exec(qy);
             rItem->setText(1,BandSetLineEdit->text());
             rItem->setText(2,BandMyBandLineEdit->text());
             rItem->setText(3,BandFreqLineEdit->text());
             rItem->setText(4,BandRigLineEdit->text());
            break;
    case 2: // MODE
             qy = "UPDATE wmode SET work="+ModeSetLineEdit->text();
             qy += " WHERE mode='"+val+"'";
             query.exec(qy);
             rItem->setText(0,ModeSetLineEdit->text());
            break;
    case 3: // PWR
             qy = "UPDATE wpwr SET work="+PwrWorkLineEdit->text();
             qy += ",watt='"+PwrWattLineEdit->text();
             qy += "' WHERE pwr="+val;
             query.exec(qy);
             rItem->setText(1,PwrWorkLineEdit->text());
             rItem->setText(2,PwrWattLineEdit->text());
            break;
    case 4: // Line
             qy = "UPDATE wline SET line='"+LineNameEdit->text();
             qy += "' WHERE id="+val;
             query.exec(qy);
             rItem->setText(1,LineNameEdit->text());
            break;
    case 5:  {  // AWD 
	      qy = "SELECT id FROM wawdlist WHERE atype='"+rItem->text(1)+"'";
	      query.exec(qy);
              query.next();
              if(query.size() != 1) {                             
		 QMessageBox::information(this,
                 tr("Information"),
                 tr("\n\nFehler - AWD Type ist mehrfach vorhanden !\nUpdate nicht möglich"),
                 QMessageBox::Ok);
		return;
	      }
	      
	      id = query.value(0).toString().toInt();
	      qDebug() << "id" << id << query.size();
	      
              if(AwdSetLineEdit->text().count() == 0)
                 AwdSetLineEdit->setText("0");
	      
              qy = "UPDATE wawdlist SET aset='"+AwdSetLineEdit->text();  // sort
              qy += "',atype='"+NameTypeLineEdit->text();                // AWD Name
	      
	      if((id == 3) || (id == 4)) {                               // IOTA + STATE    
                if(rItem->text(1).indexOf("IOTA") != -1)     // keine Aenderung - ist ARRL_feld
                   qy += "',adiftype='IOTA";
                else
                 if(rItem->text(1).indexOf("US-CA") != -1)   // keine Aenderung - ist ARRL_feld
                   qy += "',adiftype='STATE";
	      }
              else
                qy += "',adiftype='APP_QTLOG_"+NameTypeLineEdit->text();
 
              qy += "' WHERE id="+s.setNum(id); 
              query.exec(qy);
              qDebug() << qy;
	      
              rItem->setText(0,AwdSetLineEdit->text());       // aset - sort
              rItem->setText(1,NameTypeLineEdit->text());     // atype
              AwdSetLineEdit->setEnabled(FALSE);              // sperren
              NameTypeLineEdit->setEnabled(FALSE);            // sperren
             
              NewPushButton->hide();
             }
           break;
    case 6: // customs
           if(NameTypeLineEdit->text().count() == 0) {      
                // check ob custom1 oder custom2 eingebunden sind
               qy = "SELECT qhname,qstring FROM wlayout";
               query.exec(qy);
               while(query.next()) {
                  QString s = query.value(0).toString();
                  val = query.value(1).toString();  
                  if(val.indexOf("custom1") != -1) {          // custom_index suchen
                    QString f = "\nDas Feld 'custom1' ist im Layout '"+s+"' eingebunden\nLoeschen geht daher nicht";
                    QMessageBox::information( this,
                    tr("Customs"),f,
                    QMessageBox::Ok | QMessageBox::Default,
                    QMessageBox::NoButton, QMessageBox::NoButton);
                    return;
                  }
                  if(val.indexOf("custom2") != -1) {
                    QString f = "\nDas Feld  'custom2'  ist im Layout '"+s+"' eingebunden\nLoeschen geht daher nicht";
                    QMessageBox::information( this,
                    tr("Customs"),f,
                    QMessageBox::Ok | QMessageBox::Default,
                    QMessageBox::NoButton, QMessageBox::NoButton);
                   return;
                  }
               }
               qy = "UPDATE refnamen SET refnam=''";          // custom_feld_name loeschen
               qy += " WHERE dbfield='"+rItem->text(0)+"'";
               rItem->setText(1,"");
           }
           else {
             qy = "UPDATE refnamen SET refnam='"+NameTypeLineEdit->text();  // Custom_name setzen
             qy += "' WHERE dbfield='"+ rItem->text(0)+"'";
           }
           query.exec(qy);
           rItem->setText(1,NameTypeLineEdit->text());       // custom_user_namen zeigen              
           if(rItem->text(0).compare("custom1") == 0) {      // custom1 verändert     
              custom1 = rItem->text(0);
              val = NameTypeLineEdit->text();
              customId1 = 1;
           }
           else {                                            // ist custom2                    
             custom2 = rItem->text(0);
             val2 = NameTypeLineEdit->text();
             customId2 = 1;
           }
           NameTypeLineEdit->setEnabled(FALSE);
           break;
   }
}

// ---------------------------------------------
// Neuen AWARD_Type anlegen
// ---------------------------------------------
void confMode::NewAwdValues()
{
      QSqlQuery query;
      if(NameTypeLineEdit->text().count() == 0) {
        QMessageBox::information( this,
        tr("INFO"),
        tr("\n\nKeinen leeren AWD Type !"),
        QMessageBox::Ok);
        return;
      }
      
      qy = "SELECT aset FROM wawdlist WHERE atype='"+NameTypeLineEdit->text()+"'";
      query.exec(qy);
      if(query.size() != 0) {                                         // schon vorhanden   
           QMessageBox::information(this,
           tr("Information"),
           tr("\n\nAWD Type ist schon vorhanden"),
           QMessageBox::Ok);
          return;
      }
      if(AwdSetLineEdit->text().count() == 0)
         AwdSetLineEdit->setText("0");
      qy = "INSERT INTO wawdlist VALUES(0,'"+AwdSetLineEdit->text();  // aset : sortNo
      qy += "','"+NameTypeLineEdit->text();                           // atype : Name_Award-Type
      qy += "','APP_QTLOG_"+NameTypeLineEdit->text()+"')";            // Applocation_QtLog + Name-AWtype
      query.exec(qy);
         
      QTreeWidgetItem *item = new QTreeWidgetItem(awdList);
      item->setText(0,AwdSetLineEdit->text());                        // aset
      item->setText(1, NameTypeLineEdit->text());                     // atype
      NewPushButton->hide();
}
