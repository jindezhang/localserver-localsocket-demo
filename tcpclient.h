#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>

class tcpClient : public QObject
{
    Q_OBJECT
public:
    explicit tcpClient(QObject *parent = nullptr);
    ~tcpClient();
    void init_client(QString ip, QString port);//初始化客户端。
    bool write_Msg(QString msg);//向服务器写数据

    bool is_connect();//等待连接服务器
signals:
    void receive_Msg(QString msg,int len);//接收到的数据。以及len表示长度。
    void sig_connect(bool connect);//是否断开连接
public slots:
    void read_msg();//当接收到服务器数据的槽函数
    void connect_suc();//连接成功槽函数
    void server_dis();//断开连接的槽函数
    void get_ip_port(QString &val,QTcpSocket *obj);



private:
    QTcpSocket *socket;
};

#endif // TCPCLIENT_H
