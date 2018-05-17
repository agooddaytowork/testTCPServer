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
#include <QTimer>



class fountainServer: public QObject
{
    Q_OBJECT


    QTcpServer *tcpServer = nullptr;
    QTcpSocket *tcpSocket = nullptr;
    int m_Serverport;
    //    QDataStream in;

    QList<QTcpSocket*> tcpSocketList;
    QList<clientTcpSocket> clientList;
    QDataStream in;
    QString m_currentProgram;
    QString m_currentClientAddress;
    bool m_isFountainOnline;
    bool isClientExist(const QString &id);


public:
    fountainServer( QObject *parent = nullptr);
    void setIsFountainOnline(const bool &input);
    void informClientFountainStatus();
    void informClientCurrentPlayingProgram();

    void sendTcpPackageToClients(const QByteArray &tcpPackage);

public slots:

    void newConnectionHandler();
    void fromSerialHandler(const QByteArray &data);
    void serialConnectedHandler();
    void serialDisconnectedHandler();
    void clientDisconnectedHandler();

    void fromCommunicationCheckerFountainStatusHandler(const bool &status);
    void socketErrorHandler();


private slots:
    void readyReadHandler();
    //    void clientDisconnectionHandler();


signals:

    void toSerial(QByteArray);
    void stillAvailable();

};

#endif // FOUNTAINSERVER_H
