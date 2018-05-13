#ifndef COMMUNICATIONCHECKER_H
#define COMMUNICATIONCHECKER_H
#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QTimer>
#include <QRegExpValidator>
#include <QValidator>

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
    void in(const QHash<int, QVariant> &data);
public:
    communicationChecker(QObject *parent = nullptr);
    void start();

signals:

    void stateChanged(const int &state);
    void requestWifiInfo();
    void requestUserInputForWifiInfo();
    void wifiOK();


};

#endif // COMMUNICATIONCHECKER_H
