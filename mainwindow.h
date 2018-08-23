#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "localserver_.h"
#include <QMessageBox>
#include "localclinet_.h"
#include <QDateTime>
#include "tcpclient.h"
#include "tcpserver.h"

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
    void client_disconnect();

    void get_receiveMsg(QString msg,int len);
    void get_clients(QStringList clents);

    void client_suc(bool suc);
    void on_btn_clear_clicked();

private:
    Ui::MainWindow *ui;
    localserver_ server;
    localclinet_ client;
    tcpClient *tcpclient;
    tcpServer *tcpserver;
    bool net_status;
    void set_net(bool b);
    bool isEnglish(QString &qstrSrc);//判断是否纯英文。
    QStringList protocol_net;
};

#endif // MAINWINDOW_H
