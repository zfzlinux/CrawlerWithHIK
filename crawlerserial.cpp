#include "crawlerserial.h"
#include "globaldefine.h"
#include "configparmeter.h"
#include "crawlerstatusparam.h"

CrawlerSerial *CrawlerSerial::m_pInstance;

CrawlerSerial *CrawlerSerial::getInstance()
{
    if(NULL == m_pInstance)
    {
        m_pInstance = new CrawlerSerial();
    }
    return m_pInstance;
}

void CrawlerSerial::setCrawlerSerial(QSerialPort *crawlerSerial)
{
    this->m_crawlerSerial = crawlerSerial;
    connect(m_crawlerSerial,SIGNAL(readyRead()),this,SLOT(slReadData()));

    connect(m_crawlerSerial,SIGNAL(aboutToClose()),this,SLOT(slSerialError()));
}

QSerialPort *CrawlerSerial::getCrawlerSerial()
{
    return this->m_crawlerSerial;
}

bool CrawlerSerial::openCrawlerSerial(QSerialPort *crawlerSerial)
{
    this->setCrawlerSerial(crawlerSerial);
    m_isSerialInit = m_crawlerSerial->open(QIODevice::ReadWrite);
    return m_isSerialInit;
}

void CrawlerSerial::closeCrawlerSerial()
{
    this->m_crawlerSerial->close();
    m_isSerialInit =false;
}

bool CrawlerSerial::writerData(QByteArray &hexData)
{
    if(!m_isSerialInit) return false;
    if(!m_crawlerSerial->isOpen()) return false;
    int len = m_crawlerSerial->write(hexData);
    if(len==0) return false;
    return true;
}

bool CrawlerSerial::writerData(QString strData)
{
    if(!m_isSerialInit) return false;
    if(!m_crawlerSerial->isOpen()) return false;
    if(strData.isEmpty()) return false;
    QString strTemp;
    configParmeter *setting = configParmeter::getInstance();
    StruPacketParameter struPacket = setting->getPacketConfig();
    strTemp.append(struPacket.head);
    strTemp.append(strData);
    strTemp.append(struPacket.tail);
    strTemp =  strTemp.simplified();
    QByteArray byData = QByteArray::fromHex(strTemp.toLatin1());
    int len = m_crawlerSerial->write(byData);
    if(len == 0) return false;
    return true;
}

bool CrawlerSerial::writerData(unsigned char cmdflagA, unsigned char cmdflagB, unsigned char valA, unsigned char valB)
{
    QByteArray byData;
    byData.resize(6);
    byData[0] = (unsigned char)ID_PacketHead;
    byData[1] = (unsigned char)cmdflagA;
    byData[2] = (unsigned char)cmdflagB;
    byData[3] = (unsigned char)valA;
    byData[4] = (unsigned char)valB;
    byData[5] = (unsigned char)ID_PacketTail;
    return this->writerData(byData);
}

bool CrawlerSerial::writerData(unsigned char cmdflagA, unsigned char cmdflagB, quint16 dval)
{
    QByteArray byData;
    byData.resize(6);
    byData[0] = (unsigned char)ID_PacketHead;
    byData[1] = (unsigned char)cmdflagA;
    byData[2] = (unsigned char)cmdflagB;
    byData[3] = dval>>8;
    byData[4] = dval;
    byData[5] = (unsigned char)ID_PacketTail;
    return this->writerData(byData);
}

bool CrawlerSerial::askCrawlerState(EnumAskCrawlerStatus askCrawlerStatus)
{
    configParmeter *setting = configParmeter::getInstance();
    QString strAskCMD = setting->getAskStatueCMD(askCrawlerStatus);
    if(!strAskCMD.isEmpty())
    {
        this->writerData(strAskCMD);
        return true;
    }else
    {
        return false;
    }
}

