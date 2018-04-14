#ifndef CLIENTTCPSOCKET_H
#define CLIENTTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>

class clientTcpSocket: public QTcpSocket
{

    Q_OBJECT

    QString m_UUID;
    int     m_Type;
    bool m_isControlling;
    bool m_isMainController;

public:
    clientTcpSocket(QTcpSocket *parent = nullptr);
    void setUUID(const QString &id);
    QString getUUID() const;
    void setClientType(const int &type);
    int getClientType() const;

    bool isClientExist(const QString &id);
};

#endif // CLIENTTCPSOCKET_H
