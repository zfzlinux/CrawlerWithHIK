#include "configparmeter.h"
#include <QCoreApplication>
#include <QFile>
#include <QMessageBox>

configParmeter *configParmeter::m_pInstance;

configParmeter *configParmeter::getInstance()
{
    if(m_pInstance ==NULL)
        m_pInstance = new configParmeter();
    return m_pInstance;
}

configParmeter::configParmeter()
    :QObject(),
      actionMode(speedMode)
{
    this->initSettings();
}

void configParmeter::initSettings()
{
    QString iniFilePath = QCoreApplication::applicationDirPath();
    iniFilePath.append("/config.ini");
    //the ini file is exist?
    QFile file(iniFilePath);
    if(!file.exists())
    {
        //creat file
        file.setFileName("config.ini");
        file.open(QIODevice::WriteOnly);
        file.close();
    }
    m_settings = new QSettings(iniFilePath,QSettings::IniFormat);
    m_listSerialInfo = this->readSerialInfo();
    m_listCameraCntCfg = this->readCameraCntConfig();
    m_stallParam = this->readCrawlerStall();
    m_packetConfig = this->readPacketConfig();
    m_speedMdoeParam = this->readSpeedMode();
    m_dispModeParam = this->readDispMode();
    m_difSpeedModeParam = this->readDifSpeedMode();
    m_servoParam = this->readServo();
    m_liftShaftParam =this->readLiftShaft();
    m_ackStatesParam = this->readAckStates();
    m_savePicturePath = this->readSavePicturePath();;
    m_saveVideoPath = this->readSaveVideoPath();

    this->initKeyToDirectionMap();
    this->initAskStatusMap();
}

QList<StruSerialInfo> configParmeter::getSerialInfo()
{
    return this->m_listSerialInfo;
}

QList<StruCameraCntCfg> configParmeter::getCameraCntConfigInfo()
{
    return this->m_listCameraCntCfg;
}

StruStallParameter configParmeter::getCrawlerStall()
{
    return this->m_stallParam;
}

StruPacketParameter configParmeter::getPacketConfig()
{
    return this->m_packetConfig;
}

StruSpeedModeParameter configParmeter::getSpeedModeParam()
{
    return this->m_speedMdoeParam;
}

StruDisplacementModeParameter configParmeter::getDisplacementModeParam()
{
    return this->m_dispModeParam;
}

StruDifSpeedModeParameter configParmeter::getDifSpeedModeParam()
{
    return this->m_difSpeedModeParam;
}

StruServo_Angle configParmeter::getServoParam()
{
    return this->m_servoParam;
}

Strulift_shaft configParmeter::getLiftShaftParam()
{
    return this->m_liftShaftParam;
}

StruAckStates configParmeter::getAckStatuesParam()
{
    return this->m_ackStatesParam;
}

QString configParmeter::getSavePicturePath()
{
    return this->m_savePicturePath;
}

QString configParmeter::getSaveVideoPath()
{
    return this->m_saveVideoPath;
}

void configParmeter::writeSerialInfo(QList<StruSerialInfo> &listStruSerialInfo)
{
    m_settings->beginWriteArray(PREFIX_SERIAL);
    int size = listStruSerialInfo.size();
    for(int i =0;i<size ;i ++)
    {
        m_settings->setArrayIndex(i);
        m_settings->setValue(KEY_PORTNAME,listStruSerialInfo.at(i).PortName);
        m_settings->setValue(KEY_BAUDRATE,listStruSerialInfo.at(i).m_BaudRate);
        m_settings->setValue(KEY_DATABITS,listStruSerialInfo.at(i).m_DataBits);
        m_settings->setValue(KEY_PARITY,listStruSerialInfo.at(i).m_Parity);
        m_settings->setValue(KEY_STOPBITS,listStruSerialInfo.at(i).m_StopBits);
        m_settings->setValue(KEY_FLOWCONTROL,listStruSerialInfo.at(i).m_FlowControl);
    }
    m_settings->endArray();
    m_listSerialInfo = listStruSerialInfo;
    emit this->sgUpdataSerialInfo();
}

