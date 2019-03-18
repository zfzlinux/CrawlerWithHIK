#include "modbusmaster.h"
#include <QMessageBox>
ModbusMaster *ModbusMaster::m_pInstance;
bool ModbusMaster::b_isConnected =false;
MODBUS_REG  ModbusMaster::MD_REG = {0};
modbus_t* ModbusMaster::m_pModbus;
ModbusMaster *ModbusMaster::getInstance()
{
    if(NULL== m_pInstance)
    {
        m_pInstance = new ModbusMaster();
    }
    return m_pInstance;
}

ModbusMaster::ModbusMaster(QObject *parent) : QObject(parent),
    m_curSlaveNum(1),m_curMode(speedMode),pMD_REG(&MD_REG)
{
    //pMD_REG = &MD_REG;
}

ModbusMaster::~ModbusMaster()
{
    this->closeModbus();
}

char ModbusMaster::formatParity(const QSerialPort *crawlerSerial)
{

    char strParity = 'N';
    QSerialPort::Parity curPy= crawlerSerial->parity();
    switch (curPy) {
    case QSerialPort::Parity::NoParity:
        strParity = 'N';
        break;
    case QSerialPort::Parity::EvenParity:
        strParity = 'E';
        break;
    case QSerialPort::Parity::OddParity:
        strParity = 'O';
        break;
    default:
        strParity = 'N';
        break;
    }
    return strParity;
}

bool ModbusMaster::openModbus(const QSerialPort *crawlerSerial,quint16 slaveNum)
{
    int isConnected=-1;
    QString strportName = crawlerSerial->portName();
    QByteArray ba = strportName.toLatin1(); //不能省略合并这一步骤。
    char* portName = ba.data();
    int baud = crawlerSerial->baudRate();
    char strParity = formatParity(crawlerSerial);
    int dataBit =(int)crawlerSerial->dataBits();
    int stopBit =(int)crawlerSerial->stopBits();
    this->m_pModbus = modbus_new_rtu(portName,baud,strParity,dataBit,stopBit);
    this->setSlaveNum(slaveNum);
    isConnected = modbus_connect(m_pModbus);
    if(0==isConnected)
    {
        this->b_isConnected = true;
    }else
    {
        this->b_isConnected = false;
        //
//        QMessageBox("Modbus","Connect to crawler failure!",);
        QMessageBox msgbox;
        msgbox.setText("open serial failure!");
        msgbox.exec();
    }
    return this->b_isConnected;
}

void ModbusMaster::closeModbus()
{
    if(this->b_isConnected)
    {
        modbus_close(m_pModbus);
        modbus_free(m_pModbus);
        b_isConnected = false;
    }
}

void ModbusMaster::setResponseTimeout(const unsigned int sec_ms)
{
    struct timeval timeout;
    timeout.tv_sec=0;
    timeout.tv_usec = sec_ms*1000; // //设置modbus超时时间为1000毫秒
    modbus_set_response_timeout(this->m_pModbus,timeout.tv_sec,timeout.tv_usec);
}

bool ModbusMaster::setSlaveNum(quint16 slaveNum)
{
    bool isSet = false;
    quint16 rtValue =0;
    this->m_curSlaveNum = slaveNum;
    if(b_isConnected)
    {
        rtValue = modbus_set_slave(m_pModbus,slaveNum);
    }
    if(0 ==rtValue) isSet = true;

    return isSet;
}

bool ModbusMaster::initRegWithSlaveNum(quint16 slaveNum)
{
    quint16 addr = 0;
    quint16 length = MODBUS_ADD_MAX;
//    bool isOK = setSlaveNum(slaveNum);
//    if(!isOK) return false;
    this->setResponseTimeout(1000);//1000ms timeout
    uint16_t* regAddr = (uint16_t*)this->pMD_REG; //地址
    bool isOK = readRegisters(addr,length,regAddr);
    if(isOK)
    {
        emit this->sgModbusReady();
    }else
    {
        QMessageBox msgbox;
        msgbox.setText("modbus miscommunication!");
        msgbox.exec();
    }

    return isOK;
}

