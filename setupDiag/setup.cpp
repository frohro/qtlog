/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5
********************************************************************** */
//setup.cpp   20.08.2008 / 06.06.09

#include <QtGui>
#include "setup.h"
#include "../qtlogDiag/version.h"
#include "../qtlogDiag/dirmngr.h"
extern DirMngr dirMngr;

setup::setup(QWidget * parent) : QMainWindow(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
   setupUi(this);
   
   n = settings.value("FontSize").toString().toInt();
   QFont font;
   font.setPointSize(n);                             
   setFont(font);
   
   connect(actionInstallations_Hilfe,SIGNAL(triggered(bool)), this,SLOT(hilfeAc()));
   connect(ButtonClose1, SIGNAL(clicked()), this, SLOT(goExit()));
   connect(ButtonInfoClose, SIGNAL(clicked()), this, SLOT(closeUpdate()));
   connect(ButtonReInstallClose, SIGNAL(clicked()), this, SLOT(closeReInstall()));
   connect(ButtonParamUpdateClose, SIGNAL(clicked()), this, SLOT(closeParamUpdate()));
   connect(checkBoxParam, SIGNAL(pressed()), this, SLOT(checkBoxParamCb()));
   connect(checkBoxUpdate, SIGNAL(pressed()), this, SLOT(checkBoxUpdateCb()));
   connect(checkBoxReInstall, SIGNAL(pressed()), this, SLOT(checkBoxReInstallCb()));
   connect(ButtonSetParam, SIGNAL(clicked()), this, SLOT(updateParam()));
   connect(ButtonDoInstall, SIGNAL(clicked()), this, SLOT(installQtlog()));
   connect(ButtonUpdate, SIGNAL(clicked()), this, SLOT(updatework()));
   connect(ButtonInstallold, SIGNAL(clicked()), this, SLOT(installOldCb()));
   s = tr("Programm Version  ");
   s +=VERSION;
   s += tr(" Rel: ");
   s += RELEASE;
   s +=" - DL1HBD";
   labelVersion->setText(s);
   labelVersion1->setText(s);
   labelVersion2->setText(s);
   labelVersion3->setText(s);
   s = getenv("HOME");                       
   s += "/.qtlog/qlogfile";
   QFile lfile(s);
   lfile.open(QIODevice::Append | QIODevice::Text);
   QTextStream out(&lfile);
   QDateTime heute = QDateTime::currentDateTime();
   out << "\nSetup Info - "+heute.toString (Qt::TextDate);
   out << "\n";
   s = getenv("HOME");
   s += "/log/initTable/";
   PathInitTable->setText(s);              
   timer = new QTimer(this);               
   
//--
   dbFlg = 0;
   reinstall = 0;
   
   QFile bfile("/usr/local/bin/hilfedb");  
   if(bfile.exists() == FALSE) {
     QMessageBox::information( this,tr("INFO"),tr("\nKopiere erst alle bin Dateien mit:\n\nsudo ./install_bin\nNach /usr/local/bin/"),
     QMessageBox::Ok);
     exit(0);
   }
  
   //db = QSqlDatabase::addDatabase(settings.value("qsqlDatabase").toString());
   db = QSqlDatabase::addDatabase("QMYSQL");
   b = getenv("HOME");
   s = b+"/.config/QtLog/qtlog.old";   
   QFile file;
   if(file.exists(s) == TRUE) {
     lfile.close();                                        
     checkUpGrade();
   }
   else { 
    b += "/.config/QtLog/qtlog.ini";                       
    QFile inifile(b);
   
    if(settings.value("dbconf").toString().toInt() != 0) { 
      lfile.close();                                       
      checkUpGrade();
    }
    else {                                                 
      lfile.close();
      stackedWidget->setCurrentIndex(1);                   
      checkBoxReInstallCb();                               
    }
  }
}

setup::~setup()
{ }

