#ifndef CLIENTTCPSOCKET_H
#define CLIENTTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>

class clientTcpSocket: public QTcpSocket
{

    Q_OBJECT

    QString m_UUID;
    int     m_Type;

public:
    clientTcpSocket(QTcpSocket *parent = nullptr);
    void setUUID(const QString &id);
    QString getUUID() const;
    void setClientType(const int &type);
    int getClientType() const;
};

#endif // CLIENTTCPSOCKET_H
