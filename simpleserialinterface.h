#ifndef SIMPLESERIALINTERFACE_H
#define SIMPLESERIALINTERFACE_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMap>
#include <QDataStream>

class SimpleSerialInterface: public QObject
{
    Q_OBJECT

QSerialPort mSerialPort;

    QByteArray mPortName;
    quint16 mBaudrate;
    bool mIsWriting;
    QList<QByteArray> toWriteList;
    QDataStream dataStreamInput;

signals:
    void connected();
    void disconnected();
    void errorsOcurred();
    void output(const QByteArray &data);
    void writeToDeviceRequest(const QList<QByteArray> &buffer);

public:
    explicit SimpleSerialInterface(QObject * parent = nullptr);
    QMap<uint32_t, double> Data;

public slots:
    Q_INVOKABLE void setPortName(const QByteArray &name);
    Q_INVOKABLE void setBaudRate(const quint16 &baudrate);
    Q_INVOKABLE bool connect();
    Q_INVOKABLE bool disconnect();
     void input(const QByteArray &input);
     void writeToDevice(const QList<QByteArray> &buffer);

private slots:
    void receivedDataHandler();
    void serialPortErrorHandler( const QSerialPort::SerialPortError &error);

};

#endif // SIMPLESERIALINTERFACE_H
