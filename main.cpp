#include <QCoreApplication>
#include "fountainserver.h"
#include "simpleserialinterface.h"
#include <QSerialPortInfo>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    fountainServer aServer;

    foreach (QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        qDebug() << port.portName();
    }
    return a.exec();
}
