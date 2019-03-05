#include "crawlerstatusparam.h"
#include "crawlerserial.h"

CrawlerStatusParam *CrawlerStatusParam::m_pInstance;

CrawlerStatusParam *CrawlerStatusParam::getInstance()
{
    if(m_pInstance == NULL)
        m_pInstance = new CrawlerStatusParam();
    return m_pInstance;
}

CrawlerStatusParam::CrawlerStatusParam(QObject *parent) : QObject(parent),
    m_movingState(crawler_stop),
    m_curMode(speedMode),
    m_speedIndex(InitSpeedIndex),
    m_angleHor(InitServoAngleHor),m_angleVer(InitServoAngleHor),
    m_VerMotorStatus(LS_MotorStop),m_HorMotorStatus(LS_MotorStop),
    m_UTValueA(0),m_UTValueB(0),
    m_curUTStatusA(Sen_DisConnect),m_curUTStatusB(Sen_DisConnect),m_curUTStatusC(Sen_DisConnect),
    m_PreValueA(0),m_PreValueB(0),
    m_curPreStatusA(Sen_DisConnect),m_curPreStatusB(Sen_DisConnect),
    m_isTopVer(false),m_isBtmVer(false),m_isTopHor(false),m_isBtmHor(false),
    m_PWMLeftConfig(InitPWMValue),m_PWMRightConfig(InitPWMValue),m_curPWMLeft(0),m_curPWMRight(0),
    m_curDispMoveStatus(Disp_MoveStop),m_curDistanceByConfig(InitDistanceValue)
{
    initConnect();
}

void CrawlerStatusParam::keepCurCrawlerMovingState(EnumCrawlerMovingState movingState)
{
    m_movingState = movingState;
    emit sgUpdateCrawlerMovingState();
}

EnumCrawlerMovingState CrawlerStatusParam::getCurCrawlerMovingState()
{
    return m_movingState;
}

void CrawlerStatusParam::keepCurCrawlerSpeedIndex(quint8 speedIndex)
{
    m_speedIndex = speedIndex;
    emit sgUpdateCrawlerSpeedIndex(speedIndex);
}

quint8 CrawlerStatusParam::getCurCrawlerSpeedIndex()
{
    return m_speedIndex;
}

void CrawlerStatusParam::keepCurCrawlerSerial(QSerialPort *serialDevice)
{
    m_crawlerSerial = serialDevice;
}

QSerialPort *CrawlerStatusParam::getCurCrawlerSerial()
{
    if(m_crawlerSerial ==NULL)
        m_crawlerSerial = new QSerialPort();
    return m_crawlerSerial;
}

void CrawlerStatusParam::keepCurLiftShaftStatus(EnumLiftShaftMotorType MotorType, EnumLiftShaftStatus status)
{
    switch (MotorType) {
    case LS_VerMotor:
        this->m_VerMotorStatus = status;
        emit sgUpdateLiftShaftStatus(MotorType,status);
        break;
    case LS_HorMotor:
        this->m_HorMotorStatus = status;
        emit sgUpdateLiftShaftStatus(MotorType,status);
        break;
    default:
        break;
    }
}

EnumLiftShaftStatus CrawlerStatusParam::getCurLiftShaftStatus(EnumLiftShaftMotorType MotorType)
{
    EnumLiftShaftStatus status;
    switch (MotorType) {
    case LS_VerMotor:
        status = this->m_VerMotorStatus;
        break;
    case LS_HorMotor:
        status = this->m_HorMotorStatus;
        break;
    default:
        break;
    }
    return status;
}

void CrawlerStatusParam::setActionMode(EnumActionMode mode)
{
    configParmeter *setting = configParmeter::getInstance();
    setting->setActionMode(mode);
    m_curMode = mode;
}

EnumActionMode CrawlerStatusParam::getActionMode()
{
//     configParmeter *setting = configParmeter::getInstance();
//     return setting->getActionMode();
    return m_curMode;
}

void CrawlerStatusParam::keepCurServoAngleVer(quint8 angle)
{
    m_angleVer = angle;
    emit sgUpdateServoAngleVer(m_angleVer);
}

quint8 CrawlerStatusParam::getCurServoAngleVer()
{
    return m_angleVer;
}

void CrawlerStatusParam::keepCurServoAngleHor(quint8 angle)
{
    m_angleHor = angle;
    emit sgUpdateServoAngleHor(m_angleHor);
}

quint8 CrawlerStatusParam::getCurServoAngleHor()
{
    return m_angleHor;
}

void CrawlerStatusParam::keepCurUTSensorValue(EnumUTSensorType sensorType, quint16 utValue)
{
    switch (sensorType) {
    case UT_SensorA:
        m_UTValueA = utValue;
        break;
    case UT_SensorB:
        m_UTValueB = utValue;
        break;
    default:
        break;
    }
    emit this->sgUpdateUTSensorValue(sensorType,utValue);
}

