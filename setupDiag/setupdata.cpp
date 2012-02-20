/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007 / 20.10.2008 / 08.07.09 / 27-05-2010
********************************************************************** */
//updatedb.cpp
#include <QtGui>
#include <QtSql>
#include "setup.h"
#include "../qtlogDiag/version.h"
#include "../qtlogDiag/dirmngr.h"
extern DirMngr dirMngr;


void setup::loadTable(QString tablename)
{
       timer->stop();
       QString pathToMysql = settings.value("mySQLPath").toString();  
       QString p = PathInitTable->text()+tablename;
       QFile file;
       if(!file.exists(p)) {
         QApplication::restoreOverrideCursor();
         QMessageBox::information( this,
         tr("INFO"),
         tr("\nRESTORE nicht moeglich - Datei nicht vorhanden !"),
         QMessageBox::Ok);
           return;
       }
       s = pathToMysql + "mysql -u"+settings.value("dbuser").toString()+" -p"+settings.value("dbpasswd").toString()+" logdb < ";
       s += p;
       
       StartProcess(s.toAscii());
}



//---------------------------------------------------------
void setup::updatework()
{
    if( release.compare(RELEASE) == 0 ) {
       textEdit->clear();
       s = tr("\nEin Upgrade der Datenbank ist nicht erforderlich.\n");
       s += tr("Die Datenbank-Release Nr: ")+release+tr(" entspricht der aktuellen QtLog Version\n");
       textEdit->insertPlainText(s);
       ButtonUpdate->hide();
       ButtonInstallold->hide();
       checkBoxInstallNeu->hide();
       stackedWidget->setCurrentIndex(0);                
       return;
    }
    else                                                 
    if((release.compare("5.6") == 0) || (release.compare("5.5") == 0) || (release.compare("5.4") == 0)) { 
      upgradeto58();                                     
    }
    else
    if( release.compare("5.5") == 0) {                   
       upgradeto56();                                    
    }
    else
    if( release.compare("5.4") == 0 ) {                  
      upgradeto55();                                     
    }
    else
    if( release.compare("5.3") == 0 ) {                  
      upgradeto54();
    }
    else
    if( release.compare("5.2") == 0 ) {
       update52();
    }
    else
    if( release.compare("5.1") == 0 ) {
       update51();
    }
    else 
      qDebug() << release << "ERROR_update";
}



void setup::upgradeto58()                        
{
    QString val;
    s = getenv("HOME");                          
    s += "/.qtlog/qlogfile";
    QFile lfile(s);
    QTextStream out(&lfile);
    if(! lfile.open(QIODevice::Append | QIODevice::Text)) {
       s = tr("SetUp Info ");
       b = tr("\nqLogfile nicht vorhanden\nGravierender FEHLER im TarArchiv\nInstallation nicht noeglich !\n");
       QMessageBox::information( this,s,b,
       QMessageBox::Ok | QMessageBox::Default,
       QMessageBox::NoButton, QMessageBox::NoButton);
       out << b;
       lfile.close();
       exit(0);
    }
    out << "UpDate running Rel 5.8\n";
    QSqlQuery query;
    qy = "SELECT logname,relnr FROM dblogs";    
    query.exec(qy);
    while(query.next()) {
       s = query.value(0).toString();           
       loglist.append(s);
       t = query.value(1).toString();           
       out << "log:" << s << " Rel: "+t+" ";
    }
 
    //loadTable("backup-tApplist.sql");
    loadTable("backup-toperator.sql");
    //loadTable("backup-uswasref.sql");
    loadTable("backup-uswas.sql");
    loadTable("backup-wCband.sql");
    loadTable("backup-wCmaster.sql");
    loadTable("backup-wContest.sql");
    loadTable("backup-wContRqtc.sql");
    loadTable("backup-wCqso.sql");
    loadTable("backup-wCruls.sql");
    loadTable("backup-wproz.sql");
    
    qy = "DROP TABLE IF EXISTS `wCtype`";
    query.exec(qy);
 
    QString release = RELEASE;                  
    qy = "UPDATE dblogs SET relnr='"+release+"'";
    query.exec(qy);
    out << qy << "\ndone\n\n";
    lfile.close();
    s = tr("\n\n  -------- Upgrate durchgeführt -- Rel. NEU ");
    s += release+"  ------ \n";
    textEdit->insertPlainText(QString::fromUtf8(s.toAscii()));
    ButtonUpdate->hide();
}