QList<StruSerialInfo> configParmeter::readSerialInfo()
{
    QList<StruSerialInfo> listStruSerialInfo;
    StruSerialInfo struSerialInfo;
    int size = m_settings->beginReadArray(PREFIX_SERIAL);
    for(int i=0;i<size;i++)
    {
        m_settings->setArrayIndex(i);
        struSerialInfo.PortName         = m_settings->value(KEY_PORTNAME).toString();
        struSerialInfo.m_BaudRate       = (StruSerialInfo::BaudRate)m_settings->value(KEY_BAUDRATE).toInt();
        struSerialInfo.m_DataBits       = (StruSerialInfo::DataBits)m_settings->value(KEY_DATABITS).toInt();
        struSerialInfo.m_Parity         = (StruSerialInfo::Parity)m_settings->value(KEY_PARITY).toInt();
        struSerialInfo.m_StopBits       = (StruSerialInfo::StopBits)m_settings->value(KEY_STOPBITS).toInt();
        struSerialInfo.m_FlowControl    = (StruSerialInfo::FlowControl)m_settings->value(KEY_FLOWCONTROL).toInt();
        listStruSerialInfo.append(struSerialInfo);
    }
    m_settings->endArray();
    return listStruSerialInfo;
}

//#define PREFIX_CAMERA   "CameraDevice"
//#define KEY_IPADDRESS   "IPAddress"
//#define KEY_USERNAME    "UserName"
//#define KEY_PASSWORD    "Password"
//#define KEY_PORT        "Port"

void configParmeter::writeCameraCntConfigInfo(QList<StruCameraCntCfg> &listStruCamerConfig)
{
    m_settings->beginWriteArray(PREFIX_CAMERA);
    int size = listStruCamerConfig.size();
    for(int i =0;i<size ;i ++)
    {
        m_settings->setArrayIndex(i);
        m_settings->setValue(KEY_IPADDRESS,listStruCamerConfig.at(i).IPAddress);
        m_settings->setValue(KEY_USERNAME,listStruCamerConfig.at(i).UserName);
        m_settings->setValue(KEY_PASSWORD,listStruCamerConfig.at(i).Password);
        m_settings->setValue(KEY_PORT,listStruCamerConfig.at(i).port);
    }
    m_settings->endArray();
    m_listCameraCntCfg = listStruCamerConfig;
    emit sgUpdateCameraInfo();
}

void configParmeter::writeCrawlerStall(StruStallParameter &stallParam)
{

}

void configParmeter::writePacketConfig(StruPacketParameter &packetConfig)
{

}

void configParmeter::writeSpeedModeParam(StruSpeedModeParameter &speedModeParam)
{

}

void configParmeter::writeDisplacementParam(StruDisplacementModeParameter &dispParam)
{

}

void configParmeter::writeDifSpeedModeParam(StruDifSpeedModeParameter &difParam)
{

}

void configParmeter::writeServoParam(StruServo_Angle &servoParam)
{

}

void configParmeter::writeLiftShaftParam(Strulift_shaft &liftShaftParam)
{

}

void configParmeter::writeAckStatesParam(StruAckStates &ackStatesParam)
{

}

void configParmeter::writeSavePicturePath(QString path)
{
    this->m_savePicturePath = path;
    m_settings->setValue(SAVEPATH_Picture,m_savePicturePath);
}

void configParmeter::writeSaveVideoPath(QString path)
{
    this->m_saveVideoPath =path;
    m_settings->setValue(SAVEPATH_Video,m_saveVideoPath);
}

QList<StruCameraCntCfg> configParmeter::readCameraCntConfig()
{
    QList<StruCameraCntCfg> listStruCamera;
    StruCameraCntCfg struCamerCntCfg;
    int size = m_settings->beginReadArray(PREFIX_CAMERA);
    for(int i=0;i<size;i++)
    {
        m_settings->setArrayIndex(i);
        struCamerCntCfg.IPAddress = (QString)m_settings->value(KEY_IPADDRESS).toString();
        struCamerCntCfg.UserName = (QString)m_settings->value(KEY_USERNAME).toString();
        struCamerCntCfg.Password = (QString)m_settings->value(KEY_PASSWORD).toString();
        struCamerCntCfg.port = (quint32)m_settings->value(KEY_PORT).toUInt();
        listStruCamera.append(struCamerCntCfg);
    }
    m_settings->endArray();
    return listStruCamera;
}