bool CrawlerSerial::ChangedServoAngle_Hor(quint8 angle)
{
    bool isChanged = false;
    QString strServoCMD;
    QString strAngle;
    configParmeter *setting = configParmeter::getInstance();
    m_servoParam = setting->getServoParam();

    strServoCMD = m_servoParam.cmd_angle_H;
    strAngle = QString::number(angle,16);
    if(strAngle.size() ==1) strAngle.append("0");
    strServoCMD.append(strAngle);
    strServoCMD.append("00");
    //send cmd
    isChanged = this->writerData(strServoCMD);
    //update the setting
    m_servoParam.Angle_Hor = angle;
    setting->writeServoParam(m_servoParam);

    return isChanged;
}

bool CrawlerSerial::ChangedServoAngle_Ver(quint8 angle)
{
    bool isChanged = false;
    QString strServoCMD;
    QString strAngle;
    configParmeter *setting = configParmeter::getInstance();
    m_servoParam = setting->getServoParam();

    strServoCMD = m_servoParam.cmd_angle_V;
    strAngle = QString::number(angle,16);
    if(strAngle.size() ==1) strAngle.append("0");
    strServoCMD.append(strAngle);
    strServoCMD.append("00");
    //send cmd
    isChanged = this->writerData(strServoCMD);
    m_servoParam.Angle_Ver = angle;
    setting->writeServoParam(m_servoParam);

    return isChanged;
}

bool CrawlerSerial::setupLiftShaftMotor(EnumLiftShaftMotorType motorType, EnumLiftShaftStatus status)
{
    configParmeter *setting = configParmeter::getInstance();
    Strulift_shaft liftShaft = setting->getLiftShaftParam();
    QString strCMD;
    switch (motorType)
    {
    case LS_VerMotor:
        switch (status) {
        case LS_MotorStop:
            strCMD = liftShaft.HorMotorStop;
            break;
        case LS_MotorUp:
            strCMD = liftShaft.VerMotorUp;
            break;
        case LS_MotorDown:
             strCMD = liftShaft.VerMotorDown;
            break;
        case LS_MotorInTop:
            break;
        case LS_MotorInBottom:
            break;
        default:
            break;
        }
        break;
    case LS_HorMotor:
        switch (status) {
        case LS_MotorStop:
            strCMD = liftShaft.HorMotorStop;
            break;
        case LS_MotorUp:
            strCMD = liftShaft.HorMotorUp;
            break;
        case LS_MotorDown:
            strCMD = liftShaft.HorMotorDown;
            break;
        case LS_MotorInTop:
            break;
        case LS_MotorInBottom:
            break;
        default:
            break;
        }
        break;
    }
    CrawlerStatusParam *crawlerStatusParam = CrawlerStatusParam::getInstance();
    crawlerStatusParam->keepCurLiftShaftStatus(motorType,status);
    return this->writerData(strCMD);
}

bool CrawlerSerial::enableUTSensor(EnumUTSensorType senType, bool enable)
{
    bool isSended =false;
    switch (senType) {
    case UT_SensorA:
        if(enable)
        {
           isSended = this->writerData(ID_UTDistanceA,UTSen_Enable,Sensor_Open,Sensor_Close);
        }else
        {
           isSended = this->writerData(ID_UTDistanceA,UTSen_Enable,Sensor_Close,Sensor_Close);
        }
        break;
    case UT_SensorB:
        if(enable)
        {
           isSended = this->writerData(ID_UTDistanceB,UTSen_Enable,Sensor_Open,Sensor_Close);
        }else
        {
           isSended = this->writerData(ID_UTDistanceB,UTSen_Enable,Sensor_Close,Sensor_Close);
        }
        break;
    default:
        break;
    }
    return isSended;
}

bool CrawlerSerial::setUTSensorWarningValue(EnumUTSensorType senType, quint16 utValue)
{
    bool isSended =false;
    switch (senType) {
    case UT_SensorA:
        isSended = this->writerData(ID_UTDistanceA,UTSen_WarnDistance,utValue);
        break;
    case UT_SensorB:
        isSended = this->writerData(ID_UTDistanceB,UTSen_WarnDistance,utValue);
        break;
    default:
        break;
    }
    return isSended;
}

