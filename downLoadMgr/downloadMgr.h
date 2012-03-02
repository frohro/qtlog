/* ************************************************************
  20.02.2009 dl1hbd
************************************************************* */
#ifndef DOWNLOADMGR_H
#define DOWNLOADMGR_H
#include <QFile>
#include <QObject>
#include <QQueue>
#include <QTime>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QSettings>
class QUdpSocket;
class DownloadMgr: public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;
    QList<QNetworkReply *> currentDownloads;
    
public:
  
    DownloadMgr();
    ~DownloadMgr();
    QSettings settings;
    QByteArray datagram;
    int mode;
    int replyflg;
    QString fpath, fname;
    QString pUrl;
    void doDownload(const QUrl &url);
    QString saveFileName(const QUrl &url);
    bool saveToDisk(const QString &filename, QIODevice *data);
    
public slots:
  
    void execute();
    void downloadFinished(QNetworkReply *reply);
    
private slots:
  
    void processWriteDatagram(int, QString);
    void processPendingDatagram();
    
private:
    QUdpSocket *udpSocket;
};
#endif
