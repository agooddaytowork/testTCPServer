#include <QCoreApplication>
#include "fountainserver.h"
#include "simpleserialinterface.h"
#include "communicationchecker.h"
#include <QSerialPortInfo>
#include <QSettings>
#include <QDebug>
#include "tcppackager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // setup Application Setting
    qDebug() << QCoreApplication::applicationDirPath()+"/daiphunnuoc.ini";

    QSettings daiphunSetting(QCoreApplication::applicationDirPath()+"/daiphunnuoc.ini", QSettings::IniFormat);

    if(!daiphunSetting.contains("portName")) daiphunSetting.setValue("portName", "ttyUSB0");
    if(!daiphunSetting.contains("baudRate")) daiphunSetting.setValue("baudRate", 19200);
    if(!daiphunSetting.contains("daiphunID")) daiphunSetting.setValue("daiphunID",0);



    fountainServer aServer;
    SimpleSerialInterface asimpleSerialInterface;
    communicationChecker aCommuChecker;

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


   // QObject::connect(&asimpleSerialInterface,&SimpleSerialInterface::output,&aServer,&fountainServer::fromSerialHandler);


    foreach (QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        qDebug() << port.portName();
        if(port.portName() == "ttyUSB0")
        {
            asimpleSerialInterface.setBaudRate(daiphunSetting.value("baudRate").toInt());
            asimpleSerialInterface.setPortName(daiphunSetting.value("portName").toByteArray());

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

    QObject::connect(&asimpleSerialInterface, &SimpleSerialInterface::output, &aCommuChecker, &communicationChecker::in);
    QObject::connect(&aCommuChecker,&communicationChecker::requestUserInputForWifiInfo,&asimpleSerialInterface,&SimpleSerialInterface::input);
    QObject::connect(&aCommuChecker,&communicationChecker::requestWifiInfo,&asimpleSerialInterface,&SimpleSerialInterface::input);
    QObject::connect(&aCommuChecker,&communicationChecker::wifiOK,&asimpleSerialInterface,&SimpleSerialInterface::input);
    QObject::connect(&aCommuChecker,&communicationChecker::wifiNotOK,&asimpleSerialInterface,&SimpleSerialInterface::input);

    aCommuChecker.start();

    tcpPackager::setFountainID(daiphunSetting.value("daiphunID").toInt());



    return a.exec();
}
