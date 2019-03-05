#ifndef CRAWLERSERIAL_H
#define CRAWLERSERIAL_H

#include <QObject>
#include <QSerialPort>
#include "globaldefine.h"
#include <QTimer>

class CrawlerSerial : public QObject
{
    Q_OBJECT
public:
    static CrawlerSerial *getInstance();

public:
    void setCrawlerSerial(QSerialPort *crawlerSerial);
    QSerialPort *getCrawlerSerial();

    bool openCrawlerSerial(QSerialPort *crawlerSerial);
    void closeCrawlerSerial();
    bool writerData(QByteArray &hexData);
    bool writerData(QString strData);
    bool writerData(unsigned char cmdflagA=0,unsigned char cmdflagB=0,unsigned char valA=0,unsigned char valB=0);
    bool writerData(unsigned char cmdflagA=0,unsigned char cmdflagB=0,quint16 dval=0);

    bool askCrawlerState(EnumAskCrawlerStatus askCrawlerStatus);
    bool ChangedServoAngle_Hor(quint8 angle);
    bool ChangedServoAngle_Ver(quint8 angle);
    bool setupLiftShaftMotor(EnumLiftShaftMotorType motorType, EnumLiftShaftStatus status);

    bool enableUTSensor(EnumUTSensorType senType,bool enable);
    bool setUTSensorWarningValue(EnumUTSensorType senType,quint16 utValue);

    bool enablePreSensor(EnumPreSensorType senType,bool enable);
    bool setPreSensorWarningValue(EnumPreSensorType senType, quint16 preValue);

    bool setMaxDistanceMove(quint16 value);

    bool setPWMValue(EnumPWMType pwmType,quint16 value);

    bool setSpeedUpByActionMode();
    bool setSpeedDownByActionMode();

    void SendLoopHeartToCrawlerWith(bool isKeep);
private:
    void DealwithSerialDataReceive(QByteArray HexData);
    void recognitionCommand(QByteArray HexCommand);
    void dealFeedBackCMD(QByteArray &hexCMD);
    void dealFB_MotorSpeed(QByteArray hexCMDData);
    void dealFB_MotorStatus(QByteArray hexCMDData);
    void dealFB_ServoAngle(QByteArray hexCMDData);
    void dealFB_LiftShaftStatus(QByteArray hexCMDData);
    void dealFB_UTSensor(QByteArray hexCMDData);
    void dealFB_PressureSensor(QByteArray hexCMDData);
    void dealFB_LimitSensor(QByteArray hexCMDData);
    void dealFB_MotorPWM(QByteArray hexCMDData);
    void dealFB_DistanceMove(QByteArray hexCMDData);
signals:
    void sgSpeedUpByActionMode();
    void sgSpeedDownByActionMode();
private slots:
    void slReadData();
    void slChangedServoAngle(Qt::Key key);
    void slSerialError();
    void slLoopSendHeartToCrawler();
private:
    explicit CrawlerSerial(QObject *parent = nullptr);
private:
    static CrawlerSerial *m_pInstance;
    QSerialPort *m_crawlerSerial;
    StruServo_Angle m_servoParam;
    bool  m_isSerialInit;
    QTimer *m_TMHeart;
};

#endif // CRAWLERSERIAL_H
