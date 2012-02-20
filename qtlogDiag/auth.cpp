/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */
//auth.cpp  01.05.2007
#include <QSettings>
#include <QString>
#include <QTabWidget>
#include <QtGui>
#include <QtSql>
#include "auth.h"
#include "version.h"

auth::auth(QWidget * parent) : QDialog(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
   setupUi(this);
   int n = settings.value("FontSize").toString().toInt();
   QFont font;
   font.setPointSize(n); 			
   setFont(font);
   connect(ButtonEnde, SIGNAL(clicked()), this, SLOT(goExit()));
     QString s = tr("\n\n  QtLog Version : ");
     s += VERSION;
     s += tr("   /  DB Release  ");
     s += RELEASE;
     s += tr(" \n  Kompiliert  ");
     s += __DATE__;
     s += tr("\n  Qt Version ");
     s += QT_VERSION_STR;
     s += tr("  -  MySQL Server Version 5.0.45");
     s += "\n\n  ";
     s += AUTH;
     s += " \n  ";
     s += tr("Unterstuetzung und Betatest : Andreas DK9HE\n\n  ");
     s += HPAGE;
     authtextEdit->insertPlainText(s);
}

auth::~auth()
{
}
void auth::goExit()
{
    accept();                                       
}
