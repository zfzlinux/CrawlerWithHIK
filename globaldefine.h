#ifndef GLOBALDEFINE_H
#define GLOBALDEFINE_H

#include <QSerialPort>
#include <QSerialPortInfo>

typedef struct _StruSerialInfo
{
    QString PortName;
    enum BaudRate {
        Baud1200 = 1200,
        Baud2400 = 2400,
        Baud4800 = 4800,
        Baud9600 = 9600,
        Baud19200 = 19200,
        Baud38400 = 38400,
        Baud57600 = 57600,
        Baud115200 = 115200,

    }m_BaudRate;
    enum DataBits {
        Data5 = 5,
        Data6 = 6,
        Data7 = 7,
        Data8 = 8,

    }m_DataBits;

    enum Parity {
        NoParity = 0,
        EvenParity = 2,
        OddParity = 3,
        SpaceParity = 4,
        MarkParity = 5,

    }m_Parity;

    enum StopBits {
        OneStop = 1,
        OneAndHalfStop = 3,
        TwoStop = 2,
    }m_StopBits;

    enum FlowControl {
        NoFlowControl,
        HardwareControl,
        SoftwareControl,
    }m_FlowControl;

}StruSerialInfo;

//typedef struct _StruCameraCntCfg
//{
//    QString IPAddress;
//    QString UserName;
//    QString Password;
//    quint32 port;
//}StruCameraCntCfg;

struct StruCameraCntCfg
{

    StruCameraCntCfg():IPAddress("192.168.8.1"),UserName("Admin"),Password("NWNEAC"),port(8000)
    {}
    QString IPAddress;
    QString UserName;
    QString Password;
    quint32 port;
};

typedef struct _StruSTALL_PARAMETER_{
    int max;
    float *speed;
}StruStallParameter;

typedef struct _StruSERIAL_PACKET_PARAMETER_{
    QString head;
    QString tail;
} StruPacketParameter;

typedef struct _StruSpeedModeParameter
{
    QString up;
    QString down;
    QString left;
    QString right;
    QString stop ;
    QString speedUp;
    QString speedDown;
    QString front_left;
    QString front_right;
    QString leftWheelStall;
    QString rightWheelStall;
    QString left_alone;
    QString right_alone;
    bool    is_stop;
}StruSpeedModeParameter;

typedef struct _StruDIF_SPEED_MODE_PARAMETER{

    QString stop;
    QString up;
    QString down;
    QString left;
    QString right;
    QString speedUP;
    QString speedDown;
    QString leftWheelStall;
    QString rightWheelStall;
//    QString pwm_all;
//    int PWM_MIN;
//    int PWM_MAX;
//    int PWM_STEP;
//    float PWMTOSPEED;
}StruDifSpeedModeParameter;

typedef struct _StruDISPLACEMENT_MODE_PARAMETER_{
    QString up;
    QString down;
    QString stop;
    QString left;
    QString right;
    QString speedUp;
    QString speedDown;
    QString displacement;
    QString leftWheelStall;
    QString rightWheelStall;
} StruDisplacementModeParameter;

typedef struct _StruCLIENT_SYNC_{
    QString heartBeat       ;
    QString clientConnect   ;
    QString clientDisconnect;
    QString serverReplyYes  ;
    QString serverReplyNo   ;
    QString serverRequset   ;
    QString clientRreplyYes ;
    QString clientReplyNo   ;
}StruClientSyncParameter;

typedef struct _StruWHEEL_STALL_{
    quint8 left;
    quint8 right;
    bool   isSpeedUp; //加速按钮按下的标志,true-加速按钮按下,false-加速按钮弹起
    bool   isSpeedDown;//减速按钮按下的标志,true-加速按钮按下,false-加速按钮弹起
} StruWheelStall;

typedef struct _StruPWMRECORD_{
    int MIN;
    int MAX;
    int STEP;
    float PWMTOSPEED;
    quint16 left;
    quint16 right;
    bool isPWMUp;
    bool isPWMDown;
}StruPWMRecord;

typedef struct _StruTIMEROUT_PARAMETER_{
    QString timer_cmd		;
}StruTimer_Out;