//-----------------------------------------------
void setup::checkBoxParamCb()
{
     stackedWidget->setCurrentIndex(3);
     pRootPasswd->setText(settings.value("rootPasswd").toString());
     pQsqlDb->setText(settings.value("qsqlDatabase").toString());
     pDbUser->setText(settings.value("dbuser").toString());
     pDbName->setText(settings.value("dbname").toString());
     pUserPasswd->setText(settings.value("dbpasswd").toString());
     pHost->setText(settings.value("host").toString());
     pPort->setText(settings.value("port").toString());
     pPathtoMySql->setText(settings.value("MySQLPath").toString());
}

//-----------------------------------------------
void setup::checkBoxUpdateCb()
{
     updatework();
     checkBoxUpdate->setChecked(TRUE);
}


void setup::checkBoxReInstallCb()
{
    stackedWidget->setCurrentIndex(1);
    RootPasswd->setText(settings.value("rootPasswd").toString());
    QsqlDb->setText(settings.value("qsqlDatabase").toString());
    DbUser->setText(settings.value("dbuser").toString());
    UserPasswd->setText(settings.value("dbpasswd").toString());
    Host->setText(settings.value("host").toString());
    Port->setText(settings.value("port").toString());
    PathtoMySql->setText(settings.value("MySQLPath").toString()); 
    if(settings.value("dbconf").toString().toInt())  
      reinstall = 1;   
                       
}

//------------------------------------------
void setup::closeParamUpdate()
{
    stackedWidget->setCurrentIndex(2);
    checkBoxParam->setChecked(FALSE);  
}

//------------------------------------------
void setup::closeUpdate()
{
    stackedWidget->setCurrentIndex(2);
    checkBoxUpdate->setChecked(FALSE);  
}


void setup::closeReInstall()
{
    stackedWidget->setCurrentIndex(2);
    checkBoxReInstall->setChecked(FALSE);
}


//------------------------------------------------
void setup::updateParam()
{
    s = getenv("HOME");                              
    s += "/.qtlog/qlogfile";
    QFile lfile(s);
    lfile.open(QIODevice::Append | QIODevice::Text); 
    QTextStream out(&lfile);
    int status = QMessageBox::question(this,
         tr("INFO"),tr("\nNeuen Pfad zu MySQL setzen ?\n"),
         QMessageBox::Yes | QMessageBox::Default,
         QMessageBox::No | QMessageBox::Escape,
         QMessageBox::NoButton);
    if(status != QMessageBox::Yes)
       return;
    settings.setValue("dbname",pDbName->text());          
    settings.setValue("host",pHost->text());              
    settings.setValue("port",pPort->text());              
    settings.setValue("mySQLPath",pPathtoMySql->text());  
    out << "new_dbpath:"+pDbName->text()+","+pHost->text()+"," + pPort->text()+","+pPathtoMySql->text()+"\n";
    
    
    QMessageBox::information( this,tr("INFO"),tr("\nNeue Verbindung gesetzt."),
      QMessageBox::Ok);
   lfile.close();
}


void setup::goExit() 
{
    db.close();
    exit(0);
}

//------------------------------------------------------
int setup::openDatabase()
{
    db.setHostName(settings.value("host").toString());           
    db.setDatabaseName(settings.value("dbname").toString());
    db.setUserName(settings.value("dbuser").toString());
    db.setPassword(settings.value("dbpasswd").toString());
    if( ! db.open() ) {
       return 0;
    }
   return 1;
}




