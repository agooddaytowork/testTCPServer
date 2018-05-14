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
    QString m_wifiID;
    QString m_wifiPassword;
    QTimer *m_timeOutTimer;

    void S1CheckWifi();
    void S2RequestWifiInfo();
    void S3ConnectWifi();
    void S4RequestUserInputForWifi();
    void S5Idle();

    bool isIPValid(const QString &IP);

private slots:

    void stateChangedHandler(const int &state);

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


};

#endif // COMMUNICATIONCHECKER_H