typedef struct _StruSERVO_PARAMETER_{
    QString cmd_init;
    QString cmd_angle_H;
    QString cmd_angle_V;
    quint8 MaxAngle_Hor;
    quint8 MinAngle_Hor;
    quint8 Angle_Hor;
    quint8 MaxAngle_Ver;
    quint8 MinAngle_Ver;
    quint8 Angle_Ver;
    int  initAngle_H;
    int  initAngle_V;
    bool is_angle_H_up;
    bool is_angle_V_up;
}StruServo_Angle;

typedef struct _StruACK_STATES{
    QString speed_all;				//整体的小车档位
    QString speed_left;				//左轮的档位
    QString speed_right;			//右轮的档位
    QString motionStatus;					//小车的运动状态
    QString distanceStatus;				//位移
    QString curDistance_pulse;			//位移值
    QString servo_angle;			//2个舵机的角度
    QString heart;                  //心跳保持。必须每5秒发送一次到下位机
    QString verMotor_LF;            //垂直电机状态
    QString horMotor_LF;            //水平电机状态
    QString PWMValue_left;          //设置的左侧PWM值
    QString PWMValue_right;         //设置的右侧PWM值
    QString curPWMValue_left;       //当前左侧电机的PWM值，每秒钟
    QString curPWMValue_right;       //当前右侧电机的PWM值，每秒钟
    QString curUTSensorValueA;      //询问当前超声测距传感器的数值
    QString curUTSensorValueB;      //询问当前超声测距传感器的数值
    QString curPerSensorValueA;     //询问当前压力传感器的数值
    QString curPerSensorValueB;     //询问当前压力传感器的数值
    QString curVerLimitStatus;      //询问垂直电机的2个限位开关状态
    QString curHorLimitStatus;      //询问水平电机的2个限位开关状态
    QString curUTSensorStatus;      //询问距离状态
    QString curPerSensorStatus;     //询问压力状态
}StruAckStates;

typedef struct _StruSTATE_CMD{
    QString cmd_motor_speed;
    QString cmd_motor_move;
    QString cmd_distance;
    QString cmd_servo;
    QString cmd_ask_state;
    QString cmd_feed_state;
    QString cmd_error_msg;
}StruState_cmd;

typedef struct _StruLIFT_SHAFT_CMD{
    QString VerMotorStop;
    QString VerMotorUp;
    QString VerMotorDown;
    bool isVerMotorInTop;
    bool isVerMotorInBottom;
    QString HorMotorStop;
    QString HorMotorUp;
    QString HorMotorDown;
    bool isHorMotorInTop;
    bool isHorMotorInBottom;
}Strulift_shaft;

typedef struct __DISPLAY_BUTTON{
    bool display_trun_mode_choice;
    bool display_change_language;
}Display_Button;

typedef struct __USB_CAMERA{
    QString camera0_name;
    QString camera0_name_def;
    QString camera1_name;
    QString camera1_name_def;
    QString camera2_name;
    QString camera2_name_def;
    QString camera3_name;
    QString camera3_name_def;
}Usb_Camera;


typedef struct __SCREEN_RESOLUTION{
    int width;
    int height;
}Screen_Resolution;


typedef enum _EnumActionMode{
    speedMode = 0,
    distanceMode,
    difSpeedMode
}EnumActionMode;

typedef enum _EnumDirectionKey{
    em_stop =0,
    em_forworad ,
    em_backward,
    em_left,
    em_right,
    em_frontLeft,
    em_frontRight,
    em_backLeft,
    em_backRgiht,
    em_speedUP,
    em_speedDown
}EnumDirectionKey;

typedef enum _EnumCrawlerMovingState
{
    crawler_stop = 0,
    crawler_forward ,
    crawler_backward ,
    crawler_trunleft ,
    crawler_trunright ,
    crawler_frontleft,
    crawler_frontright,
    crawler_backleft,
    crawler_backright
}EnumCrawlerMovingState;

