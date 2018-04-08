#ifndef SIMPLESERIALINTERFACE_H
#define SIMPLESERIALINTERFACE_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include "shared/commonthings.h"
#include <QMap>

class SimpleSerialInterface: public QObject
{
    Q_OBJECT

QSerialPort mSerialPort;

    QByteArray mPortName;
    quint16 mBaudrate;

signals:
    void connected();
    void disconnected();
    void errorsOcurred();
    void output(const QByteArray &data);

public:
    explicit SimpleSerialInterface(QObject * parent = nullptr);
    QMap<uint32_t, double> Data;

public slots:
    Q_INVOKABLE void setPortName(const QByteArray &name);
    Q_INVOKABLE void setBaudRate(const quint16 &baudrate);
    Q_INVOKABLE bool connect();
    Q_INVOKABLE bool disconnect();
    Q_INVOKABLE void input(const QByteArray &input);

private slots:
    void receivedDataHandler();
    void serialPortErrorHandler( const QSerialPort::SerialPortError &error);

};

#endif // SIMPLESERIALINTERFACE_H