quint16 CrawlerStatusParam::getCurUTSensorValue(EnumUTSensorType sensorType)
{
    quint16 utValue;
    switch (sensorType) {
    case UT_SensorA:
        utValue =m_UTValueA;
        break;
    case UT_SensorB:
        utValue =m_UTValueB ;
        break;
    default:
        break;
    }
    return utValue;
}

void CrawlerStatusParam::keepCurUTSensorStatus(EnumUTSensorType sensorType, EnumSensorStatus status)
{
    switch (sensorType) {
    case UT_SensorA:
        m_curUTStatusA = status;
        break;
    case UT_SensorB:
        m_curUTStatusB = status;
        break;
    default:
        break;
    }
    emit this->sgUpdateUTSensorStatus(sensorType,status);
}

EnumSensorStatus CrawlerStatusParam::getCurUTSensorStatus(EnumUTSensorType sensorType)
{
    EnumSensorStatus status;
    switch (sensorType) {
    case UT_SensorA:
        status = m_curUTStatusA ;
        break;
    case UT_SensorB:
        status = m_curUTStatusB;
        break;
    default:
        break;
    }
    return status;
}

void CrawlerStatusParam::keepCurPressureSensorValue(EnumPreSensorType sensorType, quint16 preValue)
{
    switch (sensorType) {
    case Pre_SensorA:
        m_PreValueA = preValue;
        break;
    case Pre_SensorB:
        m_PreValueB = preValue;
        break;
    default:
        break;
    }
    emit this->sgUpdatePressureValue(sensorType,preValue);
}

quint16 CrawlerStatusParam::getCurPressureSensorValue(EnumPreSensorType sensorType)
{
    quint16  preValue;
    switch (sensorType) {
    case Pre_SensorA:
        preValue = m_PreValueA;
        break;
    case Pre_SensorB:
        preValue = m_PreValueB;
        break;
    default:
        break;
    }
    return preValue;
}

void CrawlerStatusParam::keepCurPreSenStatus(EnumPreSensorType sensorType, EnumSensorStatus status)
{
    switch (sensorType) {
    case Pre_SensorA:
        m_curPreStatusA = status;
        break;
    case Pre_SensorB:
        m_curPreStatusB = status;
        break;
    default:
        break;
    }
    emit this->sgUpdatePressureStatus(sensorType,status);
}

EnumSensorStatus CrawlerStatusParam::getCurPreSenStatus(EnumPreSensorType sensorType)
{
    EnumSensorStatus status;
    switch (sensorType) {
    case Pre_SensorA:
        status = m_curPreStatusA;
        break;
    case Pre_SensorB:
        status = m_curPreStatusB;
        break;
    default:
        break;
    }
    return status;
}

void CrawlerStatusParam::keepCurLimitStatus(EnumLimitSwitchType limitType, bool isLimit)
{
    switch (limitType) {
    case VerTopLimit:
        m_isTopVer = isLimit;
        break;
    case VerBottomLimit:
        m_isBtmVer = isLimit;
        break;
    case HorTopLimit:
        m_isTopHor = isLimit;
        break;
    case HorBottomLimit:
        m_isBtmHor = isLimit;
        break;
    default:
        break;
    }
    emit this->sgUpdateLimitStatus(limitType,isLimit);
}

bool CrawlerStatusParam::getCurLimitStatus(EnumLimitSwitchType limitType)
{
    bool isLimit = false;
    switch (limitType) {
    case VerTopLimit:
        isLimit = m_isTopVer;
        break;
    case VerBottomLimit:
        isLimit = m_isBtmVer;
        break;
    case HorTopLimit:
        isLimit = m_isTopHor;
        break;
    case HorBottomLimit:
        isLimit = m_isBtmHor;
        break;
    default:
        break;
    }
    return isLimit;
}

void CrawlerStatusParam::keepCurPWMValueByConfig(EnumPWMType PWMType, quint16 value)
{
    switch (PWMType) {
    case PWMLeftByConfig:
        m_PWMLeftConfig = value;
        break;
    case PWMRightByConfig:
        m_PWMRightConfig =value;
        break;
    default:
        break;
    }
    emit this->sgUpdatePWMValueByConfig(PWMType,value);
}

quint16 CrawlerStatusParam::getCurPWMValueByConfig(EnumPWMType PWMType)
{
    quint16 value;
    switch (PWMType) {
    case PWMLeftByConfig:
        value = m_PWMLeftConfig;
        break;
    case PWMRightByConfig:
        value = m_PWMRightConfig;
        break;
    default:
        break;
    }
    return value;
}

void CrawlerStatusParam::keepPWMValueEverySecond(EnumPWMType PWMType, quint16 value)
{
    switch (PWMType) {
    case CurPWMLeft:
        m_curPWMLeft = value;
        break;
    case CurPWMRight:
        m_curPWMRight = value;
        break;
    default:
        break;
    }
    emit this->sgUpdatePWMValueEverySecond(PWMType,value);
    //
    double speed = this->PWMValueTomm(m_curPWMLeft);
    emit this->sgUpdateCurSpeed(speed);
}

