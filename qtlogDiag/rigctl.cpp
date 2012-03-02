
/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.

   V 1.5 : 16-03-2010
********************************************************************** */

#include <QtSql>
#include <qmessagebox.h>

#include "rigctl.h"
#include "../addClog/defs.h"


// constructor
// ------------------------------------------------------------------
rigctl::rigctl(QWidget * parent) : QDialog(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
    setupUi(this);

    int n = settings.value("FontSize").toString().toInt();
    QFont font;
    font.setPointSize(n);
    setFont(font);
    
    connect(ButtonESC, SIGNAL(pressed()), this, SLOT(goExit()));
    connect(ButtonHilfe, SIGNAL(pressed()), this, SLOT(goHilfe()));
    connect(ButtonRigList, SIGNAL(pressed()), this, SLOT(showRigList()));
    connect(ButtonInit, SIGNAL(pressed()), this, SLOT(hamlibServerInit()));
    connect(ButtonStart, SIGNAL(pressed()), this, SLOT(goStart()));
    connect(ButtonStop, SIGNAL(pressed()), this, SLOT(goStop()));
    connect(buttonReturn, SIGNAL(pressed()), this, SLOT(showRigPage()));
    connect(InterfaceBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(intefaceBoxChanged(QString)));
    connect(wrigList, SIGNAL(itemClicked(QTreeWidgetItem *,int)), this, SLOT(itemClickedCb(QTreeWidgetItem *,int)));
    
    tcpSocket = new QTcpSocket(this);              // tspSocket
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(sentCommand()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readRigReply()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                                            this, SLOT(displayError(QAbstractSocket::SocketError)));     
    QString home = getenv("HOME");                 // lade alle devices
    s = "ls /dev/ttyS* > "+home+"/.qtlog/rigstatus";
    i = system(s.toAscii());                       // suche device ttyS0 .. n
    home += "/.qtlog/rigstatus";                   // prüfe rigstatus
    QFile iniFile(home);
    iniFile.open(QIODevice::ReadOnly);
    QTextStream istream( &iniFile);                // lese device_file
       
    RigDevBox->clear();
    PttBitDevBox->clear();
    i = 0;
    while(istream.atEnd() != true) {               // RigBox_dev füllen
       s = istream.readLine(0);
       RigDevBox->insertItem(i,s);
       PttBitDevBox->insertItem(i++,s);
    }
    iniFile.close();
         
    home = getenv("HOME"); 
    s = "ls /dev/ttyU* > "+home+"/.qtlog/rigstatus";
    n = system(s.toAscii());                       // suche device ttyUSB0 .. n
    home += "/.qtlog/rigstatus";
    QFile inuFile(home);
    inuFile.open(QIODevice::ReadOnly);
    QTextStream iustream( &inuFile);               // oeffne device_file
    while(iustream.atEnd() != true) {
      s = iustream.readLine(0);
      RigDevBox->insertItem(i,s);                  // alle vorhndenen USB0 .. n devices übenehmen
      PttBitDevBox->insertItem(i++,s);
    }
    inuFile.close();
    
    Retry = 0;
    Runing = 0;
    r = 0;
    Runing = -1;
    i = getRigModell();                                      // lade alle Rig_Modelle
    
    if(i != 0) {                                             // falls hamlib installiert ist - lade Rig_Modelle
      if(settings.value("RigPid").toString().count() != 0) { // und Rig configuriert wurde
       QSqlQuery query;                                      // hole config_parameter_discriptor
       qy = "SELECT * FROM wproz WHERE rigpid='"+settings.value("RigPid").toString()+"'";
       query.exec(qy);
       while(query.next()) {
          proz = query.value(r++).toString();              // proz
          owner = query.value(r++).toString();             // owner
          editRig->setText(query.value(r++).toString());   // Rig
          editPid->setText(query.value(r++).toString());   // Pid
          editRetry->setText(query.value(r++).toString()); // retry
          Retry = editRetry->text().toInt();
          i = RigDevBox->findText(query.value(r++).toString());
          RigDevBox->setCurrentIndex(i);                   // Rig_device_Box
          i = BaudBox->findText(query.value(r++).toString());
          BaudBox->setCurrentIndex(i);                     // Baud_device_box
	  // -
          i = InterfaceBox->findText(query.value(r++).toString()); // FAM_Interface ONE
          InterfaceBox->setCurrentIndex(i);
          i = PttBitDevBox->findText(query.value(r++).toString());
          PttBitDevBox->setCurrentIndex(i);                // PTT_device_Box
          editRts->setText(query.value(r++).toString());   // RTS = OFF
          editDtr->setText(query.value(r++).toString());   // DTR = OFF
          editCts->setText(query.value(r++).toString());   // CTS = OFF
	  // -
          if(InterfaceBox->currentText().compare("NO") == 0) {
            PttBitDevBox->setEnabled(FALSE);
            editRts->setEnabled(FALSE);
            editDtr->setEnabled(FALSE);
            editCts->setEnabled(FALSE);
          }
          editHost->setText(query.value(r++).toString());  // localhost
          editPort->setText(query.value(r++).toString());  // port
          Runing = query.value(r).toInt();                 // runing_bit
        }
      }
     
      updateFlg = 0;                                      // alles ok
      if( Runing == -1 )
         ButtonInit->setPalette( QPalette(QColor(180, 210, 200)));  // grün
      else
       if( Runing )
          ButtonStop->setPalette( QPalette(QColor(180, 210, 200))); // STOP grün
       else
         ButtonStart->setPalette( QPalette(QColor(180, 210, 200))); // grün
    }
    else 
       QTimer::singleShot(10, this, SLOT(hlibNoInstalled()));       // Hamlib nicht installiert
}

