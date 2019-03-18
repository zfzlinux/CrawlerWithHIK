#ifndef CONFIGPARMETER_H
#define CONFIGPARMETER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSettings>
#include <QMap>
#include "globaldefine.h"


//SerialDevice
#define PREFIX_SERIAL   "SerialDevice"
#define KEY_PORTNAME    "PortName"
#define KEY_BAUDRATE    "BaudRate"
#define KEY_DATABITS    "DataBits"
#define KEY_PARITY      "Parity"
#define KEY_STOPBITS    "StopBits"
#define KEY_FLOWCONTROL "FlowControl"
//CameraDevice
#define PREFIX_CAMERA   "CameraDevice"
#define KEY_IPADDRESS   "IPAddress"
#define KEY_USERNAME    "UserName"
#define KEY_PASSWORD    "Password"
#define KEY_PORT        "Port"

//stall
#define PREFIX_STALL    "STALL"
#define MaxSTALL        "MAX"

//Packet
#define PACKET_HEAD     "SERIAL_PACKET/HEAD"
#define PACKET_TAIL     "SERIAL_PACKET/TAIL"

//speed mode
#define SPEEDMODE_UP    "/SPEED_MODE/UP"
#define SPEEDMODE_down  "/SPEED_MODE/DOWN"
#define SPEEDMODE_left  "/SPEED_MODE/LEFT"
#define SPEEDMODE_right "/SPEED_MODE/RIGHT"
#define SPEEDMODE_stop  "/SPEED_MODE/STOP"
#define SPEEDMODE_front_left  "/SPEED_MODE/FRONT_LEFT"
#define SPEEDMODE_front_right "/SPEED_MODE/FRONT_RIGHT"
#define SPEEDMODE_speedUP "/SPEED_MODE/SPEED_UP"
#define SPEEDMODE_speedDown "/SPEED_MODE/SPEED_DOWN"
#define SPEEDMODE_leftStall "/SPEED_MODE/LEFT_WHEEL_STALL"
#define SPEEDMODE_rightStall "/SPEED_MODE/RIGHT_WHEEL_STALL"

//displacement mode
#define DispMode_up                 "/DISPLACEMENT_MODE/UP"
#define DispMode_down               "/DISPLACEMENT_MODE/DOWN"
#define DispMode_stop               "/DISPLACEMENT_MODE/STOP"
#define DispMode_speedup            "/DISPLACEMENT_MODE/SPEED_UP"
#define DispMode_speedDown          "/DISPLACEMENT_MODE/SPEED_DOWN"
#define DispMode_displacement       "/DISPLACEMENT_MODE/SPEED_DOWN"
#define DispMode_leftWheelStall     "/DISPLACEMENT_MODE/LEFT_WHEEL_STALL"
#define DispMode_rightWheelStall    "/DISPLACEMENT_MODE/RIGHT_WHEEL_STALL"
//different speed mode
#define DifSpeedMode_stop            "/DIF_SPEED_MODE/STOP"
#define DifSpeedMode_up              "/DIF_SPEED_MODE/UP"
#define DifSpeedMode_down            "/DIF_SPEED_MODE/DOWN"
#define DifSpeedMode_left            "/DIF_SPEED_MODE/LEFT"
#define DifSpeedMode_right           "/DIF_SPEED_MODE/RIGHT"
#define DifSpeedMode_speedUp         "/DIF_SPEED_MODE/SPEED_UP"
#define DifSpeedMode_speedDown       "/DIF_SPEED_MODE/SPEED_DOWN"

//#define DifSpeedMode_all             "/DIF_SPEED_MODE/PWM_ALL"
//#define DifSpeedMode_pwmMin          "/DIF_SPEED_MODE/MIN"
//#define DifSpeedMode_pwmMax          "/DIF_SPEED_MODE/MAX"
//#define DifSpeedMode_pwmStep         "/DIF_SPEED_MODE/STEP"
//#define DifSpeedMode_pwmToSpeed      "/DIF_SPEED_MODE/PWMTOSPEED"

