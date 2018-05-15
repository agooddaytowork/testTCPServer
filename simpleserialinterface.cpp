#include "simpleserialinterface.h"
#include <QDebug>
#include <QElapsedTimer>
#include "fountainserialpackager.h"

SimpleSerialInterface::SimpleSerialInterface(QObject *parent): mSerialPort(parent), mPortName("COM1"), mBaudrate(9600), mIsWriting(false),QObject(parent)
{
    Data.clear();

//    dataStreamInput.setDevice(mSerialPort);
    QObject::connect(&mSerialPort,SIGNAL(readyRead()),this,SLOT(receivedDataHandler()));
    QObject::connect(&mSerialPort,SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(serialPortErrorHandler(QSerialPort::SerialPortError)));
    QObject::connect(this,SIGNAL(writeToDeviceRequest(QList<QByteArray>)),this,SLOT(writeToDevice(QList<QByteArray>)));

}

void SimpleSerialInterface::setPortName(const QByteArray &name)
{
    if(mPortName != name)
    {
        mPortName = name;
        if(mSerialPort.isOpen())
        {
            this->disconnect();
        }
    }
}

void SimpleSerialInterface::setBaudRate(const quint16 &baudrate)
{
    if(mBaudrate != baudrate)
    {
        mBaudrate = baudrate;
        if(mSerialPort.isOpen())
        {
            this->disconnect();
        }
    }
}

bool SimpleSerialInterface::connect()
{
    if(mSerialPort.isOpen())
    {
        this->disconnect();
    }

    mSerialPort.setPortName(mPortName);
    mSerialPort.setBaudRate(mBaudrate);
    if(!mSerialPort.open(QIODevice::ReadWrite))
    {
        emit errorsOcurred();
        return false;
    }
    else
    {
        emit connected();

    }
    return true;
}

bool SimpleSerialInterface::disconnect()
{

    if(mSerialPort.disconnect())
    {
        mSerialPort.close();
        emit disconnected();
        return true;
    }
    return false;
}

void SimpleSerialInterface::input(const QByteArray &input)
{
    //    qDebug() << input;

    // add Data to Buffer


    static QList<QByteArray> buffer;

    if(!mIsWriting)
    {
        buffer.append(input);

        emit writeToDeviceRequest(buffer);

        buffer.clear();
    }
    else
    {
        buffer.append(input);
    }
//    if(mSerialPort.isOpen())
//    {
//        qDebug() << "writing to Serial";
//        mSerialPort.write(input);
//    }

}

void SimpleSerialInterface::writeToDevice(const QList<QByteArray> &buffer)
{
    mIsWriting = true;
    QElapsedTimer timeOutTimer;
     timeOutTimer.start();
    for(int i = 0; i < buffer.count(); i++)
    {
                qDebug() << "writing to Serial";
                mSerialPort.write(buffer.at(i));
                timeOutTimer.restart();
                while (timeOutTimer.elapsed() < 1000) {

                }

    }
    mIsWriting = false;

}

void SimpleSerialInterface::receivedDataHandler()
{
    QByteArray tmpdata;
    static QByteArray data;

    tmpdata = mSerialPort.readAll();
    data +=tmpdata;

    fountainSerialPackager aPackage(data);

    if(aPackage.isPackageValid())
    {
        emit output(data);
        input(data);
        data.clear();

    }

}


void SimpleSerialInterface::serialPortErrorHandler(const QSerialPort::SerialPortError &error)
{

    qDebug() << "Serial ERRORRR";
}
