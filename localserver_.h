#ifndef LOCALSERVER__H
#define LOCALSERVER__H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QTextStream>

class localserver_:public QObject
{
    Q_OBJECT
public:
    localserver_();
    ~localserver_();
    int init(QString &servername);
    bool sendMsg(QString &msg);
    void close_();
private slots:
    void newConnect();
    void readyRead();
signals:
    void newMessage(QString &msg);
private:
    QLocalServer *m_server;
    QLocalSocket*  newsocket;
    int isServerRun(QString &servername);

};

#endif // LOCALSERVER__H