StruStallParameter configParmeter::readCrawlerStall()
{
    StruStallParameter stallconfig;
    //
    QString key ,maxKey,speedValue;
    key.append(PREFIX_STALL);
    key.append('/');

    maxKey.append(key);
    maxKey.append(MaxSTALL);
    stallconfig.speed = new float[255];
    stallconfig.max = m_settings->value(maxKey).toInt();

    for(int i=0;i< stallconfig.max;i++)
    {
        speedValue.append(key);
        speedValue.append('/');
        speedValue.append(PREFIX_STALL);
        speedValue.setNum(i,10);
        stallconfig.speed[i] = m_settings->value(speedValue).toFloat();
        speedValue.clear();
    }
    return stallconfig;
}

StruPacketParameter configParmeter::readPacketConfig()
{
    StruPacketParameter packetParam;
    packetParam.head = m_settings->value(PACKET_HEAD).toString();
    packetParam.tail = m_settings->value(PACKET_TAIL).toString();
    return packetParam;
}

StruSpeedModeParameter configParmeter::readSpeedMode()
{
    StruSpeedModeParameter speedMode;
    speedMode.up = m_settings->value(SPEEDMODE_UP).toString();
    speedMode.down = m_settings->value(SPEEDMODE_down).toString();
    speedMode.left = m_settings->value(SPEEDMODE_left).toString();
    speedMode.right = m_settings->value(SPEEDMODE_right).toString();
    speedMode.stop = m_settings->value(SPEEDMODE_stop).toString();
    speedMode.front_left = m_settings->value(SPEEDMODE_front_left).toString();
    speedMode.front_right = m_settings->value(SPEEDMODE_front_right).toString();
    speedMode.speedUp = m_settings->value(SPEEDMODE_speedUP).toString();
    speedMode.speedDown = m_settings->value(SPEEDMODE_speedDown).toString();
    speedMode.leftWheelStall = m_settings->value(SPEEDMODE_leftStall).toString();
    speedMode.rightWheelStall = m_settings->value(SPEEDMODE_rightStall).toString();
    speedMode.is_stop =true;

    speedModeCMDMap.insert(em_stop,speedMode.stop);
    speedModeCMDMap.insert(em_forworad,speedMode.up);
    speedModeCMDMap.insert(em_backward,speedMode.down);
    speedModeCMDMap.insert(em_left,speedMode.left);
    speedModeCMDMap.insert(em_right,speedMode.right);
    speedModeCMDMap.insert(em_frontLeft,speedMode.front_left);
    speedModeCMDMap.insert(em_frontRight,speedMode.front_right);
    speedModeCMDMap.insert(em_speedUP,speedMode.speedUp);
    speedModeCMDMap.insert(em_speedDown,speedMode.speedDown);

    return speedMode;
}

StruDisplacementModeParameter configParmeter::readDispMode()
{
    StruDisplacementModeParameter dispMode;

    if(ENABLE_PRIVATE_SERIAL)
    {
        dispMode.up = m_settings->value(DispMode_up).toString();
        dispMode.down = m_settings->value(DispMode_down).toString();
        dispMode.stop = m_settings->value(DispMode_stop).toString();
    }
    if(ENABLE_MODBUS)
    {
        dispMode.up = m_settings->value(SPEEDMODE_UP).toString();
        dispMode.down = m_settings->value(SPEEDMODE_down).toString();
        dispMode.stop = m_settings->value(SPEEDMODE_stop).toString();
    }
    dispMode.left = m_settings->value(SPEEDMODE_left).toString();
    dispMode.right = m_settings->value(SPEEDMODE_right).toString();

    dispMode.speedUp = m_settings->value(DispMode_speedup).toString();
    dispMode.speedDown = m_settings->value(DispMode_speedDown).toString();
    dispMode.displacement = m_settings->value(DispMode_displacement).toString();
    dispMode.leftWheelStall = m_settings->value(DispMode_leftWheelStall).toString();
    dispMode.rightWheelStall = m_settings->value(DispMode_rightWheelStall).toString();

    distanceModeCMDMap.insert(em_stop,dispMode.stop);
    distanceModeCMDMap.insert(em_forworad,dispMode.up);
    distanceModeCMDMap.insert(em_backward,dispMode.down);
    distanceModeCMDMap.insert(em_left,dispMode.left);
    distanceModeCMDMap.insert(em_right,dispMode.right);
//    distanceModeCMDMap.insert(em_frontLeft,dispMode.front_left);
//    distanceModeCMDMap.insert(em_frontRight,dispMode.front_right);
    distanceModeCMDMap.insert(em_speedUP,dispMode.speedUp);
    distanceModeCMDMap.insert(em_speedDown,dispMode.speedDown);

    return dispMode;
}

