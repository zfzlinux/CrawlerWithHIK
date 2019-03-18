#ifndef MODBUSMASTER_H
#define MODBUSMASTER_H

#include <QObject>
#include <QSerialPort>
#include <modbus.h>
#include <modbusreg.h>
#include "globaldefine.h"
#include "configparmeter.h"

class ModbusMaster : public QObject
{
    Q_OBJECT
public:
    static ModbusMaster* getInstance();
private:
    explicit ModbusMaster(QObject *parent = nullptr);
    ~ModbusMaster();
    char formatParity(const QSerialPort *crawlerSerial);
public:
    bool openModbus(const QSerialPort *crawlerSerial, quint16 slaveNum);
    void closeModbus();
    void setResponseTimeout(const unsigned int sec_ms);
    bool setSlaveNum(quint16 slaveNum);
    bool readRegisters(quint16 addr,quint16 length,quint16 *reg);
    bool initRegWithSlaveNum(quint16 slaveNum);

    MODBUS_REG getModbusReg();
    bool writeOneRegisters(quint16 addr,quint16 value);
    bool writeRegisters(quint16 addr,quint16 num,const quint16* src);

    //
    void setActionMode(EnumActionMode mode);
    EnumActionMode getActionMode();
    bool setSpeedUpByActionMode();
    bool setSpeedDownByActionMode();
    //write
    void setALLSpeedGear(quint8 gear);
    void setAllSpeedPWM(quint16 pwm);
    void setPWMValueByConfig(EnumPWMType PWMType,quint16 value);

    //
    void setCrawlerMovingState(EnumCrawlerMovingState movingState); //根据当前模式，控制车体。
    //
    void setObjectDistance(quint16 objValue);

    void enableHeart(bool enable);
    void resetHeartReg();

    void setAngleHor(quint8 angle);
    void setAngleVer(quint8 angle);

    void setLiftShaftStatus(EnumLiftShaftMotorType type, LiftShaftStatus status);
    //read
    quint16 getCurLeftEncoder(EnumPWMType PWMType);
    quint16 getCurPattern();
    quint16 getCurAction();
    quint16 getDistanceMoved();

signals:
    void sgModbusReady();
    void sgSpeedUpByActionMode();
    void sgSpeedDownByActionMode();
    void sgUpdateCrawlerSpeedGear(quint8 gear);
    //发送当前速度信号
    void sgUpdatePWMValueEverySecond(EnumPWMType PWMType,quint16 value);

    void sgUpdateDistanceMoved(quint16 value);
    void sgUpdateLiftShaftStatus(EnumLiftShaftMotorType type, LiftShaftStatus status);

private slots:
    void slCloseModbus();
    void slGetModbusReg();
    //速度档位
    void slUpdateCrawlerSpeedGears(quint8 gear);
    void slUpdatePWMValueByConfig(EnumPWMType PWMType,quint16 value);
    void slgetCurPWMEverySecond(EnumPWMType PWMType);

    void slUpdateCrawlerMovingState(EnumCrawlerMovingState movingState);

    //
    void slUpdateAngleHor(quint8 value);
    void slUpdateAngleVer(quint8 value);

    void slUpdateObjectDistance(quint16 objValue);
    void slEnableHeart(bool enable);
    void slResetHeartReg();

private:
    static ModbusMaster* m_pInstance;
    static bool b_isConnected;
    static MODBUS_REG MD_REG;
    int m_curSlaveNum;
    MODBUS_REG* pMD_REG;
    static modbus_t* m_pModbus;
    EnumActionMode m_curMode;
};

#endif // MODBUSMASTER_H
