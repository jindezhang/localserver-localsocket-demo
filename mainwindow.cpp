#include "mainwindow.h"
#include "ui_mainwindow.h"

#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //网络设置区初始化
    QStringList qsl_sel;
    qsl_sel<<"client"<<"server";
    ui->cb_select->addItems(qsl_sel);
    //connect(ui->cb_select, SIGNAL(activated(QString)), this, SLOT(changeText(QString)));//下拉框变化时候
    //ui->ed_ip->setText("127.0.0.1");
    ui->ed_port->setText("8080");
    //ui->ed_ip->setEnabled(false);
    ui->ed_port->setEnabled(false);
    ui->ed_port->hide();
    ui->l_port->hide();

    net_status = false;
    ui->bt_send->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeText(QString str)
{
    if(str == "client"){

        ui->l_ip->setText("（2）服务器名称");
        ui->l_port->setText("（3）服务器端口");

    }else if(str == "server"){

        ui->l_ip->setText("（2）服务器名称");
        ui->l_port->setText("（3）本地端口");

    }
}

void MainWindow::on_pushButton_clicked()
{

    QString sel = ui->cb_select->currentText();
    QString name = ui->ed_ip->text();
    qDebug()<<"name="<<name;

//    if(!isEnglish(name)||name == ""){
//        QMessageBox::information(this, "info", "The server name is no english!");
//        return;
//    }

    if(sel == "server"){
        if(net_status){
            server.close_();
        }else if(!server.init(name)){
            // 初使化失败, 说明已经有一个在运行了
           QMessageBox::information(this, "info", "There is already exist one!");
           return;
        }
        connect(&server, SIGNAL(newMessage(QString &)),this, SLOT(showMsg(QString&)));


    }
    if(sel == "client"){
        if(net_status){

            client.close_();
        }else if(client.init(name)){
            QMessageBox::information(this, "info", "connect server failed!");
            return;
        }
        connect(&client, SIGNAL(newMessage(QString&)), this, SLOT(showMsg(QString&)));

    }
    set_net(net_status);
    net_status = !net_status;



}

void MainWindow::showMsg(QString &msg)
{
    QDateTime local(QDateTime::currentDateTime());
    QString localTime = QString("【%1】：%2").arg(local.toString("yyyy-MM-dd:hh:mm:ss")).arg(msg);

    ui->edit_show->appendPlainText(localTime);
}

void MainWindow::on_bt_send_clicked()
{
//    if(!net_status){
//        QMessageBox::information(this, "info", "no connect!");
//        return;
//    }
    QString sel = ui->cb_select->currentText();
    QString msg = ui->textEdit->toPlainText();
    if(sel == "server"){
        if(server.sendMsg(msg)){
            ui->textEdit->clear();
        }else
          QMessageBox::information(this, "info", "send failed!");
    }
    if(sel == "client"){
        if(client.sendMsg(msg)){
            ui->textEdit->clear();
        }else
          QMessageBox::information(this, "info", "send failed!");
    }
}

void MainWindow::set_net(bool b)
{
    ui->bt_send->setEnabled(!b);
    ui->cb_select->setEnabled(b);
    ui->ed_ip->setEnabled(b);
    if(!b){

        ui->l_status->setText("已连接");
        ui->pushButton->setText("断开");
    }
    else{
//        ui->cb_select->setEnabled(!b);
//        ui->ed_ip->setEnabled(!b);
        ui->pushButton->setText("连接");
        ui->l_status->setText("未连接");
    }
}

bool MainWindow::isEnglish(QString &qstrSrc)
{
    QByteArray ba = qstrSrc.toLatin1();
        const char *s = ba.data();
        bool bret = true;
        while(*s)
        {
            if((*s>='A' && *s<='Z') || (*s>='a' && *s<='z'))
            {

            }
            else
            {
                bret = false;
                break;
            }
            s++;
        }
        return bret;
}