typedef enum _EnumAskCrawlerStatus
{
    askCrawler_speed_all,
    askCrawler_speed_left,
    askCrawler_speed_right,
    askCrawler_motion,//stop forward....
    askCrawler_distance,
    askCrawler_distance_pluse,
    askCrawler_servo,
    askCrawler_heart,
    askCrawler_verMotor_LF,
    askCrawler_horMotor_LF,
    askCrawler_PWMValue_left,
    askCrawler_PWMValue_right,
    askCrawler_curPWM_left,
    askCrawler_curPWM_right,
    askCrawler_valueUTSensorA,
    askCrawler_valueUTSensorB,
    askCrawler_valuePressureA,
    askCrawler_valuePressureB,
    askCrawler_curVerLimitStatus,
    askCrawler_curHorLimitStatus,
    askCrawler_UTSensorStatus,
    askCrawler_PressureSensorStatus,
}EnumAskCrawlerStatus;

typedef enum _EnumServoDirection
{
    horizontal_servo,
    vertical_servo
}EnumServoDirection;

typedef enum _EnumLiftShaftStatus
{
    LS_MotorStop =0,
    LS_MotorUp,
    LS_MotorDown,
    LS_MotorInTop,
    LS_MotorInBottom,
}EnumLiftShaftStatus;

typedef enum _EnumLiftShaftMotorType
{
    LS_VerMotor =0,
    LS_HorMotor,
    LS_MotorMax
}EnumLiftShaftMotorType;

typedef enum _EnumUTSensorType
{
    UT_SensorA=0,
    UT_SensorB,
    UT_SensorC,
    UT_SensorMax
}EnumUTSensorType;

typedef enum _EnumPreSensorType
{
    Pre_SensorA =0,
    Pre_SensorB,
    Pre_SensorMax,
}EnumPreSensorType;

typedef enum _EnumSensorStatus
{
    Sen_DisConnect      =0,
    Sen_InConnect       =0x01,
    Sen_SafeValue       =0x02,
    Sen_WarningValue    =0x04,
    Sen_Error           =0x0F,
}EnumSensorStatus;

typedef enum _EnumLimitSwitchType
{
    VerTopLimit =0,
    VerBottomLimit,
    HorTopLimit,
    HorBottomLimit
}EnumLimitSwitchType;

typedef enum _EnumPWMType
{
    PWMLeftByConfig =0,
    PWMRightByConfig,
    CurPWMLeft,
    CurPWMRight
}EnumPWMType;

//位移模式当前的状态
typedef enum _EnumDispModeStatus
{
    Disp_MoveStop = 0,
    Disp_MoveForwrad = 0x01,
    Disp_MoveBackwrad = 0x02,
    Disp_FinishMove   = 0xFF
}EnumDispModeStatus;

//串口通讯指令ID
#define PacketLength            6
#define ID_PacketHead           0x5A
#define ID_PacketTail           0xA5
//
#define	ID_MotorSpeed           0x01
#define	ID_MotorMove            0x02
//#define	ID_MotorPWM             0x03
#define	ID_Distance             0x04
#define	ID_Servo                0x06
#define	ID_VerMotorMove         0x07
#define	ID_HorMotorMove         0x08

#define	ID_UTDistanceA          0x09
#define	ID_UTDistanceB          0x0A
#define	ID_UTDistanceC          0x0B
#define	ID_PressureA            0x0C
#define	ID_PressureB            0x0D
#define	ID_VerLimitSwitch       0x0E
#define	ID_HorLimitSwitch       0x0F
#define ID_UTSensorStatus       0xE1
#define ID_PressureStatus       0xE2

#define	ID_AskStatus 			0xA0
#define	ID_Feedback  			0xB0
#define	ID_CMD_Error 			0xF0
#define	ID_Heart                0xFA
#define ID_DebugMode            0xDB

#define SpeedIndex_Up           0xCD
#define SpeedIndex_Down         0xCE
#define SetIndex_Left           0xC4
#define SetIndex_Right          0xC5
#define SetIndex_All            0xC8

#define PWM_Up                  0xBD
#define PWM_Down                0xBE
#define SetPWM_Left             0xB4
#define SetPWM_Right            0xB5
#define SetPWM_All              0xB8

