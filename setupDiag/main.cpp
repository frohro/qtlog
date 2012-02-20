/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.

   V 1.5 : 20.08.2008
********************************************************************** */

// main.cpp

#include <QApplication>
#include <QSettings>
#include "setup.h"
#include "../qtlogDiag/dirmngr.h"
#include "../qtlogDiag/version.h"


extern DirMngr dirMngr;


int main(int argc, char *argv[])
{
    QString b, s, h, p;
    int i;
    
    QString home = QDir::homePath(); 
    s = home+"/.qtlog";                         // prüfe ob Ordner $HOME.qtlog vorhanden ist
    QDir dir(s);
    if(!dir.exists())
       dir.mkdir(s);                            // nein; anlegen          
    
    p = "ps -e | grep mysqld > "+home+"/.qtlog/rigstatus";
    i = system(p.toAscii());                    // suche den mysqld_prozess
    h = home;
    h += "/.qtlog/rigstatus";                   // prüfe status auf erfolg
    QFile iniFile(h);
    iniFile.open(QIODevice::ReadOnly);
    QTextStream istream( &iniFile);             // oeffne status_file
    p = istream.readLine(0);                    // nur 1.Zeile
    iniFile.close();
    if(p.indexOf("mysqld") != -1) {             // ist mysqld_proz vorhanden
      b = home+"/.config/QtLog/qtlog.ini";      // versuche qtlog.ini zu öffnen
      QFile iniFile(b);
      if(!iniFile.open(QIODevice::ReadOnly)) {  // wenn FEHLER - ist ini nicht vorhanden
         s = home+"/.config/QtLog";             // vorbereitung  - NEU_installation -
         QDir dir(s);
         dir.mkdir(s);                          // DIR anlegen 
         s = "cp "+home+"/log/initTable/qtlog.ini "+home+"/.config/QtLog/qtlog.ini"; // copy ini
         StartProcess(s);
      }
      else {                                   // ini.file ist vorhanden, prüfe AUDIT
        QTextStream istream( &iniFile);
        s = istream.readLine(0);               // 1.zeile [General]
        s = istream.readLine(0);               // 2.zeile audit=5.15
        b = s.mid(6);
        if(b.compare(AUDIT) != 0) {            // ist ini_audit != source_AUDIT "dann UPGRADE !"
           s = "mv "+home+"/.config/QtLog/qtlog.ini "+home+"/.config/QtLog/qtlog.old"; // rename.old
           StartProcess(s);
           s = "cp "+home+"/log/initTable/qtlog.ini "+home+"/.config/QtLog/qtlog.ini"; // copy NEW
           StartProcess(s);
        }
        iniFile.close();
      }
    }
    else {
      qDebug() << "FEHLER : Der MYSQL-Server ist nicht vorhanden";
      exit(0);
    }
    
// ------------------------------------

    QApplication a(argc, argv);
    setup setupD;
    setupD.show();
    return a.exec();
}

