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
#include "mediumText.h"
#include "../qtlogDiag/dirmngr.h"

mediumTxt::mediumTxt(QWidget * parent) : QDialog(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
  setupUi(this);
  n = settings.value("FontSize").toString().toInt();
  QFont font;
  font.setPointSize(n); 			
  setFont(font);
  connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(textPageChanged(int)));
  connect(ButtonEnde, SIGNAL(clicked()), this, SLOT(goExit()));
  connect(ButtonUpdate, SIGNAL(clicked()), this, SLOT(updateMediumData()));
  connect(ButtonAdd, SIGNAL(clicked()), this, SLOT(addMediumData()));
  connect(ButtonDel, SIGNAL(clicked()), this, SLOT(delMediumData()));
  connect(ButtonHelp, SIGNAL(clicked()), this, SLOT(getHilfeAc()));
  connect(dxccWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(showLabelDataCb(QListWidgetItem *)));
  connect (dxccW, SIGNAL(textChanged(QString)), this, SLOT(dxccTextChanged(QString)));
  pageIndex = settings.value("Val").toInt();
  tabWidget->setCurrentIndex(pageIndex);
  row = 0;
  n = 0;
  textPageChanged(pageIndex);
  selectItem = 0;
}

mediumTxt::~mediumTxt()
{
}


void mediumTxt::getHilfeAc()
{
    settings.setValue("Val","Qsl Text");
    settings.sync();
    StartProcess("hilfedb &");
}

void mediumTxt::textPageChanged(int page)
{
    QSqlQuery query;
    row = 0;
    n = 0;
    dxccWidget->clear();
    pageIndex = page;
    switch(page) {
    case 0 :  query.exec("SELECT name FROM qslk WHERE ktype='L'"); 
              while(query.next()) {
                QListWidgetItem * nItem = new QListWidgetItem;
                nItem->setText( query.value(n).toString());
                dxccWidget->insertItem(row++,nItem);
              }
              LdearW->setText("");
              LwellW->setText("");
              Ltext1W->setText("");
              Ltext2W->setText("");
              Ltext3W->setText("");
              break;
    case 1 :  query.exec("SELECT name FROM qslk WHERE ktype='LS'");
              while(query.next()) {
                QListWidgetItem * nItem = new QListWidgetItem;
                nItem->setText( query.value(n).toString());
                dxccWidget->insertItem(row++,nItem);
              }
              SLtext1W->setText("");
              SLtext2W->setText("");
              break;
    case 2 :  query.exec("SELECT name FROM qslk WHERE ktype='K'");
              while(query.next()) {
                QListWidgetItem * nItem = new QListWidgetItem;
                nItem->setText( query.value(n).toString());
                dxccWidget->insertItem(row++,nItem);
              }
              KdearW->setText("");
              KwellW->setText("");
              KcqzW->setText("");
              Ktext1W->setText("");
              Ktext2W->setText("");
              Ktext3W->setText("");
              break;
    case 3 :  query.exec("SELECT name FROM qslk WHERE ktype='KS'");
              while(query.next()) {
                QListWidgetItem * nItem = new QListWidgetItem;
                nItem->setText( query.value(n).toString());
                dxccWidget->insertItem(row++,nItem);
              }
              SKtext1W->setText("");
              SKtext2W->setText("");
              SKtext3W->setText("");
              break;
    case 4 :  query.exec("SELECT name FROM qslk WHERE ktype='LSWL'");
              while(query.next()) {
                QListWidgetItem * nItem = new QListWidgetItem;
                nItem->setText( query.value(n).toString());
                dxccWidget->insertItem(row++,nItem);
              }
              LSdearW->setText("");
              LSwellW->setText("");
              LScqzW->setText("");
              LStext3W->setText("");
              break;
    case 5 :  query.exec("SELECT name FROM qslk WHERE ktype='KSWL'");
              while(query.next()) {
                QListWidgetItem * nItem = new QListWidgetItem;
                nItem->setText( query.value(n).toString());
                dxccWidget->insertItem(row++,nItem);
              }
              KSdearW->setText("");
              KSwellW->setText("");
              KScqzW->setText("");
              KStext3W->setText("");
              break;
    }
}


void mediumTxt::dxccTextChanged(QString str)
{
   str = str.toUpper();
   dxccW->setText(str);
}


void mediumTxt::showLabelDataCb(QListWidgetItem * item)
{
   selectItem = item;
   if( pageIndex == 0 ) {              
       showLabelData(item);
   }
   else
     if( pageIndex == 1 ) {            
        showSLabelData(item);
     }
   else                                
     if( pageIndex == 2 ) {            
        showKartData(item);
     }
   else
     if( pageIndex == 3 ) {            
        showSKartData(item);
     }
   else                                
     if( pageIndex == 4 ) {            
        showSWL_LabelData(item);
     }
   else
     if( pageIndex == 5 ) {            
        showSWL_KartData(item);
     }
}

void mediumTxt::updateMediumData()
{
   if( pageIndex == 0 ) {              
       updateLabelData();
   }
   else
     if( pageIndex == 1 ) {            
        updateSLabelData();
     }
   else                           
     if( pageIndex == 2 ) {             
        updateKartData();
     }
   else
     if( pageIndex == 3 ) {             
        updateSKartData();
     }
   else                           
     if( pageIndex == 4 ) {             
        updateSWL_LabelData();
     }
   else
     if( pageIndex == 5 ) {             
        updateSWL_KartData();
     }
}

void mediumTxt::addMediumData()
{
   if( pageIndex == 0 ) {              
       addLabelData();
   }
   else
     if( pageIndex == 1 ) {            
        addSLabelData();
     }
   else                                
     if( pageIndex == 2 ) {            
        addKartData();
     }
   else
     if( pageIndex == 3 ) {            
        addSKartData();
     }
   else                                
     if( pageIndex == 4 ) {            
        addSWL_LabelData();
     }
   else
     if( pageIndex == 5 ) {            
        addSWL_KartData();
     }
}

void mediumTxt::delMediumData()
{
   if( pageIndex == 0 ) {              
       delLabelData();
   }
   else
     if( pageIndex == 1 ) {            
        delSLabelData();
     }
   else                                
     if( pageIndex == 2 ) {            
        delKartData();
     }
  else
     if( pageIndex == 3 ) {            
        delSKartData();
     }
  else                                 
     if( pageIndex == 4 ) {            
        delSWL_LabelData();
     }
   else
     if( pageIndex == 5 ) {            
        delSWL_KartData();
     }
}

void mediumTxt::goExit()
{
   accept();                              
}
