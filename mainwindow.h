#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <imageconvert.h>
#include "http.h"
#include "tablewindow.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slotOpenFile();

    void on_imageButton_clicked();

    void on_mysqlSaveBtn_clicked();

    void on_excelSaveBtn_clicked();

private:
    Ui::MainWindow *ui;


private:
    QString Imgfilepath;
    tableWindow *tableWindow;
    QJsonObject obj;
};
#endif // MAINWINDOW_H
