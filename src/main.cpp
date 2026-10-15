#include "main.h"
#include "haws/auto.h"
#include "haws/motorcontrol/chassis.h"
#include "haws/motorcontrol/xdrive.h"
#include "haws/logger/logger.h"
#include "haws/motorcontrol/roller.h"
#include "pros/llemu.hpp"
#include "pros/motors.h"




bool autoMoveRunning = false;// 标记是否正在自动移动
pros::Task *autoMoveTask = nullptr; // 存储线程对象
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	// 底盘初始化, 陀螺仪初始化
	chassis::init();
	xdrive::init();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
    chassis::cancelAllMotions();
    roller::stopAll();
    xdrive::stop();
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
 int autoSelection = 4;
 const char *autoTitles[] = {
     "技能",    
     "红左9",
     "红左3+4",
     "红右9",
     "红右3+4",
     "蓝左9",
     "蓝左3+4",
     "蓝右7",
     "蓝右3+4"};
void autonomous() {
    logger::init();
    logger::info("auto selection: %d", autoSelection);
    switch (autoSelection) {
    case 0:
        skill();
        break;
    case 1:
        redLeft1();
        break;
    case 2:
        redLeft2();
        break;
    case 3:
        redRight1();
        break;
    case 4:
        redRight2();
        break;
    case 5:
        blueLeft1();
        break;
    case 6:
        blueLeft2();
        break;
    case 7:
        blueRight1();
        break;
    case 8:
        blueRight2();
        break;
    }
}

// 是否进入选自动程序
bool isSelectAuto = true;
// 选自动
void autoSelector() {
    controller.clear_line(2);
    pros::delay(50);
    controller.print(2, 0, "[ ]%s", autoTitles[autoSelection]);
    while (isSelectAuto) {
    //     // TODO: 显示陀螺仪传感器的值
    //     // master.print(2, 20, "gyro:%.3f", gyro.get_rotation());
        if (controller.get_digital_new_press(DIGITAL_LEFT)) {
            autoSelection = CONSTRAIN(autoSelection - 1, 0, 8);
            controller.clear_line(2);
            pros::delay(50);
            controller.print(2, 0, "[ ]%s", autoTitles[autoSelection]);
        }
        if (controller.get_digital_new_press(DIGITAL_RIGHT)) {
            autoSelection = CONSTRAIN(autoSelection + 1, 0, 8);
            controller.clear_line(2);
            pros::delay(50);
            controller.print(2, 0, "[ ]%s", autoTitles[autoSelection]);
        }
        if (controller.get_digital_new_press(DIGITAL_B)) {
            controller.print(2, 1, "x");
            pros::delay(800);
            controller.clear_line(2);
            break;
        }
        pros::delay(50);
    }
    pros::delay(50);
    isSelectAuto = false;
}

