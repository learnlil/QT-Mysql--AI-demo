#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include <QJsonObject>
namespace Ui {
class inputDialog;
}

class inputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit inputDialog(QWidget *parent = nullptr,QJsonObject obj = QJsonObject());
    ~inputDialog();
    QString getText();
    QJsonObject getObj();
private slots:
    void on_okBtn_clicked();

private:
    QString text;
    QJsonObject obj;
    Ui::inputDialog *ui;
};

#endif // INPUTDIALOG_H