void setup::upgradeto56()                        
{
    QString val;
    s = getenv("HOME");                          
    s += "/.qtlog/qlogfile";
    QFile lfile(s);
    QTextStream out(&lfile);
    if(! lfile.open(QIODevice::Append | QIODevice::Text)) {
       s = tr("SetUp Info ");
       b = tr("\nqLogfile nicht vorhanden\nGravierender FEHLER im TarArchiv\nInstallation nicht noeglich !\n");
       QMessageBox::information( this,s,b,
       QMessageBox::Ok | QMessageBox::Default,
       QMessageBox::NoButton, QMessageBox::NoButton);
       out << b;
       lfile.close();
       exit(0);
    }
    out << "UpDate running Rel 5.6\n";
    QSqlQuery query;
    qy = "SELECT logname,relnr FROM dblogs";    
    query.exec(qy);
    while(query.next()) {
       s = query.value(0).toString();           
       loglist.append(s);
       t = query.value(1).toString();           
       out << "log:" << s << " Rel: "+t+" ";
    }
    
    loadTable("backup-uswas.sql");              
    QString release = RELEASE;                  
    qy = "UPDATE dblogs SET relnr='"+release+"'";
    query.exec(qy);
    out << qy << "\ndone\n\n";
    lfile.close();
    s = tr("\n\n  -------- Upgrate durchgeführt -- Rel. NEU ");
    s += release+"  ------ \n";
    textEdit->insertPlainText(QString::fromUtf8(s.toAscii()));
    ButtonUpdate->hide();
}



void setup::upgradeto55()
{
    
    QString val;
    s = getenv("HOME");                         
    s += "/.qtlog/qlogfile";
    QFile lfile(s);
    QTextStream out(&lfile);
    if(! lfile.open(QIODevice::Append | QIODevice::Text)) {
       s = tr("SetUp Info ");
       b = tr("\nqLogfile nicht vorhanden\nGravierender FEHLER im TarArchiv\nInstallation nicht noeglich !\n");
       QMessageBox::information( this,s,b,
       QMessageBox::Ok | QMessageBox::Default,
       QMessageBox::NoButton, QMessageBox::NoButton);
       out << b;
       lfile.close();
       exit(0);
    }
    
    out << "UpDate running Rel 5.5\n";
    QSqlQuery query;
    qy = "SELECT logname,relnr FROM dblogs";    
    query.exec(qy);
    while(query.next()) {
       s = query.value(0).toString();           
       loglist.append(s);
       t = query.value(1).toString();           
       out << "log:" << s << " Rel: "+t+" ";
    }
    
    //QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    loadTable("backup-wproz.sql");              
    
    QString release = RELEASE;                  
    qy = "UPDATE dblogs SET relnr='"+release+"'";
    query.exec(qy);
    out << qy << "\ndone\n\n";
    lfile.close();
    s = tr("\n\n  ------ Upgrate durchgeführt -- Rel. NEU ");
    s += release+"  ------ \n";
    textEdit->insertPlainText(QString::fromUtf8(s.toAscii()));
    ButtonUpdate->hide();
    //QApplication::restoreOverrideCursor();
}


