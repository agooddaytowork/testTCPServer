#ifndef FOUNTAINSERVER_H
#define FOUNTAINSERVER_H

#include <QTcpServer>
#include <QNetworkSession>
#include <QDebug>
#include <QDataStream>
#include <QList>

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

      QList<QTcpSocket*> clientList;

      QString m_currentProgram;

public:
    fountainServer(QObject *parent = nullptr);

public slots:

    void newConnectionHandler();
    void fromSerialHandler(const QByteArray &data);

private slots:
    void readyReadHandler();


signals:

    void toSerial(QByteArray);

};

#endif // FOUNTAINSERVER_H
