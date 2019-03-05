#ifndef CRAWLERSTATUSPARAM_H
#define CRAWLERSTATUSPARAM_H

#include <QObject>
#include "globaldefine.h"
#include "configparmeter.h"
#include <QSerialPort>

class CrawlerStatusParam : public QObject
{
    Q_OBJECT
public:
    static CrawlerStatusParam *getInstance();
private:
    static CrawlerStatusParam *m_pInstance;
    explicit CrawlerStatusParam(QObject *parent = nullptr);
public:
    void keepCurCrawlerMovingState(EnumCrawlerMovingState movingState);
    EnumCrawlerMovingState getCurCrawlerMovingState();

    void keepCurCrawlerSpeedIndex(quint8 speedIndex);
    quint8 getCurCrawlerSpeedIndex();

    void keepCurCrawlerSerial(QSerialPort *serialDevice);
    QSerialPort * getCurCrawlerSerial();

    void keepCurLiftShaftStatus(EnumLiftShaftMotorType MotorType,EnumLiftShaftStatus status);
    EnumLiftShaftStatus getCurLiftShaftStatus(EnumLiftShaftMotorType MotorType);

    void setActionMode(EnumActionMode mode);
    EnumActionMode getActionMode();

    void keepCurServoAngleVer(quint8 angle);
    quint8 getCurServoAngleVer();
    void keepCurServoAngleHor(quint8 angle);
    quint8 getCurServoAngleHor();

    void keepCurUTSensorValue(EnumUTSensorType sensorType,quint16 utValue);
    quint16 getCurUTSensorValue(EnumUTSensorType sensorType);
    void keepCurUTSensorStatus(EnumUTSensorType sensorType,EnumSensorStatus status);
    EnumSensorStatus getCurUTSensorStatus(EnumUTSensorType sensorType);

    void keepCurPressureSensorValue(EnumPreSensorType sensorType,quint16 preValue);
    quint16 getCurPressureSensorValue(EnumPreSensorType sensorType);
    void keepCurPreSenStatus(EnumPreSensorType sensorType,EnumSensorStatus status);
    EnumSensorStatus getCurPreSenStatus(EnumPreSensorType sensorType);

    void keepCurLimitStatus(EnumLimitSwitchType limitType, bool isLimit);
    bool getCurLimitStatus(EnumLimitSwitchType limitType);

    void keepCurPWMValueByConfig(EnumPWMType PWMType,quint16 value);
    quint16 getCurPWMValueByConfig(EnumPWMType PWMType);

    void keepPWMValueEverySecond(EnumPWMType PWMType,quint16 value);
    quint16 getPWMValueEverySecond(EnumPWMType PWMType);

    quint16 SpeedIndexToPWMValue(quint8 speedIndex);
    double PWMValueTomm(quint16 PWMValue);

    void keepCurDispMoveStatus(EnumDispModeStatus status);
    EnumDispModeStatus getCurDispMoveStatus();

    void keepCurMoveDistanceValueByConfig(quint16 value);
    quint16 getCurMoveDistanceValueByConfig();
signals:
    void sgUpdateCrawlerMovingState();
    void sgUpdateCrawlerSpeedIndex(quint8 speedIndex);
    void sgUpdateServoAngleVer(quint8 angle);
    void sgUpdateServoAngleHor(quint8 angle);
    void sgUpdateLiftShaftStatus(EnumLiftShaftMotorType MotorType,EnumLiftShaftStatus status);
    void sgUpdateUTSensorValue(EnumUTSensorType sensorType,quint16 utValue);
    void sgUpdateUTSensorStatus(EnumUTSensorType sensorType,EnumSensorStatus status);
    void sgUpdatePressureValue(EnumPreSensorType sensorType,quint16 preValue);
    void sgUpdatePressureStatus(EnumPreSensorType sensorType,EnumSensorStatus status);
    void sgUpdateLimitStatus(EnumLimitSwitchType limitType, bool isLimit);
    void sgUpdatePWMValueByConfig(EnumPWMType PWMType,quint16 value);
    void sgUpdatePWMValueEverySecond(EnumPWMType PWMType,quint16 value);
    void sgUpdateCurSpeed(double curSpeed);
    void sgUpdateCurDistanceStatus(EnumDispModeStatus status);
    void sgUpdateCurMoveDistanceValue(quint16 value);
private slots:
    void slSpeedUpByActionMode();
    void slSpeedDownByActionMode();
private:
    void initConnect();
    void addSpeedIndex();
    void subSpeedIndex();
    void addPWMValue();
    void subPWMValue();
private:
    EnumCrawlerMovingState m_movingState;
    quint8 m_speedIndex,m_angleVer,m_angleHor;
    QSerialPort *m_crawlerSerial;
    EnumLiftShaftStatus m_curliftShaftState;
    EnumActionMode m_curMode;
    EnumLiftShaftStatus m_VerMotorStatus,m_HorMotorStatus;
    quint16 m_UTValueA,m_UTValueB;
    EnumSensorStatus m_curUTStatusA,m_curUTStatusB,m_curUTStatusC;
    quint16 m_PreValueA,m_PreValueB;
    EnumSensorStatus m_curPreStatusA,m_curPreStatusB;
    bool m_isTopVer,m_isBtmVer,m_isTopHor,m_isBtmHor;
    quint16 m_PWMLeftConfig,m_PWMRightConfig,m_curPWMLeft,m_curPWMRight;
    quint16 m_curDistanceByConfig;
    EnumDispModeStatus m_curDispMoveStatus;
};

#endif // CRAWLERSTATUSPARAM_H