StruDifSpeedModeParameter configParmeter::readDifSpeedMode()
{
    StruDifSpeedModeParameter difSpeedMode;
    //
    difSpeedMode.stop = m_settings->value(DifSpeedMode_stop).toString();
    difSpeedMode.up = m_settings->value(DifSpeedMode_up).toString();
    difSpeedMode.down = m_settings->value(DifSpeedMode_down).toString();
    difSpeedMode.left = m_settings->value(DifSpeedMode_left).toString();
    difSpeedMode.right = m_settings->value(DifSpeedMode_right).toString();
    difSpeedMode.speedUP = m_settings->value(DifSpeedMode_speedUp).toString();
    difSpeedMode.speedDown = m_settings->value(DifSpeedMode_speedDown).toString();

    difspeedModeCMDMap.insert(em_stop,difSpeedMode.stop);
    difspeedModeCMDMap.insert(em_forworad,difSpeedMode.up);
    difspeedModeCMDMap.insert(em_backward,difSpeedMode.down);
    difspeedModeCMDMap.insert(em_left,difSpeedMode.left);
    difspeedModeCMDMap.insert(em_right,difSpeedMode.right);
    difspeedModeCMDMap.insert(em_speedUP,difSpeedMode.speedUP);
    difspeedModeCMDMap.insert(em_speedDown,difSpeedMode.speedDown);

    return difSpeedMode;
}

StruServo_Angle configParmeter::readServo()
{
    StruServo_Angle servoAngle;
    servoAngle.cmd_init = m_settings->value(ServoCtrl_init).toString();
    servoAngle.cmd_angle_H = m_settings->value(ServoCtrl_cmdAngle_H).toString();
    servoAngle.cmd_angle_V = m_settings->value(ServoCtrl_cmdAngle_V).toString();
    servoAngle.MaxAngle_Hor = (quint8)m_settings->value(ServoCtrl_MaxAngle_Hor).toInt();
    servoAngle.MinAngle_Hor = (quint8)m_settings->value(ServoCtrl_MinAngle_Hor).toInt();
    servoAngle.Angle_Hor = (quint8)m_settings->value(ServoCtrl_StartAngle_Hor).toInt();
    servoAngle.MaxAngle_Ver =(quint8) m_settings->value(ServoCtrl_MaxAngle_Ver).toInt();
    servoAngle.MinAngle_Ver =(quint8) m_settings->value(ServoCtrl_MinAngle_Ver).toInt();
    servoAngle.Angle_Ver =(quint8) m_settings->value(ServoCtrl_StartAngle_Ver).toInt();
    return servoAngle;
}

Strulift_shaft configParmeter::readLiftShaft()
{
    Strulift_shaft liftShaft;
    liftShaft.VerMotorStop = m_settings->value(LiftShaft_VMstop).toString();
    liftShaft.VerMotorUp = m_settings->value(LiftShaft_VMup).toString();
    liftShaft.VerMotorDown = m_settings->value(LiftShaft_VMdown).toString();
    liftShaft.HorMotorStop = m_settings->value(LiftShaft_HMstop).toString();
    liftShaft.HorMotorUp = m_settings->value(LiftShaft_HMup).toString();
    liftShaft.HorMotorDown = m_settings->value(LiftShaft_HMdown).toString();
    return liftShaft;
}