void setup::upgradeto54()
{
    
    QString val;
    s = getenv("HOME");                         
    s += "/.qtlog/qlogfile";
    QFile lfile(s);
    QTextStream out(&lfile);
    if(! lfile.open(QIODevice::Append | QIODevice::Text)) {
       s = tr("SetUp Info ");
       b = tr("\nqLogfile nicht vorhanden\nGravierender FEHLER im TarArchiv\nInstallation nicht noeglich !\n");
       QMessageBox::information( this,s,b,
       QMessageBox::Ok | QMessageBox::Default,
       QMessageBox::NoButton, QMessageBox::NoButton);
       out << b;
       lfile.close();
       exit(0);
    }
    
    out << "UpDate running\n";
    QSqlQuery query;
    qy = "SELECT logname,relnr FROM dblogs";    
    query.exec(qy);
    while(query.next()) {
       s = query.value(0).toString();           
       loglist.append(s);
       t = query.value(1).toString();           
       out << "log:" << s << " Rel: "+t+" ";
    }
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    
    loadTable("backup-refnamen.sql");           
    loadTable("backup-uscounty.sql");           
    loadTable("backup-tla.sql");                
    loadTable("backup-tpref.sql");              
    loadTable("backup-wband.sql");              
    loadTable("backup-uswasref.sql");           
    loadTable("backup-uswas.sql");              
    loadTable("backup-wCband.sql");             
    loadTable("backup-wContest.sql");           
    loadTable("backup-wContRqtc.sql");          
    loadTable("backup-wCqso.sql");              
    loadTable("backup-wCtype.sql");             
    loadTable("backup-wproz.sql");              
    loadTable("backup-wquery.sql");             
    loadTable("backup-wspotnodes.sql");         
    loadTable("backup-wspotsparm.sql");         
    
    QString release = RELEASE;                  
    qy = "UPDATE dblogs SET relnr='"+release+"'";
    query.exec(qy);
    out << qy << "\ndone\n\n";
    lfile.close();
    s = tr("\n\n  ------ Upgrate durchgeführt -- Rel. NEU ");
    s += release+"  ------ \n";
    textEdit->insertPlainText(QString::fromUtf8(s.toAscii()));
    ButtonUpdate->hide();
    QApplication::restoreOverrideCursor();
}


void setup::update52()
{
    QString val;
    s = getenv("HOME");                                  
    s += "/.qtlog/qlogfile";
    QFile lfile(s);
    QTextStream out(&lfile);
    if(! lfile.open(QIODevice::Append | QIODevice::Text)) {
       s = tr("SetUp Info ");
       b = tr("\nqLogfile nicht vorhanden\nGravierender FEHLER im TarArchiv\nInstallation nicht noeglich !\n");
       QMessageBox::information( this,s,b,
       QMessageBox::Ok | QMessageBox::Default,
       QMessageBox::NoButton, QMessageBox::NoButton);
       out << b;
       lfile.close();
       exit(0);
    }
    out << "UpDate running\n";
    QSqlQuery query;
    qy = "SELECT logname,relnr FROM dblogs";    
    query.exec(qy);
    while(query.next()) {
       s = query.value(0).toString();           
       loglist.append(s);
       t = query.value(1).toString();           
       out << "log:" << s << " Rel: "+t+" ";
    }
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    for( int i = 0; i < loglist.size(); ++i) {
      s = loglist.at(i);
      qy = "ALTER TABLE "+s+" ADD qslmsg varchar(255) NOT NULL AFTER ktext";  
      query.exec(qy);
      out << qy << "\n\n";
      qy = "ALTER TABLE "+s+"qsl ADD ag varchar(30) not NULL AFTER contestid"; 
      query.exec(qy);
      out << qy << "\n\n";
      
      qy = "DROP TABLE IF EXISTS `"+s+"card`";
      query.exec(qy);
      qy = "CREATE TABLE `"+s+"card` (";
      qy += "`cid` int(11) NOT NULL auto_increment,";
      qy += "`qsocid` int(11) default NULL,";
      qy += "`qcall` varchar(16) default NULL,";
      qy += "`qsoday` date default NULL,";
      qy += "`cband` varchar(10) default NULL,";
      qy += "`cmode` varchar(8) default NULL,";
      qy += "`cbtime` time default NULL,";
      qy += "`qsl` varchar(4) default NULL,";
      qy += "`strlen` int(11) default NULL,";
      qy += "`qcard` mediumblob default NULL,";
      qy += "PRIMARY KEY (`cid`),";
      qy += "KEY `qslidx` (`cid`,`qsocid`))";
      query.exec(qy);
    }
    loadTable("backup-refnamen.sql");
    
    
    QString release = RELEASE;
    qy = "UPDATE dblogs SET relnr='"+release+"'";
    query.exec(qy);
    out << qy << "\ndone\n\n";
    lfile.close();
    s = tr("\n\n  ------ Upgrate durchgeführt -- Rel Nr 5.3  ------ \n");
    textEdit->insertPlainText(QString::fromUtf8(s.toAscii()));
    ButtonUpdate->hide();
    QApplication::restoreOverrideCursor();
}