#define SetDistanceMoveValue    0xCC

#define FBID_SpeedIndexAll      0x00
#define FBID_SpeedIndexLeft     0x01
#define FBID_SpeedIndexRight    0x02
#define FBID_MoveStatus         0x03
#define FBID_DistanceModeStatus 0x04
#define FBID_DistanceValue      0x05
#define FBID_ServoAngle         0x06
#define FBID_VerMotor           0x07
#define FBID_HorMotor           0x08
#define FBID_ValueUTSensorA     0x09
#define FBID_ValueUTSensorB     0x0A
#define FBID_ValueUTSensorC     0x0B
#define FBID_ValuePreSenA       0x0C
#define FBID_ValuePreSenB       0x0D
#define FBID_VerLimitSenStatus  0x0E
#define FBID_HorLimitSenStatus  0x0F
#define FBID_PWMValueLeftBySet  0x10
#define FBID_PWMValueRightBySet 0x11
#define FBID_CurPWMValueLeft    0x12
#define FBID_CurPWMValueRight   0x13
#define FBID_CurUTSensorStatus  0xe1
#define FBID_CurPreSensorStatus 0xE2

#define UTSen_Enable            0x00
#define UTSen_MinDistance       0x01
#define UTSen_SafeDistance      0x02
#define UTSen_WarnDistance      0x03

#define PreSen_Enable           0x00
#define PreSen_WarnPressure     0x01

#define Sensor_Open             1
#define Sensor_Close            0

#define Pluse_one_Millimeter 15.6
#define DIASTANCE_MAX        5000 //5000mmºÁÃ×
#define DIASTANCE_MIN        10   //10mmºÁÃ×
#define DIASTANCE_INIT       50   //50mmºÁÃ×

enum LimitParam
{
    MinSpeedIndex          = 1,
    MaxSpeedIndex          = 25,
    SpeedIndexTrunLeft     = 1,
    SpeedIndexTrunRight    = 1,
    MinPWMValue            = 300,
    MaxPWMValue            = 2100,
    PWM_PRECISION          = 75,     //(PWM_FRE_MAX-PWM_FRE_MIN)/(SPEED_MAX-1)
    MinDistanceValue       = 10,
    MaxDistanceValue       = 5000,
    HeartLoopSendInterval  = 5000,
};

enum InitParam
{
    InitSpeedIndex         = 5,
    InitServoAngleHor      = 50,
    InitServoAngleVer      = 50,   
    InitPWMValue           = 600, // MinPWMValue + PWM_PRECISION*(InitSpeedIndex-1);
    InitDistanceValue      = 50,
};

typedef enum _enumStatusLED
{
    RedLED =0,
    RedFlicker,
    GreenLED,
    GreenFlicker,
}EnumStatusLED;

////err_who_reg
//#define ERROR_UART              0xE0
//#define ERROR_SPEED             0xE1
//#define ERROR_MOTOR             0xE2
//#define ERROR_DISTA             0xE3
//#define ERROR_SERVO             0xE4

#define KEY_FORWARD			Qt::Key_Up
#define KEY_BACKWARD		Qt::Key_Down
#define KEY_GOLEFT			Qt::Key_Left
#define KEY_GORIGHT			Qt::Key_Right
#define KEY_FRONT_LEFT      Qt::Key_L
#define KEY_FRONT_RIGHT     Qt::Key_R

#define KEY_SPEED_UP		Qt::Key_PageUp
#define KEY_SPEED_DOWN		Qt::Key_PageDown

#define KEY_SEVOER_H_UP		Qt::Key_A
#define KEY_SEVOER_H_DOWN	Qt::Key_D
#define KEY_SEVOER_V_UP		Qt::Key_W
#define KEY_SEVOER_V_DOWN	Qt::Key_S

#define KEY_LIFTSHAFT_VUP   Qt::Key_T
#define KEY_LIFTSHAFT_VDOWN Qt::Key_B

#define KEY_LIFTSHAFT_HUP   Qt::Key_F
#define KEY_LIFTSHAFT_HDOWN Qt::Key_N

#endif // GLOBALDEFINE_H