// ---------------------------------
rigctl::~rigctl()
{
}

// Aufruf von buttonESC
// ---------------------------------
void rigctl::goExit()
{
   accept();
}

// ---------------------------------
void rigctl::goHilfe()
{
   settings.setValue("Val","Configuriere");
   QString s = "hilfedb &";
   i = system(s.toAscii());  
}

// ----------------------------------
void rigctl::showRigList()
{
   stackedWidget->setCurrentIndex(1);
}

// Aufruf von buttonReturn
// ----------------------------------
void rigctl::showRigPage()
{
  stackedWidget->setCurrentIndex(0);
}

// -------------------------------------------------------
void rigctl::itemClickedCb(QTreeWidgetItem * item, int i)
{
   int p;
   p = i;
   QString f;
   editPid->setText(item->text(0));
   f = item->text(1)+" "+item->text(2)+" ("+item->text(4)+")";;
   editRig->setText(f);
   stackedWidget->setCurrentIndex(0);
                                                                       // init_Button anzeigen
   ButtonRigList->setPalette( QPalette(QColor(224, 224, 224)));        // grau
   ButtonInit->setPalette( QPalette(QColor(180, 210, 200)));           // grün
}

// Interface_Box verändert
// -------------------------------------
void rigctl::intefaceBoxChanged(QString str)
{
   if(str.compare("NO") != 0) {
     PttBitDevBox->setEnabled(TRUE);
     editRts->setEnabled(TRUE);
     editDtr->setEnabled(TRUE);
     editCts->setEnabled(TRUE);
   }
   else {
     PttBitDevBox->setEnabled(FALSE);
     editRts->setEnabled(FALSE);
     editDtr->setEnabled(FALSE);
     editCts->setEnabled(FALSE);
  }
}


// Aufruf von Button_Start
// -----------------------------------------------------
void rigctl::goStart()
{
    qy = "UPDATE wproz SET ";                                // udpate new server values
    qy += "rig='"+editRig->text();                           // Rig_text
    qy += "',rigpid='"+editPid->text();                      // Pid
    qy += "',retry='"+editRetry->text();                     // retry_value
    qy += "',rigdev='"+RigDevBox->currentText();             // /dev/ttyUSB0  CV-I_port     
    qy += "',baud='"+BaudBox->currentText();                 // baud
    qy += "',iface='"+InterfaceBox->currentText();           // Interface
    qy += "',pttdev='"+PttBitDevBox->currentText();          // dev/ttyUSB0 ptt_port
    qy += "',rts='"+editRts->text();                         // RTS = OFF
    qy += "',dtr='"+editDtr->text();                         // DTR = OFF
    qy += "',cts='"+editCts->text();                         // CTS = OFF
    qy += "',host='"+editHost->text();
    qy += "',port='"+editPort->text();
    qy += "',runing=0";
    qy += " WHERE rigpid='HamLib'";
    QSqlQuery query(qy);
    
    tcphost = editHost->text();                        // starte server
    tcpport = editPort->text();
  
    if(!deleteServerProz())                            // lösche Proz, wenn vorhanden
      startServer();                                   // starte neuen server_Proz
 
    QTimer::singleShot(500, this, SLOT(requestRig())); // starte 1 x Frequenz Abfrage
}


// Aufruf vom Button_Stop
// -----------------------------------------------------
void rigctl::goStop()
{
   qy = "UPDATE wproz SET runing=0 WHERE proz='HamLib'";
   QSqlQuery query(qy); 
  
   updateFlg = 2;                                     // timer->stop, polling = 0 setzen
   Retry = 0;

   goExit();
}

