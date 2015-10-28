#ifndef AUDIOGET_H
#define AUDIOGET_H

#include <QObject>
#include <QString>
#include <QURL>
#include <QByteArray>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QtNetwork>
#include <QVariant>
#include <QTextStream>
#include <QByteArray>

class audioget : public QObject
{
    Q_OBJECT
public:
    explicit audioget(QString token,QString id,QObject *parent = 0);
    //QNetworkReply *getReply() {return reply;}
    QString getJstring() {return replyJson;}

private:
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QUrl apiUrl;
    QByteArray requestString;
    QString replyJson;

signals:
    void gotReply();

public slots:
    void getReplyFinished();
    void readyReadReply();
    //void replyReady();
};

#endif // AUDIOGET_H
