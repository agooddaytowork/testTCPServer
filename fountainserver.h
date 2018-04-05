#ifndef FOUNTAINSERVER_H
#define FOUNTAINSERVER_H

#include <QTcpServer>
#include <QNetworkSession>
#include <QDebug>

#define fountainServerDebug (1)


class fountainServer: public QObject
{
    Q_OBJECT


    QTcpServer *tcpServer = nullptr;

    int m_Serverport;

public:
    fountainServer(QObject *parent = nullptr);

public slots:

    void newConnectionHandler();
};

#endif // FOUNTAINSERVER_H
