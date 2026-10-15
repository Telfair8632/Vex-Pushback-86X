#include "lemlib/api.hpp" // IWYU pragma: keep
#include "../config.hpp"
#include "pros/motors.h"

#ifndef _HAWS_KERNEL_H_
#define _HAWS_KERNEL_H_

class Kernel {
public:
    /// 构造函数
    Kernel();
    /// 析构函数
    ~Kernel();
    
    Imu *sensor_gyro; //< 陀螺仪

    /// 获取底盘指针
    Chassis* getChassis();
    /// 重置底盘编码器
    void reset();
    /// 底盘刹车
    void brake(motor_brake_mode_e brakeMode = E_MOTOR_BRAKE_COAST);
    /// 获取底盘编码器值
    int getPosition();
    /// 获取底盘电机转速
    int getActualVelocity();
    std::string selfCheck();

private:
    MotorGroup *leftMotors;   //< 左电机组
    MotorGroup *rightMotors;  //< 右电机组

    Rotation *verticalEnc;   //< 纵向编码器
    Rotation *horizontalEnc; //< 横向编码器

    TrackingWheel *verticalTrackingWheel;   //< 纵向定位轮 
    TrackingWheel *horizontalTrackingWheel; //< 横向定位轮

    lemlib::Drivetrain *drivetrain; //< 传动系统/底盘

    ControllerSettings *linearController;  //< 线性控制器
    ControllerSettings *angularController; //< 转向控制器

    OdomSensors *sensors; //< 定位传感器组合

    ExpoDriveCurve *throttleCurve; //< 直行曲线
    ExpoDriveCurve *steerCurve;    //< 转向曲线
public:
    Chassis *chassis;
};
#endif