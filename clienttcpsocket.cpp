#include "clienttcpsocket.h"

clientTcpSocket::clientTcpSocket(): m_Type(0),m_isControlling(false), m_isMainController(false), m_ClientId("")
{

}

void clientTcpSocket::setClientId(const QString &id)
{

     m_ClientId = id;

}

QString  clientTcpSocket::getClientId() const
{
    return m_ClientId;
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

bool clientTcpSocket::isControlling() const
{
    return m_isControlling;
}

void clientTcpSocket::setIsControlling(const bool &control)
{
    if(m_isControlling != control)
    {
        m_isControlling = control;
    }
}

QString clientTcpSocket::getClientAddress() const
{
    return m_Address;
}

void clientTcpSocket::setClientAddress(const QString &address)
{
    m_Address = address;
}

