#include "tcppackager.h"

tcpPackager::tcpPackager()
{

}


QByteArray tcpPackager::playProgram(const QString &programName, const QByteArray &Program)
{
    QJsonObject thePackage;

    qint64 theTimeStamp = QDateTime::currentMSecsSinceEpoch();
    QByteArray time;
    time.append(QString::number(theTimeStamp));
    thePackage.insert("UUID", (QString) QCryptographicHash::hash(theSecretKey + time, QCryptographicHash::Sha256));
    thePackage.insert("TimeStamp",QString::number(theTimeStamp) );
    thePackage.insert("Command", "playProgram");
    thePackage.insert("ProgramName", programName);
    thePackage.insert("ProgramData", (QString) Program.toHex());

    QJsonDocument aDocument(thePackage);
            return aDocument.toJson();
}


bool tcpPackager::isPackageValid(const QByteArray &input)
{
    QJsonObject thePackage = packageToJson(input);

    if(!thePackage.isEmpty())
    {
        QByteArray time;
        time.append(thePackage["TimeStamp"].toString());
        QString UUID = QCryptographicHash::hash(theSecretKey + time, QCryptographicHash::Sha256);

        if (UUID == thePackage["UUID"].toString())
        {
            return true;
        }
    }
    return false;
}

QJsonObject tcpPackager::packageToJson(const QByteArray &input)
{
    QJsonDocument aDocument(QJsonDocument::fromJson(input));
    return aDocument.object();
}


QByteArray tcpPackager::isFountainOnline()
{
    QJsonObject thePackage;

    qint64 theTimeStamp = QDateTime::currentMSecsSinceEpoch();
    QByteArray time;
    time.append(QString::number(theTimeStamp));
    thePackage.insert("UUID", (QString) QCryptographicHash::hash(theSecretKey + time, QCryptographicHash::Sha256));
    thePackage.insert("TimeStamp",QString::number(theTimeStamp) );
    thePackage.insert("Command", "isFountainOnline");

    QJsonDocument aDocument(thePackage);
            return aDocument.toJson();
}

QByteArray tcpPackager::fountainResponse(const QByteArray &response)
{
    QJsonObject thePackage;

    qint64 theTimeStamp = QDateTime::currentMSecsSinceEpoch();
    QByteArray time;
    time.append(QString::number(theTimeStamp));
    thePackage.insert("UUID", (QString) QCryptographicHash::hash(theSecretKey + time, QCryptographicHash::Sha256));
    thePackage.insert("TimeStamp",QString::number(theTimeStamp) );
    thePackage.insert("Command", "fountainResponse");
    thePackage.insert("Data", (QString) response.toHex());
    QJsonDocument aDocument(thePackage);
            return aDocument.toJson();
}

QByteArray tcpPackager::fountainStatus(const bool &status)
{
    QJsonObject thePackage;

    qint64 theTimeStamp = QDateTime::currentMSecsSinceEpoch();
    QByteArray time;
    time.append(QString::number(theTimeStamp));
    thePackage.insert("UUID", (QString) QCryptographicHash::hash(theSecretKey + time, QCryptographicHash::Sha256));
    thePackage.insert("TimeStamp",QString::number(theTimeStamp) );
    thePackage.insert("Command", "fountainStatus");
    thePackage.insert("Data", status);
    QJsonDocument aDocument(thePackage);
            return aDocument.toJson();
}


QByteArray tcpPackager::fountainCurrentPlayingProgram(const QString &program)
{
    QJsonObject thePackage;

    qint64 theTimeStamp = QDateTime::currentMSecsSinceEpoch();
    QByteArray time;
    time.append(QString::number(theTimeStamp));
    thePackage.insert("UUID", (QString) QCryptographicHash::hash(theSecretKey + time, QCryptographicHash::Sha256));
    thePackage.insert("TimeStamp",QString::number(theTimeStamp) );
    thePackage.insert("Command", "fountainCurrentPlayingProgram");
    thePackage.insert("Data", program);
    QJsonDocument aDocument(thePackage);
            return aDocument.toJson();
}