// ----------------------------------------------------
// zeige Erfolg oder Fehler
// ----------------------------------------------------
bool rigctl::showSuccess(QString freq)
{
    int status = QMessageBox::question( this,              // frage alles ok
                 tr("HamLib"),
                 tr("\nRx - %1 Herz\n\n"
                    "Ist die Frequenz Anzeige richtig ?\n\n"
                    "Bei 'Yes', wird beim Logging und Contestlogging\nimmer Rigcontrol gestartet.")
                 .arg(freq),
                 QMessageBox::Yes|QMessageBox::Default,
                 QMessageBox::No|QMessageBox::Escape,
                 QMessageBox::NoButton);
        if(status != QMessageBox::Yes) {
          qy = "DELETE FROM wproz WHERE proz='HamLib'";    // lösche wrigctl
          QSqlQuery query(qy); 
          return false;
        }
 
        Retry = editRetry->text().toInt();                 // setzt timer interval
        Runing = 1;                                        // ready for runing
        settings.setValue("RigPid",editPid->text());
        qy = "UPDATE wproz set runing=1 WHERE proz='HamLib'"; 
        QSqlQuery query(qy); 
        updateFlg = 1;
        goExit();
        return true;                                       // daemon wird jetzt gepollt
}


// get - hamlib_Modells - hole alle Hamlib_modelle
// ---------------------------------------------------------
int rigctl::getRigModell()
{
  QString s, home, t;
  int i,x;
     
     home = getenv("HOME");               // suche nach rigctl
     s = "rigctl -l > ";
     s += home;
     s +="/.qtlog/rigstatus";
     i = 0;
     i = system(s.toAscii());             // rufe hamlib auf
 
     home += "/.qtlog/rigstatus";         // versuche die Rig-Modelle zu laden
     QFile inFile(home);
     inFile.open(QIODevice::ReadOnly);
     if(!inFile.size())
       return 0;                          // FEHLER hamlib nicht gefunden ( nicht installiert )
      
     QTextStream istream(&inFile);        // oeffne file, lade alle Rig-Modelle
     ButtonRigList->setPalette(QPalette(QColor(180, 210, 200)));   // grün
     i = 0; 
     wrigList->clear(); 
     s = istream.readLine(0);             // 1.Zeile überspringen - ist header
     while(istream.atEnd() != true  ) {   // alle folgenden Zeilen bearbeiten
        i = 0;
        x = 0;
        s = istream.readLine(0);          // 2.Zeile
        x = s.indexOf("\t");

        QTreeWidgetItem *item = new QTreeWidgetItem(wrigList);
        t = s.left(x);
        t = t.simplified();
        item->setText(i++,t);             // pid
        s.remove(0,x);
        x = 16;
        t = s.left(x);
        t = t.simplified();
        item->setText(i++,t);             // mfg
        s.remove(0,x);
        x = 24;
        t = s.left(x);
        t = t.simplified();
        item->setText(i++,t);             // model
        s.remove(0,x);
        x = 8;
        t = s.left(x);
        t = t.simplified();
        item->setText(i++,t);             // version
        s.remove(0,x);
        s = s.simplified();
        item->setText(i++,s);             // status
     }
     inFile.close();
     return 1;
}


// Aufruf von - ButtonInit
// Proz_alt löschen falls vorhanden
// rigctld mit neuen Parametern starten
// --------------------------------------------------------
void rigctl::hamlibServerInit()
{
    QSqlQuery query;                                    // clear Tabelle
    qy = "DELETE FROM wproz WHERE proz='HamLib'";
    query.exec(qy);                                     // alte Config löschen
    
    qy = "INSERT INTO wproz VALUES ('HamLib";           // neue Config speichern
    qy += "','hamlib";
    qy += "','"+editRig->text();                        // Rig_name
    qy += "','"+editPid->text();                        // Pid
    qy += "','"+editRetry->text();                      // retry_value
    qy += "','"+RigDevBox->currentText();               // /dev/ttyUSB0  CV-I_port     
    qy += "','"+BaudBox->currentText();                 // baud
    qy += "','"+InterfaceBox->currentText();            // Interface
    qy += "','"+PttBitDevBox->currentText();            // dev/ttyUSB0 ptt_port
    qy += "','"+editRts->text();                        // RTS=OFF
    qy += "','"+editDtr->text();                        // Dtr=OFF
    qy += "','"+editCts->text();                        // CTS=OFF
    
    qy += "','"+editHost->text();                       // localhost
    qy += "','"+editPort->text();                       // port
    qy += "',0,0)";                                     // runing + id
    query.exec(qy);
    
    tcphost = editHost->text();
    tcpport = editPort->text();
  
    deleteServerProz();                                 // erst alten Proz löschen
    startServer();                                      // jetzt starte neuen rigctld .. ... &
    QTimer::singleShot(500, this, SLOT(requestRig()));  // warte ms, dann 1 x Frequenz_Abfrage durchführen
}

