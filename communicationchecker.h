#ifndef COMMUNICATIONCHECKER_H
#define COMMUNICATIONCHECKER_H
#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QTimer>
#include <QRegExpValidator>
#include <QValidator>
#include "fountainserialpackager.h"

class communicationChecker: public QObject
{

    Q_OBJECT
    int m_currentState;
    int m_timeoutCounter;
    int m_fountainTimeOutCounter;
    QString m_wifiID;
    QString m_wifiPassword;
    QTimer *m_timeOutTimer;
//    QTimer *m_HandShakeTimer;

    void S1CheckWifi();
    void S2RequestWifiInfo();
    void S3ConnectWifi();
    void S4RequestUserInputForWifi();
    void S5Idle();
    void S6HandShake();

    bool isIPValid(const QString &IP);

private slots:

    void stateChangedHandler(const int &state);
    void timerTimeoutHandler();

public slots:
    void in(const QByteArray &data);

public:
    communicationChecker(QObject *parent = nullptr);
    void start();

signals:

    void stateChanged(const int &state);
    void requestWifiInfo( const QByteArray &data);
    void requestUserInputForWifiInfo(const QByteArray &data);
    void wifiOK(const QByteArray &data);
    void wifiNotOK(const QByteArray &data);

    void fountainStatus(const bool &status);


    // for fountainServer
    void fountainOnline(const bool &status);


};

#endif // COMMUNICATIONCHECKER_H
