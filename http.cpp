#include "http.h"
#include <QMap>
#include <QByteArray>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QEventLoop>
#include <QMapIterator>
http::http() {}

bool http::http_postRequest(QString Url, QMap<QString, QString> header, QByteArray &requestData, QByteArray &replyData)
{
    QNetworkAccessManager manager;
    QNetworkRequest request;
    request.setUrl(Url);
    QMapIterator<QString,QString> it(header);
    while(it.hasNext())
    {
        it.next();
        request.setRawHeader(it.key().toLatin1(),it.value().toLatin1());
    }
    QNetworkReply *Reply = manager.post(request,requestData);
    QEventLoop l;
    connect(Reply,&QNetworkReply::finished,&l,&QEventLoop::quit);
    l.exec();

    if(Reply!=nullptr&&Reply->error()==QNetworkReply::NoError)
    {
        replyData = Reply->readAll();
        return true;
    }
    return false;
}
