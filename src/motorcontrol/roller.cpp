#include "haws/motorcontrol/roller.h"
#include "haws/config.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include <cstdint>

namespace roller {
Mutex isRunSaveMutex; // 互斥锁
bool isRunSave = false;
bool isAuto = true;
int32_t lastProximity = 0;

void roll(int power, motor_brake_mode_e brakeMode) {
  if (power == 0) {
    motor_roller.set_brake_mode(brakeMode);
    motor_upperSorter.set_brake_mode(brakeMode);
    motor_lowerSorter.set_brake_mode(brakeMode);
    motor_roller.brake();
    motor_upperSorter.brake();
    motor_lowerSorter.brake();
  } else {
    motor_roller.move(power * 1.27);
    motor_upperSorter.move(power * 1.27);
    motor_lowerSorter.move(power * 1.27);
  }
}

void save() {
  motor_roller.move(127);
  motor_lowerSorter.move(127);
  int32_t proximity = sensor_color.get_proximity();
  if (proximity >= 140) {
    if (lastProximity < 140) {
      motor_upperSorter.tare_position();
    }
    if (motor_upperSorter.get_position() < 100) {
      motor_upperSorter.move(90);
    } else {
      motor_upperSorter.move(30);
    }
  } else {
    motor_upperSorter.move(100);
  }
  lastProximity = proximity;
}

void scoreHigh() {
  motor_upperSorter.move(127);
  motor_lowerSorter.move(127);
  motor_roller.move(127);
  digit_upperCannon.set_value(true);
}

void reload() {
  motor_upperSorter.move(127);
  motor_lowerSorter.move(0);
  motor_roller.move(-60);
}
void contorl() {
  motor_upperSorter.move(0);
  motor_lowerSorter.move(127);
  motor_roller.move(-60);
}
void scoreMiddle() {
  motor_upperSorter.move(-60);
  motor_lowerSorter.move(90);
  motor_roller.move(127);
  digit_trackCap.set_value(true);
}
void scoreMiddleAuto() {
  motor_upperSorter.move(-80);
  motor_lowerSorter.move(90);
  motor_roller.move(127);
  digit_trackCap.set_value(true);
}
void scoreLowskill() {
  motor_upperSorter.move(-70);
  motor_lowerSorter.move(-60);
  motor_roller.move(-90);
  digit_Roller.set_value(true);
}
void scoreLowfast() {
  motor_upperSorter.move(-127);
  motor_lowerSorter.move(-127);
  motor_roller.move(-127);
  digit_Roller.set_value(true);
}
void scoreLowAuto() {
  int timer = 0;
  int totalTimer = 0;
  while (isAuto) {
    if (timer >= 800 || totalTimer >= 2000) {
      break;
    }

    scoreLowskill();
    pros::delay(20);
    timer += 20;
    totalTimer += 20;
  }
  roll(0, pros::E_MOTOR_BRAKE_HOLD);
}

void scoreLowBoom() {
  // motor_upperSorter.move(-127);
  motor_roller.move(-127);
  motor_lowerSorter.move(-100);
}

void prepareMiddleAuto() {
  while (sensor_color.get_proximity() < 50) {
    motor_roller.move(90);
    motor_lowerSorter.move(50);
    motor_upperSorter.move(40);
  }
  roll(0);
}

void saveAuto() {
  while (true) {
    isRunSaveMutex.take(200);
    if (!isRunSave) {
      isRunSaveMutex.give();
      break;
    }
    isRunSaveMutex.give();

    save();
    pros::delay(20);
  }
}

void startSaveAuto() {
  isRunSaveMutex.take(200);
  if (isRunSave) {
    isRunSave = false;
    pros::delay(20);
  }
  isRunSave = true;
  isRunSaveMutex.give();
  Task autoSave(saveAuto);
  logger::info("startSaveAuto");
}

void stopSaveAuto() {
  isRunSaveMutex.take(200);
  isRunSave = false;
  isRunSaveMutex.give();
  roll(0);
}

void scoreHighUntilColorAuto(bool isBlueStop, int maxTime) {
  int timer = 0;
  int totalTimer = 0;
  while (isAuto) {
    if (timer >= 800 || totalTimer >= maxTime) {
      break;
    }
    if (sensor_color.get_proximity() > 140) {
      timer = 0;
      if (isBlueStop && sensor_color.get_hue() > 120 ||
          !isBlueStop && sensor_color.get_hue() < 60) {
        break;
      }
    }
    scoreHigh();
    pros::delay(20);
    timer += 20;
    totalTimer += 20;
  }
  scoreLowskill();
  pros::delay(150);
  roll(0, pros::E_MOTOR_BRAKE_HOLD);
}

void scoreHighAuto() {
  int timer = 0;
  int totalTimer = 0;
  while (isAuto) {
    if (timer >= 800 || totalTimer >= 2000) {
      break;
    }

    scoreHigh();
    pros::delay(20);
    timer += 20;
    totalTimer += 20;
  }
  roll(0, pros::E_MOTOR_BRAKE_HOLD);
}

void scoreLow2Auto() {
  while (isAuto) {
    int dis = sensor_dis.get_distance();
    if (dis < 90) {
      break;
    }
    scoreLowskill();
    pros::delay(20);
  }

  motor_roller.tare_position();
  while (isAuto) {
    motor_roller.move(-80);
    motor_lowerSorter.move(-70);
    if (motor_roller.get_position() < -660) {
      break;
    }
    pros::delay(20);
  }
  roll(0, pros::E_MOTOR_BRAKE_HOLD);
}

void stopAll() {
  isAuto = false;
  stopSaveAuto();
}

void startAuto() { isAuto = true; }
} // namespace roller