//***********************************************************************************
void setup::checkUpGrade()
{
   QString b, f, p, au;
   int i;
      s = getenv("HOME"); 
      s += "/.qtlog/qlogfile";
      QFile lfile(s);
      QTextStream out(&lfile);
      lfile.open(QIODevice::Append | QIODevice::Text); 
      b =  QDir::homePath();                  
      f = b+"/.config/QtLog/qtlog.ini";
      QFile iniFile(f);
      iniFile.open(QIODevice::ReadOnly);
      QTextStream istream( &iniFile);         
      s = istream.readLine(0);                
      s = istream.readLine(0);                
      au = s.mid(6);                          
      if(au.compare(AUDIT) != 0) {            
        iniFile.close();                      
        f = b+"/.config/QtLog/qtlog.old";     
        QFile iniFile(f);
        iniFile.open(QIODevice::ReadOnly);
        QTextStream istream( &iniFile);        
        s = istream.readLine(0);               
        while( istream.atEnd() != true ) {     
          s = istream.readLine(0);
          if(s.count() == 0) break;            
          i = s.indexOf("=");
          b = s.left(i);
          p = s.mid(i +1);
          settings.setValue(b,p);
        }
        iniFile.close();
	iniFile.remove();
        settings.setValue("audit",AUDIT);      
       }
       //--------------------
       if( !openDatabase() ) {                 
         err = db.lastError ();                
         s = err.databaseText();     
         s += tr("\nDatenbank kann nicht geoeffnet werden.\n");
         s += tr("Pruefe user_Name = ");
         t = settings.value("dbuser").toString();
         out << s;
         QMessageBox::information( this,
             tr("Datenbank INFO"),
             tr(s.toAscii()),
             QMessageBox::Ok | QMessageBox::Default,
             QMessageBox::NoButton, QMessageBox::NoButton);
         lfile.close();
         return;
       }
       dbFlg = 1;
       QSqlQuery query;                       
       qy = "SELECT relnr FROM dblogs";
       query.exec(qy);
       query.next();
       release = query.value(0).toString();   
       out << "Release : "+release;
       
       if((release.compare("5.6") == 0) || (release.compare("5.5") == 0) || (release.compare("5.4") == 0)) { 
	  s = tr("\nDie laufende Datenbank   -  Rel: ")+release+" \n";
          s += tr("Entspricht nicht der neuen Programm Version  QtLog ")+VERSION+".\n";
          s += tr("Ein Datenbank-Upgrade ist erforderlich.\n\n");
          s += "----------------------------------------------------------------------\n";
          s += tr("Upgrade ?");
	    
          textEdit->insertPlainText(QString::fromUtf8(s.toAscii()));
          ButtonInstallold->hide();
          stackedWidget->setCurrentIndex(0); 
       }
       else
       if(release.compare(RELEASE) == 0) {    
         if(au.compare("5.07") == 0) {        
                                              
           connect(timer,SIGNAL(timeout()), this, SLOT(loadTable("backup-refnamen.sql")));
           timer->start(10);                  
          }
          
          stackedWidget->setCurrentIndex(2);  
       }
       else 
	if(release.compare("5.5") == 0) {     
            s = tr("\nDie laufende Datenbank   -  Rel: ")+release+" \n";
            s += tr("Entspricht nicht der neuen Programm Version  QtLog ")+VERSION+".\n";
            s += tr("Ein Datenbank-Upgrade ist erforderlich.\n\n");
            s += tr("Es werden folgende Erweiterungen durchgefuehrt:\n");
            s += "----------------------------------------------------------------------\n";
            s += tr("Erweiterte Hamlib-Prozess Tabelle einbinden\n\n");
            s += tr("Upgrade ?");
	    
            textEdit->insertPlainText(QString::fromUtf8(s.toAscii()));
            ButtonInstallold->hide();
            stackedWidget->setCurrentIndex(0); 
	 }
	else
	 if(release.compare("5.4") == 0) {     
            s = tr("\nDie laufende Datenbank   -  Rel: ")+release+" \n";
            s += tr("Entspricht nicht der neuen Programm Version  QtLog ")+VERSION+".\n";
            s += tr("Ein Datenbank-Upgrade ist erforderlich.\n\n");
            s += tr("Es werden folgende Erweiterungen durchgefuehrt:\n");
            s += "----------------------------------------------------------------------\n";
            s += tr("Erweiterte Hamlib-Prozess Tabelle einbinden\n\n");
            s += tr("Upgrade ?");
	    
            textEdit->insertPlainText(QString::fromUtf8(s.toAscii()));
            ButtonInstallold->hide();
            stackedWidget->setCurrentIndex(0); 
	 }
	else
	 if(release.compare("5.3") == 0) {     
            s = tr("\nDie laufende Datenbank   -  Rel: ")+release+" \n";
            s += tr("Entspricht nicht der neuen Programm Version  QtLog ")+VERSION+".\n";
            s += tr("Ein Datenbank-Upgrade ist erforderlich.\n\n");
            s += tr("Es werden folgende Erweiterungen durchgefuehrt:\n");
            s += "----------------------------------------------------------------------\n";
            s += tr("Hilfs-Tabellen für den Support: eQSL, Rigctl und Dx-Spot eingebunden\n\n");
            s += tr("Upgrade ?");
	    
            textEdit->insertPlainText(QString::fromUtf8(s.toAscii()));
            ButtonInstallold->hide();
            stackedWidget->setCurrentIndex(0); 
         }
       else
         if(release.compare("5.2") == 0) {     //
           s = tr("\nDie laufende Datenbank   -  Rel: ")+release+" \n";
           s += tr("Entspricht nicht der neuen Programm Version  QtLog ")+VERSION+".\n";
           s += tr("Ein Datenbank-Upgrade ist erforderlich.\n\n");
           s += tr("Es werden folgende Erweiterungen durchgefuehrt:\n");
           s += "----------------------------------------------------------------------\n";
           s += tr(" - In der qso_Tabelle wird die Spalte 'qslmsg' eingefügt.\n");
           s += tr(" - Es wird eine Tabelle zur Aufnahme der 'eQSL-Karten' angelegt.\n\n");
           s += tr("Upgrade ?");
           out << s+"\n";
           textEdit->insertPlainText(QString::fromUtf8(s.toAscii()));
           ButtonInstallold->hide();
           stackedWidget->setCurrentIndex(0); 
         }
       else
         if(release.compare("5.1") == 0) {    
           s = tr("\n\nDie laufende Datenbank  - Rel: ")+release+" -\n";
           s += tr("entspricht nicht mehr der QtLog ")+VERSION+".\n";
           s += tr("Ein Datenbank-Upgrade ist erforderlich.\n\n");
           s += "Es werden folgende Aenderungen durchgefuehrt:\n";
           s += "------------------------------------------------------------------\n";
           s += tr("  - Die Datenbank wird erweitert.\n");
           s += tr("  - Die neuen Tabellen unterstuetzen jetzt ADIF V2.0  \n");
           s += tr("  - und bieten LoTW support.\n\n");
           s += tr("  - Der QsoDatensatz wird modifiziert.\n");
           out << s+"\n";
           textEdit->insertPlainText(QString::fromUtf8(s.toAscii()));
           ButtonInstallold->hide();
           stackedWidget->setCurrentIndex(0); 
        }
        else {                                
          s = tr("\nDie vorhandene Datenbank laeuft nicht mit der QtLog Version 1.5.07\n");
          s += tr("Der Datenbank-Releasestand ist zu alt.\n");
          s += tr("Eine NEU-Installation ist zu empfehlen.\n\n");
          s += tr("Hierbei wird die Datenbank komplett gelöscht.\n");
          s += tr("Vorsorglich ist daher mit DUMP die Logtabelle zu sichern.\n");
          s += tr("Ein RESTORE der alten Tabelle in die neue Datenbank V1.5.07 -Rel 5.2 ist z.Z. leider nicht verfügbar.");
          s += tr(" Sende mir eine Mail, evtl. kann ich weiter helfen\n\n");
          s += tr("Eine weitere Möglichkeit ist einen ADIF-Export der Logtabelle durchzuführen\n");
          s += tr("und die Tabelle in die neue QtLog Version zu importieren.\n");
          s += tr("Leider gehen hierbei alle Award-Kenner verloren.");
          textEdit->append(QString::fromUtf8(s.toAscii()));
          out << s+"\n";
          lfile.close();
          ButtonUpdate->hide();
          stackedWidget->setCurrentIndex(0);   
       }
       lfile.close();
}