bool CrawlerSerial::enablePreSensor(EnumPreSensorType senType, bool enable)
{
    bool isSended =false;
    switch (senType) {
    case Pre_SensorA:
        if(enable)
        {
           isSended = this->writerData(ID_PressureA,UTSen_Enable,Sensor_Open,Sensor_Close);
        }else
        {
           isSended = this->writerData(ID_PressureA,UTSen_Enable,Sensor_Close,Sensor_Close);
        }
        break;
    case Pre_SensorB:
        break;
    default:
        break;
    }
    return isSended;
}

bool CrawlerSerial::setPreSensorWarningValue(EnumPreSensorType senType, quint16 preValue)
{
    bool isSended =false;
    switch (senType) {
    case Pre_SensorA:
        isSended = this->writerData(ID_PressureA,PreSen_WarnPressure,preValue);
        break;
    case Pre_SensorB:
        break;
    default:
        break;
    }
    return isSended;
}

bool CrawlerSerial::setMaxDistanceMove(quint16 value)
{
     bool isSended =false;
     isSended = this->writerData(ID_Distance,SetDistanceMoveValue,value);
     return false;
}

bool CrawlerSerial::setPWMValue(EnumPWMType pwmType, quint16 value)
{
    bool isSended =false;
    CrawlerStatusParam *CrawlerStatus = CrawlerStatusParam::getInstance();
    switch (pwmType) {
    case PWMLeftByConfig:
        isSended = this->writerData(ID_MotorSpeed,SetPWM_Left,value);
        CrawlerStatus->keepCurPWMValueByConfig(PWMLeftByConfig,value);
        break;
    case PWMRightByConfig:
        isSended = this->writerData(ID_MotorSpeed,SetPWM_Right,value);
        CrawlerStatus->keepCurPWMValueByConfig(PWMRightByConfig,value);
        break;
    default:
        break;
    }
    return isSended;
}

bool CrawlerSerial::setSpeedUpByActionMode()
{
    bool isSended =false;
    CrawlerStatusParam *CrawlerStatus = CrawlerStatusParam::getInstance();
    EnumActionMode curMode = CrawlerStatus->getActionMode();
    switch (curMode) {
    case speedMode:
    case distanceMode:
        isSended = this->writerData(ID_MotorSpeed,SpeedIndex_Up,0,0);
        break;
    case difSpeedMode:
        isSended = this->writerData(ID_MotorSpeed,PWM_Up,0,0);
        break;
    default:
        break;
    }
    if(isSended)
    {
        emit this->sgSpeedUpByActionMode();
    }
    return isSended;
}

bool CrawlerSerial::setSpeedDownByActionMode()
{
    bool isSended =false;
    CrawlerStatusParam *CrawlerStatus = CrawlerStatusParam::getInstance();
    EnumActionMode curMode = CrawlerStatus->getActionMode();
    switch (curMode) {
    case speedMode:
    case distanceMode:
        isSended = this->writerData(ID_MotorSpeed,SpeedIndex_Down,0,0);
        break;
    case difSpeedMode:
        isSended = this->writerData(ID_MotorSpeed,PWM_Down,0,0);
        break;
    default:
        break;
    }
    if(isSended)
    {
        emit this->sgSpeedDownByActionMode();
    }
    return isSended;
}

void CrawlerSerial::SendLoopHeartToCrawlerWith(bool isKeep)
{
    if(isKeep)
    {
        this->askCrawlerState(askCrawler_heart);
        this->m_TMHeart->start();
    }else
    {
        this->m_TMHeart->stop();
    }
}

void CrawlerSerial::DealwithSerialDataReceive(QByteArray HexData)
{
    //
    static QByteArray byPacket;
    QByteArray byCommand;
    byPacket.append(HexData);
    bool isHaveHead = byPacket.contains((unsigned char)ID_PacketHead);
    bool isHaveTail = byPacket.contains((unsigned char)ID_PacketTail);

    if( isHaveHead && isHaveTail)
    {
        int packsize,TailPos,HeadPos;
        TailPos = byPacket.indexOf((unsigned char)ID_PacketTail);
        HeadPos = byPacket.indexOf((unsigned char)ID_PacketHead);
        packsize = TailPos - HeadPos;
        if(packsize == PacketLength-1)
        {
            //
            byCommand = byPacket.mid(HeadPos+1,PacketLength-2);
            byPacket  = byPacket.mid(TailPos+1);
            //read a frame
            recognitionCommand(byCommand);
        }
    }
}

