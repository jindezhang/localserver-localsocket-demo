#ifndef LOCALCLINET__H
#define LOCALCLINET__H

#include <QObject>
#include <QLocalSocket>
#include <QTextStream>

class localclinet_ : public QObject
{
    Q_OBJECT
public:
    explicit localclinet_(QObject *parent = nullptr);
    int init(QString &str);
    bool sendMsg(QString &msg);
    void close_();

signals:
    void disconnect_sig();
    void newMessage(QString & msg);
public slots:
    void readMsg();
    void disconnect_();
private:
    QLocalSocket ls;
};

#endif // LOCALCLINET__H
