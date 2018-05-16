#include "communicationchecker.h"

#define folderPath "/home/orange/scripts/"
#define communicationCheckerDebug (1)

#define requestTimeOutInterval 30000 // 30s
#define requestTimeOutMaxCounter 3
#define idleTimeOutInterval 10000 // 2 phut
#define deviceTimeOutCounterThreshold 6

#define checkActualFountainDeviceOnlineStatus (0)

#include <QEventLoop>

communicationChecker::communicationChecker(QObject *parent): QObject(parent), m_currentState(1), m_timeoutCounter(0), m_timeOutTimer(new QTimer(this)), m_fountainTimeOutCounter(0)
{
    QObject::connect(this,SIGNAL(stateChanged(int)),this,SLOT(stateChangedHandler(int)));
    m_timeOutTimer->setSingleShot(true);

    QObject::connect(m_timeOutTimer, &QTimer::timeout, this,&communicationChecker::timerTimeoutHandler);

    //    m_HandShakeTimer->setSingleShot(false);
    //    QObject::connect(m_HandShakeTimer,&QTimer::timeout,this,&communicationChecker::S6HandShake);
    //    m_HandShakeTimer->setInterval(handShakeInterval);
    //    m_HandShakeTimer->start();

}

void communicationChecker::start()
{
    // start state machine
    emit stateChanged(1);
}

void communicationChecker::S1CheckWifi()
{
    QProcess process;
    process.start((QString)folderPath + "getWifiIP.sh");
    process.waitForFinished();
    QString output = process.readAllStandardOutput();

#if communicationCheckerDebug
    qDebug() << output;
    QString err = process.readAllStandardError();
    qDebug() << err;
#endif

    if(output == "" || output.isEmpty())
    {
        emit stateChanged(2); // request Wifi Info from fountain
    }
    else
    {
        if(isIPValid(output))
        {
            m_timeoutCounter = 0;
            emit stateChanged(5); // Wifi OK, go to idle state
        }
        else
        {
            emit stateChanged(2); // request Wifi Info from fountain
        }
    }

}

void communicationChecker::S2RequestWifiInfo()
{
    emit requestWifiInfo(fountainSerialPackager::fountainDeviceRequestWifi());

    m_timeOutTimer->stop();
    m_timeOutTimer->setInterval(requestTimeOutInterval);
    m_timeOutTimer->start();


}

void communicationChecker::S3ConnectWifi()
{
    QEventLoop loop;
    QProcess *proc = new QProcess();
    connect(proc, SIGNAL(finished(int)), &loop, SLOT(quit()));
    proc->setEnvironment(QStringList() << "PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:");
    proc->start("/bin/bash", QStringList() << (QString)folderPath + "connectwifi.sh" << m_wifiID << m_wifiPassword);
    loop.exec();

#if communicationCheckerDebug
    QString output = proc->readAllStandardOutput();
    qDebug() << output;
    QString err = proc->readAllStandardError();
    qDebug() << err;
#endif
    emit stateChanged(1); // check wifi again

}

void communicationChecker::S4RequestUserInputForWifi()
{

    m_timeoutCounter = 0;
    //    emit requestUserInputForWifiInfo(fountainSerialPackager::fountainDeviceRequestUserInputForWifi());
    emit requestWifiInfo(fountainSerialPackager::fountainDeviceRequestWifi());
    m_timeOutTimer->stop();
    m_timeOutTimer->setInterval(requestTimeOutInterval);
    m_timeOutTimer->start();
}

void communicationChecker::S5Idle()
{
    m_timeoutCounter = 0;
    emit wifiOK(fountainSerialPackager::fountainDeviceWifiOK());

    m_timeOutTimer->stop();
    m_timeOutTimer->setInterval(idleTimeOutInterval);
    m_timeOutTimer->start();
}

void communicationChecker::S6HandShake()
{
    emit wifiOK(fountainSerialPackager::fountainDeviceHandshake());
}

void communicationChecker::stateChangedHandler(const int &state)
{
    m_currentState = state;

#if communicationCheckerDebug
    qDebug() << "current State: " + QString::number(m_currentState);
#endif
    switch (m_currentState) {
    case 1:
        S1CheckWifi();

        break;
    case 2:
        if(m_timeoutCounter == requestTimeOutMaxCounter)
        {
            emit stateChanged(4);
        }
        else
        {
            S2RequestWifiInfo();
        }
        break;
    case 3:
        S3ConnectWifi();
        break;
    case 4:

        S4RequestUserInputForWifi();
        break;
    case 5:
        S5Idle();
        break;
    }
}

void communicationChecker::in(const QByteArray &data)
{

    fountainSerialPackager aPackage(data);

    if(aPackage.isPackageValid())
    {
        if(aPackage.getOpCode() == m_OpCode_FountainDeviceInternalHandShake) // HandShake
        {
            quint8 statusCode = aPackage.getStatusCode();
            if(statusCode == m_Status_RequestWifi)
            {
                m_wifiID = aPackage.getWifiName();
                m_wifiPassword = aPackage.getWifiPassword();
#if communicationCheckerDebug
                qDebug() << "wifi Name: " + m_wifiID;
                qDebug() << "wifi Password " + m_wifiPassword;
#endif
                emit stateChanged(3);
            }
#if checkActualFountainDeviceOnlineStatus
         emit fountainStatus(true);
#endif

            m_fountainTimeOutCounter = 0;
        }
        else
        {
            toFountainServer(data);
        }
    }
    else
    {
        emit stateChanged(2);
    }

}


bool communicationChecker::isIPValid( const QString &IP)
{

    QRegExp param("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3})");

    QString data = IP;

    data.remove(data.count()-1,1);
    int pos =0;

    QRegExpValidator checker(param, 0);

    if(checker.validate( data,pos) == QValidator::Acceptable)
    {
        return true;
    }

    return false;
}

void communicationChecker::timerTimeoutHandler()
{
    if(m_fountainTimeOutCounter >= deviceTimeOutCounterThreshold)
    {

#if checkActualFountainDeviceOnlineStatus
        emit fountainStatus(false);

#endif
    }

    if(m_currentState == 2)
    {
        m_timeoutCounter++;
        emit stateChanged(2);
    }
    else if(m_currentState == 4)
    {
        m_timeoutCounter++;
        emit stateChanged(4);
    }
    else if(m_currentState == 5)
    {
        m_timeoutCounter++;
#if !checkActualFountainDeviceOnlineStatus
         emit fountainStatus(true);
#endif
        emit stateChanged(1);
    }

    m_fountainTimeOutCounter++;
}
