#ifndef _HAWS_ROLLER_H_
#define _HAWS_ROLLER_H_

#include "api.h"
#include "haws/logger/logger.h"
#include <type_traits>

using namespace pros;

namespace roller {

/**
 * @brief 控制所有吸轮&履带,上行/下行函数
 * @param power     吸入/吐出功率(-100至100, 正数为吸入, 负数为吐出, 0为刹车)
 * @param brakeMode 刹车模式, 当`power`参数为0时生效, 控制电机刹车模式(默认为coast模式)
 */
void roll(int power, motor_brake_mode_e brakeMode = E_MOTOR_BRAKE_COAST);

/**
 * @brief 控制吸轮存球
 */
void save();
void contorl();
/**
 * @brief 高框得分
 */
void scoreHigh();

/**
 * @brief 中框得分
 */
void scoreMiddle();
void scoreMiddleAuto();
/**
 * @brief 底框得分
 */
void scoreLowskill();
void scoreLowfast();
void scoreLowBoom();
// 自动吐中塔前准备
void prepareMiddleAuto();
// 自动程序开始存球
void startSaveAuto();
// 自动程序停止存球
void stopSaveAuto();
// 吐高塔知道检测到颜色停, true为蓝色停, false 为红色停
void scoreHighUntilColorAuto(bool isBlueStop, int maxTime = 3000);
// 吐高塔知道都吐干净停
void scoreHighAuto();
// 低塔吐两个
void scoreLow2Auto();
// 停止全部自动控制滚轮
void stopAll();
// 自动开始时调用
void startAuto();
//吐低塔自动
void scoreLowAuto();
void reload();
}
#endif