void setup::update51()
{
    QString val;
    s = getenv("HOME");                                  
    s += "/.qtlog/qlogfile";
    QFile lfile(s);
    QTextStream out(&lfile);
    if(! lfile.open(QIODevice::Append | QIODevice::Text)) {
       s = tr("SetUp Info ");
       b = tr("\nqLogfile nicht vorhanden\nGravierender FEHLER im TarArchiv\nInstallation nicht noeglich !\n");
       QMessageBox::information( this,s,b,
       QMessageBox::Ok | QMessageBox::Default,
       QMessageBox::NoButton, QMessageBox::NoButton);
       out << b;
       lfile.close();
       exit(0);
    }
    out << "UpDate running\n";
    QSqlQuery query;
    qy = "SELECT logname,relnr FROM dblogs";    
    query.exec(qy);
    while(query.next()) {
       s = query.value(0).toString();           
       loglist.append(s);
       t = query.value(1).toString();           
       out << "log:" << s << " Rel: "+t+" ";
    }
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    for( int i = 0; i < loglist.size(); ++i) {
      s = loglist.at(i);
      qy = "DROP TABLE IF EXISTS "+s+"qsl";
      out << qy << "\n\n";
      query.exec(qy);
                                                       
      qy = "CREATE TABLE `"+s+"qsl` (";                
      qy += "`eid` int(11) NOT NULL auto_increment,";  
      qy += "`qsoid` int(11) NOT NULL,";               
      qy += "`qcall` varchar(16) default NULL,";       
      qy += "`qsoday` date default NULL,";             

      qy += "`contestid` varchar(80) default NULL,";   
      qy += "`eqslr` char(4) default NULL,";           
      qy += "`eqsls` char(4) default NULL,";           
      qy += "`eqslrd` date default NULL,";             
      qy += "`eqslsd` date default NULL,";             
      qy += "`lotqslr` char(4) default NULL,";    
      qy += "`lotqsls` char(4) default NULL,";    
      qy += "`lotqslrd` date default NULL,";      
      qy += "`lotqslsd` date default NULL,";      
      qy += "`contactdOp` varchar(12) default NULL,"; 
      qy += "`freqrx` varchar(10) default NULL,";     
      qy += "`srx` varchar(12) default NULL,";        
      qy += "`srxstr` varchar(12) default NULL,";     
      qy += "`stx` varchar(12) default NULL,";        
      qy += "`stxstr` varchar(12) default NULL,";     
      qy += "`qinfo` varchar(12) default NULL,";  
      qy += "`comment` char(80) default NULL,";   
      qy += "`custom1` char(128) default NULL,";  
      qy += "`custom2` char(128) default NULL,";  
      qy += "`sel` char(8) default NULL,";        
      qy += "PRIMARY KEY  (`qsoid`),";
      qy += "KEY `eidx` (`eid`,`qsoid`))";
      query.exec(qy);
      out << qy << "\n\n";
     
     
      QString id;
      QString r;
      QString c;
      QString dx;
      QString qsl;
      qy = "SELECT id,qslr,qsls FROM "+s;
      query.exec(qy);
      n = query.size(); 
      out << qy << " size=" << n << "\n\n";
      while(query.next()) {
        n = 0;
        id = query.value(n++).toString();       
        r = query.value(n++).toString();        
        t = query.value(n++).toString();        
        
        if(r.compare("x") == 0) 
          r = "R";
        else 
          if(r.compare("e") == 0)
            r = "Y";
        if(t.compare("x") == 0) 
           t = "R";
        else 
          if(t.compare("b") == 0)
            t = "Y";
        QSqlQuery query;
        qy = "UPDATE "+s+" SET qslr='"+r+"',qsls='"+t+"' WHERE id="+id;   
        query.exec(qy);
       }
       
       
       int Id;
       qy = "SELECT id,rufz,day,qsls,dxcc,contest FROM "+s+"om LEFT JOIN "+s+" ON (omid=oid) ORDER BY day";
       query.exec(qy);
       out << qy << " size=" << query.size() << "\n\n";
       while(query.next()) {
          n = 0;
          Id = query.value(n++).toInt();          
          r = query.value(n++).toString();        
          t = query.value(n++).toString();        
          qsl = query.value(n++).toString();      
          dx = query.value(n++).toString();       
          c = query.value(n++).toString();        
          id.setNum(Id);
          qy = "INSERT INTO "+s+"qsl VALUES (0,"+id+",'"+r+"','"+t+"','"+c+"','";
          if(qsl.compare("R") == 0)               
            qy += "','";                          
          else
            qy += "-','-";
          qy += "','','','','R','','','','','','','','','','','','','')";
          QSqlQuery query(qy);                    
       }
       
       
       qy = "UPDATE "+s+" SET srx=''";                             
       query.exec(qy);
       out << qy << "\n\n";
       qy = "ALTER TABLE "+s+" CHANGE srx qslRvia varchar(6)";     
       query.exec(qy);
       out << qy << "\n";
       qy = "ALTER TABLE "+s+" ADD qslSvia varchar(6) NOT NULL AFTER qslRvia"; 
       query.exec(qy);
       out << qy << "\n\n";
       qy = "UPDATE "+s+" SET contest=''";                         
       query.exec(qy);
       out << qy << "\n\n";
       qy = "ALTER TABLE "+s+" CHANGE contest prflg varchar(4)";   
       query.exec(qy);
       out << qy << "\n";
       
       qy = "UPDATE "+s+" SET qslRvia='B' WHERE qslr='Y'";         
       query.exec(qy);
       out << qy << "\n\n";
       qy = "UPDATE "+s+" SET qslSvia='B' WHERE qsls='Y'";         
       query.exec(qy);
       out << qy << "\n\n";
       qy = "UPDATE "+s+" SET prflg='B' WHERE qsls='R'";           
       query.exec(qy);
       out << qy << "\n\n";
     } //- end for( ... )
     
     QString id;
     qy = "ALTER TABLE wawdlist DROP akey";                         
     query.exec(qy);
     out << qy << "\n\n";
     qy = "ALTER TABLE wawdlist ADD adiftype varchar(18) NOT NULL"; 
     query.exec(qy);
     out << qy << "\n\n";
     qy = "SELECT id,atype FROM wawdlist WHERE aset != '0'";       
     query.exec(qy);
     out << qy << " size=" << query.size() << "\n\n";
     while(query.next()) {
       n = 0;
       id = query.value(n++).toString();
       val = query.value(n++).toString();
       if(val.compare("IOTA") == 0)
        b = "IOTA";                                                
       else
       if(val.compare("USA_CA") == 0)
        b = "STATE";                                               
       else {
         b = "APP_QTLOG_";
         b += val;
       }
       qy = "UPDATE wawdlist SET adiftype='"+b+"' WHERE id="+id;
       out << qy << "\n\n";
       QSqlQuery query(qy);
     }
     
     QString pathToMysql = settings.value("mySQLPath").toString(); 
     QString p = PathInitTable->text()+"backup-refnamen.sql";
     QFile file;
     if(!file.exists(p)) {
         QApplication::restoreOverrideCursor();
         QMessageBox::information( this,
         tr("INFO"),
         tr("\nRESTORE nicht moeglich - Datei nicht vorhanden !"),
         QMessageBox::Ok);
         out << p;
         out << " nicht vorhanden\n";
         return;
     }
     s = pathToMysql + "mysql -u"+settings.value("dbuser").toString()+" -p"+settings.value("dbpasswd").toString()+" logdb < ";
     s += p;
     out << s << "\n\n";
     StartProcess(s.toAscii());
    
     pathToMysql = settings.value("mySQLPath").toString();        
     p = PathInitTable->text()+"backup-uscounty.sql";
     if(!file.exists(p)) {
         QApplication::restoreOverrideCursor();
         QMessageBox::information( this,
         tr("INFO"),
         tr("\nRESTORE nicht moeglich - Datei nicht vorhanden !"),
         QMessageBox::Ok);
         return;
     }
     s = pathToMysql + "mysql -u"+settings.value("dbuser").toString()+" -p"+settings.value("dbpasswd").toString()+" logdb < ";
     s += p;
     out << s << "\n\n";
     StartProcess(s.toAscii());
    
     pathToMysql = settings.value("mySQLPath").toString();        
     p = PathInitTable->text()+"backup-prmedium.sql";
     if(!file.exists(p)) {
         QApplication::restoreOverrideCursor();
         QMessageBox::information( this,
         tr("INFO"),
         tr("\nRESTORE nicht moeglich - Datei nicht vorhanden !"),
         QMessageBox::Ok);
         return;
     }
     s = pathToMysql + "mysql -u"+settings.value("dbuser").toString()+" -p"+settings.value("dbpasswd").toString()+" logdb < ";
     s += p;
     out << s << "\n\n";
     StartProcess(s.toAscii());
    
     pathToMysql = settings.value("mySQLPath").toString();        
     p = PathInitTable->text()+"backup-wquery.sql";
     if(!file.exists(p)) {
         QApplication::restoreOverrideCursor();
         QMessageBox::information( this,
         tr("INFO"),
         tr("\nRESTORE nicht moeglich - Datei nicht vorhanden !"),
         QMessageBox::Ok);
         return;
     }
     s = pathToMysql + "mysql -u"+settings.value("dbuser").toString()+" -p"+settings.value("dbpasswd").toString()+" logdb < ";
     s += p;
     out << s << "\n\n";
     StartProcess(s.toAscii());
   
     pathToMysql = settings.value("mySQLPath").toString();        
     p = PathInitTable->text()+"backup-wlayout.sql";
     if(!file.exists(p)) {
         QApplication::restoreOverrideCursor();
         QMessageBox::information( this,
         tr("INFO"),
         tr("\nRESTORE nicht moeglich - Datei nicht vorhanden !"),
         QMessageBox::Ok);
         return;
     }
     s = pathToMysql + "mysql -u"+settings.value("dbuser").toString()+" -p"+settings.value("dbpasswd").toString()+" logdb < ";
     s += p;
     out << s << "\n\n";
     StartProcess(s.toAscii());
     
     
     qy = "UPDATE dblogs SET relnr='5.2'";
     query.exec(qy);
     out << qy << "\ndone\n\n";
     lfile.close();
     release = RELEASE;
     s = tr("\n\n  ------ Update durchgeführt -- Rel Nr 5.2 ------ \n");
     textEdit->insertPlainText(QString::fromUtf8(s.toAscii()));
     ButtonUpdate->hide();
     QApplication::restoreOverrideCursor();
}