void CrawlerSerial::recognitionCommand(QByteArray HexCommand)
{
    unsigned char firstID;
    firstID = HexCommand.at(0);
//    secondID=HexCommand.at(1);
//    thirdID = HexCommand.at(2);
//    fourthID = HexCommand.at(3);

    switch (firstID) {
        case ID_Feedback:
            dealFeedBackCMD(HexCommand);
            break;
        default:
            break;
    }
}

void CrawlerSerial::dealFeedBackCMD(QByteArray &hexCMD)
{
    switch ((unsigned char)hexCMD.at(1)) {
    case FBID_SpeedIndexAll:
    case FBID_SpeedIndexLeft:
    case FBID_SpeedIndexRight:
        this->dealFB_MotorSpeed(hexCMD);
        break;
    case FBID_MoveStatus:
        this->dealFB_MotorStatus(hexCMD);
        break;
    case FBID_DistanceModeStatus:
    case FBID_DistanceValue:
        this->dealFB_DistanceMove(hexCMD);
        break;
    case FBID_ServoAngle:
        this->dealFB_ServoAngle(hexCMD);
        break;
    case FBID_VerMotor:
    case FBID_HorMotor:
        this->dealFB_LiftShaftStatus(hexCMD);
        break;
    case FBID_ValueUTSensorA:
    case FBID_ValueUTSensorB:
    case FBID_ValueUTSensorC:
    case FBID_CurUTSensorStatus:
        this->dealFB_UTSensor(hexCMD);
        break;
    case FBID_ValuePreSenA:
    case FBID_ValuePreSenB:
    case FBID_CurPreSensorStatus:
        this->dealFB_PressureSensor(hexCMD);
        break;
    case FBID_VerLimitSenStatus:
    case FBID_HorLimitSenStatus:
        this->dealFB_LimitSensor(hexCMD);
        break;
    case FBID_PWMValueLeftBySet:
    case FBID_PWMValueRightBySet:
    case FBID_CurPWMValueLeft:
    case FBID_CurPWMValueRight:
        this->dealFB_MotorPWM(hexCMD);
        break;
    default:
        break;
    }
}

void CrawlerSerial::dealFB_MotorSpeed(QByteArray hexCMDData)
{
    CrawlerStatusParam *CrawlerStatus = CrawlerStatusParam::getInstance();
    CrawlerStatus->keepCurCrawlerSpeedIndex(hexCMDData.at(3));
}

void CrawlerSerial::dealFB_MotorStatus(QByteArray hexCMDData)
{
    CrawlerStatusParam *CrawlerStatus = CrawlerStatusParam::getInstance();
    StruSpeedModeParameter speedmode;
    bool ok;
    quint8 ucStop,ucForward,ucBackword,ucLeft,ucRight;
    ucStop = speedmode.stop.mid(3,2).toInt(&ok,16);
    ucForward = speedmode.up.mid(3,2).toInt(&ok,16);
    ucBackword = speedmode.down.mid(3,2).toInt(&ok,16);
    ucLeft = speedmode.left.mid(3,2).toInt(&ok,16);
    ucRight = speedmode.right.mid(3,2).toInt(&ok,16);
    if(ucStop == (unsigned char)hexCMDData.at(1))
    {
        CrawlerStatus->keepCurCrawlerMovingState(crawler_stop);
    }
    if(ucForward == (unsigned char)hexCMDData.at(1))
    {
        CrawlerStatus->keepCurCrawlerMovingState(crawler_forward);
    }
    if(ucBackword == (unsigned char)hexCMDData.at(1))
    {
        CrawlerStatus->keepCurCrawlerMovingState(crawler_backward);
    }
    if(ucLeft ==(unsigned char) hexCMDData.at(1))
    {
        CrawlerStatus->keepCurCrawlerMovingState(crawler_trunleft);
    }
    if(ucRight ==(unsigned char) hexCMDData.at(1))
    {
        CrawlerStatus->keepCurCrawlerMovingState(crawler_trunright);
    }
}

