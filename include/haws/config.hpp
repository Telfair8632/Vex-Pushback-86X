#include "api.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.h"

#define _12966A didi

using namespace pros;
using namespace lemlib;

#ifndef _HAWS_CONFIG_H_
#define _HAWS_CONFIG_H_
#define CONSTRAIN(x, lower, upper) ((x)<(lower)?(lower):((x)>(upper)?(upper):(x)))
// 遥控器
#define controller Controller(pros::E_CONTROLLER_MASTER)

// 底盘电机端口定义
#define PORT_MOTOR_LEFT1 -15
#define PORT_MOTOR_LEFT2 -16
// #define PORT_MOTOR_LEFT3 17
#define PORT_MOTOR_RIGHT1 18
#define PORT_MOTOR_RIGHT2 19
// #define PORT_MOTOR_RIGHT3 -20

// 底盘电机齿轮组定义
#define CHASSIS_MOTOR_GEAR_SET pros::MotorGearset::blue

// X-Drive电机端口 (使用负数表示反转)
#define PORT_XDRIVE_FL 15
#define PORT_XDRIVE_FR 18
#define PORT_XDRIVE_BL 16
#define PORT_XDRIVE_BR 19

// 底盘控制方式
// #define CHASSIS_CONTROL_TANK
// #define CHASSIS_CONTROL_ARCADE_ONE
// #define CHASSIS_CONTROL_ARCADE_TWO
#define CHASSIS_CONTROL_XDRIVE

// 陀螺仪端口定义
#define PORT_SENSOR_IMU 4

// 定位轮角度传感器端口定义
#define PORT_SENSOR_ENC_VERTICAL 9
#define PORT_SENSOR_ENC_HORIZONTAL 10

// 底盘外其他电机定义
const Motor motor_roller(2, MotorGear::green);// motor 5.5w
const Motor motor_upperSorter(-1, MotorGear::green); // motor 5.5w
const Motor motor_lowerSorter(3, MotorGear::blue); 

#define sensor_color Optical(7)   // 颜色传感器
#define sensor_dis Distance(5)

// 三线输出
const adi::DigitalOut digit_trackCap('G', LOW);     //球道盖子
const adi::DigitalOut digit_loaderArm('H', LOW);    //吸装填摇臂
const adi::DigitalOut digit_pushArm('E', LOW);      //推高塔小手
const adi::DigitalOut digit_upperCannon('F',LOW);   //吐高塔大炮
const adi::DigitalOut digit_Roller('A',LOW);//提滚子
const adi::DigitalOut digit_Rotation('D',LOW); //提轮

// 全场定位配置参数
#define HOR_DRIFT 8

// #define LINEAR_KP 7.6
// #define LINEAR_KI 0.1
// #define LINEAR_KD 0.47
#define LINEAR_KP 8.0
#define LINEAR_KI 0.1
#define LINEAR_KD 0.37

#define ANGULAR_KP 1.5
#define ANGULAR_KI 0.02
#define ANGULAR_KD 1.5
#endif