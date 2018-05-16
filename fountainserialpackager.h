#ifndef FOUNTAINSERIALPACKAGER_H
#define FOUNTAINSERIALPACKAGER_H

#include <QObject>
#include <QString>
#define  m_OpCode_runProgramOnFountainDirecly                       0x01
#define  m_OpCode_restartProgramOnFountain                          0x22
#define  m_OpCode_setRTCOnElectricalBox                             0x02
#define  m_OpCode_setLightSavingTimer                               0x10
#define  m_OpCode_setMotorSavingtimer                               0x11
#define  m_OpCode_setSyncModeOfallFountainsPerElectricalBox         0x03
#define  m_OpCode_setOperationModeOfAllFountainPerElectricalBox     0x04
#define  m_OpCode_setSpeedSingleProgramSingleFountain               0x25
#define  m_OpCode_setProgramSingleFountain                          0x05
#define  m_OPCode_setSpeedAllProgramsSingleFountain                 0x26
#define  m_OpCode_setAllProgramsSingleFountain                      0x06
#define  m_OpCode_FountainDeviceInternalHandShake                   0x00
#define  m_Status_FountainDeviceHandShake                           0x01
#define  m_Status_WifiOK                                            0x01
#define  m_Stauus_WifiNoTOK                                         0x00
//#define  m_Status_VPNOK                                           0x04
#define  m_Status_RequestWifi                                       0x02
#define  m_Status_RequestUserInputForWifi                           0x08
#define  m_Status_ControlDevicesConnectedToPi                       0x05


class fountainSerialPackager : public QObject
{

    Q_OBJECT

    static constexpr quint8 m_startFlag = 0x02;
    static constexpr quint8 m_stopFlag = 0x03;



    quint8 m_OperationCode;
    quint16 m_PackageLength;
    QByteArray m_Data;

    quint8 m_BoxID;
    quint8 m_FOID;
    quint8 m_ProgramID;
    quint8 m_Repeat;
    quint8 m_StatusCode;
    QString m_WifiName;
    QString m_WifiPassword;

    bool m_IsPackageValid;



public:

    explicit fountainSerialPackager(const QByteArray &data, QObject * parent = nullptr);
    explicit fountainSerialPackager(QObject * parent = nullptr);
    fountainSerialPackager &setOpcode(const quint8 &opcode);
    fountainSerialPackager &setData(const QByteArray &data);
    fountainSerialPackager &setData(const quint8 &data);
    fountainSerialPackager &setPackageLength();
    fountainSerialPackager &setBoxID(const quint8 &boxID);
    fountainSerialPackager &setFOID(const quint8 &fOID);
    fountainSerialPackager &setRepeat(const quint8 &repeat);
    fountainSerialPackager &setProgramID(const quint8 &programID);
    fountainSerialPackager &setHour(const quint8 &hour);
    fountainSerialPackager &setMinute(const quint8 &minute);
    fountainSerialPackager &setSecond(const quint8 &second);

    Q_INVOKABLE QByteArray runProgramOnFountainDirectly(const quint8 &Box_ID, const quint8 &FO_ID, const quint8 &programID, const quint8 &repeat);
    Q_INVOKABLE QByteArray restartProgramOnFountain(const quint8 &Box_ID, const quint8 &FO_ID, const quint8 &programID);
    Q_INVOKABLE QByteArray setRTCTimeForElectricalBox(const quint8 &Box_ID, const quint8 &hour, const quint8 &minute, const quint8 &second);
    Q_INVOKABLE QByteArray setLightSavingTimeForElctricalbox(const quint8 &Box_ID, const quint8 &Onhour, const quint8 &Onminute, const quint8 &OffHour, const quint8 &OffMinute);
    Q_INVOKABLE QByteArray setMotorSavingTimeForElectricalBox(const quint8 &Box_ID, const quint8 &Onhour1, const quint8 &Onminute1, const quint8 &OffHour1, const quint8 &OffMinute1
                                                              , const quint8 &Onhour2, const quint8 &Onminute2, const quint8 &OffHour2, const quint8 &OffMinute2);
    Q_INVOKABLE QByteArray setSyncModeForFountainsPerElectricalBox(const quint8 & Box_ID, const QByteArray &syncMode);
    Q_INVOKABLE QByteArray setSyncModeForSingleFountainPerElectricalBox(const quint8 &Box_ID, const quint8 &FO_ID, const quint8 &syncFountain);
    Q_INVOKABLE QByteArray setOperationModeFountainsPerElectricalBOx(const quint8 & Box_ID, const QByteArray &operationMode);
    Q_INVOKABLE QByteArray setSpeedSingleProgramPerFountain(const quint8 &Box_ID, const quint8 &FO_ID, const quint8 &Program_ID, const quint8 &speed);
    Q_INVOKABLE QByteArray setSpeedAllProgramsPerFountain(const quint8 &Box_ID, const quint8 &FO_ID, const QByteArray &speeds);
    Q_INVOKABLE QByteArray setProgramEffectForSingleFountain(const quint8 &Box_ID, const quint8 &FO_ID, const quint8  &Program_ID, const quint8 &effectID, const quint8 &speed, const quint8 &repeat);

    static QByteArray fountainDeviceHandshake();
    static QByteArray fountainDeviceWifiOK();
    static QByteArray fountainDeviceRequestWifi();
    static QByteArray fountainDeviceRequestUserInputForWifi();
    static QByteArray fountainDeviceWifiNotOK();

    QByteArray generateSerialPackage();
    QByteArray generateSerialPackage(const quint8 &opcode, const QByteArray &data);


    // decode function

    bool isPackageValid();

    void decodePackage(const QByteArray &data);
    QString getWifiName();
    QString getWifiPassword();

    quint8 getStatusCode();
};

#endif // FOUNTAINSERIALPACKAGER_H
