#include "fountainserver.h"
#include <QTcpSocket>

fountainServer::fountainServer(QObject *parent): QObject(parent)
{

    tcpServer = new QTcpServer(this);

    if(tcpServer->listen(QHostAddress("192.168.1.34"),5050))
    {
        qDebug() << "DZOO";
        connect(tcpServer,SIGNAL(newConnection()),this,SLOT(newConnectionHandler()));
    }

    qDebug() << "deo dzo";


}

void fountainServer::newConnectionHandler()
{

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();

    qDebug()<< clientConnection->readAll();

}