// ------------------------------------------------
int rigctl::deleteServerProz()
{
     QString home, p;
      
       home = getenv("HOME"); 
       p = "ps -e | grep rigctld > "+home+"/.qtlog/rigstatus";
       i = system(p.toAscii());           // suche rigctld prozess

       home += "/.qtlog/rigstatus";       // öffne Ergebnis_file
       QFile iniFile(home);
       iniFile.open(QIODevice::ReadOnly);
       QTextStream istream( &iniFile);    // oeffne rigstatus
       p = istream.readLine(0);           // nur die 1.Zeile, falls vorhanden
       iniFile.close();
       
       if(p.indexOf("rigctld") != -1) {   // check - wenn rigctld_proz vorhanden, kill
         p.remove(0,1);
         // "7437 pts/1  S  0:00 rigctld -m 357 -r /dev/ttyUSB0 -s 19200 -p /dev/ttyUSB0 -P RTS" ...
         i = p.indexOf(" ");
         s = p.left(i);                  // 7437 
         p = "kill "+s;
         i = system(p.toAscii());        // kill proz_Id
      }
      return 0;                          // rigctld_proz entfernt - falls er vorhanden war
}

// ------------------------------------------------
void rigctl::startServer()
{
       QString p;
       int n;
       QSqlQuery query;                  // initialisiere hamlib_server
       qy = "SELECT rigpid,rigdev,baud,pttdev,rts,dtr FROM wproz WHERE proz='HamLib'";
       query.exec(qy);
       query.next();
       n = 0;
       p = "rigctld -m "+query.value(n++).toString();     // RigPid
       p += " -r "+query.value(n++).toString();           // RigDev
       p += " -s "+query.value(n++).toString();           // Baud
       
       if(InterfaceBox->currentText().compare("Funkamateur ONE") == 0) { // wenn F..ONE
         p += " -p "+query.value(n++).toString();                        // PttDev
         p += " -C rts_state="+query.value(n++).toString();              // RTS=OFF
         p += " -C dtr_state="+query.value(n++).toString();              // DTR=OFF
       }
       p += " &";
       i = system(p.toAscii()); // rigctld -m 357 -r /dev/ttyUSB0 -s 19200 -p /dev/ttyUSB0 ......
}


// check Server ( nur ein poll )
// =======================================================
void rigctl::requestRig()
{
     tcpSocket->abort();             // starte eine Anfrage
     tcpSocket->connectToHost(tcphost,tcpport.toInt());
}

// -------------------------------------------------------
// tcpSocket SIGNAL connected()    - sende jetzt Command
// -------------------------------------------------------
void rigctl::sentCommand()
{
    QString message;
    message = "f";                  // frage Frequenz ab
    tcpSocket->write(message.toAscii());
    tcpSocket->flush();
}

// -------------------------------------------------------------------
// Antwort vom Server lesen         - SIGNAL readyRead() eingegangen
// -------------------------------------------------------------------
void rigctl::readRigReply()
{
    QString st;
    QString message(tcpSocket->readAll());      // hole message
    
    if(message.indexOf("RPRT") != -1) {
      QMessageBox::information(this,tr("Hamlib Client"),
                tr("\nError ocuured - timedout - \n\n"
                   "Check the transceiver connection.\n"
                   "if Hardware available\ncheck config\n\n"
                   "* Server stopped. *"));
      return;
    }
    
    message = message.simplified();
    showSuccess(message);
}

// Client FEHLER Meldungen
// ----------------------------------------------------------------
void rigctl::displayError(QAbstractSocket::SocketError socketError)
{ 
    switch(socketError) {
      case QAbstractSocket::RemoteHostClosedError:
        break;
      case QAbstractSocket::HostNotFoundError:
           QMessageBox::information(this,tr("Hamlog Client"),
                                    tr("\nThe host was not found.\n"
                                    "Please check the host name and port setting."));
        break;
      case QAbstractSocket::ConnectionRefusedError:
           QMessageBox::information(this,tr("Hamlog Client"),
                                    tr("\nThe connection was refused by the peer.\n\n"
                                    "Make sure the hamlib server is running.\n"
                                    "and check that the host name and port settings\n"
                                    "are correct."));
        break;
      default: 
           QMessageBox::information(this,tr("Hamlog Client"),
                                    tr("\nThe folloing error ocuured: %1.")
                                    .arg(tcpSocket->errorString()));
    }
}


// --------------------------
void rigctl::hlibInfo()
{
    QMessageBox::information(this,tr("Hamlib"),
    tr("\nHamlib ist nicht configuriert"));
}

// --------------------------
void rigctl::hlibReadError()
{
    QMessageBox::information(this,tr("Hamlib"),
    tr("\nLeseFehler : HamLib Rigmodelle"));
}

// --------------------------
void rigctl::hlibNoInstalled()
{
    QMessageBox::information(this,tr("Hamlib"),
    tr("\nHamlib ist nicht installiert"));
}




