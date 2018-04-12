#ifndef FOUNTAINSERVER_H
#define FOUNTAINSERVER_H

#include <QTcpServer>
#include <QNetworkSession>
#include <QDebug>
#include <QDataStream>
#include <QList>
#include "clienttcpsocket.h"

#define fountainServerDebug (1)
#define fountainServerForwarder (0)
#define fountainDeviceMode (1)


class fountainServer: public QObject
{
    Q_OBJECT


    QTcpServer *tcpServer = nullptr;
    QTcpSocket *tcpSocket = nullptr;
    int m_Serverport;
      QDataStream in;

      QList<clientTcpSocket*> clientList;

      QString m_currentProgram;
      bool m_isFountainOnline;

public:
    fountainServer(QObject *parent = nullptr);
    void setIsFountainOnline(const bool &input);
    void informClientFountainStatus();
    void informClientCurrentPlayingProgram();

    void sendTcpPackageToClients(const QByteArray &tcpPackage);

public slots:

    void newConnectionHandler();
    void fromSerialHandler(const QByteArray &data);
    void serialConnectedHandler();
    void serialDisconnectedHandler();

private slots:
    void readyReadHandler();
//    void clientDisconnectionHandler();


signals:

    void toSerial(QByteArray);

};

#endif // FOUNTAINSERVER_H
