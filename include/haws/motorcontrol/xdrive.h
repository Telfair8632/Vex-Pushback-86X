#ifndef _HAWS_XDRIVE_H_
#define _HAWS_XDRIVE_H_

#include "api.h"

namespace xdrive {

/**
 * @brief 初始化X-Drive四个电机
 */
void init();

/**
 * @brief X-Drive手柄控制
 * @param x  左摇杆X轴 (-127~127, 正=右移)
 * @param y  左摇杆Y轴 (-127~127, 正=前进)
 * @param r  右摇杆X轴 (-127~127, 正=右转)
 */
void control(int x, int y, int r);

/**
 * @brief 停止所有电机
 */
void stop();

/**
 * @brief 刹车
 * @param mode 刹车模式, 默认coast
 */
void brake(pros::motor_brake_mode_e mode = pros::E_MOTOR_BRAKE_COAST);

} // namespace xdrive

#endif
