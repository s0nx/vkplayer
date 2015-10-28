#include "audioget.h"
#include <QTextStream>
#include <QFile>
#include <QByteArray>

audioget::audioget(QString token, QString id, QObject *parent) : QObject(parent)
{
    apiUrl = "https://api.vk.com/method/audio.get?owner_id="+id+"&access_token="+token;
    QNetworkRequest request(apiUrl);
    reply = manager.get(request);

    connect(reply,SIGNAL(finished()),this,SLOT(getReplyFinished()));
    connect(reply,SIGNAL(finished()),this,SLOT(readyReadReply()));
    //connect(reply,SIGNAL(readyRead()),this,SIGNAL(gotReply()));
}

void audioget::getReplyFinished()
{
    reply->deleteLater();
}

void audioget::readyReadReply()
{
    replyJson = QString::fromUtf8(reply->readAll());
    //qDebug()<<replyJson;
    if(!replyJson.isEmpty())
        emit gotReply();
}


