#include "fountainserver.h"
#include <QTextCodec>
#include <QTcpSocket>

#include <QJsonDocument>
#include <QJsonObject>

fountainServer::fountainServer(QObject *parent): QObject(parent), tcpSocket(new QTcpSocket(this))
{

    tcpServer = new QTcpServer(this);

    if(tcpServer->listen(QHostAddress(QHostAddress::Any),8080))
    {
        qDebug() << "DZOO";
        connect(tcpServer,SIGNAL(newConnection()),this,SLOT(newConnectionHandler()));
    }
}

void fountainServer::newConnectionHandler()
{
     tcpSocket = tcpServer->nextPendingConnection();
     in.setDevice(tcpSocket);
     in.setVersion(QDataStream::Qt_5_8);

    connect(tcpSocket, SIGNAL(readyRead()),this,SLOT(readyReadHandler()));

}

void fountainServer::readyReadHandler()
{

    in.startTransaction();

    QByteArray nextFortune;
    in >> nextFortune;

    QJsonDocument aDocument(QJsonDocument::fromJson(nextFortune));

    QJsonObject testOboject = aDocument.object();

    QByteArray cc;
    cc.append(testOboject["ProgramData"].toString());

#if fountainServerDebug
    qDebug() << testOboject["ProgramName"].toString();
    qDebug() << QByteArray::fromHex(testOboject["ProgramData"].toString().toUtf8());

#endif

#if fountainDeviceMode
   // qDebug() << nextFortune;
    emit toSerial(QByteArray::fromHex(testOboject["ProgramData"].toString().toUtf8()));
#endif

}
