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

    clientList.append(tcpServer->nextPendingConnection());

//     tcpSocket = tcpServer->nextPendingConnection();
     in.setDevice(clientList.last());
     in.setVersion(QDataStream::Qt_5_8);

    connect(clientList.last(), SIGNAL(readyRead()),this,SLOT(readyReadHandler()));


    clientList.last()->write(m_currentProgram);


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

    m_currentProgram = testOboject["ProgramName"].toString();
   // qDebug() << nextFortune;
    emit toSerial(QByteArray::fromHex(testOboject["ProgramData"].toString().toUtf8()));
#endif

}


//void fountainServer::returnDataToClient(const QByteArray &data)
//{

//    QByteArray block;

//    QDataStream out(&block, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_5_8);
//    out << data;
//    tcpSocket->write(block);
//}

void fountainServer::fromSerialHandler(const QByteArray &data)
{
    foreach (QTcpSocket* theClient, clientList) {
            QByteArray block;
            QDataStream out(&block, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_8);
            out << data;
            theClient->write(block);
    }
}