void CrawlerSerial::dealFB_ServoAngle(QByteArray hexCMDData)
{
    //
    CrawlerStatusParam *CrawlerStatus = CrawlerStatusParam::getInstance();
    unsigned char VerAngle,HorAngle;
    VerAngle =(unsigned char) hexCMDData.at(2);
    HorAngle =(unsigned char) hexCMDData.at(3);
    CrawlerStatus->keepCurServoAngleVer(VerAngle);
    CrawlerStatus->keepCurServoAngleHor(HorAngle);
}

void CrawlerSerial::dealFB_LiftShaftStatus(QByteArray hexCMDData)
{
    CrawlerStatusParam *CrawlerStatus = CrawlerStatusParam::getInstance();
    char status;
    switch ((unsigned char)hexCMDData.at(1)) {
    case FBID_VerMotor:
        status = (unsigned char)hexCMDData.at(2);
        CrawlerStatus->keepCurLiftShaftStatus(LS_VerMotor,(EnumLiftShaftStatus)status);
        break;
    case FBID_HorMotor:
        status = (unsigned char)hexCMDData.at(2);
        CrawlerStatus->keepCurLiftShaftStatus(LS_HorMotor,(EnumLiftShaftStatus)status);
        break;
    default:
        break;
    }
}

void CrawlerSerial::dealFB_UTSensor(QByteArray hexCMDData)
{
    CrawlerStatusParam *CrawlerStatus = CrawlerStatusParam::getInstance();
    quint16 DistanceValueA,DistanceValueB,DistanceValueC;
    quint8  UTSenStatusA,UTSenStatusB;
    switch ((unsigned char)hexCMDData.at(1)) {
    case FBID_ValueUTSensorA:
        DistanceValueA = (unsigned char)hexCMDData.at(2)<<8 | (unsigned char)hexCMDData.at(3);
        CrawlerStatus->keepCurUTSensorValue(UT_SensorA,DistanceValueA);
        break;
    case FBID_ValueUTSensorB:
        DistanceValueB =(unsigned char) hexCMDData.at(2)<<8 | (unsigned char)hexCMDData.at(3);
        CrawlerStatus->keepCurUTSensorValue(UT_SensorB,DistanceValueB);
        break;
    case FBID_ValueUTSensorC:
        DistanceValueC = (unsigned char)hexCMDData.at(2)<<8 | (unsigned char)hexCMDData.at(3);
        break;
    case FBID_CurUTSensorStatus:
        UTSenStatusA =(unsigned char) hexCMDData.at(2);
        UTSenStatusB = (unsigned char)hexCMDData.at(3);
        CrawlerStatus->keepCurUTSensorStatus(UT_SensorA,(EnumSensorStatus)UTSenStatusA);
        CrawlerStatus->keepCurUTSensorStatus(UT_SensorB,(EnumSensorStatus)UTSenStatusB);
        break;
    default:
        break;
    }
}

void CrawlerSerial::dealFB_PressureSensor(QByteArray hexCMDData)
{
    CrawlerStatusParam *CrawlerStatus = CrawlerStatusParam::getInstance();
    quint16 pressureValueA,pressureValueB;
    quint8 pressureSenStatusA,pressureSenStatusB;
    switch ((unsigned char)hexCMDData.at(1)) {
    case FBID_ValuePreSenA:
        pressureValueA = (unsigned char)hexCMDData.at(2)<<8 |(unsigned char) hexCMDData.at(3);
        CrawlerStatus->keepCurPressureSensorValue(Pre_SensorA,pressureValueA);
        break;
    case FBID_ValuePreSenB:
        pressureValueB = (unsigned char)hexCMDData.at(2)<<8 |(unsigned char)hexCMDData.at(3);
        CrawlerStatus->keepCurPressureSensorValue(Pre_SensorB,pressureValueB);
        break;
    case FBID_CurPreSensorStatus:
        pressureSenStatusA = (unsigned char)hexCMDData.at(2);
        pressureSenStatusB = (unsigned char)hexCMDData.at(3);
        CrawlerStatus->keepCurPreSenStatus(Pre_SensorA,(EnumSensorStatus)pressureSenStatusA);
        CrawlerStatus->keepCurPreSenStatus(Pre_SensorB,(EnumSensorStatus)pressureSenStatusB);
        break;
    default:
        break;
    }
}