void autoMoveTaskFunc() {
    autoMoveRunning = true;
    // 这里面填想走的自动, 每一句之后都加一个判断
    chassis::moveTime(80, -80, 300); 
    if (!autoMoveRunning) {
        chassis::brake(pros::E_MOTOR_BRAKE_COAST);
        return;
    }
    chassis::moveTime(65, 65, 100); 
    if (!autoMoveRunning) {
        chassis::brake(pros::E_MOTOR_BRAKE_COAST);
        return;
    }
    chassis::moveTime(-80, 80, 130);
    
    chassis::brake(pros::E_MOTOR_BRAKE_COAST);
}
/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
      
    chassis::brake(pros::E_MOTOR_BRAKE_COAST);
    bool trackCapState = false;
    bool pushArmState = false;
    bool loadArmState = false;
    bool upperCannonState = true;
    bool rollerState = true;

    if (isSelectAuto) {
        autoSelector();
    }
    while (true) {
        if (controller.get_analog(ANALOG_RIGHT_X) > 3 ||
           controller.get_analog(ANALOG_RIGHT_X) < -3 ||
           controller.get_analog(ANALOG_RIGHT_Y) > 3 ||
           controller.get_analog(ANALOG_RIGHT_Y) < -3 ||
           controller.get_analog(ANALOG_LEFT_X) > 3 ||
           controller.get_analog(ANALOG_LEFT_X) < -3 ||
           controller.get_analog(ANALOG_LEFT_Y) > 3 ||
           controller.get_analog(ANALOG_LEFT_Y) < -3)
           break;
        pros::lcd::register_btn0_cb(logger::pageUp);
		pros::lcd::register_btn2_cb(logger::pageDown);
		chassis::printPose(7);
        pros::delay(10);
    }
    // 这里写抬气动
    digit_Rotation.set_value(true);

	while (true) {
        
		// pros::lcd::print(7, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		//                  (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		//                  (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  // Prints status of the emulated screen LCDs
        pros::lcd::register_btn0_cb(logger::pageUp);
		pros::lcd::register_btn2_cb(logger::pageDown);
		chassis::printPose(7);
        // pros::lcd::print(6,"%d", sensor_color.get_proximity() );
        
		// Arcade control scheme
		int ch1 = controller.get_analog(ANALOG_RIGHT_X);
        int ch2 = controller.get_analog(ANALOG_RIGHT_Y);
        int ch3 = controller.get_analog(ANALOG_LEFT_Y);
        int ch4 = controller.get_analog(ANALOG_LEFT_X);
        bool l1 = controller.get_digital_new_press(DIGITAL_L1);
        bool l2 = controller.get_digital(DIGITAL_L2);
        bool r1 = controller.get_digital(DIGITAL_R1);
        bool r2 = controller.get_digital(DIGITAL_R2);
        bool btnA = controller.get_digital(DIGITAL_A);
        bool btnB = controller.get_digital_new_press(DIGITAL_B);
        bool btnX = controller.get_digital(DIGITAL_X);
        bool btnY = controller.get_digital_new_press(DIGITAL_Y);
        bool btnU = controller.get_digital_new_press(DIGITAL_UP);
        bool btnD = controller.get_digital_new_press(DIGITAL_DOWN);
        bool btnL = controller.get_digital_new_press(DIGITAL_LEFT);
        bool btnR = controller.get_digital_new_press(DIGITAL_RIGHT);

         //如果摇杆有输入，则立即打断自动动作
        if (abs(ch3) > 10 || abs(ch4) > 10 || abs(ch1) > 10) {
            autoMoveRunning = false;  // 通知线程退出
        }

		#ifdef CHASSIS_CONTROL_TANK
			chassis::tank(ch3, ch2);
		#elifdef CHASSIS_CONTROL_ARCADE_ONE
			if(!autoMoveRunning){
                chassis::arcade(ch3, ch4);
            }
		#elifdef CHASSIS_CONTROL_ARCADE_TWO
			chassis::arcade(ch3, ch1);
		#elifdef CHASSIS_CONTROL_XDRIVE
			if(!autoMoveRunning){
                xdrive::control(ch4, -ch3, ch1);
            }
		#endif

		if (r1) {
            roller::save();
		}
		else if (r2) {
            roller::scoreLowfast();
		}
		else if (btnX) {
			roller::scoreHigh();
		}
        else if(btnA){
            roller::scoreMiddle();
        }

        else if(l2){
            roller::reload();
        }

        else{
            roller::roll(0,pros::E_MOTOR_BRAKE_HOLD);
        }
            
		if (!btnX) {
            digit_upperCannon.set_value(false);
		}
		if (!btnA) {
            digit_trackCap.set_value(false);
		}
        if (!r2) {
            digit_Roller.set_value(false);
		}
        if (btnB){
            pushArmState = !pushArmState;
            digit_pushArm.set_value(pushArmState);
        }
        if (btnY){
            loadArmState = !loadArmState;
            digit_loaderArm.set_value(loadArmState);
        }
        if(btnU){
            rollerState = !rollerState;
            digit_Roller.set_value(rollerState);

        }

        if (btnD) {
            if (autoMoveRunning) {
                autoMoveRunning = false;
                pros::delay(20);
            }
            Task autoMove(autoMoveTaskFunc);
        }

		pros::delay(20);  // Run for 20 ms then update
	}
}