StruAckStates configParmeter::readAckStates()
{
    StruAckStates ackStates;
    ackStates.speed_all = m_settings->value(ACKSTATES_speedAll).toString();
    ackStates.speed_left = m_settings->value(ACKSTATES_speedleft).toString();
    ackStates.speed_right = m_settings->value(ACKSTATES_speedright).toString();
    ackStates.motionStatus = m_settings->value(ACKSTATES_motion).toString();
    ackStates.distanceStatus =m_settings->value(ACKSTATES_distance).toString();
    ackStates.curDistance_pulse = m_settings->value(ACKSTATES_distance_pulse).toString();
    ackStates.servo_angle = m_settings->value(ACKSTATES_servo).toString();
    ackStates.heart = m_settings->value(ACKSTATES_heart).toString();
    ackStates.verMotor_LF = m_settings->value(ACKSTATES_verMotor_LF).toString();
    ackStates.horMotor_LF = m_settings->value(ACKSTATES_horMotor_LF).toString();
    ackStates.PWMValue_left = m_settings->value(ACKSTATES_PWMValue_left).toString();
    ackStates.PWMValue_right = m_settings->value(ACKSTATES_PWMValue_right).toString();
    ackStates.curPWMValue_left = m_settings->value(ACKSTATES_curPWMValue_left).toString();
    ackStates.curPWMValue_right = m_settings->value(ACKSTATES_curPWMValue_right).toString();
    ackStates.curUTSensorValueA = m_settings->value(ACKSTATES_curUTSensorValueA).toString();
    ackStates.curUTSensorValueB = m_settings->value(ACKSTATES_curUTSensorValueB).toString();
    ackStates.curPerSensorValueA = m_settings->value(ACKSTATES_curPerSensorValueA).toString();
    ackStates.curPerSensorValueB = m_settings->value(ACKSTATES_curPerSensorValueB).toString();
    ackStates.curVerLimitStatus = m_settings->value(ACKSTATES_curVerLimitStatus).toString();
    ackStates.curHorLimitStatus = m_settings->value(ACKSTATES_curHorLimitStatus).toString();
    ackStates.curUTSensorStatus = m_settings->value(ACKSTATES_curUTSensorStatus).toString();
    ackStates.curPerSensorStatus = m_settings->value(ACKSTATES_curPerSensorStatus).toString();

    return ackStates;
}

QString configParmeter::readSavePicturePath()
{
    return m_settings->value(SAVEPATH_Picture).toString();
}

QString configParmeter::readSaveVideoPath()
{
    return m_settings->value(SAVEPATH_Video).toString();
}

void configParmeter::initKeyToDirectionMap()
{
    this->keyToDirMap.insert(KEY_FORWARD,em_forworad);
    this->keyToDirMap.insert(KEY_BACKWARD,em_backward);
    this->keyToDirMap.insert(KEY_GOLEFT,em_left);
    this->keyToDirMap.insert(KEY_GORIGHT,em_right);
    this->keyToDirMap.insert(KEY_FRONT_LEFT,em_frontLeft);
    this->keyToDirMap.insert(KEY_FRONT_RIGHT,em_frontRight);
    this->keyToDirMap.insert(KEY_SPEED_UP,em_speedUP);
    this->keyToDirMap.insert(KEY_SPEED_DOWN,em_speedDown);
//    this->keyToDirMap.insert(KEY_SEVOER_H_UP,em_forworad);
//    this->keyToDirMap.insert(KEY_SEVOER_H_DOWN,em_forworad);
//    this->keyToDirMap.insert(KEY_SEVOER_V_UP,em_forworad);
//    this->keyToDirMap.insert(KEY_SEVOER_V_DOWN,em_forworad);
//    this->keyToDirMap.insert(KEY_LIFT_SHAFT_UP,em_forworad);
    //    this->keyToDirMap.insert(KEY_LIFT_SHAFT_DOWN,em_forworad);
}

