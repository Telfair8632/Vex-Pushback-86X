#include "haws/motorcontrol/chassis.h"
#include "fmt/core.h"
#include "pros/llemu.hpp"

namespace chassis
{
Kernel kernel;

void init() {
    kernel.getChassis()->calibrate();
}

void tank(int left, int right) {
    kernel.getChassis()->tank(left, right);
}

void arcade(int throttle, int turn) {
    kernel.getChassis()->arcade(throttle, turn);
}

void curvature(int throttle, int turn) {
    kernel.getChassis()->curvature(throttle, turn);
}

void cancelAllMotions() {
    kernel.getChassis()->cancelAllMotions();
}

void resetPose(float x, float y, float theta) {
    Pose oldPose = kernel.getChassis()->getPose();
    Pose newPose(x, y, theta);
    logger::info("reset() (%f,%f,%f) ->", oldPose.x, oldPose.y, oldPose.theta);
    logger::info("        (%f,%f,%f)", newPose.x, newPose.y, newPose.theta);
    kernel.getChassis()->setPose(newPose, false);
}

void turnToHeading(float theta, TurnToHeadingParams params) {
    kernel.getChassis()->turnToHeading(theta, 10000, params, false);
    logger::info("turn() tar: %f, cur %f", theta, kernel.getChassis()->getPose().theta);
}
void move(int lPower, int rPower) {
    kernel.getChassis()->tank(lPower * 1.27, rPower * 1.27);
}

void turnToPoint(float x, float y, TurnToPointParams params) {
    kernel.getChassis()->turnToPoint(x, y, 10000, params, false);
    logger::info("turn() tar: (%f,%f), cur %f", x, y, kernel.getChassis()->getPose().theta);
}

void moveToPoint(float x, float y, MoveToPointParams params) {
    kernel.getChassis()->moveToPoint(x, y, 10000, params, false);
    Pose pose = kernel.getChassis()->getPose();
    logger::info("move2 tar: (%f,%f)", x, y);
    logger::info("point cur: (%f,%f)", pose.x, pose.y);
}

void moveToPose(float x, float y, float theta, MoveToPoseParams params, int timeout, bool async) {
    kernel.getChassis()->moveToPose(x, y, theta, timeout, params, async);
    Pose pose = kernel.getChassis()->getPose();
    logger::info("move2 tar: (%f,%f,%f),", x, y, theta);
    logger::info("pose  cur: (%f,%f,%f)", pose.x, pose.y, pose.theta);
}

void waitUntil(float dist) {
    kernel.getChassis()->waitUntil(dist);
}

void waitUntilDone(){
    kernel.getChassis()->waitUntilDone();
}

void printPose(int line) {
    if (kernel.getChassis() == nullptr) {
        pros::lcd::print(line - 1, "%s", kernel.selfCheck());
    }
    Pose pose = kernel.getChassis()->getPose();
    // pros::lcd::print(line, "| x %.2f | y %.2f | t %.2f | r1 %d", pose.x, pose.y, pose.theta, kernel.getPosition());
    pros::lcd::print(line, "| x %.2f | y %.2f | t %.2f | gy %.2f", pose.x, pose.y, pose.theta, (float) kernel.sensor_gyro->get_rotation());
    // fmt::format("{}", pose.x)
}

Kernel getKernel() {
    return kernel;
}

void moveTime(int lPower, int rPower, int time) {
    move(lPower, rPower);
    pros::delay(time);
    move(0, 0);
}

void brake(motor_brake_mode_e brakeMode) {
    kernel.brake(brakeMode);
}

void rushEnc(int distance, int power) {
    int err = distance;
    int timer = 0;
    kernel.reset();
    if (err > 0) {
        while (err > 10) {
            chassis::move(power, power);
            pros::delay(15);
            timer += 15;
            err = distance - kernel.getPosition();
        }
    } else {
        while (err < -10) {
            chassis::move(-power, -power);
            pros::delay(15);
            timer += 15;
            err = distance - kernel.getPosition();
        }
    }
    chassis::move(0, 0);
    kernel.brake();
    pros::delay(10);
    logger::info("rushEnc: [%d / %d]", kernel.getPosition(), distance);
    // logger::info("rushEnc: [%d / %d], time=%Ds", kernel.getPosition(), distance, double (timer / 1000.0));
}

void moveEnc(int distance, int maxPower, motor_brake_mode_e brakeMode, std::function<void()> func, int funcStart) {
    float err = 0.0, lastErr = 0.0, totalErr = 0.0, deltaErr = 0.0; // 用于计算直行pid
    float power = 0.0; // 输出功率
    err = distance;
    
    if (fabs(err) < 100) {
        rushEnc(distance, 30);
        return;
    }

    float kp = 0.1, ki = 0.01, kd = 0.55; // 直行pid值
    // 计时器
    int timer = 0; // 用于超时退出循环，未使用计时器计时，timer每次循环+15
    int timeout = fabs(err) < 500 ? 1200 : fabs(err) * 3; // 超时时间
    timeout = timeout / maxPower * 100;
    
    kernel.reset();
    while (true) {
        // 分段函数执行
        if (funcStart != 0 &&
            ((funcStart > 0 && kernel.getPosition() > funcStart) || 
            (funcStart < 0 && kernel.getPosition() < funcStart))) {
            Task execFunc(func);
        }
        // 计算直行pid
        err = distance - kernel.getPosition();
        // 控制积分值在1以下，且分段积分
        if (fabs(err) < 50) {
            totalErr += err;
        }
        deltaErr = err - lastErr;
        lastErr = err;
        // 判据为转向超过目标值，需要反转的情况
        if (err * lastErr <= 0) {
            totalErr = 0;
        }
        power = kp * err + ki * totalErr + kd * deltaErr;
        // 缓加速
        power = power * CONSTRAIN(timer, 0, 210) / 210.0;
        // 退出条件
        if (fabs(err) < 10 || timer > timeout) {
            break;
        }
        // TODO: 确认是否需要在超限时再累计一个power_gyro
        chassis::move(CONSTRAIN(power, -maxPower, maxPower),
                      CONSTRAIN(power, -maxPower, maxPower));
        // 分段积分
        if (fabs(err) > abs(distance) * 0.2 || fabs(err) < 1) {
            totalErr = 0;
        }
        // 延时
        pros::delay(15);
        timer += 15;
    }
    chassis::move(0, 0);
    kernel.brake(brakeMode);
    pros::delay(10);
    logger::info("moveEnc: [%d / %d]", kernel.getPosition(), distance);
    // logger::info("moveEnc: [%f / %d], time=%Ds", kernel.getPosition(), distance, double (timer / 1000.0));
}

void moveEncStright(int distance, int maxPower, motor_brake_mode_e brakeMode, std::function<void()> func, int funcStart) {
    float err = 0.0, lastErr = 0.0, totalErr = 0.0, deltaErr = 0.0; // 用于计算直行pid
    float power = 0.0; // 输出功率
    float heading = kernel.sensor_gyro->get_rotation();
    err = distance;
    
    if (fabs(err) < 100) {
        rushEnc(distance, 30);
        return;
    }

    float kp = 0.3, ki = 0.07, kd = 2.2; // 直行pid值
    if (abs(distance) > 1000) { kp = 0.12; ki = 0.01; kd = 0.15; }
    // if (abs(distance) < 100) { kp = 0.6; kd = 0.8; }
    // 计时器
    int timer = 0; // 用于超时退出循环，未使用计时器计时，timer每次循环+15
    int timeout = fabs(err) < 500 ? 1200 : fabs(err) * 3; // 超时时间
    timeout = timeout / maxPower * 100;
    
    kernel.reset();
    while (true) {
        // 分段函数执行
        if (funcStart != 0 &&
            ((funcStart > 0 && kernel.getPosition() > funcStart) || 
            (funcStart < 0 && kernel.getPosition() < funcStart))) {
            Task execFunc(func);
        }
        // 计算直行pid
        err = distance - kernel.getPosition();
        // 控制积分值在1以下，且分段积分
        if (fabs(err) < 50) {
            totalErr += err;
        }
        deltaErr = err - lastErr;
        lastErr = err;
        // 判据为转向超过目标值，需要反转的情况
        if (err * lastErr <= 0) {
            totalErr = 0;
        }
        power = kp * err + ki * totalErr + kd * deltaErr;
        // 缓加速
        power = power * CONSTRAIN(timer, 0, 210) / 210.0;
        // 退出条件
        if (fabs(err) < 10 || timer > timeout) {
            break;
        }
        float powerGyro = floor(heading - kernel.sensor_gyro->get_rotation()) * 5;
        if (fabs(err) < 20) {
            powerGyro = 0;
        }
        // TODO: 确认是否需要在超限时再累计一个power_gyro
        chassis::move(CONSTRAIN(power, -maxPower, maxPower) + powerGyro,
                      CONSTRAIN(power, -maxPower, maxPower) - powerGyro);
        // 分段积分
        if (fabs(err) > abs(distance) * 0.2 || fabs(err) < 1) {
            totalErr = 0;
        }
        // 延时
        pros::delay(15);
        timer += 15;
    }
    chassis::move(0, 0);
    kernel.brake(brakeMode);
    pros::delay(10);
    // logger::info("moveEnc: [%f / %d], time=%Ds", kernel.getPosition(), distance, double (timer / 1000.0));
}

void rushGyro(int angle, int power, int maxErr) {
    float err = angle - kernel.sensor_gyro->get_rotation();
    int timer = 0;
    if (err > 0) {
        while (err > maxErr) {
            chassis::move(power, -power);
            pros::delay(15);
            timer += 15;
            err = angle - kernel.sensor_gyro->get_rotation();
        }
    } else {
        while (err < -maxErr) {
            chassis::move(-power, power);
            pros::delay(15);
            timer += 15;
            err = angle - kernel.sensor_gyro->get_rotation();
        }
    }
    chassis::move(0, 0);
    kernel.brake();
    pros::delay(10);
    logger::info("rushGyro: [%f / %d]", (float) kernel.sensor_gyro->get_rotation(), angle);
    // logger::info("rushGyo: [%f / %d], time=%Ds", kernel.sensor_gyro->get_rotation(), angle, double (timer / 1000.0));
}

void turnGyro (int target) {
    float err = 0.0, lastErr = 0.0, totalErr = 0.0, deltaErr = 0.0; // 用于计算pid
    float power = 0.0; // 输出功率
    err = target - kernel.sensor_gyro->get_rotation();

    if (fabs(err) < 20) {
        rushGyro(target, 25, 3);
        return;
    }

    // pid值
    float kp = 1.25, ki = 3.0, kd = 7.0;
    // 计时器
    int timer = 0;
    int timeout = fabs(err) < 20 ? 700 : fabs(err) * 20;
    
    kernel.reset();
    while (true) {
        err = target - kernel.sensor_gyro->get_rotation();
        deltaErr = err - lastErr;
        lastErr = err;
        // 控制积分值在1以下，且分段积分
        if (fabs(totalErr) < 1 && fabs(err) < 30) {
            totalErr += err / fabs(err) * 0.01;
        }
        // 判据为转向超过目标值，需要反转的情况
        if (err * lastErr <= 0) {
            totalErr = 0;
        }
        power = kp * err + kd * deltaErr + ki * totalErr;
        // 退出条件
        if ((fabs(err) < 3 && abs(kernel.getActualVelocity()) < 5) ||
            (fabs(err) < 1 && fabs(deltaErr) <= 0.1) ||
            timer > timeout) {
            break;
        }
        // 控制转向
        chassis::move(CONSTRAIN(power, -100, 100),
                      CONSTRAIN(-power, -100, 100));
        // 延时
        pros::delay(10);
        timer += 10;
    }
    chassis::move(0, 0);
    kernel.brake();
    pros::delay(10);
    logger::info("turnGyro: [%f / %d]", (float) kernel.sensor_gyro->get_rotation(), target);
    // logger::info("turnGyo: [%d / %d], time=%Ds", kernel.sensor_gyro->get_rotation(), target, double (timer / 1000.0));
}
void turn (int target) {
    float err = 0.0, lastErr = 0.0, totalErr = 0.0, deltaErr = 0.0; // 用于计算pid
    float power = 0.0; // 输出功率
    err = target - kernel.sensor_gyro->get_yaw();

    if (fabs(err) < 20) {
        rushGyro(target, 25, 3);
        return;
    }

    // pid值
    float kp = 3.2, ki = 1.3, kd = 6.5;
    // 计时器
    int timer = 0;
    int timeout = fabs(err) < 20 ? 700 : fabs(err) * 20;
    
    kernel.reset();
    while (true) {
        err = target - kernel.sensor_gyro->get_yaw();

        deltaErr = err - lastErr;
        lastErr = err;
        // 控制积分值在1以下，且分段积分
        if (fabs(totalErr) < 1 && fabs(err) < 30) {
            totalErr += err / fabs(err) * 0.01;
        }
        // 判据为转向超过目标值，需要反转的情况
        if (err * lastErr <= 0) {
            totalErr = 0;
        }
        power = kp * err + kd * deltaErr + ki * totalErr;
        // 退出条件
        if ((fabs(err) < 3 && abs(kernel.getActualVelocity()) < 5) ||
            (fabs(err) < 1 && fabs(deltaErr) <= 0.1) ||
            timer > timeout) {
            break;
        }
        // 控制转向
        chassis::move(CONSTRAIN(power, -100, 100),
                      CONSTRAIN(-power, -100, 100));
        // 延时
        pros::delay(10);
        timer += 10;
    }
    chassis::move(0, 0);
    kernel.brake();
    pros::delay(10);
    logger::info("turn: [%f / %d]", (float) kernel.sensor_gyro->get_yaw(), target);
    // logger::info("turnGyo: [%d / %d], time=%Ds", kernel.sensor_gyro->get_rotation(), target, double (timer / 1000.0));
}

void moveDis(int distance, int maxPower, motor_brake_mode_e brakeMode, std::function<void()> func, int funcStart) {
    float cur = 0.0, err = 0.0, lastErr = 0.0, totalErr = 0.0, deltaErr = 0.0; // 用于计算直行pid
    float power = 0.0; // 输出功率

    err = sensor_dis.get_distance() - distance;
    lastErr = err;
    
    if (fabs(err) < 30) {
        rushDis(distance, 30);
        return;
    }

    float kp = 0.21, ki = 0.01, kd = 0.9;
    // 计时器
    int timer = 0; // 用于超时退出循环，未使用计时器计时，timer每次循环+15
    int timeout = fabs(err) < 500 ? 1200 : fabs(err) * 3; // 超时时间
    timeout = timeout / maxPower * 100;
    
    kernel.reset();
    while (true) {
        // 分段函数执行
        if (funcStart != 0 &&
            ((funcStart > 0 && sensor_dis.get_distance() > funcStart) || 
            (funcStart < 0 && sensor_dis.get_distance() < funcStart))) {
            Task execFunc(func);
        }
        // 计算直行pid
        err = sensor_dis.get_distance() - distance;
        if (fabs(err - lastErr) > 100) {
            err = lastErr;
        }
        // 控制积分值在1以下，且分段积分
        if (fabs(err) < 50) {
            totalErr += err;
        }
        deltaErr = err - lastErr;
        lastErr = err;
        // 判据为转向超过目标值，需要反转的情况
        if (err * lastErr <= 0) {
            totalErr = 0;
        }
        power = kp * err + ki * totalErr + kd * deltaErr;
        // 缓加速
        power = power * CONSTRAIN(timer, 0, 210) / 210.0;
        // 退出条件
        if (fabs(err) < 5 || timer > timeout) {
            break;
        }
        // TODO: 确认是否需要在超限时再累计一个power_gyro
        chassis::move(CONSTRAIN(power, -maxPower, maxPower),
                      CONSTRAIN(power, -maxPower, maxPower));
        // 分段积分
        if (fabs(err) > abs(distance) * 0.2 || fabs(err) < 1) {
            totalErr = 0;
        }
        // 延时
        pros::delay(15);
        timer += 15;
    }
    chassis::move(0, 0);
    chassis::brake(brakeMode);
    pros::delay(10);
    logger::info("moveDis: [%d / %d]", sensor_dis.get_distance(), distance);
}

void rushDis(int distance, int power) {
    float err = sensor_dis.get_distance() - distance;
    int timer = 0;
    kernel.reset();
    if (err > 0) {
        while (err > 10) {
            chassis::move(power, power);
            pros::delay(15);
            timer += 15;
            err = sensor_dis.get_distance() - distance;
        }
    } else {
        while (err < -10) {
            chassis::move(-power, -power);
            pros::delay(15);
            timer += 15;
            err = sensor_dis.get_distance() - distance;
        }
    }
    chassis::move(0, 0);
    chassis::brake();
    pros::delay(10);
    logger::info("rushDis: [%d / %d]", sensor_dis.get_distance(), distance);
}
}