#define ServoCtrl_init               "/SERVO_ANGLE/SERVO_INIT"
#define ServoCtrl_cmdAngle_H         "/SERVO_ANGLE/SERVO_CMD_H"
#define ServoCtrl_cmdAngle_V         "/SERVO_ANGLE/SERVO_CMD_V"
#define ServoCtrl_MaxAngle_Hor       "/SERVO_ANGLE/MAXANGLE_HOR"
#define ServoCtrl_MinAngle_Hor       "/SERVO_ANGLE/MINANGLE_HOR"
#define ServoCtrl_StartAngle_Hor     "/SERVO_ANGLE/STARTANGLE_HOR"
#define ServoCtrl_MaxAngle_Ver       "/SERVO_ANGLE/MAXANGLE_VER"
#define ServoCtrl_MinAngle_Ver       "/SERVO_ANGLE/MINANGLE_VER"
#define ServoCtrl_StartAngle_Ver     "/SERVO_ANGLE/STARTANGLE_VER"

#define LiftShaft_VMstop               "/LIFT_SHAFT/LIFT_SHAFT_VMSTOP"
#define LiftShaft_VMup                 "/LIFT_SHAFT/LIFT_SHAFT_VMUP"
#define LiftShaft_VMdown               "/LIFT_SHAFT/LIFT_SHAFT_VMDOWN"

#define LiftShaft_HMstop               "/LIFT_SHAFT/LIFT_SHAFT_HMSTOP"
#define LiftShaft_HMup                 "/LIFT_SHAFT/LIFT_SHAFT_HMUP"
#define LiftShaft_HMdown               "/LIFT_SHAFT/LIFT_SHAFT_HMDOWN"

#define ACKSTATES_speedAll           "/ACK_STATES_CAR/STATE_SPEED_ALL"
#define ACKSTATES_speedleft          "/ACK_STATES_CAR/STATE_SPEED_LEFT"
#define ACKSTATES_speedright         "/ACK_STATES_CAR/STATE_SPEED_RIGHT"
#define ACKSTATES_motion             "/ACK_STATES_CAR/STATE_MOTION"
#define ACKSTATES_distance           "/ACK_STATES_CAR/STATE_DISTANCE"
#define ACKSTATES_distance_pulse     "/ACK_STATES_CAR/STATE_DISTANCE_PULSE"
#define ACKSTATES_servo              "/ACK_STATES_CAR/STATE_SERVO"
#define ACKSTATES_heart              "/ACK_STATES_CAR/STATE_HEART"
#define ACKSTATES_verMotor_LF              "/ACK_STATES_CAR/STATE_verMotor_LF"
#define ACKSTATES_horMotor_LF              "/ACK_STATES_CAR/STATE_horMotor_LF"
#define ACKSTATES_PWMValue_left              "/ACK_STATES_CAR/STATE_PWMValue_left"
#define ACKSTATES_PWMValue_right              "/ACK_STATES_CAR/STATE_PWMValue_right"
#define ACKSTATES_curPWMValue_left              "/ACK_STATES_CAR/STATE_curPWMValue_left"
#define ACKSTATES_curPWMValue_right              "/ACK_STATES_CAR/STATE_curPWMValue_right"
#define ACKSTATES_curUTSensorValueA              "/ACK_STATES_CAR/STATE_curUTSensorValueA"
#define ACKSTATES_curUTSensorValueB              "/ACK_STATES_CAR/STATE_curUTSensorValueB"
#define ACKSTATES_curPerSensorValueA              "/ACK_STATES_CAR/STATE_curPerSensorValueA"
#define ACKSTATES_curPerSensorValueB              "/ACK_STATES_CAR/STATE_curPerSensorValueB"
#define ACKSTATES_curVerLimitStatus             "/ACK_STATES_CAR/STATE_curVerLimitStatus"
#define ACKSTATES_curHorLimitStatus             "/ACK_STATES_CAR/STATE_curHorLimitStatus"
#define ACKSTATES_curUTSensorStatus             "/ACK_STATES_CAR/STATE_curUTSensorStatus"
#define ACKSTATES_curPerSensorStatus              "/ACK_STATES_CAR/STATE_curPerSensorStatus"

