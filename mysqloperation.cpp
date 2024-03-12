#include "mysqloperation.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QString>
#include <QJsonDocument>

mysqlOperation::mysqlOperation() {}

void mysqlOperation::connMysql()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("127.0.0.1");        //TCP/IP Server
    db.setDatabaseName("qt2mysql32");   //ODBC中 Data Source Name
    db.setUserName("root");             //ODBC User
    db.setPassword("123456");           //ODBC Password
    if(!db.open())
        qDebug()<<"数据库打开失败:"<<db.lastError().text();
    else
        qDebug()<<"数据库打开成功";
}

void mysqlOperation::insertMysqlTable(QJsonObject obj, QString str)
{
    QSqlQuery query;
    QString insertSql = "insert into json2mysql (record_id,json_data,text_data) values(?,?,?)";
    if(!query.prepare(insertSql))
    {
        qDebug()<<"预处理失败:"<<query.lastError().text();
    }
    qDebug()<<obj["log_id"].toString();
    QString record_id = obj["log_id"].toString();
    qDebug()<<record_id;
    QJsonDocument jsonDoc(obj);
    QString json2string = jsonDoc.toJson(QJsonDocument::Compact);

    query.addBindValue(record_id);
    query.addBindValue(json2string);
    query.addBindValue(str);

    if(!query.exec())
    {
        qDebug()<<"插入失败:"<<query.lastError().text();
    }
}