MODBUS_REG ModbusMaster::getModbusReg()
{
    quint16 addr = 0;
    quint16 length = MODBUS_ADD_MAX;
//    bool isOK = setSlaveNum(slaveNum);
//    if(!isOK) return false;
    this->setResponseTimeout(1000);//1000ms timeout
    uint16_t* regAddr = (uint16_t*)this->pMD_REG; //地址
    bool isOK = readRegisters(addr,length,regAddr);
    return this->MD_REG;
}

bool ModbusMaster::readRegisters(quint16 addr, quint16 length, quint16 *reg)
{
   quint16 rtNum =0;
   bool isRead =false;
   if(b_isConnected)
   {
       modbus_set_slave(m_pModbus,m_curSlaveNum);//必须在这里重新设置一次
       rtNum= modbus_read_registers(m_pModbus,addr,length,reg);
   }
   if(length==rtNum) isRead=true;
   return isRead;
}

bool ModbusMaster::writeOneRegisters(quint16 addr, quint16 value)
{
    quint16 rtNum =0;
    bool isRead =false;
    if(b_isConnected)
    {
        modbus_set_slave(m_pModbus,m_curSlaveNum);//必须在这里重新设置一次
        rtNum = modbus_write_register(m_pModbus,addr,value);
    }
    if(1==rtNum)
    {
        qDebug("YES");
        isRead=true;
    }
    return isRead;
}

bool ModbusMaster::writeRegisters(quint16 addr, quint16 num, const quint16 *src)
{
    quint16 rtNum =0;
    bool isRead =false;
    if(b_isConnected)
    {
        modbus_set_slave(m_pModbus,m_curSlaveNum);//必须在这里重新设置一次
        rtNum =modbus_write_registers(m_pModbus,addr,num,src);
    }
    if(num == rtNum) isRead =true;
    return isRead;
}

void ModbusMaster::setActionMode(EnumActionMode mode)
{
    configParmeter *setting = configParmeter::getInstance();
    setting->setActionMode(mode);
    m_curMode = mode;
}

EnumActionMode ModbusMaster::getActionMode()
{
    //读取寄存器并返回。
    return m_curMode;
}

bool ModbusMaster::setSpeedDownByActionMode()
{
    bool isOK = false;
    switch(this->m_curMode)
    {
    case speedMode:
    case distanceMode:
        isOK = this->writeOneRegisters(speed_flag_addr,SpeedIndex_Down);
        break;
    case difSpeedMode:
        isOK = this->writeOneRegisters(speed_flag_addr,PWM_Down);
        break;
    }
    if(isOK)
    {
        emit this->sgSpeedDownByActionMode();
    }
    return isOK;
}

bool ModbusMaster::setSpeedUpByActionMode()
{
    bool isOK = false;
    switch(this->m_curMode)
    {
    case speedMode:
    case distanceMode:
        isOK = this->writeOneRegisters(speed_flag_addr,SpeedIndex_Up);
        break;
    case difSpeedMode:
        isOK = this->writeOneRegisters(speed_flag_addr,PWM_Up);
        break;
    }
    if(isOK)
    {
        emit this->sgSpeedUpByActionMode();
    }
    return isOK;
}

void ModbusMaster::setALLSpeedGear(quint8 gear)
{
    //将2个寄存器的值都赋值
    uint16_t speedGear[2]={0};
    speedGear[0]=gear;
    speedGear[1]=gear;
    bool isWrited = this->writeRegisters(left_gears_addr,2,speedGear);
    if(isWrited)
    {
       emit this->sgUpdateCrawlerSpeedGear(gear);
    }else
    {

    }
}

void ModbusMaster::setPWMValueByConfig(EnumPWMType PWMType, quint16 value)
{
    switch (PWMType) {
    case PWMLeftByConfig:
        this->writeOneRegisters(left_pwm_addr,value);
        break;
    case PWMRightByConfig:
        this->writeOneRegisters(right_pwm_addr,value);
        break;
    default:
        break;
    }
}

