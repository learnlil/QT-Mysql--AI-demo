#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "inputdialog.h"
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QPixmap>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSslSocket>
#include <QJsonArray>
#include <QTableWidget>
#include <QStringList>
#include <QInputDialog>

const QString url = "https://aip.baidubce.com/oauth/2.0/token?client_id=%1&client_secret=%2&grant_type=client_credentials";
const QString client_id = "";       //自己的API key
const QString secret = "";          //自己的Secret key

const QString BaiduDetectUrl = "https://aip.baidubce.com/rest/2.0/ocr/v1/table?access_token=%1";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->imageLabel->setScaledContents(true);
    connect(ui->actionOpen,&QAction::triggered,this,&MainWindow::slotOpenFile);


    qDebug()<< "输出当前QT支持的openSSL版本:" << QSslSocket::sslLibraryBuildVersionString();

    qDebug() << "OpenSSL支持情况:" << QSslSocket::supportsSsl();

    qDebug() << "OpenSSL运行时SSL库版本:" << QSslSocket::sslLibraryVersionString();
}

MainWindow::~MainWindow()
{
    delete ui;
}
//选择图片文件
void MainWindow::slotOpenFile()
{
    Imgfilepath = QFileDialog::getOpenFileName(this,"选择一张图片",QCoreApplication::applicationFilePath());
    if(Imgfilepath.isEmpty())
    {
        QMessageBox::warning(this,"提示","请选择一张图片");
    }
    else
    {
        QPixmap pix(Imgfilepath);
        pix = pix.scaled(ui->imageLabel->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);//把图片按比例缩放
        ui->imageLabel->setPixmap(pix);
    }
}

void MainWindow::on_imageButton_clicked()
{
    if(Imgfilepath.isEmpty() == true)
    {
        return;
    }
    QByteArray img = imageConvert::imageToBase64(Imgfilepath);  //发送格式image=img
    QByteArray imageData = "image=" + img;
    //获取access_token
    QByteArray replyData;

    QString Url = QString(url).arg(client_id).arg(secret);
    QMap<QString,QString>header;
    header.insert(QString("Content-Type"),QString("application/x-www-form-urlencoded"));
    bool ret = http::http_postRequest(Url,header,imageData,replyData);
    if(ret == false)
    {
        return;
    }
    //获得token解析
    QJsonObject obj = QJsonDocument::fromJson(replyData).object();
    QString AccessToken = obj.value("access_token").toString();

    //请求识别
    replyData.clear();
    QString imgUrl = BaiduDetectUrl.arg(AccessToken);
    ret = http::http_postRequest(imgUrl,header,imageData,replyData);
    if(ret == false)
    {
        return;
    }
    obj = QJsonDocument::fromJson(replyData).object();
    /*
JSON格式:
{
    "tables_result":[
        {
            "header":[],
            "body":[
                {
                    "col_end":1,
                    "row_end":1,
                    "cell_location":[
                    ],
                    "row_start":0,
                    "col_start":0,
                    "words":""
                },
                ......
                ],
                "table_location":[
                ],
                "footer":[]
        }
    ],
    "table_num":1,
    "log_id":"14234324"
}
    */
    //解析JSON数据
    QJsonArray tablesJson = obj["tables_result"].toArray();
    QJsonObject tableJson = tablesJson[0].toObject();
    QJsonArray bodyJson = tableJson["body"].toArray();

    //删除cell_location这个无用的数据,然后更新到obj中
    int cellCount = bodyJson.size();
    for(int i=0;i<cellCount;i++)
    {
        QJsonObject cell = bodyJson.at(i).toObject();
        cell.remove("cell_location");
        bodyJson.replace(i,cell);
    }
    //tablesJson.replace(1,bodyJson);
    //qDebug()<<bodyJson;
    tableJson.insert("body",bodyJson);
    tablesJson.replace(0,tableJson);
    obj.insert("tables_result",tablesJson);
    //qDebug()<<obj;
//----------------------------------------------------------------------------------
    this->obj = obj;
    qDebug()<<obj;
    tableWindow = new class tableWindow(this,obj);
    tableWindow->show();


}


void MainWindow::on_mysqlSaveBtn_clicked()
{
    inputDialog *dialog = new inputDialog(this,obj);
    dialog->show();
    connect(dialog, &inputDialog::finished, dialog, &inputDialog::deleteLater);
}


void MainWindow::on_excelSaveBtn_clicked()
{

}

