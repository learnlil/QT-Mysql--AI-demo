#ifndef TABLEWINDOW_H
#define TABLEWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QGridLayout>
#include <QJsonObject>
namespace Ui {
class tableWindow;
}

class tableWindow : public QWidget
{
    Q_OBJECT

public:
    explicit tableWindow(QWidget *parent = nullptr,QJsonObject obj = QJsonObject());
    ~tableWindow();
    void initTable(QJsonObject obj);
private slots:
    void on_customContextMenuRequested();
    void mergeCells();
    void splitCells();
    void clearCells();
    void onButtonClicked();
private:
    QTableWidget *tableWidget;
    QGridLayout *layout;
    Ui::tableWindow *ui;
    QList<QPair<QPair<int,int>,QPair<int,int>>> mergedCells;
};

#endif // TABLEWINDOW_H
