#include <QCoreApplication>
#include "fountainserver.h"
#include "simpleserialinterface.h"
#include <QSerialPortInfo>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    fountainServer aServer;
    SimpleSerialInterface asimpleSerialInterface;


    QObject::connect(&aServer,&fountainServer::toSerial,&asimpleSerialInterface,&SimpleSerialInterface::input);
    QObject::connect(&asimpleSerialInterface,&SimpleSerialInterface::connected,[=](){

        qDebug() << "Serial Connected";

    });
    QObject::connect(&asimpleSerialInterface,&SimpleSerialInterface::errorsOcurred,[=](){

        qDebug() << "Serial Error Occured";
    });
    QObject::connect(&asimpleSerialInterface,&SimpleSerialInterface::connected,&aServer,&fountainServer::serialConnectedHandler);
    QObject::connect(&asimpleSerialInterface,&SimpleSerialInterface::disconnected,&aServer,&fountainServer::serialDisconnectedHandler);
    QObject::connect(&asimpleSerialInterface,&SimpleSerialInterface::errorsOcurred,&aServer,&fountainServer::serialDisconnectedHandler);



    QObject::connect(&asimpleSerialInterface,&SimpleSerialInterface::output,&aServer,&fountainServer::fromSerialHandler);

    foreach (QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        qDebug() << port.portName();
        if(port.portName() == "ttyS0")
        {
            asimpleSerialInterface.setBaudRate(9600);
            asimpleSerialInterface.setPortName("ttyS0");

            if(asimpleSerialInterface.connect())
            {
                qDebug() << "Serial connected";

            }
            else
            {
                qDebug() << "Serial failed to Connect";
            }
        }

    }


    return a.exec();
}
