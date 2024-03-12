#ifndef EXCELOPERATION_H
#define EXCELOPERATION_H
#include <QAxObject>
#include <QTableWidget>
class excelOperation
{
public:
    excelOperation(QString wordName,QTableWidget *tableWidget);
    excelOperation(QString wordName,QTableWidget *tableWidget,QList<QPair<QPair<int,int>,QPair<int,int>>> mergedCells);
    void createExcelDocument();
private:
    QString excelName;
    QTableWidget *tableWidget;
    QList<QPair<QPair<int,int>,QPair<int,int>>> mergedCells;
};

#endif // EXCELOPERATION_H