quint16 CrawlerStatusParam::getPWMValueEverySecond(EnumPWMType PWMType)
{
    quint16 value ;
    switch (PWMType) {
    case CurPWMLeft:
         value = m_curPWMLeft;
        break;
    case CurPWMRight:
        value = m_curPWMRight;
        break;
    default:
        break;
    }
    return value;
}

quint16 CrawlerStatusParam::SpeedIndexToPWMValue(quint8 speedIndex)
{
    quint16 PWMValue =0;
    if(speedIndex>= MinSpeedIndex && speedIndex <= MaxSpeedIndex)
    {
        PWMValue = MinPWMValue + (PWM_PRECISION * (speedIndex -1));
        if(PWMValue < MinPWMValue) PWMValue = MinPWMValue;
        if(PWMValue > MaxPWMValue) PWMValue = MaxPWMValue;
    }
    return PWMValue;
}

double CrawlerStatusParam::PWMValueTomm(quint16 PWMValue)
{
    double value = 0;
    value = PWMValue / Pluse_one_Millimeter;
    return value;
}

void CrawlerStatusParam::keepCurDispMoveStatus(EnumDispModeStatus status)
{
    this->m_curDispMoveStatus = status;
    emit this->sgUpdateCurDistanceStatus(m_curDispMoveStatus);
}

EnumDispModeStatus CrawlerStatusParam::getCurDispMoveStatus()
{
    return this->m_curDispMoveStatus;
}

void CrawlerStatusParam::keepCurMoveDistanceValueByConfig(quint16 value)
{
    this->m_curDistanceByConfig = value;
    emit this->sgUpdateCurMoveDistanceValue(this->m_curDistanceByConfig);
}

quint16 CrawlerStatusParam::getCurMoveDistanceValueByConfig()
{
    return this->m_curDistanceByConfig;
}

void CrawlerStatusParam::slSpeedUpByActionMode()
{
    switch (m_curMode) {
    case speedMode:
        this->addSpeedIndex();
        break;
    case distanceMode:
        this->addSpeedIndex();
        break;
    case difSpeedMode:
        this->addPWMValue();
        break;
    default:
        break;
    }
}

void CrawlerStatusParam::slSpeedDownByActionMode()
{
    switch (m_curMode) {
    case speedMode:
        this->subSpeedIndex();
        break;
    case distanceMode:
        this->subSpeedIndex();
        break;
    case difSpeedMode:
        this->subPWMValue();
        break;
    default:
        break;
    }
}

void CrawlerStatusParam::initConnect()
{
    CrawlerSerial *crawlerSerial = CrawlerSerial::getInstance();
    connect(crawlerSerial,SIGNAL(sgSpeedUpByActionMode()),this,SLOT(slSpeedUpByActionMode()));
    connect(crawlerSerial,SIGNAL(sgSpeedDownByActionMode()),this,SLOT(slSpeedDownByActionMode()));
}

void CrawlerStatusParam::addSpeedIndex()
{
    if(m_speedIndex < MaxSpeedIndex)
    {
        m_speedIndex ++;
    }
    emit this->sgUpdateCrawlerSpeedIndex(m_speedIndex);
}

void CrawlerStatusParam::subSpeedIndex()
{
    if(m_speedIndex > MinSpeedIndex)
    {
        m_speedIndex --;
    }
    emit this->sgUpdateCrawlerSpeedIndex(m_speedIndex);
}

void CrawlerStatusParam::addPWMValue()
{
    if((m_PWMLeftConfig+PWM_PRECISION)<MaxPWMValue)
    {
        m_PWMLeftConfig = m_PWMLeftConfig +PWM_PRECISION;
    }else
    {
        m_PWMLeftConfig = MaxPWMValue;
    }

    if((m_PWMRightConfig+PWM_PRECISION)<MaxPWMValue)
    {
        m_PWMRightConfig = m_PWMRightConfig +PWM_PRECISION;
    }else
    {
        m_PWMRightConfig = MaxPWMValue;
    }
    emit this->sgUpdatePWMValueByConfig(PWMLeftByConfig,m_PWMLeftConfig);
    emit this->sgUpdatePWMValueByConfig(PWMRightByConfig,m_PWMRightConfig);
}

void CrawlerStatusParam::subPWMValue()
{
    if((m_PWMLeftConfig - PWM_PRECISION)>MinPWMValue)
    {
        m_PWMLeftConfig = m_PWMLeftConfig - PWM_PRECISION;
    }else
    {
        m_PWMLeftConfig = MinPWMValue;
    }

    if((m_PWMRightConfig - PWM_PRECISION)>MinPWMValue)
    {
        m_PWMRightConfig = m_PWMRightConfig - PWM_PRECISION;
    }else
    {
        m_PWMRightConfig = MinPWMValue;
    }
    emit this->sgUpdatePWMValueByConfig(PWMLeftByConfig,m_PWMLeftConfig);
    emit this->sgUpdatePWMValueByConfig(PWMRightByConfig,m_PWMRightConfig);
}
