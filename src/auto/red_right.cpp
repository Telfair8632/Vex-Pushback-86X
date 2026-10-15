#include "haws/auto.h"
#include "haws/motorcontrol/chassis.h"
#include "haws/motorcontrol/roller.h"
#include "main.h"
#include "pros/rtos.h"
#include "pros/rtos.hpp"

void redRight1() {
  chassis::resetPose(-0.85, -7.92, 0); // 设零点
  roller::startSaveAuto();             // 开始吸入
  digit_pushArm.set_value(true);       // 抬钩子
  // 吸三球
  chassis::moveToPoint(5.39, 6.39, {.minSpeed = 80});
  // 吸塔下球
  chassis::moveToPose(
      17.12, 29.54, 80.68,
      {.lead = 0.23, .minSpeed = 22, .earlyExitRange = 4.5}); // 22 4.5
  chassis::rushEnc(550, 38);
  digit_loaderArm.set_value(true);
  delay(400);
  // 对中低塔
  motor_roller.move(-30);
  motor_lowerSorter.move(55);
  digit_upperCannon.set_value(true);
  digit_loaderArm.set_value(false);
  chassis::moveToPose(0.95, 18.48, -46, {.forwards = false, .lead = 0.18});
  chassis::turnToHeading(-45);
  // 吐低塔
  chassis::moveTime(30, 30, 200);
  digit_Roller.set_value(true);
  roller::scoreLowfast();
  delay(600);
  digit_Roller.set_value(false);
  digit_upperCannon.set_value(true);
  roller::startSaveAuto();
  // //对装填桶
  // chassis::moveToPose(35.99, -6.64, -90, {.forwards=false, .lead=0.5,
  // .minSpeed=30, .earlyExitRange=1} );
  // chassis::brake(pros::E_MOTOR_BRAKE_BRAKE);
  // chassis::turnGyro(-180);
  // chassis::moveTime(30, 30, 600);
  // chassis::moveTime(18, 18, 400);
  // //吐高塔
  // chassis::rushGyro(-179, 15, 1);
  // chassis::moveEnc(-3670, 100, pros::E_MOTOR_BRAKE_COAST);
  // roller::stopSaveAuto();
  // chassis::moveTime(-30, -30, 300);
  // roller::scoreHigh();
  // pros::delay(400);
  // chassis::brake(E_MOTOR_BRAKE_BRAKE);
  // delay(500);
  // digit_loaderArm.set_value(false);
  // digit_trackCap.set_value(true);
}

void redRight2() {
  //    chassis::resetPose(-0.85, -7.92, 0);
  //     roller::startSaveAuto();
  //     digit_pushArm.set_value(true);
  //     //吸三球
  //    chassis::moveToPose(5.8, 14.57,34.58 );

  //    //去低塔
  //     chassis::moveToPose(-3.85, 27.38, -45);
  //     roller::stopSaveAuto();
  //     digit_Roller.set_value(true);
  //     roller::scoreLowfast();
  //     c::delay(800);
  //     roller::roll(0);
  //     digit_Roller.set_value(false);
  //     roller::startSaveAuto();
  //     //对装填桶
  //     chassis::moveToPoint(30, -4.8, {.forwards=false} );
  //     chassis::turnToHeading(180) ;
  //     digit_loaderArm.set_value(true);
  //     delay(100);
  //     chassis::brake(pros::E_MOTOR_BRAKE_BRAKE);
  //     chassis::moveTime(30, 30, 600);
  //     chassis::moveTime(18, 18, 500);
  //     //吐高塔，4球
  //     chassis::rushGyro(-179,15,1);
  //     chassis::moveEnc(-3670, 80, pros::E_MOTOR_BRAKE_COAST);
  //     chassis::moveTime(-40, -40, 300);
  //     roller::stopSaveAuto();
  //     roller::scoreHigh();
  //     delay(900);
  //     roller::roll(0,pros::E_MOTOR_BRAKE_HOLD);
  //     digit_loaderArm.set_value(false);
  //     // roller::roll(0, pros::E_MOTOR_BRAKE_HOLD);
  //     //准备钩
  //     chassis::moveEnc(1200);
  //     chassis::moveTime(-50, 50, 220);
  //     delay(100);
  //     chassis::moveTime(-50, -50, 440);
  //     delay(100);
  //     chassis::moveTime(50, -50, 240);
  //     delay(100);
  //     digit_pushArm.set_value(false);
  //     chassis::moveTime(-40,-40,650);
  //     chassis::moveTime(-60,60,100);
  //     chassis::brake(pros::E_MOTOR_BRAKE_HOLD);

  // 新3+4
  roller::contorl();
  digit_pushArm.set_value(true);
  // 吸装填桶
  chassis::moveEnc(4400, 60);
  chassis::turnGyro(90);
  digit_loaderArm.set_value(true);
  chassis::moveTime(18, 18, 600);
  chassis::moveTime(30, 30, 600);
  // 吐高塔
  chassis::moveEnc(-3670, 80, pros::E_MOTOR_BRAKE_COAST);
  chassis::moveTime(-20, -20, 300);
  roller::roll(0);
  roller::scoreHigh();
  c::delay(2050);
  roller::roll(0);
  digit_loaderArm.set_value(false);
  // 吸三球
  roller::startSaveAuto();
  // chassis::moveToPose(-19.19, 11.89, 235 ,{.lead=0.8});
  chassis::moveTime(60, -20, 650);
  digit_upperCannon.set_value(false);
  // 吐低塔
  chassis::moveToPose(-36.03, -3.64, -135, {.lead = 0.6, .maxSpeed = 60});
  roller::stopSaveAuto();
  digit_Roller.set_value(true);
  roller::scoreLowfast();
  c::delay(800);
  roller::roll(0);
  digit_Roller.set_value(false);
  // 钩子
  chassis::moveToPoint(-22.4, 17, {.forwards = false, .earlyExitRange = 1});
  chassis::turnGyro(90);
  digit_pushArm.set_value(false);
  chassis::moveTime(-60, -60, 500);
  chassis::moveTime(-60, 60, 100);
  chassis::brake(pros::E_MOTOR_BRAKE_HOLD);
}