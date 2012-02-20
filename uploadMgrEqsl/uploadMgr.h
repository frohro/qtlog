/* ************************************************************
  20.02.2009 dl1hbd
************************************************************* */
#ifndef DOWNLOADMGR_H
#define DOWNLOADMGR_H
#include <QtSql>
#include <QFile>
#include <QObject>
#include <QQueue>
#include <QTime>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QSettings>
#include <qiodevice.h>
#include <QStringList>
class QUdpSocket;
class QIODevice;
class UploadMgr: public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;
    QList<QNetworkReply *> currentUploads;
public:
    UploadMgr();
    ~UploadMgr();
    QSettings settings;
    QSqlDatabase db;             
    QTimer * timer;
    int uploadAc;
    int id, val, rport;
    QHash <int, int> uploadHash;
    QHash <QString, QString> toAdifBand;   
    QString qy, logFile, s, u, h, eqslUser, eqslPasswd, datum;
    int i, n, ErrMsg, rPort, bPort;
    QByteArray datagram;
    QString Call;
    void openDatabase();
    QString fileField;
    QString fileName;
    void addFile(QString, QString, QString);
    void addFile(QString, QByteArray, QString, QString);
    void doUpload(QUrl);
    QString saveFileName(const QUrl &url);
    bool saveToDisk(const QString &filename, QIODevice *data);
    void mkuploadEqslData();
    void setBandTable();
    QString getAdifBand(QString);
public slots:
    void execute();
    void uploadFinished(QNetworkReply *reply);
    void timerCheckforUpload();
private slots:
    void processWriteDatagram(int, QString, int);
    void processPendingDatagram();
private:
    QByteArray data;
    QByteArray strToEnc(QString s);
    QUdpSocket *udpSocket;
};
#endif
