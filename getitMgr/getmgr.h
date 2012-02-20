/* ************************************************************
  20.02.2009 dl1hbd
************************************************************* */
#ifndef GETMGR_H
#define GETMGR_H
#include <QFile>
#include <QObject>
#include <QSettings>
#include <QTimer>
#include <QHttp>
class QUdpSocket;
class getMgr: public QObject
{
    Q_OBJECT
public:
    getMgr();
    ~getMgr();
   
    QSettings settings;
    QTimer *timer;
    
    QByteArray datagram;
    QString website, urlstr, callsign, Call, str, lineData, result, com, s;
    int Com;             
    int rport;           
    QString ErrorStr;
 
    void parseit();
    void getit();
    void goExit();
    
public slots:
    void execute();
 
private slots:
    
    void httpDone(bool);
    
    void processPendingDatagram();
    void processWriteDatagram(int, QString, int);
    
private:
  
    QHttp *http;
    bool flag;
    
    QByteArray data;
    QUdpSocket *udpSocket;
};
#endif
