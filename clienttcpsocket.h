#ifndef CLIENTTCPSOCKET_H
#define CLIENTTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>

class clientTcpSocket
{

    QString m_ClientId;
    int     m_Type;
    bool m_isControlling;
    bool m_isMainController;
public:

    clientTcpSocket();
    void setClientId(const QString &id);
    QString getClientId() const;
    void setClientType(const int &type);
    int getClientType() const;


    bool isControlling() const;
    void setIsControlling(const bool &control);
};

#endif // CLIENTTCPSOCKET_H