void CrawlerSerial::dealFB_LimitSensor(QByteArray hexCMDData)
{
    CrawlerStatusParam *CrawlerStatus = CrawlerStatusParam::getInstance();
    bool isInTopVer,isInBottomVer;
    bool isInTopHor,isInBottomHor;
    switch ((unsigned char)hexCMDData.at(1)) {
    case FBID_VerLimitSenStatus:
        isInTopVer = ((unsigned char)hexCMDData.at(2)>0)? true:false;
        isInBottomVer = ((unsigned char)hexCMDData.at(3)>0)? true:false;
        CrawlerStatus->keepCurLimitStatus(VerTopLimit,isInTopVer);
        CrawlerStatus->keepCurLimitStatus(VerBottomLimit,isInBottomVer);
        break;
    case FBID_HorLimitSenStatus:
        isInTopHor = ((unsigned char)hexCMDData.at(2)>0)? true:false;
        isInBottomHor = ((unsigned char)hexCMDData.at(3)>0)? true:false;
        CrawlerStatus->keepCurLimitStatus(HorTopLimit,isInTopHor);
        CrawlerStatus->keepCurLimitStatus(HorBottomLimit,isInBottomHor);
        break;
    default:
        break;
    }
}

void CrawlerSerial::dealFB_MotorPWM(QByteArray hexCMDData)
{
    CrawlerStatusParam *CrawlerStatus = CrawlerStatusParam::getInstance();
    quint16 PWMValueLeft,PWMValueRight;
    quint16 CurPWMValueLeft,CurPWMValueRight;
    switch ((unsigned char)hexCMDData.at(1)) {
    case FBID_PWMValueLeftBySet:
        PWMValueLeft = (unsigned char)hexCMDData.at(2)<<8 | (unsigned char)hexCMDData.at(3);
        CrawlerStatus->keepCurPWMValueByConfig(PWMLeftByConfig,PWMValueLeft);
        break;
    case FBID_PWMValueRightBySet:
        PWMValueRight = (unsigned char)hexCMDData.at(2)<<8 | (unsigned char)hexCMDData.at(3);
        CrawlerStatus->keepCurPWMValueByConfig(PWMRightByConfig,PWMValueRight);
        break;
    case FBID_CurPWMValueLeft:
        CurPWMValueLeft = (unsigned char)hexCMDData.at(2)<<8 | (unsigned char)hexCMDData.at(3);
        CrawlerStatus->keepPWMValueEverySecond(CurPWMLeft,CurPWMValueLeft);
        break;
    case FBID_CurPWMValueRight:
        CurPWMValueRight = (unsigned char)hexCMDData.at(2)<<8 | (unsigned char)hexCMDData.at(3);
        CrawlerStatus->keepPWMValueEverySecond(CurPWMRight,CurPWMValueRight);
        break;
    default:
        break;
    }
}

void CrawlerSerial::dealFB_DistanceMove(QByteArray hexCMDData)
{
    quint16 distanceByConfig=0;
    CrawlerStatusParam *CrawlerStatus = CrawlerStatusParam::getInstance();
    switch ((unsigned char)hexCMDData.at(1)) {
    case FBID_DistanceModeStatus:
        switch ((unsigned char)hexCMDData.at(2)) {
        case Disp_MoveStop:
        case Disp_MoveForwrad:
        case Disp_MoveBackwrad:
        case Disp_FinishMove:
            CrawlerStatus->keepCurDispMoveStatus((EnumDispModeStatus)(unsigned char)hexCMDData.at(2));
            break;
        default:
            CrawlerStatus->keepCurDispMoveStatus(Disp_MoveStop);
            break;
        }
        break;
    case FBID_DistanceValue:
        distanceByConfig = (unsigned char)hexCMDData.at(2) | (unsigned char)hexCMDData.at(3);
        CrawlerStatus->keepMoveDistanceValue(distanceByConfig);
        break;
    default:
        break;
    }
}

