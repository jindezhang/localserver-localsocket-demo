#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "localserver_.h"
#include <QMessageBox>
#include "localclinet_.h"
#include <QDateTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void changeText(QString str);
private slots:
    void on_pushButton_clicked();
    void showMsg(QString &msg);
    void on_bt_send_clicked();

private:
    Ui::MainWindow *ui;
    localserver_ server;
    localclinet_ client;
    bool net_status;
    void set_net(bool b);
    bool isEnglish(QString &qstrSrc);//判断是否纯英文。
};

#endif // MAINWINDOW_H
