#include "haws/motorcontrol/xdrive.h"
#include "haws/config.hpp"
#include "pros/motors.h"
#include <stdlib.h>

namespace xdrive {

pros::Motor *motorFL = nullptr;
pros::Motor *motorFR = nullptr;
pros::Motor *motorBL = nullptr;
pros::Motor *motorBR = nullptr;

void init() {
    motorFL = new pros::Motor(abs(PORT_XDRIVE_FL), pros::MotorGearset::blue);
    motorFR = new pros::Motor(abs(PORT_XDRIVE_FR), pros::MotorGearset::blue);
    motorBL = new pros::Motor(abs(PORT_XDRIVE_BL), pros::MotorGearset::blue);
    motorBR = new pros::Motor(abs(PORT_XDRIVE_BR), pros::MotorGearset::blue);

    if (PORT_XDRIVE_FL < 0) motorFL->set_reversed(true);
    if (PORT_XDRIVE_FR < 0) motorFR->set_reversed(true);
    if (PORT_XDRIVE_BL < 0) motorBL->set_reversed(true);
    if (PORT_XDRIVE_BR < 0) motorBR->set_reversed(true);

    motorFL->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    motorFR->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    motorBL->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    motorBR->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}

void control(int x, int y, int r) {
    // X-Drive 运动学公式
    // 四个全向轮呈X形安装, 辊子朝向中心
    int fl = y + x + r;
    int fr = y - x - r;
    int bl = y - x + r;
    int br = y + x - r;

    // 限幅: 任一值超过127则等比缩放
    int maxVal = abs(fl);
    if (abs(fr) > maxVal) maxVal = abs(fr);
    if (abs(bl) > maxVal) maxVal = abs(bl);
    if (abs(br) > maxVal) maxVal = abs(br);

    if (maxVal > 127) {
        fl = fl * 127 / maxVal;
        fr = fr * 127 / maxVal;
        bl = bl * 127 / maxVal;
        br = br * 127 / maxVal;
    }

    motorFL->move(fl);
    motorFR->move(fr);
    motorBL->move(bl);
    motorBR->move(br);
}

void stop() {
    motorFL->move(0);
    motorFR->move(0);
    motorBL->move(0);
    motorBR->move(0);
}

void brake(pros::motor_brake_mode_e mode) {
    motorFL->set_brake_mode(mode);
    motorFR->set_brake_mode(mode);
    motorBL->set_brake_mode(mode);
    motorBR->set_brake_mode(mode);
    motorFL->brake();
    motorFR->brake();
    motorBL->brake();
    motorBR->brake();
}

} // namespace xdrive
