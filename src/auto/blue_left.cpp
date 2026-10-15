#include "haws/auto.h"
#include "haws/motorcontrol/chassis.h"
#include "haws/motorcontrol/roller.h"
#include "pros/abstract_motor.hpp"
#include "pros/rtos.h"
#include "pros/rtos.hpp"

void blueLeft1() {

  chassis::resetPose(0.85, -7.92, 0);
  roller::startSaveAuto();
  digit_pushArm.set_value(false);
  // 吸三球
  chassis::moveToPoint(-5.39, 6.39, {.minSpeed = 80});
  // 吸塔下球
  //  chassis::moveToPose(24.5, 31.07, 79.43, {.lead=0.25} );
  chassis::moveToPose(
      -17.12, 29.54, -80.68,
      {.lead = 0.23, .minSpeed = 22, .earlyExitRange = 4.5}); // 22 4.5
  chassis::rushEnc(500, 35);
  digit_loaderArm.set_value(true);
  delay(300);
  // 对中高塔
  //  chassis::moveEnc(-7400,60);
  chassis::moveToPose(3.43, 28.64, 225, {.forwards = false});
  roller::stopSaveAuto();
  delay(100);

  // 吐中高塔
  roller::scoreMiddleAuto();
  delay(1000);
  roller::roll(0);
  motor_upperSorter.move(100);
  motor_roller.move(-127);
  delay(100);

  // 对装填桶
  chassis::moveToPose(-28.5, -6.64, 180, {.forwards = true, .lead = 0.2});
  roller::roll(0);
  digit_trackCap.set_value(false);
  roller::startSaveAuto();
  chassis::brake(pros::E_MOTOR_BRAKE_BRAKE);
  chassis::moveTime(30, 30, 600);
  chassis::moveTime(18, 18, 500);
  // 吐高塔，4球
  chassis::rushGyro(-181, 15, 1);
  chassis::moveEnc(-3670, 60, pros::E_MOTOR_BRAKE_COAST);
  chassis::moveTime(-40, -40, 300);
  roller::stopSaveAuto();
  roller::scoreHigh();
  delay(1000); // Q54 all in
  roller::scoreLowfast();
  delay(100);
  roller::scoreHigh();
  delay(1000);
  digit_loaderArm.set_value(false);
  roller::roll(0, pros::E_MOTOR_BRAKE_HOLD);

  // 对高塔钩钩子
  chassis::moveTime(50, -50, 450);
  chassis::moveTime(50, 50, 240);
  digit_pushArm.set_value(true);
  chassis::moveTime(-50, 50, 240);
  chassis::moveTime(-50, -50, 440);
  // delay(100);
  // chassis::moveTime(50, -50, 240);
  // delay(100);
  // digit_pushArm.set_value(false);
  // chassis::moveTime(-40,-40,200);//last 650ms slow hook and short
  // chassis::moveTime(-60,60,100);
  // chassis::brake(pros::E_MOTOR_BRAKE_HOLD);

  // chassis::moveEnc(5300);
  // chassis::turnToHeading(360);

  // // chassis::moveToPose(-22.08, 23.59, 360,{.lead=0.15});
  // digit_pushArm.set_value(false);
  // chassis::moveEnc(2500, 50);
  // // chassis::turnToHeading(390);
  // roller::stopSaveAuto();
}
void blueLeft2() {
  chassis::resetPose(0.85, -7.92, 0);
  roller::startSaveAuto();
  digit_pushArm.set_value(true);
  // 吸三球
  chassis::moveToPoint(-5.09, 6.09, {.minSpeed = 80});
  delay(300);
  digit_loaderArm.set_value(true);
  delay(300);
  // 吐中高塔
  //  chassis::moveToPose(1.33, 24.6, 225,{.forwards=false});
  chassis::moveToPose(5.57, 27.91, -135, {.forwards = false});
  roller::stopSaveAuto();
  chassis::moveTime(-40, -40, 180);
  roller::scoreMiddleAuto();
  c::delay(900);
  roller::roll(0);
  motor_roller.move(-100);
  motor_upperSorter.move(90);
  motor_lowerSorter.move(127);
  // 对装填桶
  delay(100);
  chassis::moveToPose(-29.9, -6.64, -180, {.forwards = true, .lead = 0.2});
  digit_trackCap.set_value(false);
  roller::roll(0);
  roller::startSaveAuto();
  chassis::brake(pros::E_MOTOR_BRAKE_BRAKE);
  chassis::moveTime(30, 30, 550);
  chassis::moveTime(18, 18, 400);
  // 吐高塔，4球
  chassis::rushGyro(-181, 15, 1);
  chassis::moveEnc(-3670, 80, pros::E_MOTOR_BRAKE_COAST);
  chassis::moveTime(-40, -40, 300);
  roller::stopSaveAuto();
  // roller::scoreHighUntilColorAuto(false);//false识别红色停
  roller::scoreHigh();
  delay(900);
  roller::roll(0, pros::E_MOTOR_BRAKE_HOLD);
  digit_loaderArm.set_value(false);
  // roller::roll(0, pros::E_MOTOR_BRAKE_HOLD);
  // 准备钩
  chassis::moveEnc(1200);
  chassis::moveTime(-50, 50, 220);
  delay(100);
  chassis::moveTime(-50, -50, 440);
  delay(100);
  chassis::moveTime(50, -50, 240);
  delay(100);
  digit_pushArm.set_value(false);
  chassis::moveTime(-40, -40, 650);
  chassis::moveTime(-60, 60, 100);
  chassis::brake(pros::E_MOTOR_BRAKE_HOLD);
}