#define SAVEPATH_Picture             "/SAVEPATH/Picture"
#define SAVEPATH_Video               "/SAVEPATH/Video"
//做成单例

class configParmeter:public QObject
{
    Q_OBJECT
public:
    static configParmeter *getInstance();
private:
    configParmeter();
    static configParmeter * m_pInstance;
public:
    void initSettings();
    QList<StruSerialInfo> getSerialInfo();
    QList<StruCameraCntCfg> getCameraCntConfigInfo();
    StruStallParameter getCrawlerStall();
    StruPacketParameter getPacketConfig();
    StruSpeedModeParameter getSpeedModeParam();
    StruDisplacementModeParameter getDisplacementModeParam();
    StruDifSpeedModeParameter getDifSpeedModeParam();
    StruServo_Angle getServoParam();
    Strulift_shaft getLiftShaftParam();
    StruAckStates getAckStatuesParam();
    QString getSavePicturePath();
    QString getSaveVideoPath();
public:
    void writeSerialInfo(QList<StruSerialInfo> &listStruSerialInfo);
    void writeCameraCntConfigInfo(QList<StruCameraCntCfg> &listStruCamerConfig);
    void writeCrawlerStall(StruStallParameter &stallParam);
    void writePacketConfig(StruPacketParameter &packetConfig);
    void writeSpeedModeParam(StruSpeedModeParameter &speedModeParam);
    void writeDisplacementParam(StruDisplacementModeParameter &dispParam);
    void writeDifSpeedModeParam(StruDifSpeedModeParameter &difParam);
    void writeServoParam(StruServo_Angle &servoParam);
    void writeLiftShaftParam(Strulift_shaft &liftShaftParam);
    void writeAckStatesParam(StruAckStates &ackStatesParam);
    void writeSavePicturePath(QString path);
    void writeSaveVideoPath(QString path);
public:
    EnumActionMode getActionMode();
    void setActionMode(EnumActionMode mode);
    QString getMotorCMDByMode(EnumDirectionKey key);
    EnumCrawlerMovingState getMotorStatusFlag(EnumDirectionKey key);
    QString getMotorCMDByModeQtKey(Qt::Key qtkey);
    QString getAskStatueCMD(EnumAskCrawlerStatus askCrawlerStatus);
private:
    QList<StruSerialInfo> readSerialInfo();
    QList<StruCameraCntCfg> readCameraCntConfig();
    StruStallParameter readCrawlerStall();
    StruPacketParameter readPacketConfig();
    StruSpeedModeParameter readSpeedMode();
    StruDisplacementModeParameter  readDispMode();
    StruDifSpeedModeParameter readDifSpeedMode();
    StruServo_Angle readServo();
    Strulift_shaft readLiftShaft();
    StruAckStates readAckStates();
    QString readSavePicturePath();
    QString readSaveVideoPath();

    void initKeyToDirectionMap();
    void initAskStatusMap();
signals:
    void sgUpdataSerialInfo();
    void sgUpdateCameraInfo();
private:
    QSettings *m_settings;
    QList<StruSerialInfo> listStruSerialInfo;
    EnumActionMode actionMode;
    QMap<Qt::Key,EnumDirectionKey> keyToDirMap;
    QMap<EnumDirectionKey,QString> speedModeCMDMap;
    QMap<EnumDirectionKey,QString> distanceModeCMDMap;
    QMap<EnumDirectionKey,QString> difspeedModeCMDMap;
    QMap<EnumAskCrawlerStatus,QString> askCrawlerCMDMap;

    QList<StruSerialInfo> m_listSerialInfo;
    QList<StruCameraCntCfg> m_listCameraCntCfg;
    StruStallParameter m_stallParam;
    StruPacketParameter m_packetConfig;
    StruSpeedModeParameter m_speedMdoeParam;
    StruDisplacementModeParameter m_dispModeParam;
    StruDifSpeedModeParameter m_difSpeedModeParam;
    StruServo_Angle m_servoParam;
    Strulift_shaft m_liftShaftParam;
    StruAckStates m_ackStatesParam;
    QString m_savePicturePath,m_saveVideoPath;


};

#endif // CONFIGPARMETER_H
