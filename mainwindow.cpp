#include "mainwindow.h"
#include "ui_mainwindow.h"

#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("form RD330");

    //网络设置区UI初始化

    protocol_net<<"client"<<"server"<<"tcpServer"<<"tcpClient";
    ui->cb_select->addItems(protocol_net);
    connect(ui->cb_select, SIGNAL(activated(QString)), this, SLOT(changeText(QString)));//下拉框变化时候
    //ui->ed_ip->setText("127.0.0.1");
    ui->ed_port->setText("8080");
    //ui->ed_ip->setEnabled(false);
   // ui->ed_port->setEnabled(false);
//    ui->ed_port->hide();
//    ui->l_port->hide();

    net_status = false;
    ui->bt_send->setEnabled(false);

    //TCP
    tcpclient = nullptr;
    tcpserver = nullptr;
    ui->cb_clients->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeText(QString str)
{
    ui->l_ip->setEnabled(true);
    if(str == "tcpClient"){

        ui->l_ip->setText("（2）服务器IP");
        ui->ed_ip->setText("127.0.0.1");
        ui->l_port->setText("（3）服务器端口");

    }else if(str == "tcpServer"){

        //ui->l_ip->setText("（2）服务器名称");
        ui->l_ip->setEnabled(false);
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


    int pos = protocol_net.indexOf(sel);

    switch (pos) {
    case 0:
        qDebug()<<"sel "<<pos<<" ="<<sel;
        if(net_status){
            qDebug()<<"server close";
            server.close_();
        }else if(!server.init(name)){
            // 初使化失败, 说明已经有一个在运行了
           QMessageBox::information(this, "info", "There is already exist one!");
           return;
        }
        connect(&server, SIGNAL(newMessage(QString &)),this, SLOT(showMsg(QString&)));

        break;

    case 1:
        qDebug()<<"sel "<<pos<<" ="<<sel;
        if(net_status){

            client.close_();
        }else if(client.init(name)){
            QMessageBox::information(this, "info", "connect server failed!");
            return;
        }
        connect(&client, SIGNAL(newMessage(QString&)), this, SLOT(showMsg(QString&)));
        connect(&client,SIGNAL(disconnect_sig()), this, SLOT(client_disconnect()));

        break;

    case 2:
        qDebug()<<"sel "<<pos<<" ="<<sel;
        if(net_status){
            qDebug()<<"server close";
            delete tcpserver;
            ui->cb_clients->setEditable(false);//
        }else{
            tcpserver = new tcpServer();
            if(!tcpserver->init_Server(ui->ed_port->text())){
                ui->edit_show->appendPlainText("open tcpServer failed....");
            }
            connect(tcpserver,SIGNAL(receiveMsg(QString,int)),this,SLOT(get_receiveMsg(QString,int)));
            connect(tcpserver,SIGNAL(sig_clients(QStringList)),this, SLOT(get_clients(QStringList)));
            ui->cb_clients->setEnabled(true);//开启客户端列表
        }
        break;

    case 3:
        qDebug()<<"sel "<<pos<<" ="<<sel;
        if(net_status){
            qDebug()<<"client close";
            if(tcpclient != nullptr)
                delete tcpclient;
            //ui->cb_clients->setEditable(false);
        }else{
            tcpclient = new tcpClient();//要先connect建立信号--槽函数 ，才能触发对应的信号。
            connect(tcpclient,SIGNAL(receive_Msg(QString,int)),this,SLOT(get_receiveMsg(QString,int)));
            connect(tcpclient,SIGNAL(sig_connect(bool)),this, SLOT(client_suc(bool)));
            tcpclient->init_client(ui->ed_ip->text(),ui->ed_port->text());

            if(!tcpclient->is_connect()){//判断是否连接成功
                get_receiveMsg("未连接服务器....",0);
                return;
             }
            //ui->cb_clients->setEnabled(true);
        }
        break;

    default:
        break;
    }
/*
    if(sel == "server"){
        if(net_status){
            qDebug()<<"server close";
            server.close_();
        }else if(!server.init(name)){
            // 初使化失败, 说明已经有一个在运行了
           QMessageBox::information(this, "info", "There is already exist one!");
           return;
        }
        connect(&server, SIGNAL(newMessage(QString &)),this, SLOT(showMsg(QString&)));


    }
    else if(sel == "client"){
        if(net_status){

            client.close_();
        }else if(client.init(name)){
            QMessageBox::information(this, "info", "connect server failed!");
            return;
        }
        connect(&client, SIGNAL(newMessage(QString&)), this, SLOT(showMsg(QString&)));
        connect(&client,SIGNAL(disconnect_sig()), this, SLOT(client_disconnect()));

    }
    //连接tcp服务器
    else if(sel == "tcpServer"){
        if(net_status){
            qDebug()<<"server close";
            delete tcpserver;
            ui->cb_clients->setEditable(false);//
        }else{
            tcpserver = new tcpServer();
            if(!tcpserver->init_Server(ui->ed_port->text())){
                ui->edit_show->appendPlainText("open tcpServer failed....");
            }
            connect(tcpserver,SIGNAL(receiveMsg(QString,int)),this,SLOT(get_receiveMsg(QString,int)));
            connect(tcpserver,SIGNAL(sig_clients(QStringList)),this, SLOT(get_clients(QStringList)));
            ui->cb_clients->setEnabled(true);//开启客户端列表
        }
   }

    //连接tcp客户端
    else if(sel == "tcpClient"){
        if(net_status){
            qDebug()<<"client close";
            if(tcpclient != nullptr)
                delete tcpclient;
            //ui->cb_clients->setEditable(false);
        }else{
            tcpclient = new tcpClient();//要先connect ，才能触发对应的信号。
            connect(tcpclient,SIGNAL(receive_Msg(QString,int)),this,SLOT(get_receiveMsg(QString,int)));
            connect(tcpclient,SIGNAL(sig_connect(bool)),this, SLOT(client_suc(bool)));
            tcpclient->init_client(ui->ed_ip->text(),ui->ed_port->text());

            if(!tcpclient->is_connect()){//判断是否连接成功
                get_receiveMsg("未连接服务器....",0);
                return;
             }
            //ui->cb_clients->setEnabled(true);
        }
   }
*/
//    对网络状态进行判断。点击按钮的两种形式进行切换
    set_net(net_status);
    net_status = !net_status;//网络状态



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
    else if(sel == "client"){
        if(client.sendMsg(msg)){
            ui->textEdit->clear();
        }else
          QMessageBox::information(this, "info", "send failed!");
    }

    else if(sel == "tcpServer"){
        QString str_client_ip = ui->cb_clients->currentText();
        if(tcpserver->write_Msg(str_client_ip,msg)){
            ui->textEdit->clear();
        }else
          QMessageBox::information(this, "info", "send failed!");
    }
    else if(sel == "tcpClient"){
        if(tcpclient->write_Msg(msg)){
            ui->textEdit->clear();
        }else
          QMessageBox::information(this, "info", "send failed!");
    }

}

void MainWindow::client_disconnect()
{
    QMessageBox::information(this, "info", "disconnect server !");
    set_net(net_status);
    net_status = !net_status;
}

void MainWindow::get_receiveMsg(QString msg, int len)
{
    QString msg_len = QString("%1").arg(len+ui->l_len->text().toInt());
    ui->l_len->setText(msg_len);
    QDateTime local(QDateTime::currentDateTime());
    QString localTime = QString("【%1】：\n%2").arg(local.toString("yyyy-MM-dd:hh:mm:ss")).arg(msg);

    ui->edit_show->appendPlainText(localTime);

}

void MainWindow::get_clients(QStringList clents)
{
    ui->cb_clients->clear();
    ui->cb_clients->addItem("all");
    ui->cb_clients->addItems(clents);
}

void MainWindow::client_suc(bool suc)
{
    qDebug()<<"client_suc "<<suc;
    if(suc){
        set_net(false);
        get_receiveMsg("已连接服务器....",0);
    }else{
        set_net(true);
        get_receiveMsg("未连接服务器....",0);
    }
}

void MainWindow::set_net(bool b)
{
    qDebug()<<"set_net"<<b;
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

void MainWindow::on_btn_clear_clicked()
{
    ui->edit_show->clear();
}
