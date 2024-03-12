#include "tablewindow.h"
#include "ui_tablewindow.h"
#include "exceloperation.h"
#include <QWidget>
#include <QHeaderView>
#include <QJsonValue>
#include <QJsonArray>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QInputDialog>
tableWindow::tableWindow(QWidget *parent,QJsonObject obj)
    : QWidget(parent)
    , ui(new Ui::tableWindow)
{
    ui->setupUi(this);
    initTable(obj);
}

tableWindow::~tableWindow()
{
    delete ui;
    delete tableWidget;
    delete layout;
}

void tableWindow::initTable(QJsonObject obj)
{
    this->setWindowTitle("表格显示");
    this->resize(1000,800);

    QPushButton *button = new QPushButton("导出为excel",this);
    button->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    connect(button,&QPushButton::clicked,this,&tableWindow::onButtonClicked);
    tableWidget = new QTableWidget();
    tableWidget->setParent(this);
    tableWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableWidget,&QTableWidget::customContextMenuRequested,this,&tableWindow::on_customContextMenuRequested);

    QJsonArray tablesJson = obj["tables_result"].toArray();
    QJsonObject tableJson = tablesJson[0].toObject();
    QJsonArray headerJson = tableJson["header"].toArray();
    QJsonArray bodyJson = tableJson["body"].toArray();

    QList<int> rowList;
    QList<int> colList;
    QList<int>::iterator max;

    for(const QJsonValue &value : qAsConst(bodyJson))
    {
        QJsonObject cell = value.toObject();
        int row_start = cell["row_start"].toInt();
        int col_start = cell["col_start"].toInt();
        rowList.append(row_start);
        colList.append(col_start);
    }
    max = std::max_element(rowList.begin(),rowList.end());
    int rowCount = *max+1;
    max = std::max_element(colList.begin(),colList.end());
    int colCount = *max+1;
    tableWidget->setRowCount(rowCount);
    tableWidget->setColumnCount(colCount);
    QVariantList variantList = headerJson.toVariantList();
    QStringList stringList;
    for(const QVariant &v : variantList)
        stringList.append(v.toString());
    tableWidget->setHorizontalHeaderLabels(stringList);
    //填充数据
    for(int i=0;i<bodyJson.size();i++)
    {
        QJsonObject cell = bodyJson[i].toObject();
        int row_start = cell["row_start"].toInt();
        int col_start = cell["col_start"].toInt();
        QString words = cell["words"].toString();
        tableWidget->setItem(row_start,col_start,new QTableWidgetItem(words));
    }
    layout = new QGridLayout();
    layout->addWidget(tableWidget,0,0);
    layout->addWidget(button,1,0);
    this->setLayout(layout);
}

void tableWindow::on_customContextMenuRequested()
{
    QMenu menu;
    menu.addAction(tr("合并单元格"),this,SLOT(mergeCells()),QKeySequence("Ctrl+M"));
    menu.addAction(tr("拆分单元格"),this,SLOT(splitCells()),QKeySequence("Ctrl+K"));
    menu.addAction(tr("初始化单元格"),this,SLOT(clearCells()),QKeySequence("Crtl+Z"));
    menu.exec(QCursor::pos());
}

void tableWindow::mergeCells()
{
    QModelIndexList list = tableWidget->selectionModel()->selectedIndexes();
    if(list.size()<2)
    {
        QMessageBox::warning(this,"单元格合并","所选中单元格中为单个单元格,无法合并","确定");
        return;
    }
    int minRow=0;
    int maxRow=0;
    int minCol=0;
    int maxCol=0;

    QList<QTableWidgetSelectionRange> vec = tableWidget->selectedRanges();

    if(vec.size()>0)
    {
        minRow = vec[0].topRow();
        maxRow = vec[0].bottomRow();
        minCol = vec[0].leftColumn();
        maxCol = vec[0].rightColumn();
    }

    for(auto &v: qAsConst(vec))
    {
        if(minRow>v.topRow())
            minRow = v.topRow();
        if(maxRow<v.bottomRow())
            maxRow = v.bottomRow();
        if(minCol>v.leftColumn())
            minCol = v.leftColumn();
        if(maxCol<v.rightColumn())
            maxCol = v.rightColumn();
    }

    int rowSpan = (maxRow-minRow)+1;
    int colSpan = (maxCol-minCol)+1;
    tableWidget->setSpan(minRow,minCol,rowSpan,colSpan);
    mergedCells.append(qMakePair(qMakePair(minRow,minCol),qMakePair(rowSpan,colSpan)));
}

void tableWindow::splitCells()
{
    QModelIndexList list = tableWidget->selectionModel()->selectedIndexes();
    if(list.size()<1)
    {
        QMessageBox::warning(this,"单元格拆分","没有选择合并的单元格,无法拆分","确定");
        return;
    }
    const QModelIndex index = list.first();
    int row = index.row();
    int col = index.column();
    tableWidget->setSpan(row,col,1,1);
}

void tableWindow::clearCells()
{
    tableWidget->clearSpans();
}

void tableWindow::onButtonClicked()
{
    QInputDialog * dialog = new QInputDialog(this);
    dialog->setInputMode(QInputDialog::TextInput);
    dialog->setWindowTitle("表格名字");
    dialog->setLabelText("请输入表格名字");
    dialog->setTextValue("");
    dialog->setTextEchoMode(QLineEdit::Normal);
    int result = dialog->exec();
    if(result == QDialog::Accepted && !dialog->textValue().isEmpty())
    {
        excelOperation excelOP(dialog->textValue(),tableWidget);
        excelOP.createExcelDocument();
    }

    delete dialog;
}
