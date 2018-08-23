#include "tcpclient.h"

tcpClient::tcpClient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket();

}

tcpClient::~tcpClient()
{
    emit receive_Msg("you disconnect client",0);
    if(socket->isOpen())
        socket->close();
    delete socket;

}
//初始化客户端内容，包括信号--槽连接以及访问的服务器ip和port
void tcpClient::init_client(QString ip, QString port)
{
    connect(socket,SIGNAL(readyRead()),this,SLOT(read_msg()));
    connect(socket,SIGNAL(connected()),this,SLOT(connect_suc()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(server_dis()));

    socket->connectToHost(ip,port.toInt());

}

//当写入数据的时候，返回true；
bool tcpClient::write_Msg(QString msg)
{
    int count = socket->write(msg.toUtf8());
    if(-1 == count)
        return false;
    return true;
}
//定时两秒，连接服务器
bool tcpClient::is_connect()
{
//    该方法会阻塞挂起，当服务器连接成功时候会返回true。超时返回false
    if(socket->waitForConnected(1000*2))
    {
        QString msg;
        get_ip_port(msg, socket);
        msg = "your ID:"+msg;
        emit receive_Msg(msg,0);
        return true;
    }
    else
        return false;
}


//接收服务器数据，并且做格式化处理
void tcpClient::read_msg()
{
    QString msg = socket->readAll();
    int len = msg.length();
    msg = "from server :"+msg;

    emit receive_Msg(msg,len);
}

void tcpClient::connect_suc()
{
    emit sig_connect(true);
}

void tcpClient::server_dis()
{
    emit sig_connect(false);
}

void tcpClient::get_ip_port(QString &val, QTcpSocket *obj)
{
    //得到：ip:port

    QString ip = obj->peerAddress().toString().toUtf8();
    QString port = QString("%1").arg(obj->peerPort());
    val = ip+":"+port;
}
