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
#include "hilfe.h"
#include "../qtlogDiag/dirmngr.h"
extern DirMngr dirMngr;

hilfe::hilfe(QWidget * parent) : QMainWindow(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
    setupUi(this);
    connect(ButtonEnde, SIGNAL(clicked()), this, SLOT(goExit()));
    connect(listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(itemChangedCb(QListWidgetItem *)));
    splitter->restoreState(settings.value("hilfe/splitterSizes").toByteArray());
    resize(settings.value("hilfe/Size",sizeHint()).toSize());
    restoreState(settings.value("hilfe/Properties").toByteArray());
    timer = new QTimer(this);                     
    connect(timer,SIGNAL(timeout()), this, SLOT(getHilfe()));
    //timer->start(1000);                         
    timer->start(300);                            
    
    textEdit->setAcceptRichText(TRUE);
    
}

hilfe::~hilfe()
{
   settings.setValue("hilfe/splitterSizes", splitter->saveState());
   settings.setValue("hilfe/Size",size());
   settings.setValue("hilfe/Properties",saveState());
}

void hilfe::goExit()
{
  qApp->quit();
}

void hilfe::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_F10 :                           
             goExit();
           break;
   }
}


void hilfe::getHilfe()
{
    timer->stop();
#ifdef Q_WS_WIN
    s=dirMngr.CreatePath(QString("doc/%1.html").arg(settings.value("Val").toString()),DIR_PROGRAM);
#else
    s = getenv("HOME");
    s += "/log/doc/";
    s += settings.value("Val").toString();
    s += ".html";
#endif
    s.remove(QChar(' '), Qt::CaseInsensitive);
    showText(s);
}


void hilfe::itemChangedCb(QListWidgetItem * item)
{
    b = item->text();
    if(b.compare("DX-SubPrefixListe") == 0)
         b = "DX-LaenderListe";
#ifdef Q_WS_WIN
    s=dirMngr.CreatePath(QString("doc/%1.html").arg(b),DIR_PROGRAM);
#else
    s = getenv("HOME");
    s += "/log/doc/"+b+".html";
#endif
    s.remove(QChar(' '), Qt::CaseInsensitive);
    showText(s);
}


void hilfe::showText(QString name)
{
    QFile datei(name);
    if(! datei.open(QIODevice::ReadOnly)) {
        textEdit->clear();
        return;                                       
    }
    textEdit->clear();
    QTextStream instream( &datei);
    s = instream.readAll();
    textEdit->insertHtml(s);
}