void ModbusMaster::setCrawlerMovingState(EnumCrawlerMovingState movingState)
{
    switch(m_curMode)
    {
    case speedMode:
    case difSpeedMode:
        this->writeOneRegisters(motor_move_addr,movingState);
        break;
    case distanceMode:
        this->writeOneRegisters(distance_move_addr,movingState);
        break;
    default:
        break;
    }
}

void ModbusMaster::setObjectDistance(quint16 objValue)
{
    this->writeOneRegisters(distance_data_addr,objValue);
}

quint16 ModbusMaster::getCurLeftEncoder(EnumPWMType PWMType)
{
    quint16 value =0;
    bool isOk =false;
    switch (PWMType) {
    case CurPWMLeft:
        isOk = this->readRegisters(left_encoder_addr,1,&value);
        break;
    case CurPWMRight:
        isOk =this->readRegisters(right_encoder_addr,1,&value);
        break;
    default:
        break;
    }
    if(isOk)
    {
        emit this->sgUpdatePWMValueEverySecond(PWMType,value);
    }
    return value;
}

quint16 ModbusMaster::getCurPattern()
{
    quint16 value =0;
    this->readRegisters(pattern_flag_addr,1,&value);
    return value;
}

quint16 ModbusMaster::getCurAction()
{
    quint16 value =0;
    this->readRegisters(cur_action_addr,1,&value);
    return value;
}

quint16 ModbusMaster::getDistanceMoved()
{
    quint16 value =0;
    this->readRegisters(cur_distance_value_addr,1,&value);
    emit this->sgUpdateDistanceMoved(value);
    return value;
}

void ModbusMaster::slCloseModbus()
{
    this->closeModbus();
}

void ModbusMaster::slGetModbusReg()
{
    this->getModbusReg();
}

void ModbusMaster::enableHeart(bool enable)
{
    quint16 value =0;
    if(enable) value =1;
    this->writeOneRegisters(heart_switch_addr,value);
}

void ModbusMaster::resetHeartReg()
{
    static int count=0;
    count ++;
    quint16 value =0;
    this->writeOneRegisters(heart_reset_addr,value);
    qDebug("heart:%d",count);
}

void ModbusMaster::setAngleHor(quint8 angle)
{
    this->writeOneRegisters(servoAngle_H_addr,angle);
}

void ModbusMaster::setAngleVer(quint8 angle)
{
    this->writeOneRegisters(servoAngle_V_addr,angle);
}

void ModbusMaster::setLiftShaftStatus(EnumLiftShaftMotorType type, LiftShaftStatus status)
{
    switch(type)
    {
    case LS_VerMotor:
        this->writeOneRegisters(liftShaft_moveV_addr,status);
        break;
    case LS_HorMotor:
        this->writeOneRegisters(liftShaft_moveH_addr,status);
        break;
    default:
        break;
    }
    emit this->sgUpdateLiftShaftStatus(type,status);
}

void ModbusMaster::slUpdateCrawlerSpeedGears(quint8 gear)
{
    this->setALLSpeedGear(gear);
}

void ModbusMaster::slUpdatePWMValueByConfig(EnumPWMType PWMType, quint16 value)
{
    this->setPWMValueByConfig(PWMType,value);
}

void ModbusMaster::slUpdateCrawlerMovingState(EnumCrawlerMovingState movingState)
{
    this->setCrawlerMovingState(movingState);
}

void ModbusMaster::slUpdateAngleHor(quint8 value)
{
    this->setAngleHor(value);
}

void ModbusMaster::slUpdateAngleVer(quint8 value)
{
    this->setAngleVer(value);
}

void ModbusMaster::slUpdateObjectDistance(quint16 objValue)
{
    this->setObjectDistance(objValue);
}

void ModbusMaster::slEnableHeart(bool enable)
{
    this->enableHeart(enable);
}

void ModbusMaster::slResetHeartReg()
{
    this->resetHeartReg();
}

void ModbusMaster::slgetCurPWMEverySecond(EnumPWMType PWMType)
{
    this->getCurLeftEncoder(PWMType);
}
