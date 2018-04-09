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
    tcpPackager();
    static QByteArray playProgram(const QString &programName, const QByteArray &Program);
    static QByteArray isFountainOnline();
    static QByteArray fountainResponse(const QByteArray &response);
    static QByteArray fountainStatus(const bool &status);
    static QByteArray fountainCurrentPlayingProgram(const QString &program);

    static bool isPackageValid(const QByteArray &input);
    static QJsonObject packageToJson(const QByteArray &input);


};

#endif // TCPPACKAGER_H
