#include "tcppackager.h"
#include <QDebug>
#include <QSettings>

tcpPackager::tcpPackager()
{

}

int tcpPackager::m_FountainID = 0;

QString tcpPackager::m_clientId = tcpPackager::generateClientId();
QByteArray tcpPackager::m_scecretKey = "fountainController";
QString tcpPackager::m_QSettingPath = "";
bool tcpPackager::m_isMasterKeySet = false;

bool tcpPackager::isMasterkey()
{
    return m_isMasterKeySet;
}

void tcpPackager::setQSettingPath(const QString &path)
{
    m_QSettingPath = path;

}

void tcpPackager::setSecretKey(const QByteArray &newKey)
{
    m_scecretKey = newKey;

    QSettings daiphunSetting(m_QSettingPath, QSettings::IniFormat);
    daiphunSetting.setValue("secretKey",m_scecretKey);

    qDebug() << "newSecretKey " + newKey;
}


QString tcpPackager::generateClientId()
{
    return (QString) QCryptographicHash::hash("clientId" + QByteArray::number(QDateTime::currentMSecsSinceEpoch()),QCryptographicHash::Sha256);
}



QByteArray tcpPackager::playProgram(const QString &programName, const QByteArray &Program)
{
    delayPerElectricalBox();
    QJsonObject thePackage;

    qint64 theTimeStamp = QDateTime::currentMSecsSinceEpoch();
    QByteArray time;
    time.append(QString::number(theTimeStamp));
    thePackage.insert("fountainId", m_FountainID);
    thePackage.insert("ClientId", m_clientId);
    thePackage.insert("ClientType", m_clientType);
    thePackage.insert("UUID", (QString) QCryptographicHash::hash(m_scecretKey + time, QCryptographicHash::Sha256));
    thePackage.insert("TimeStamp",QString::number(theTimeStamp) );
    thePackage.insert("Command", "playProgram");
    thePackage.insert("ProgramName", programName);
    thePackage.insert("ProgramData", (QString) Program.toHex());

    QJsonDocument aDocument(thePackage);
    return aDocument.toJson();
}


