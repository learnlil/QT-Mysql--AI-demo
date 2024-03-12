#ifndef MYSQLOPERATION_H
#define MYSQLOPERATION_H
#include <QJsonObject>
class mysqlOperation
{
public:
    mysqlOperation();
    void connMysql();
    void insertMysqlTable(QJsonObject obj,QString str);

};

#endif // MYSQLOPERATION_H
