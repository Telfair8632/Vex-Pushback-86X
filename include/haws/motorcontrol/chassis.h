#ifndef _HAWS_CHASSIS_H_
#define _HAWS_CHASSIS_H_

#include "api.h"
#include "haws/motorcontrol/kernel.h"
#include "haws/logger/logger.h"
#include "lemlib/chassis/chassis.hpp"

using namespace pros;

namespace chassis
{

/**
 * @brief 初始化底盘传感器 (包括编码器 陀螺仪)
 */
void init();

/**
 * @brief 底盘以 `坦克` 方式被手柄控制移动
 * @param left  左摇杆的值
 * @param right 右摇杆的值
 */
void tank(int leftPower, int rightPower);
void move(int lPower, int rPower);
/**
 * @brief 底盘以 `单杆/双杆` 方式被手柄控制移动
 * @param throttle 直行对应的摇杆的值
 * @param turn     转向对应的摇杆的值
 */
void arcade(int throttle, int turn);

/**
 * @brief 底盘以 `单杆/双杆+曲率控制` 方式被手柄控制移动
 * @param throttle 直行对应的摇杆的值
 * @param turn     转向对应的摇杆的值
 */
void curvature(int throttle, int turn);

/**
 * @brief 取消底盘所有动作
 */
void cancelAllMotions();

/**
 * @brief 重置底盘位置, 默认重置为 (0, 0, 0)
 * @param x 重置的水平方向位置, 默认为0
 * @param y 重置的垂直方向位置, 默认为0
 * @param theta 重置的朝向角度, 默认为0
 */
void resetPose(float x = 0, float y = 0, float theta = 0);

/**
 * @brief 原地转向至指定角度
 * @param theta 相对于起始朝向(初始化朝向)的目标角度
 * @param params 拓展参数: 额外配置参数列表
 * @details {.direction} 指定转向的方向，可填入CW_CLOCKWISE/CCW_COUNTERCLOCKWISE/AUTO，分别代表顺时针/逆时针/自动，默认值AUTO
 * @details {.maxSpeed} 指定转向时底盘电机的最大速度 (1-127), 默认值127
 */
void turnToHeading(float theta, TurnToHeadingParams params = {});

/**
 * @brief 原地转向, 朝向指定点位
 * @param x 指定点位的水平方向位置
 * @param y 指定点位的垂直方向位置
 * @param params 拓展参数：额外配置参数列表
 * @details {.forwards} 机器人是否使用前向对准指定点位, 可填入true/false, 表示使用前方还是后方朝向指定点位, 默认值true
 * @details {.direction} 指定转向的方向，可填入CW_CLOCKWISE/CCW_COUNTERCLOCKWISE/AUTO，分别代表顺时针/逆时针/自动，默认值AUTO
 * @details {.maxSpeed} 指定转向时底盘电机的最大速度 (1-127), 默认值127
 */
void turnToPoint(float x, float y, TurnToPointParams params = {});

/**
 * @brief 移动至指定位置
 * @note 实际执行时会先转到对准指定位置的角度，再直线移动至指定位置
 * @param x 水平方向位置
 * @param y 垂直方向位置
 * @param params 拓展参数: 额外配置参数列表
 * @details {.forwards} 直线移动时机器的朝向，可填入true/false，表示前进还是后退，默认值true
 * @details {.maxSpeed} 底盘移动时电机的最大速度 (0-127), 默认值127
 */
void moveToPoint(float x, float y, MoveToPointParams params = {});

/**
 * @brief 移动至指定位置且确定朝向
 * @note 实际执行会生成一条弧线路径循迹，具体轨迹可查看https://www.desmos.com/calculator/sptjw5szex?lang=zh-CN
 * @param x 水平方向位置
 * @param y 垂直方向位置
 * @param theta 相对于起始朝向(初始化朝向)的目标角度
 * @param params 拓展参数: 额外配置参数列表
 * @details {.forwards} 直线移动时机器的朝向，可填入true/false，表示前进还是后退，默认值true
 * @details {.lead} 弧线曲率，lead值越大画的弧越大，可填入0-1，默认值0.4，可使用上述轨迹网站查看lead值对轨迹的影响
 * @details {.maxSpeed} 底盘移动时电机的最大速度 (0-127), 默认值127
 * @param async 是否为异步执行: 开线程执行
 */
void moveToPose(float x, float y, float theta,  MoveToPoseParams params = {.lead=0.4},int timeout = 10000, bool async = false);

/**
 * @brief 配合异步执行函数使用, 等待当前步骤移动一段距离/旋转一定角度后, 继续执行下一步代码
 * @param dist 指定的距离值
 */
void waitUntil(float dist);

void waitUntilDone();

/**
 * @brief 打印当前定位位置
 * @param line 打印的行数
 * @details 打印格式 " x: %.2f | y: %.2f | t: %.2f ", 分别对应水平位置, 垂直位置, 朝向
 */
void printPose(int line = 7);

/**
 * @brief 获取底盘实例, 用于进阶功能实现
 */
Kernel getKernel();

/**
 * @brief 时间控制底盘移动函数
 * @param lPower 左边电机的功率值(-100至100)
 * @param rPower 右边电机的功率值(-100至100)
 * @param time   移动持续的时间
 */
void moveTime(int lPower, int rPower, int time);

/**
 * @brief 底盘刹车函数
 * @param brakeMode 拓展参数: 刹车模式(默认为brake)
 * @note 如不填入刹车模式, 使用brake模式进行刹车
 */
void brake(motor_brake_mode_e brakeMode = E_MOTOR_BRAKE_BRAKE);

/**
 * @brief 编码器控制底盘直行函数
 * @param distance  移动的距离(电机的编码器目标值, 正数为前进, 负数为后退)
 * @param maxPower  拓展参数: 底盘移动的最大功率(默认为100, 可填入0至100的数值, 需为正数)
 * @param brakeMode 拓展参数: 移动至目标值后的刹车方式(默认为brake)
 * @param function  进阶参数: 并行执行(同步, 开启新线程)的函数体
 * @param funcStart 进阶参数: 并行执行函数开始的距离(如distance参数是负数, 则此值也需填负数)
 */
void moveEnc(int distance, int maxPower = 100, motor_brake_mode_e brakeMode = E_MOTOR_BRAKE_BRAKE, std::function<void()> function = {}, int funcStart = 0);

void moveEncStright(int distance, int maxPower = 100, motor_brake_mode_e brakeMode = E_MOTOR_BRAKE_BRAKE, std::function<void()> function = {}, int funcStart = 0);

void rushEnc(int distance, int power = 100);

/**
 * @brief 陀螺仪控制底盘转向函数
 * @param angle 底盘朝向的角度值(陀螺仪相对初始位置的值)
 */
void turnGyro(int angle);

void rushGyro(int angle, int power, int maxErr);

void turn (int target);

void moveDis(int distance, int maxPower = 100, motor_brake_mode_e brakeMode = E_MOTOR_BRAKE_BRAKE, std::function<void()> function = {}, int funcStart = 0);

void rushDis(int distance, int power);
}
#endif
