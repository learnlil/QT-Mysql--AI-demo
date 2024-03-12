#include "exceloperation.h"
#include <QDebug>
#include <QDateTime>
#include <QStandardPaths>
#include <QFileDialog>
#include <QTableWidgetSelectionRange>
excelOperation::excelOperation(QString excelName, QTableWidget *tableWidget)
    :excelName(excelName),tableWidget(tableWidget)
{

}

excelOperation::excelOperation(QString excelName, QTableWidget *tableWidget, QList<QPair<QPair<int, int>, QPair<int, int> > > mergedCells)
    :excelName(excelName),tableWidget(tableWidget),mergedCells(mergedCells)
{

}

void excelOperation::createExcelDocument()
{
    QAxObject* excel = new QAxObject();
    bool flag = excel->setControl("Excel.Application");
    if(!flag)
        return;
    excel->setProperty("Visible",true);
    excel->setProperty("DisplayAlerts",false);
    QAxObject *workbooks = excel->querySubObject("Workbooks");
    if(!workbooks)
        return;
    workbooks->dynamicCall("Add()");
    QAxObject* workbook = excel->querySubObject("ActiveWorkbook");
    QAxObject* sheets = workbook->querySubObject("Sheets");
    QAxObject* sheet = sheets->querySubObject("Item(int)",1);

    int rowCount = tableWidget->rowCount();
    int colCount = tableWidget->columnCount();
    for(int i=0;i<rowCount;i++)
    {
        for(int j=0;j<colCount;j++)
        {
            QString text = "";
            if(tableWidget->item(i,j)==nullptr)
                text="";
            else
                text = tableWidget->item(i,j)->text();
            QAxObject* cell = sheet->querySubObject("Cells(int,int)",i+1,j+1);
            cell->dynamicCall("SetValue(const QString&)",text);
            cell->querySubObject("Font")->setProperty("Size",10);
            cell->querySubObject("Columns")->dynamicCall("AutoFit()");
        }
    }

    //标记一处地点,有bug,不执行合并单元格
    for(const auto &mergedCell : qAsConst(mergedCells))
    {
        int startRow = mergedCell.first.first+1;
        int startCol = mergedCell.first.second+1;
        int endRow = startRow + mergedCell.second.first-1;
        int endCol = startCol + mergedCell.second.second-1;
        QString startColLetter = QString(QChar('A'+startCol-1));
        QString endColLetter = QString(QChar('A'+endCol-1));
        QString range = QString("%1%2:%3%4").arg(startColLetter).arg(startRow).arg(endColLetter).arg(endRow);
        QAxObject *rangeObject = sheet->querySubObject("Range(const QString&)",range);
        rangeObject->dynamicCall("MergeCells",true);
    }



    QDateTime currentDataTime = QDateTime::currentDateTime();
    QString currentDataTimeStr = currentDataTime.toString("yyyy-MM-dd hh-mm-ss");
    QString excelFinalName = excelName+" "+currentDataTimeStr+".xlsx";
    QString filePath;
    filePath = QFileDialog::getSaveFileName(nullptr,"保存文件","C:/Users/lenovo/Desktop/"+excelFinalName,"Excel files (*.xlsx)");
    filePath.replace("/","\\");//转换路径为绝对路径
    workbook->dynamicCall("SaveAs(const QString&)",filePath);

    workbook->dynamicCall("Close()");
    excel->dynamicCall("Quit()");
}


