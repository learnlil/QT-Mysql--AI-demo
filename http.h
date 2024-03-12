#ifndef HTTP_H
#define HTTP_H
#include <QObject>
#include <QString>

class http : public QObject
{
    Q_OBJECT
public:
    http();
    static bool http_postRequest(QString Url,QMap<QString,QString>header,QByteArray &requestData,QByteArray &replyData);
};

#endif // HTTP_H
