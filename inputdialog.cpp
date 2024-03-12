#include "inputdialog.h"
#include "ui_inputdialog.h"
#include "mysqloperation.h"
inputDialog::inputDialog(QWidget *parent,QJsonObject obj)
    : QDialog(parent),obj(obj)
    , ui(new Ui::inputDialog)
{
    ui->setupUi(this);
}

inputDialog::~inputDialog()
{
    delete ui;
}

QString inputDialog::getText()
{
    return text;
}

QJsonObject inputDialog::getObj()
{
    return obj;
}

void inputDialog::on_okBtn_clicked()
{
    text = ui->lineEdit->text();
    if(text.isEmpty())
    {
        ui->label->setText(tr("请输入,如果不想输点击取消"));
    }else
    {
        mysqlOperation op;
        op.connMysql();
        op.insertMysqlTable(obj,text);
        close();
    }
}

