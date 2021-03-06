#include "fountainserver.h"
#include <QTextCodec>
#include "clienttcpsocket.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "tcppackager.h"


fountainServer::fountainServer(QObject *parent): QObject(parent), tcpSocket(new QTcpSocket(this)), m_isFountainOnline(false), m_currentClientAddress(""), m_IntervalTimer(new QTimer(this))
{

    tcpServer = new QTcpServer(this);
    m_IntervalTimer->setInterval(pollingInterval);
    m_IntervalTimer->setSingleShot(false);

    if(tcpServer->listen(QHostAddress(QHostAddress::Any),8080))
    {
        qDebug() << "DZOO";
        connect(tcpServer,SIGNAL(newConnection()),this,SLOT(newConnectionHandler()));

        m_IntervalTimer->start();
    }

    QObject::connect(this,SIGNAL(stillAvailable()),this,SLOT(readyReadHandler()));


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


    foreach (QTcpSocket* theClient, tcpSocketList) {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_8);
        out << tcpPackage;
        theClient->write(block);
    }


}

void fountainServer::fromCommunicationCheckerFountainStatusHandler(const bool &status)
{
    m_isFountainOnline = status;
    sendTcpPackageToClients(tcpPackager::fountainStatus(m_isFountainOnline));
}

void fountainServer::newConnectionHandler()
{

    QTcpSocket* newClient = tcpServer->nextPendingConnection();

    tcpSocketList.append(newClient);

    qDebug() << "tcpSocketList count: " + QString::number(tcpSocketList.count());



    in.setDevice(newClient);
    in.setVersion(QDataStream::Qt_5_8);
    //    tcpSocketList.append(static_cast<clientTcpSocket*);

    //     tcpSocket = tcpServer->nextPendingConnection();
    //    in.setDevice(newClient);
    //    in.setVersion(QDataStream::Qt_5_8);


    connect(newClient, SIGNAL(readyRead()),this,SLOT(readyReadHandler()));
    connect(newClient,SIGNAL(disconnected()),this,SLOT(clientDisconnectedHandler()));
    connect(newClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socketErrorHandler(QAbstractSocket::SocketError)));

    informClientFountainStatus();

}


void fountainServer::socketErrorHandler(QAbstractSocket::SocketError error)
{
    qDebug() << "Socket error:" + error;

     //tcpSocketList.clear();
    if(auto client = dynamic_cast<QTcpSocket *>(sender()))
    {

        tcpSocketList.removeOne(client);

        qDebug() << "remove disconnectedClient";
    }
}
void fountainServer::clientDisconnectedHandler(){

    if(auto client = dynamic_cast<QTcpSocket *>(sender()))
    {
       bool status = false;
        tcpSocketList.removeOne(client);

        qDebug() << "remove disconnectedClient :" + status;
    }
}

void fountainServer::readyReadHandler()
{
    static int counter = 0;

    if(counter == 5)
    {
        m_IntervalTimer->stop();
        counter =0;
    }
    if(counter == 0)
    {
        m_IntervalTimer->start();
    }

    counter++;



    QTcpSocket* theTCPClient = dynamic_cast<QTcpSocket *> (sender());

    bool isDisconnecting = false;

    if(theTCPClient)
    {
        in.setDevice(theTCPClient);
    }

    in.startTransaction();


    QByteArray requestFromClient;


    if(!in.commitTransaction()) return;
    in >> requestFromClient;

    if(tcpPackager::isPackageValid(requestFromClient))
    {
        counter = 0;
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
        else if(theCommand == "playSpeed")
        {
            emit toSerial(QByteArray::fromHex(requestJsonObject["Speed"].toString().toUtf8()));
        }
        else if (theCommand == "isFountainOnline") {
#if fountainServerDebug
            qDebug() << "isFountainOnline Request";

#endif
            informClientFountainStatus();
        }

        else if(theCommand == "Disconnecting")
        {
#if fountainServerDebug
            qDebug() << "Disconnecting Request";

#endif
            if(tcpPackager::getFountainID() == 0)
            {
                sendTcpPackageToClients(requestFromClient);
            }

            isDisconnecting = true;

        }
        else if(theCommand =="addNewClient")
        {
#if fountainServerDebug
            qDebug() << "addNewClient Request";

#endif
            bool clientExist = false;
            if(clientList.count() != 0)
            {
                foreach (clientTcpSocket theClient, clientList) {
                    if(theClient.getClientId() == requestJsonObject["ClientId"].toString())
                    {
                        clientExist = true;
                        theClient.m_clientSocket = theTCPClient;
                    }
                }

            }
            if(!clientExist)
            {
                clientTcpSocket aClient;

                if(clientList.count() == 0) aClient.setIsControlling(true);

                aClient.setClientId(requestJsonObject["ClientId"].toString());
                aClient.setClientType(requestJsonObject["ClientType"].toInt());
                aClient.setClientAddress(m_currentClientAddress);
                aClient.m_clientSocket = theTCPClient;
                clientList.append(aClient);
            }



            //            sendTcpPackageToClients(tcpPackager::AnswerWhoIsControlling(clientList.last().getClientId(), clientList.last().getClientType()));

        }
        else if(theCommand == "whoIsControlling")
        {

            if(tcpPackager::getFountainID() == 0)
            {
#if fountainServerDebug
                qDebug() << "whoIsControlling Request";

#endif
                foreach (clientTcpSocket theClient, clientList) {
                    if(theClient.isControlling())
                    {
                        sendTcpPackageToClients(tcpPackager::AnswerWhoIsControlling(theClient.getClientId(), theClient.getClientType()));

                    }
                }
            }

        }
        else if(theCommand == "getControlPermission")
        {

#if fountainServerDebug
            qDebug() << "getControlPermission Request";

#endif
            foreach (clientTcpSocket theClient, clientList) {

                if(theClient.getClientId() == requestJsonObject["ClientId"].toString())
                {

                    theClient.setIsControlling(true);

                    if(tcpPackager::getFountainID() == 0)
                    {
                     sendTcpPackageToClients(tcpPackager::AnswerWhoIsControlling(theClient.getClientId(), theClient.getClientType()));
                    }

                }
                else
                {
                    theClient.setIsControlling(false);
                }
            }
        }
        else if(theCommand == "updateSecretKey")
        {
               tcpPackager::setSecretKey(requestJsonObject["key"].toString().toUtf8());
               emit sendTcpPackageToClients(tcpPackager::fountainCurrentPlayingProgram("updateSecretKey"));
        }
    }

    if(tcpSocketList.last()->bytesAvailable()>0)    emit stillAvailable();

//    if(isDisconnecting)
//    {
//        tcpSocketList.removeAll(theTCPClient);
//    }
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
    foreach (clientTcpSocket theClient, clientList) {

        if(theClient.getClientId() == id)
        {
            return true;
        }

    }

    return false;
}