void configParmeter::initAskStatusMap()
{
    this->askCrawlerCMDMap.insert(askCrawler_speed_all,m_ackStatesParam.speed_all);
    this->askCrawlerCMDMap.insert(askCrawler_speed_left,m_ackStatesParam.speed_left);
    this->askCrawlerCMDMap.insert(askCrawler_speed_right,m_ackStatesParam.speed_right);
    this->askCrawlerCMDMap.insert(askCrawler_motion,m_ackStatesParam.motionStatus);
    this->askCrawlerCMDMap.insert(askCrawler_distance,m_ackStatesParam.distanceStatus);
    this->askCrawlerCMDMap.insert(askCrawler_distance_pluse,m_ackStatesParam.curDistance_pulse);
    this->askCrawlerCMDMap.insert(askCrawler_servo,m_ackStatesParam.servo_angle);
    this->askCrawlerCMDMap.insert(askCrawler_heart,m_ackStatesParam.heart);
    this->askCrawlerCMDMap.insert(askCrawler_verMotor_LF,m_ackStatesParam.verMotor_LF);
    this->askCrawlerCMDMap.insert(askCrawler_horMotor_LF,m_ackStatesParam.horMotor_LF);
    this->askCrawlerCMDMap.insert(askCrawler_PWMValue_left,m_ackStatesParam.PWMValue_left);
    this->askCrawlerCMDMap.insert(askCrawler_PWMValue_right,m_ackStatesParam.PWMValue_right);
    this->askCrawlerCMDMap.insert(askCrawler_curPWM_left,m_ackStatesParam.curPWMValue_left);
    this->askCrawlerCMDMap.insert(askCrawler_curPWM_right,m_ackStatesParam.curPWMValue_right);
    this->askCrawlerCMDMap.insert(askCrawler_valueUTSensorA,m_ackStatesParam.curUTSensorValueA);
    this->askCrawlerCMDMap.insert(askCrawler_valueUTSensorB,m_ackStatesParam.curUTSensorValueB);
    this->askCrawlerCMDMap.insert(askCrawler_valuePressureA,m_ackStatesParam.curPerSensorValueA);
    this->askCrawlerCMDMap.insert(askCrawler_valuePressureB,m_ackStatesParam.curPerSensorValueB);
    this->askCrawlerCMDMap.insert(askCrawler_curVerLimitStatus,m_ackStatesParam.curVerLimitStatus);
    this->askCrawlerCMDMap.insert(askCrawler_curHorLimitStatus,m_ackStatesParam.curHorLimitStatus);
    this->askCrawlerCMDMap.insert(askCrawler_UTSensorStatus,m_ackStatesParam.curUTSensorStatus);
    this->askCrawlerCMDMap.insert(askCrawler_PressureSensorStatus,m_ackStatesParam.curPerSensorStatus);
}

EnumActionMode configParmeter::getActionMode()
{
    return this->actionMode;
}

void configParmeter::setActionMode(EnumActionMode mode)
{
    this->actionMode = mode;
}

QString configParmeter::getMotorCMDByMode( EnumDirectionKey key)
{
    QString strCMD;
    switch (actionMode) {
    case speedMode:
        strCMD = speedModeCMDMap.value(key);
        break;
    case distanceMode:
        strCMD =distanceModeCMDMap.value(key);
        break;
    case difSpeedMode:
        strCMD =difspeedModeCMDMap.value(key);
        break;
    default:
        break;
    }
    return strCMD;
}

EnumCrawlerMovingState configParmeter::getMotorStatusFlag(EnumDirectionKey key)
{
    QString temp;
    quint16 icmd =0;
    EnumCrawlerMovingState status = crawler_stop;
    QString strCMD = this->getMotorCMDByMode(key);
    if(strCMD.size()>=5)
    {
      temp.append(strCMD.at(3));
      temp.append(strCMD.at(4));
      icmd = temp.toUInt();
    }
//    if (distanceMode == actionMode && icmd >0)
//    {
//        icmd = icmd-1;//为了兼容之前的协议。权宜之计
//    }
    status = (EnumCrawlerMovingState)icmd;
    return status;
}

QString configParmeter::getMotorCMDByModeQtKey(Qt::Key qtkey)
{
   EnumDirectionKey dirKey;
   dirKey = keyToDirMap.value(qtkey);
   return this->getMotorCMDByMode(dirKey);
}

QString configParmeter::getAskStatueCMD(EnumAskCrawlerStatus askCrawlerStatus)
{
    return this->askCrawlerCMDMap.value(askCrawlerStatus);
}




