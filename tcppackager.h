#ifndef TCPPACKAGER_H
#define TCPPACKAGER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QCryptographicHash>
#include <QDateTime>

#define theSecretKey "fountainController"
class tcpPackager
{


public:
    static int m_FountainID;
    static QString m_clientId;
#if defined(Q_OS_MAC) || defined(Q_OS_WIN) || defined(Q_OS_UNIX)
    static constexpr int m_clientType = 0;
#else
    constexpr int m_clientType = 1;
#endif
    tcpPackager();
    static QString generateClientId();
    static QByteArray playProgram(const QString &programName, const QByteArray &Program);
    static QByteArray isFountainOnline();
    static QByteArray fountainResponse(const QByteArray &response);
    static QByteArray fountainStatus(const bool &status);
    static QByteArray fountainCurrentPlayingProgram(const QString &program);

    static bool isPackageValid(const QByteArray &input);
    static QJsonObject packageToJson(const QByteArray &input);
    static QByteArray AnswerWhoIsControlling(const QString &clientId, const int &clientType);
    static QByteArray AskWhoIsControlling();
    static void setFountainID(const int &id);



};

#endif // TCPPACKAGER_H