bool tcpPackager::isPackageValid(const QByteArray &input)
{
    delayPerElectricalBox();
    QJsonObject thePackage = packageToJson(input);

    if(!thePackage.isEmpty())
    {
        if(thePackage.contains("masterKey"))
        {
            if(thePackage["masterKey"].toString() == masterKey)
            {
                m_isMasterKeySet = true;
                return true;
            }
        }
        QByteArray time;
        time.append(thePackage["TimeStamp"].toString());
        QString UUID = QCryptographicHash::hash(m_scecretKey + time, QCryptographicHash::Sha256);

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
    delayPerElectricalBox();
    QJsonObject thePackage;

    qint64 theTimeStamp = QDateTime::currentMSecsSinceEpoch();
    QByteArray time;
    time.append(QString::number(theTimeStamp));
    thePackage.insert("fountainId", m_FountainID);
    thePackage.insert("ClientId", m_clientId);
    thePackage.insert("ClientType", m_clientType);
    thePackage.insert("UUID", (QString) QCryptographicHash::hash(m_scecretKey + time, QCryptographicHash::Sha256));
    thePackage.insert("TimeStamp",QString::number(theTimeStamp) );
    thePackage.insert("Command", "isFountainOnline");

    QJsonDocument aDocument(thePackage);
    return aDocument.toJson();
}

QByteArray tcpPackager::fountainResponse(const QByteArray &response)
{
    delayPerElectricalBox();
    QJsonObject thePackage;

    qint64 theTimeStamp = QDateTime::currentMSecsSinceEpoch();
    QByteArray time;
    time.append(QString::number(theTimeStamp));
    thePackage.insert("fountainId", m_FountainID);
    thePackage.insert("UUID", (QString) QCryptographicHash::hash(m_scecretKey + time, QCryptographicHash::Sha256));
    thePackage.insert("TimeStamp",QString::number(theTimeStamp) );
    thePackage.insert("Command", "fountainResponse");
    thePackage.insert("Data", (QString) response.toHex());
    QJsonDocument aDocument(thePackage);
    return aDocument.toJson();
}

QByteArray tcpPackager::fountainStatus(const bool &status)
{
    delayPerElectricalBox();
    QJsonObject thePackage;

    qint64 theTimeStamp = QDateTime::currentMSecsSinceEpoch();
    QByteArray time;
    time.append(QString::number(theTimeStamp));
    thePackage.insert("fountainId", m_FountainID);
    thePackage.insert("UUID", (QString) QCryptographicHash::hash(m_scecretKey + time, QCryptographicHash::Sha256));
    thePackage.insert("TimeStamp",QString::number(theTimeStamp) );
    thePackage.insert("Command", "fountainStatus");
    thePackage.insert("Data", status);
    QJsonDocument aDocument(thePackage);
    return aDocument.toJson();
}


QByteArray tcpPackager::fountainCurrentPlayingProgram(const QString &program)
{
    delayPerElectricalBox();
    QJsonObject thePackage;

    qint64 theTimeStamp = QDateTime::currentMSecsSinceEpoch();
    QByteArray time;
    time.append(QString::number(theTimeStamp));
    thePackage.insert("fountainId", m_FountainID);
    thePackage.insert("UUID", (QString) QCryptographicHash::hash(m_scecretKey + time, QCryptographicHash::Sha256));
    thePackage.insert("TimeStamp",QString::number(theTimeStamp) );
    thePackage.insert("Command", "fountainCurrentPlayingProgram");
    thePackage.insert("Data", program);
    QJsonDocument aDocument(thePackage);
    return aDocument.toJson();
}

QByteArray tcpPackager::AnswerWhoIsControlling(const QString &clientId, const int &clientType)
{
    delayPerElectricalBox();
    QJsonObject thePackage;

    qint64 theTimeStamp = QDateTime::currentMSecsSinceEpoch();
    QByteArray time;
    time.append(QString::number(theTimeStamp));
    thePackage.insert("fountainId", m_FountainID);
    thePackage.insert("UUID", (QString) QCryptographicHash::hash(m_scecretKey + time, QCryptographicHash::Sha256));
    thePackage.insert("TimeStamp",QString::number(theTimeStamp) );
    thePackage.insert("Command", "whoIsControlling");
    thePackage.insert("ClientId", clientId);
    thePackage.insert("ClientType", clientType);
    QJsonDocument aDocument(thePackage);
    return aDocument.toJson();
}

QByteArray tcpPackager::AskWhoIsControlling()
{
    delayPerElectricalBox();
    QJsonObject thePackage;
    qint64 theTimeStamp = QDateTime::currentMSecsSinceEpoch();
    QByteArray time;
    time.append(QString::number(theTimeStamp));
    thePackage.insert("fountainId", m_FountainID);
    thePackage.insert("ClientId", m_clientId);
    thePackage.insert("ClientType", m_clientType);
    thePackage.insert("UUID", (QString) QCryptographicHash::hash(m_scecretKey + time, QCryptographicHash::Sha256));
    thePackage.insert("TimeStamp",QString::number(theTimeStamp) );
    thePackage.insert("Command", "whoIsControlling");

    QJsonDocument aDocument(thePackage);
    return aDocument.toJson();
}

void tcpPackager::setFountainID(const int &id)
{
    m_FountainID = id;

    qDebug() << "Fountain ID: " + QString::number(m_FountainID);
}

int tcpPackager::getFountainID()
{
    return m_FountainID;
}

void tcpPackager::delayPerElectricalBox()
{
    QElapsedTimer timer;
        timer.start();

        while (!timer.hasExpired((tcpPackager::getFountainID()+1) * 15));

}