void setup::installOldCb()
{
   if(checkBoxInstallNeu->isChecked() == FALSE) {
     QMessageBox::information( this,"Info","\nBitte den Haken fuer die Installation setzen",
     QMessageBox::Ok);
     return;
   }
   checkBoxReInstallCb();
}



//-----------------------------------------------------------------
void setup::installQtlog()
{
   s = getenv("HOME");                        
   s += "/.qtlog/qlogfile";
   QFile lfile(s);
   lfile.open(QIODevice::Append | QIODevice::Text);
   QTextStream out(&lfile);
   QDateTime heute = QDateTime::currentDateTime();
   if(RootPasswd->text().count() == 0) {     
        out << "err 1 - Setup Info - "+heute.toString (Qt::TextDate);
        b = tr("\nDas MySQL root_Passwort fehlt !            \n");
        out << b;
        s = tr("SetUp Info ");
        QMessageBox::information( this,s,b,
        QMessageBox::Ok);
        lfile.close();
        return;
    }
    if(DbUser->text().count() == 0) {        
        out << tr("err 2 - Setup Info - ")+heute.toString (Qt::TextDate);
        b = tr("\nDer QtLog_user Name fehlt !            \n");
        out << b;
        s = tr("SetUp Info ");
        QMessageBox::information( this,s,b,
        QMessageBox::Ok);
       lfile.close();
       return;
    }
    if(UserPasswd->text().count() == 0) {        
        out << tr("err 3 - Setup Info - ")+heute.toString (Qt::TextDate);
        b = tr("\nDas QtLog_user Passwort fehlt !            \n");
        out << b;
        s = tr("SetUp Info ");
        QMessageBox::information( this,s,b,
        QMessageBox::Ok);
        lfile.close();
        return;
     }
     
     db.setHostName(Host->text());
     db.setUserName("root");
     db.setPassword(RootPasswd->text());
     if( ! db.open() ) {
        out << tr("\nerr 4 - Server Info - ")+heute.toString(Qt::TextDate);
        err = db.lastError ();
        s = "\n\n"+err.databaseText();
        s += tr("\n\nServer laesst sich nich oeffnen ?!\n\nMoegliche Fehler :\n- Kein Zugang zu /var/lib/mysql/mysql.sock\n- Die Eigentuemerrechte sind nicht richtig gesetzt,\n- Der Server nicht gestartet!\n- Das Passwort ist falsch.\n\nSiehe obigen ErrorText vom Server!\n");
        out << s.toAscii();
        QMessageBox::information( this,
           tr("root - Datenbank INFO"),
           tr(s.toAscii()),
           QMessageBox::Ok | QMessageBox::Default,
           QMessageBox::NoButton, QMessageBox::NoButton);
         lfile.close();
         goExit();
     }
     lfile.close();
     createlogdb();
}



