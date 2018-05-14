#include "communicationchecker.h"

#define folderPath "/home/orange/scripts/"
#define communicationCheckerDebug (1)

#define requestTimeOutInterval 30000 // 30s
#define requestTimeOutMaxCounter 3
#define idleTimeOutInterval 120000 // 2 phut

communicationChecker::communicationChecker(QObject *parent): QObject(parent), m_currentState(1), m_timeoutCounter(0), m_timeOutTimer(new QTimer(this))
{
    QObject::connect(this,SIGNAL(stateChanged(int)),this,SLOT(stateChangedHandler(int)));
    m_timeOutTimer->setSingleShot(true);

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
#if communicationCheckerDebug
    qDebug() << "go to sate 2";
#endif
        emit stateChanged(2); // request Wifi Info from fountain
    }
    else
    {
        if(isIPValid(output))
        {
#if communicationCheckerDebug
    qDebug() << "go to sate 5";
#endif
            m_timeoutCounter = 0;
            emit stateChanged(5); // Wifi OK, go to idle state
        }
        else
        {
#if communicationCheckerDebug
    qDebug() << "go to sate 2";
#endif
            emit stateChanged(2); // request Wifi Info from fountain
        }
    }

}

void communicationChecker::S2RequestWifiInfo()
{
    emit requestWifiInfo(fountainSerialPackager::fountainDeviceRequestWifi());

    m_timeOutTimer->stop();

    m_timeOutTimer->setInterval(requestTimeOutInterval);
    QObject::connect(m_timeOutTimer,&QTimer::timeout,[=](){

        if(m_currentState == 2)
        {
            m_timeoutCounter++;
            emit stateChanged(2);
        }
    });
    m_timeOutTimer->start();




}

void communicationChecker::S3ConnectWifi()
{



    QProcess process;
    process.start((QString)folderPath + "connectwifi.sh " + m_wifiID +" " + m_wifiPassword);
    process.waitForFinished();

    emit stateChanged(1); // check wifi again


}

void communicationChecker::S4RequestUserInputForWifi()
{

    m_timeoutCounter = 0;
    emit requestUserInputForWifiInfo(fountainSerialPackager::fountainDeviceRequestUserInputForWifi());

    m_timeOutTimer->stop();

    m_timeOutTimer->setInterval(requestTimeOutInterval);
    QObject::connect(m_timeOutTimer,&QTimer::timeout,[=](){

        if(m_currentState == 4)
        {
            m_timeoutCounter++;
            emit stateChanged(4);
        }
    });
    m_timeOutTimer->start();
}

void communicationChecker::S5Idle()
{
    emit wifiOK(fountainSerialPackager::fountainDeviceWifiOK());

    m_timeOutTimer->stop();

    m_timeOutTimer->setInterval(idleTimeOutInterval);
    QObject::connect(m_timeOutTimer,&QTimer::timeout,[=](){

        if(m_currentState == 5)
        {
            m_timeoutCounter++;
            emit stateChanged(1);
        }
    });
    m_timeOutTimer->start();
}

void communicationChecker::stateChangedHandler(const int &state)
{
    m_currentState = state;

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
        m_wifiID = aPackage.getWifiName();
        m_wifiPassword = aPackage.getWifiPassword();
        emit stateChanged(3);
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
