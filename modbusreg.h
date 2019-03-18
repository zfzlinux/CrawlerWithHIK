/*
保存modbus协议中，相关寄存器意义。
*/
#ifndef MODBUSREG_H
#define MODBUSREG_H
#include <qglobal.h>
//根据下面的MODBUS结构体计算出。
#define MODBUS_ADD_MAX 45
#define RxBuf_Length  MODBUS_ADD_MAX*2   //modbus单字节大小。
//传感器结构体
typedef struct SensorStruct{
    quint16 en_switch;
    quint16 warnValue;
    quint16 curValue;
}T_Sensor;
//限位开关
typedef struct limitSwitch{
    quint16 TopStatus;
    quint16 TailStatus;
}T_LimitSH;
//心跳
typedef struct heart_flag{
    quint16 h_switch; //0-关闭，1-打开
    quint16 h_waitReset;//外部清零。如果非零，下次心跳检测后，停止运动。
}T_Heart;
//当前模式
typedef enum _curPattern{
    NORMAL_PT =0, //自由模式，普通模式，差速模式
    DIST_PT,//位移模式。
}CurPattern;
//当前动作
typedef enum _curAction{
    STOP_AT=0,
    FORWARD_AT,
    BACKWARD_AT,
    LEFT_AT,
    RIGHT_AT,
}CurAction;

//modbus寄存器定义，起始地址为0x0000，每一个值为16位int，占两个字节*/
struct MODBUS_REG{
    //速度标志位。0xCD-增加档位，0xCE-减小档位，0xBD-增加75个PWM,0xBE-减小75个pwm.
    quint16 speed_flag; //
    //速度档位0~25
    quint16 left_gears;
    quint16 right_gears;
    //电机的PWM，300~2100
    quint16 left_pwm;
    quint16 right_pwm;
    //左侧电机编码器的值
    quint16 left_encoder;
    //右侧电机编码器的值
    quint16 right_encoder;
    //对应运动方向，0-stop，1-forward,2-backward,3-left,4-right
    quint16 motor_move;
    //位移模式运动方向，0-stop,1-forward,2-backward,
    quint16 distance_move;
    //设置当前位移的距离10~5000mm
    quint16 distance_data;
    //当前的位置值
    quint16 cur_distance_value;
    //当前的运动模式
    //0-自由模式，1，-位移模式
    quint16 pattern_flag;
    quint16 cur_action;
    //心跳标志
    T_Heart heart;
    //舵机角度0~180
    quint16 servoAngle_V;
    quint16 servoAngle_H;
    //抬升轴状态 0-停止，1上升，2下降
    quint16 liftShaft_move;
    quint16 liftShaft_speed;
    quint16 liftShaft_startPos;
    quint16 liftShaft_endPos;
    T_LimitSH limitSH_LS; // size = int*2
    //横轴状态 0-停止，1-左，2右,3-归零。
    quint16 crossShaft_move;
    quint16 crossShaft_speed;
    quint16 crossShaft_startPos;
    quint16 crossShaft_endPos;
    T_LimitSH limitSH_CS; // size = int*2
    //超声测距传感器
    //size = int *3
    T_Sensor UTSen0;
    T_Sensor UTSen1;
    //压力传感器//size = int *3
    T_Sensor PreSen0;
    T_Sensor PreSen1;
    //调试模式
    quint16 DEBUG_FLAG; //0-关闭，1-打开。
    //版本
    quint16 version;    //
    //保留位
    quint16 reserved[2];
};
enum MODBUS_REG_ADDR{
    //速度标志位。0xCD-增加档位，0xCE-减小档位，0xBD-增加75个PWM,0xBE-减小75个pwm.
    speed_flag_addr =0, //
    //速度档位0~25
    left_gears_addr,
    right_gears_addr,
    //电机的PWM，300~2100
    left_pwm_addr,
    right_pwm_addr,
    //左侧电机编码器的值
    left_encoder_addr,
    //右侧电机编码器的值
    right_encoder_addr,
    //对应运动方向，0-stop，1-forward,2-backward,3-left,4-right
    motor_move_addr,
    //位移模式运动方向，0-stop,1-forward,2-backward,
    distance_move_addr,
    //设置当前位移的距离10~5000mm
    distance_data_addr,
    //当前的位置值
    cur_distance_value_addr,
    //当前的运动模式
    //0-自由模式，1，-位移模式
    pattern_flag_addr,
    cur_action_addr,
    //心跳标志
    heart_switch_addr,
    heart_reset_addr,
    //舵机角度0~180
    servoAngle_V_addr,
    servoAngle_H_addr,
    //抬升轴状态 0-停止，1上升，2下降
    liftShaft_move_addr,
    liftShaft_speed_addr,
    liftShaft_startPos_addr,
    liftShaft_endPos_addr,
    limitSH_Top_LS_addr,
    limitSH_Tail_LS_addr,
    //横轴状态 0-停止，1-左，2右,3-归零。
    crossShaft_move_addr,
    crossShaft_speed_addr,
    crossShaft_startPos_addr,
    crossShaft_endPos_addr,
    limitSH_Top_CS_addr,
    limitSH_Tail_CS_addr,
//    //抬升轴
//    liftShaft_moveV_addr,
//    liftShaft_posV_addr,
//    //限位开关状态
//    limitSH_TopV_addr,
//    limitSH_TailV_addr,
//    // size = int*2
//    //横轴
//    liftShaft_moveH_addr,
//    liftShaft_posH_addr,
//    //限位开关状态
//    limitSH_TopH_addr,
//    limitSH_TailH_addr,
    //超声测距传感器
    UTSen0_ENswitch_addr,
    UTSen0_WarnValue_addr,
    UTSen0_curValue_addr,

    UTSen1_ENswitch_addr,
    UTSen1_WarnValue_addr,
    UTSen1_curValue_addr,
    //压力传感器//size = int *3
    PreSen0_ENswitch_addr,
    PreSen0_WarnValue_addr,
    PreSen0_curValue_addr,

    PreSen1_ENswitch_addr,
    PreSen1_WarnValue_addr,
    PreSen1_curValue_addr,
    //调试模式
    DEBUG_switch_addr, //0-关闭，1-打开。
    //版本
    version_addr,    //
    //保留位
    reserved0_addr,
    reserved1_addr,
    MaxSizeOfAddr,
};

#endif // MODBUSREG_H