void setup::createlogdb()
{
      s = getenv("HOME");                             
      s += "/.qtlog/qlogfile";
      QFile lfile(s);
      lfile.open(QIODevice::Append | QIODevice::Text);
      QTextStream out(&lfile);
      QDateTime heute = QDateTime::currentDateTime();
      QSqlQuery query; 
      if(reinstall) {
        qy = "DROP DATABASE IF EXISTS "+settings.value("dbname").toString();
        out << qy+"\n";
        query.exec(qy);
        qy = "DROP USER "+settings.value("dbuser").toString()+" @"+settings.value("host").toString();
        out << qy+"\n";
        query.exec(qy);                    
      }
      qy = "CREATE DATABASE logdb";
      out << qy+"\n";
      query.exec(qy);
      err = query.lastError();
      if(err.text().count() > 1) {           
         out << tr("err 5 - Server INFO - ")+heute.toString (Qt::TextDate);
         s = err.text();
         s += tr("\n\nEs ist etwas schiefgelaufen\n");
         s += tr("\nDie Datenbank existiert bereits\nDer User scheint noch nicht angelegt zu sein.\nSiehe Server-Text.\nPruefe jetzt ob der User auch schon angelegt ist");
         out << s.toAscii();
         QMessageBox::information( this,
           tr("Sercer  INFO"),
           tr(s.toAscii()),
         QMessageBox::Ok | QMessageBox::Default,
         QMessageBox::NoButton, QMessageBox::NoButton);
      }
      
      qy = "GRANT ALL ON *.* TO "+DbUser->text()+"@"+Host->text(); 
      qy += " IDENTIFIED BY '"+UserPasswd->text()+"'";
      out << qy+"\n";
      query.exec(qy);
      if(err.text().count() > 1) {                        
         out << tr("err 6 - Server INFO - ")+heute.toString (Qt::TextDate);
         s = err.text();
         s += tr("\n\nEs ist etwas schiefgelaufen.\n");
         s += tr("\nDer User existiert bereits.\nVersuche die Tabellen zu initialisieren.\n");
         out << s.toAscii();
         QMessageBox::information( this,
          tr("Server INFO err 6"),
          tr(s.toAscii()),
         QMessageBox::Ok | QMessageBox::Default,
         QMessageBox::NoButton, QMessageBox::NoButton);
      }
      qy = "FLUSH PRIVILEGES";
      query.exec(qy);
      dbRestore();                               
    
      settings.setValue("rootPasswd",RootPasswd->text());
      settings.setValue("host",Host->text());               
      settings.setValue("port",Port->text());               
      settings.setValue("mySQLPath",PathtoMySql->text());   
      settings.setValue("dbname","logdb");                  
      settings.setValue("dbpasswd",UserPasswd->text());     
      settings.setValue("dbuser",DbUser->text());           
      settings.setValue("dbconf",1);                        
      settings.setValue("audit",AUDIT);
      out << tr("\nErfolgreich INSTALLIERT ")+heute.toString(Qt::TextDate)+"\n";
      lfile.close();
      db.close();
      
      QMessageBox::information( this,
        tr("Installation erfolgreich beendet"),
        tr("\nQtLog wird jetzt gestartet. Gehe zum Menue:\n- Configurieren->Stationsstandort - \nDie eingetragen Namen sind Beispiele. Bitte alles ueberschreiben.\n\nViel Spass mit QtLog."),
        QMessageBox::Ok | QMessageBox::Default,
        QMessageBox::NoButton, QMessageBox::NoButton);
      StartProcess("qtlog &");                      
      goExit();                                     
}

//--------------------
void setup::dbRestore()
{
  QString p;
  QFile file;
     QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
     QString pathToMysql="";
#ifdef Q_WS_WIN
    pathToMysql=settings.value("mySQLPath","").toString();
#endif
     p = PathInitTable->text()+"backup-db.sql";
     if(!file.exists(p)) {
         QApplication::restoreOverrideCursor();
         QMessageBox::information( this,
         tr("INFO"),
         tr("\nRESTORE nicht moeglich - Datei nicht vorhanden !"),
         QMessageBox::Ok);
       return;
     }
     
     s = pathToMysql+"mysql -u"+settings.value("dbuser").toString()+" -p"+settings.value("dbpasswd").toString()+" logdb < ";
     s = "mysql -u"+DbUser->text() +" -p"+UserPasswd->text()+" logdb < ";
     s += p;
     StartProcess(s.toAscii());
     QApplication::restoreOverrideCursor();
}

void setup::hilfeAc()
{
     settings.setValue("Val","INSTALLATION");
     QFileInfo info1("/usr/local/bin/hilfedb");
     if(info1.size() == 0) {                    
        qDebug() << info1.size();
     }
     else 
       StartProcess("hilfedb &");
}
