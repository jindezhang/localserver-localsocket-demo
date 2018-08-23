#include "tcpserver.h"

tcpServer::tcpServer( QObject *parent) : QObject(parent)
{
    mServer = new QTcpServer();

}

tcpServer::~tcpServer()
{
    emit receiveMsg("you close TcpServer.",0);
    mServer->close();
    delete mServer;

}

//初始化服务器，包括监听和建立信号--槽函数
bool tcpServer::init_Server(QString port)
{
    if(! mServer->listen(QHostAddress::Any,port.toInt()))
        return false;
    connect(mServer, SIGNAL(newConnection()),this,SLOT(new_clict()));
    return true;

}

//向指定的IP发送数据，当ip=all时候，给所有客户端发送数据。发送失败返回false；
bool tcpServer::write_Msg(QString ip,QString msg)
{
    bool sender = false;
    if(ip == "all")
        sender = true;

    int i ;
    QString port;
    for(auto item:clients){
        if(sender){
            i = item->write(msg.toUtf8());
            continue;
        }
        port = QString("%1").arg(item->peerPort());
//        contains() QString 判断是否存在子串
        if( ip.contains(port) && ip.contains(item->peerAddress().toString()))
            i = item->write(msg.toUtf8());
    }
    if(-1 == i)
        return false;
    return true;
}

//获取所有在线的客户端ip+port
void tcpServer::get_clients(QStringList &clients_ip)
{
    QString id;
    for(auto item:clients){

        //peerAddress().toString()获取socket 的IP
        id = item->peerAddress().toString().toUtf8()+":"
                +QString("%1").arg(item->peerPort());
        clients_ip.append(id);
    }
}

void tcpServer::send_clientID(QString id)
{

}

void tcpServer::new_clict()
{

    mSocket = mServer->nextPendingConnection();//与客户端通信的套接字

    //关联接收客户端数据信号readyRead信号（客户端有数据就会发readyRead信号）
    connect(mSocket,SIGNAL(readyRead()),this,SLOT(read_client_data()));

    //检测掉线信号
    connect(mSocket,SIGNAL(disconnected()),this,SLOT(client_dis()));

    clients.append(mSocket);//保存连接的客户端socket
    //得到所有在线的客户端列表
    QStringList strl_clients;
    get_clients(strl_clients);
    emit sig_clients(strl_clients);

    //提示信息
    QString str_new;
    get_ip_port(str_new,mSocket);
    //回写客户端ID
    mSocket->write(str_new.toUtf8());

    str_new = str_new+" is connect....";
    emit receiveMsg(str_new,0);


}

void tcpServer::read_client_data()
{
    //可以实现同时读取多个客户端发送过来的消息
    QTcpSocket *obj = (QTcpSocket*)sender();//触发该槽函数的对象。QTcpSocket的对象
    QString msg = obj->readAll();
    int len = msg.length();
    QString ip = obj->peerAddress().toString().toUtf8();
    QString port = QString("%1").arg(obj->peerPort());

    qDebug()<<ip<<"+"<<port<<"+"<<obj->peerName();
    msg = "from [ "+ip+":"+port+" ]: "+msg;
    emit receiveMsg(msg, len);
    qDebug()<<msg;
}

void tcpServer::client_dis()
{
    QTcpSocket *obj = (QTcpSocket*)sender();//掉线对象
    int row  = clients.indexOf(obj);//找到掉线的对象，
    clients.remove(row);//删除对应的对象

    QString msg;
    get_ip_port(msg,obj);
    msg = msg+ " disconnect....";
    emit receiveMsg(msg,0);

    QStringList strl_clients;
    get_clients(strl_clients);
    emit sig_clients(strl_clients);
    //qDebug()<<obj->peerAddress().toString();//打印出掉线对象的ip
}

void tcpServer::get_ip_port(QString &val, QTcpSocket *obj)
{
    //得到：ip:port
    QString ip = obj->peerAddress().toString().toUtf8();
    QString port = QString("%1").arg(obj->peerPort());
    val = ip+":"+port;
}