void CrawlerSerial::slReadData()
{
    QByteArray readData = m_crawlerSerial->readAll();
    if(readData.isEmpty()) return ;
    DealwithSerialDataReceive(readData);
}

void CrawlerSerial::slChangedServoAngle(Qt::Key key)
{
    QString strServoCMD;
    QString strAngle;
    configParmeter *setting = configParmeter::getInstance();
    m_servoParam = setting->getServoParam();
    switch(key)
    {
    case KEY_SEVOER_H_UP:
        //get the angle and add one angel and send cmd;
        if(m_servoParam.Angle_Hor<m_servoParam.MaxAngle_Hor && m_servoParam.Angle_Hor >=m_servoParam.MinAngle_Hor)
        {
            strServoCMD = m_servoParam.cmd_angle_H;
            m_servoParam.Angle_Hor = m_servoParam.Angle_Hor +1;
            strAngle = QString::number(m_servoParam.Angle_Hor,16);
            if(strAngle.size() ==1) strAngle.append("0");
            strServoCMD.append(strAngle);
            strServoCMD.append("00");
            //send cmd
            this->writerData(strServoCMD);
            setting->writeServoParam(m_servoParam);
        }
        break;
    case KEY_SEVOER_H_DOWN:
        //get the angle and add one angel and send cmd;
        if(m_servoParam.Angle_Hor<=m_servoParam.MaxAngle_Hor && m_servoParam.Angle_Hor >m_servoParam.MinAngle_Hor)
        {
            strServoCMD = m_servoParam.cmd_angle_H;
            m_servoParam.Angle_Hor = m_servoParam.Angle_Hor -1;
            strAngle = QString::number(m_servoParam.Angle_Hor,16);
            if(strAngle.size() ==1) strAngle.append("0");
            strServoCMD.append(strAngle);
            strServoCMD.append("00");
            //send cmd
            this->writerData(strServoCMD);
            setting->writeServoParam(m_servoParam);
        }
        break;
    case KEY_SEVOER_V_UP:
        //get the angle and add one angel and send cmd;
        if(m_servoParam.Angle_Ver<=m_servoParam.MaxAngle_Ver && m_servoParam.Angle_Ver >m_servoParam.MinAngle_Ver)
        {
            strServoCMD = m_servoParam.cmd_angle_V;
            m_servoParam.Angle_Ver = m_servoParam.Angle_Ver -1;
            strAngle = QString::number(m_servoParam.Angle_Ver,16);
            if(strAngle.size() ==1) strAngle.append("0");
            strServoCMD.append(strAngle);
            strServoCMD.append("00");
            //send cmd
            this->writerData(strServoCMD);
            setting->writeServoParam(m_servoParam);
        }
        break;
    case KEY_SEVOER_V_DOWN:
        if(m_servoParam.Angle_Ver<=m_servoParam.MaxAngle_Ver && m_servoParam.Angle_Ver >m_servoParam.MinAngle_Ver)
        {
            strServoCMD = m_servoParam.cmd_angle_V;
            m_servoParam.Angle_Ver = m_servoParam.Angle_Ver -1;
            strAngle = QString::number(m_servoParam.Angle_Ver,16);
            if(strAngle.size() ==1) strAngle.append("0");
            strServoCMD.append(strAngle);
            strServoCMD.append("00");
            //send cmd
            this->writerData(strServoCMD);
            setting->writeServoParam(m_servoParam);
        }
        break;
    }
}

void CrawlerSerial::slSerialError()
{
    qDebug("serial erro");
}

void CrawlerSerial::slLoopSendHeartToCrawler()
{
    this->askCrawlerState(askCrawler_heart);
}

CrawlerSerial::CrawlerSerial(QObject *parent) : QObject(parent),
    m_isSerialInit(false),
    m_TMHeart(new QTimer)
{
    m_TMHeart->setInterval(HeartLoopSendInterval);
    m_TMHeart->stop();
    connect(m_TMHeart,SIGNAL(timeout()),this,SLOT(slLoopSendHeartToCrawler()));
}

