#include "fountainserver.h"
#include <QTextCodec>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include "tcppackager.h"

fountainServer::fountainServer(QObject *parent): QObject(parent), tcpSocket(new QTcpSocket(this)), m_isFountainOnline(false)
{

    tcpServer = new QTcpServer(this);

    if(tcpServer->listen(QHostAddress(QHostAddress::Any),8080))
    {
        qDebug() << "DZOO";
        connect(tcpServer,SIGNAL(newConnection()),this,SLOT(newConnectionHandler()));
    }
}

void fountainServer::setIsFountainOnline(const bool &input)
{
    if(m_isFountainOnline != input)
    {
        m_isFountainOnline = input;
        informClientFountainStatus();
    }
}

void fountainServer::informClientFountainStatus()
{
    sendTcpPackageToClients(tcpPackager::fountainStatus(m_isFountainOnline));
}

void fountainServer::informClientCurrentPlayingProgram()
{
    sendTcpPackageToClients(tcpPackager::fountainCurrentPlayingProgram(m_currentProgram));
}

void fountainServer::sendTcpPackageToClients(const QByteArray &tcpPackage)
{
    foreach (clientTcpSocket* theClient, clientList) {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_8);
        out << tcpPackage;
        theClient->write(block);
    }
}

void fountainServer::newConnectionHandler()
{

    clientList.append((clientTcpSocket*)tcpServer->nextPendingConnection());

//    clientList.append(static_cast<clientTcpSocket*);

    //     tcpSocket = tcpServer->nextPendingConnection();
    in.setDevice(clientList.last());
    in.setVersion(QDataStream::Qt_5_8);

    connect(clientList.last(), SIGNAL(readyRead()),this,SLOT(readyReadHandler()));
    informClientFountainStatus();

}

void fountainServer::readyReadHandler()
{

    in.startTransaction();

    QByteArray requestFromClient;

    in >> requestFromClient;

    if(tcpPackager::isPackageValid(requestFromClient))
    {
        QJsonObject requestJsonObject = tcpPackager::packageToJson(requestFromClient);

        QString theCommand = requestJsonObject["Command"].toString();

        if(theCommand == "playProgram")
        {
#if fountainServerDebug
            qDebug() << requestJsonObject["ProgramName"].toString();
            qDebug() << QByteArray::fromHex(requestJsonObject["ProgramData"].toString().toUtf8());

#endif

#if fountainDeviceMode

            m_currentProgram = requestJsonObject["ProgramName"].toString();
            // qDebug() << nextFortune;
            emit toSerial(QByteArray::fromHex(requestJsonObject["ProgramData"].toString().toUtf8()));
            informClientCurrentPlayingProgram();
#endif

        }
        else if (theCommand == "isFountainOnline") {

            informClientFountainStatus();
        }
        else if(theCommand =="whoIsControlling")
        {
            if(!isClientExist(requestJsonObject["ClientId"].toString()))
            {

            }
        }


    }

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
    sendTcpPackageToClients(tcpPackager::fountainResponse(data));
}

void fountainServer::serialConnectedHandler()
{
    setIsFountainOnline(true);
}

void fountainServer::serialDisconnectedHandler()
{
    setIsFountainOnline(false);
}

bool fountainServer::isClientExist(const QString &id)
{
    foreach (clientTcpSocket* theClient, clientList) {

        if(theClient->getUUID() == id)
        {
            return true;
        }

    }

    return false;
}
