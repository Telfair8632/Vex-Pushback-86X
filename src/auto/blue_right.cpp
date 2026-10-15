#include "haws/auto.h"
#include "haws/motorcontrol/chassis.h"
#include "haws/motorcontrol/roller.h"
#include "main.h"
#include "pros/rtos.h"
#include "pros/rtos.hpp"

void blueRight1() {
    digit_pushArm.set_value(false);
    chassis::resetPose(-0.85, -7.92, 0);
    roller::startSaveAuto();
    //吸三球
    chassis::moveToPose(5.8, 14.57,34.58 );
    digit_loaderArm.set_value(true);
    // //吸塔下球
    // // chassis::moveToPose(24.5, 31.07, 79.43, {.lead=0.25} );
    // chassis::moveToPose(17.12, 29.54, 80.68, {.lead=0.23, .minSpeed=22, .earlyExitRange=4.5}); //22 4.5
    // chassis::rushEnc(600, 35);
    // digit_loaderArm.set_value(true);
    // delay(300);
    //对装填桶
    chassis::moveToPose(35.99, -6.64, -90, {.forwards=false, .lead=0.5, .minSpeed=30, .earlyExitRange=1} );
    chassis::brake(pros::E_MOTOR_BRAKE_BRAKE);
    chassis::turnGyro(-180);
    chassis::moveTime(30, 30, 600);
    chassis::moveTime(18, 18, 500);
    //吐高塔，4球
    chassis::rushGyro(-179, 15, 1);
    chassis::moveEnc(-3670, 100, pros::E_MOTOR_BRAKE_COAST);
    roller::stopSaveAuto();
    chassis::moveTime(-60, -60, 200);
    roller::scoreHigh();
    pros::delay(700);
    chassis::brake(E_MOTOR_BRAKE_BRAKE);
    delay(650);
    digit_loaderArm.set_value(false);
    roller::roll(0, pros::E_MOTOR_BRAKE_HOLD);
    //钩子
    chassis::moveEnc(1200);
    chassis::moveTime(-50, 50, 220); 
    delay(100);
    chassis::moveTime(-50, -50, 440); 
    delay(100);
    chassis::moveTime(50, -50, 240); 
    delay(100);
    digit_pushArm.set_value(true);
    chassis::moveTime(-40,-40,650);
    chassis::moveTime(-60,60,100);
    chassis::brake(pros::E_MOTOR_BRAKE_HOLD);
    // //对中低塔
    // motor_roller.move(30);
    // // motor_roller.move(-30);
    // motor_lowerSorter.move(60);
    // chassis::rushEnc(1300, 70);//42
    // motor_lowerSorter.move(-50);
    // // motor_roller.move(-20);
    // chassis::moveToPose(-0.33, 23.54, -46);
    // //吐低塔
    // digit_Roller.set_value(true);
    // roller::scoreLowfast();
    // delay(300);
    // digit_Roller.set_value(false);
    // chassis::moveEnc(300, 30);
    // digit_upperCannon.set_value(true);
    // roller::startSaveAuto();
    // // 对高塔钩钩子
    // // chassis::moveToPose(17.87, 11.31, -160, {.forwards=false, .lead=0.4});
    // chassis::moveEnc(-5000,70);
    
    // chassis::turnGyro(-180);
    // digit_pushArm.set_value(false);
    // // chassis::rushEnc(-1000, 60);
    // // chassis::rushGyro(-180, 60, 2);  
    // chassis::moveEnc(-3500, 50);
    // chassis::brake(pros::E_MOTOR_BRAKE_HOLD);
    // chassis::turnGyro(-200);
    // chassis::brake(pros::E_MOTOR_BRAKE_HOLD);
    // roller::stopSaveAuto();

    // return;


    
}

void blueRight2() {
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

    //新3+4
    roller::startSaveAuto();
    digit_pushArm.set_value(false);
    //吸装填桶
    chassis::moveEnc(4450);
    chassis::turnGyro(90);
    digit_loaderArm.set_value(true);
    chassis::moveTime(30, 30, 600);
    chassis::moveTime(18, 18, 400);
    //吐高塔
    chassis::moveEnc(-3670, 80, pros::E_MOTOR_BRAKE_COAST);
    chassis::moveTime(-20, -20, 300);
    roller::stopSaveAuto();
    roller::scoreHigh();
    c::delay(1050);
    digit_upperCannon.set_value(false);
    roller::roll(0);
    digit_loaderArm.set_value(false);
    //吸三球
    roller::startSaveAuto();
    // chassis::moveToPose(-19.19, 11.89, 235 ,{.lead=0.8});
    chassis::moveTime(60, -80, 650);
    //吐低塔
    chassis::moveToPoint(-35.93, -0.99, {.maxSpeed=60});
    roller::stopSaveAuto();
    digit_Roller.set_value(true);
    roller::scoreLowfast();
    c::delay(800);
    roller::roll(0);
    digit_Roller.set_value(false);
    chassis::moveTime(-20, -20, 200);
    chassis::moveTime(30, 30, 200);
    //钩子
    // chassis::moveToPose(-28, 21.5, 90, {.forwards=false});
    chassis::moveEnc(-5200,80);
    chassis::turnGyro(90);
    digit_pushArm.set_value(true);
    chassis::moveTime(-60,-60,750);
    chassis::moveTime(-60,60,100);
    chassis::brake(pros::E_MOTOR_BRAKE_HOLD);
    
}