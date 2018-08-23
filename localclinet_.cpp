#include "localclinet_.h"

localclinet_::localclinet_(QObject *parent) : QObject(parent)
{

}

int localclinet_::init(QString &str)
{
    ls.connectToServer(str);

     if (ls.waitForConnected()){
         if(ls.state() != QAbstractSocket::ConnectedState){
            qDebug()<<"client state";
             return 1;
         }
         connect(&ls, SIGNAL(readyRead()), this, SLOT(readMsg()));
         connect(&ls, SIGNAL(disconnected()), this, SLOT(disconnect_()));
         return 0;

    }else
         return 1;

}

bool localclinet_::sendMsg(QString &msg)
{
    QTextStream ts(&ls);
    ts << msg;
    ts.flush();
    return ls.waitForBytesWritten();

}

void localclinet_::close_()
{
    ls.close();

}

void localclinet_::disconnect_()
{
    emit disconnect_sig();
}

void localclinet_::readMsg()
{
    QTextStream in(&ls);
    QString     readMsg;
    // 读出数据
    readMsg = in.readAll();
    // 发送收到数据信号
    emit newMessage(readMsg);
}
