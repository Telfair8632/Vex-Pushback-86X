/*
 *                        _oo0oo_
 *                       o8888888o
 *                       88" . "88
 *                       (| -_- |)
 *                       0\  =  /0
 *                     ___/`---'\___
 *                   .' \\|     |// '.
 *                  / \\|||  :  |||// \
 *                 / _||||| -:- |||||- \
 *                |   | \\\  - /// |   |
 *                | \_|  ''\---/''  |_/ |
 *                \  .-\__  '-'  ___/-. /
 *              ___'. .'  /--.--\  `. .'___
 *           ."" '<  `.___\_<|>_/___.' >' "".
 *          | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *          \  \ `_.   \_ __\ /__ _/   .-` /  /
 *      =====`-.____`.___ \_____/___.-`___.-'=====
 *                        `=---='
 *
 *
 *      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *            佛祖保佑     永不宕机     永无BUG
 */

#include "haws/auto.h"
#include "haws/motorcontrol/chassis.h"
#include "haws/motorcontrol/roller.h"
#include "lemlib/Timer.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"

void skill()
{
    digit_pushArm.set_value(1);
    digit_Rotation.set_value(0);
    digit_upperCannon.set_value(0);

    // 吸四球
    roller::save();
    chassis::moveToPoint(-5.00, 16.50, {.maxSpeed = 37});
    digit_loaderArm.set_value(1);
    chassis::moveTime(20, 20, 150);

    // 吐高塔
    chassis::moveToPose(-16.92, 6.76, 42.16, {.forwards = false, .lead = 0.35, .minSpeed = 50}, 600, true);
    chassis::waitUntil(16);
    chassis::cancelAllMotions();
    chassis::moveToPose(-28.00, 12.46, 178.68, {.forwards = false, .lead = 0.42, .minSpeed = 40});
    pros::delay(50);
    chassis::turnGyro(179);
    chassis::move(-60, -60);
    pros::delay(200);
    roller::scoreHigh();
    pros::delay(775);
    chassis::move(0, 0);
    roller::roll(0);

    // 取填装
    roller::save();
    chassis::moveEnc(1500);
    chassis::turnGyro(177.50);
    digit_upperCannon.set_value(0);
    chassis::moveEnc(1500);
    chassis::moveTime(50, 50, 600);
    chassis::moveTime(40, 40, 600);
    chassis::moveTime(-10, -10, 100);
    chassis::moveTime(40, 40, 500);

    // 取预装
    chassis::moveEnc(-2500);
    digit_loaderArm.set_value(0);
    chassis::turnGyro(90);
    pros::delay(50);
    roller::save();
    chassis::moveEnc(7000, 60);

    // 吐低塔4红
    digit_loaderArm.set_value(1);
    chassis::moveToPose(29.70, 34.33, -43.00, {.lead = 0.6}, 5000, true);
    chassis::waitUntil(23);
    digit_loaderArm.set_value(0);
    roller::roll(0);
    motor_upperSorter.move(-65);
    motor_lowerSorter.move(-75);
    pros::delay(200);
    roller::roll(0);
    chassis::waitUntilDone();
    chassis::moveTime(40, 40, 250);
    digit_Roller.set_value(1);
    roller::scoreLowskill();
    pros::delay(1000);
    roller::roll(0);
    digit_Roller.set_value(0);
    roller::save();

    // 吐中高3蓝
    chassis::moveToPose(15.00, 14.00, 90.0, {.forwards = false, .minSpeed = 30});
    chassis::moveToPose(5.83, 36.92, -135.00, {.forwards = false}, 4500, true);
    chassis::waitUntil(1);
    motor_upperSorter.move(-70);
    motor_lowerSorter.move(-70);
    pros::delay(100);
    roller::roll(0);
    chassis::waitUntilDone();
    chassis::moveTime(-20, -20, 150);
    roller::scoreMiddle();
    pros::delay(800);
    motor_upperSorter.move(-70);
    motor_lowerSorter.move(0);
    motor_roller.move(0);

    // 取蓝右4球
    chassis::moveTime(20, 20, 150);
    chassis::moveToPose(-10.00, 70.39, 0, {}, 4000, true);
    chassis::waitUntil(2);
    roller::save();
    chassis::waitUntil(50);
    digit_loaderArm.set_value(1);
    chassis::waitUntilDone();
    chassis::moveEnc(800);

    // 吐蓝右高塔
    chassis::moveToPoint(-36.50, 95.00, {.maxSpeed = 60});
    chassis::turnGyro(355);
    chassis::moveEnc(-800);
    chassis::move(-60, -60);
    pros::delay(200);
    roller::scoreHigh();
    pros::delay(900);
    chassis::move(0, 0);
    roller::roll(0);

    // 取蓝右填装
    roller::save();
    chassis::moveEnc(1500);
    chassis::turnGyro(357);
    digit_upperCannon.set_value(0);
    chassis::moveEnc(1500);
    chassis::moveTime(50, 50, 600);
    chassis::moveTime(40, 40, 600);
    chassis::moveTime(-10, -10, 100);
    chassis::moveTime(40, 40, 500);

    // 吐蓝左高塔
    chassis::moveEnc(-1900);
    chassis::turnGyro(450);
    chassis::moveToPoint(60.79, 105.04);
    chassis::turnGyro(450);
    chassis::moveDis(400);
    chassis::turnGyro(357.60);
    chassis::moveEnc(-800);
    chassis::move(-60, -60);
    pros::delay(200);
    roller::scoreHigh();
    pros::delay(775);
    chassis::move(0, 0);
    roller::roll(0);

    // 取蓝左填装
    roller::save();
    chassis::moveEnc(1500);
    chassis::turnGyro(358);
    digit_upperCannon.set_value(0);
    chassis::moveEnc(1500);
    chassis::moveTime(50, 50, 600);
    chassis::moveTime(40, 40, 600);
    chassis::moveTime(-10, -10, 100);
    chassis::moveTime(40, 40, 500);

    // 吐中高4蓝
    chassis::moveToPose(23.47, 67.06, 404.64, {.forwards = false, .lead = 0.3}, 6000);
    roller::scoreMiddle();
    pros::delay(800);
    roller::roll(0);
    motor_upperSorter.move(-50);
    motor_lowerSorter.move(-60);

    // 吐中下3红
    chassis::moveToPose(49.13, 56.17, 526.50, {.lead = 0.6, .minSpeed = 30}, 3500);
    chassis::moveToPose(28.89, 37.60, 665.00, {.lead = 0.6}, 10000, true);
    chassis::waitUntil(24);
    digit_loaderArm.set_value(0);
    chassis::waitUntilDone();
    chassis::moveTime(30, 30, 250);
    roller::scoreLowskill();
    pros::delay(1200);
    roller::roll(0);

    // 取红左装填
    chassis::moveToPoint(69.79, 10.53);
    chassis::turn(173);
    digit_loaderArm.set_value(1);
    chassis::moveTime(50, 50, 600);
    chassis::moveTime(40, 40, 600);
    chassis::moveTime(-10, -10, 100);
    chassis::moveTime(40, 40, 500);

    // 吐红左高塔
    chassis::turn(173);
    chassis::moveEnc(-3200);
    chassis::move(-30, 30);
    roller::scoreHigh();
    pros::delay(900);
    chassis::move(0, 0);
    roller::roll(0);
    chassis::moveTime(30, 30, 250);
    chassis::moveTime(-50, -50, 300);
    digit_loaderArm.set_value(0);

    // 清停泊+停泊
    chassis::moveToPose(55.30, 2.20, 606.30);
    digit_Rotation.set_value(1);
    roller::scoreHigh();
    chassis::move(100, 100);
    pros::delay(1000);
    chassis::move(40, 40);
    pros::delay(800);
    roller::roll(0);
}