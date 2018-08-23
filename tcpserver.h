#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>

class tcpServer : public QObject
{
    Q_OBJECT
public:
    explicit tcpServer(QObject *parent = nullptr);
    ~tcpServer();
    bool init_Server(QString port);//初始化服务器
    bool write_Msg(QString ip, QString msg);//向指定IP客户端写入内容
    void get_clients(QStringList &clients_ip);//获取所有连接客户端的IP
    void send_clientID(QString id);
signals:
    void receiveMsg(QString msg, int len);//接收到的数据。以及len表示长度。
    void sig_clients(QStringList clients_ip);//当客户端有新连接和断开时候触发
public slots:
    void new_clict();//有新连接的槽函数
    void read_client_data();//有数据的时候的槽函数
    void client_dis();//当客户端断开连接的槽函数

private slots:
    void get_ip_port(QString &val, QTcpSocket *obj);//获取socket对象的IP和port
private:
    QTcpServer *mServer;
    QTcpSocket *mSocket;
    QVector <QTcpSocket *> clients;

};

#endif // TCPSERVER_H
