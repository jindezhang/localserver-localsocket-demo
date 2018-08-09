#include "localserver_.h"

localserver_::localserver_()
{
    m_server = 0;
    newsocket = 0;
}

localserver_::~localserver_()
{
    if(m_server)
        delete m_server;
}

int localserver_::init(QString &servername)
{
    // 如果已经有一个实例在运行了就返回0
    if (isServerRun(servername)) {
        return 0;
    }
    m_server = new QLocalServer;
    // 先移除原来存在的,如果不移除那么如果
    // servername已经存在就会listen失败
    QLocalServer::removeServer(servername);
    // 进行监听
    m_server->listen(servername);
    connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnect()));
    return 1;
}

bool localserver_::sendMsg(QString &msg)
{
    QTextStream ts(newsocket);
    ts << msg;
    ts.flush();
    return newsocket->waitForBytesWritten();


}

void localserver_::close_()
{
    m_server->close();

}

void localserver_::newConnect()
{
    if(newsocket == 0){

    }else if(newsocket->isValid()){
        return;
    }
    newsocket = m_server->nextPendingConnection();

    connect(newsocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

}

void localserver_::readyRead()
{
    qDebug()<<"new data";
    // 取得是哪个localsocket可以读数据了
//    QLocalSocket *local = static_cast<QLocalSocket *>(sender());
//    if (!local)
//        return;
    QTextStream in(newsocket);
    QString     readMsg;
    // 读出数据
    readMsg = in.readAll();
    // 发送收到数据信号
    emit newMessage(readMsg);
}

int localserver_::isServerRun(QString &servername)
{
    QLocalSocket ls;
    ls.connectToServer(servername);
    if(ls.waitForConnected(1000)){
        ls.disconnectFromServer();
        ls.close();

        return 1;
    }
    return 0;
}
