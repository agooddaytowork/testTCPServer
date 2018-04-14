#include "clienttcpsocket.h"

clientTcpSocket::clientTcpSocket(QTcpSocket *parent): QTcpSocket(parent), m_UUID(""), m_Type(0),m_isControlling(false), m_isMainController(false)
{

}

void clientTcpSocket::setUUID(const QString &id)
{
 if(id != m_UUID)
 {
     m_UUID =id;
 }
}

QString  clientTcpSocket::getUUID() const
{
    return m_UUID;
}

void clientTcpSocket::setClientType(const int &type)
{
    if(m_Type != type)
    {
        m_Type = type;
    }
}

int clientTcpSocket::getClientType() const
{
    return m_Type;
}


