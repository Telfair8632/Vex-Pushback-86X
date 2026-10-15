#include "haws/motorcontrol/kernel.h"
#include "haws/motorcontrol/chassis.h"
#include "main.h"

Kernel::Kernel() {
    // 电机组定义
#ifndef PORT_MOTOR_LEFT3
    this->leftMotors = new MotorGroup({PORT_MOTOR_LEFT1, PORT_MOTOR_LEFT2}, CHASSIS_MOTOR_GEAR_SET);
    this->rightMotors = new MotorGroup({PORT_MOTOR_RIGHT1, PORT_MOTOR_RIGHT2}, CHASSIS_MOTOR_GEAR_SET);
#else
    this->leftMotors = new MotorGroup({PORT_MOTOR_LEFT1, PORT_MOTOR_LEFT2, PORT_MOTOR_LEFT3}, CHASSIS_MOTOR_GEAR_SET);
    this->rightMotors = new MotorGroup({PORT_MOTOR_RIGHT1, PORT_MOTOR_RIGHT2, PORT_MOTOR_RIGHT3}, CHASSIS_MOTOR_GEAR_SET);
#endif

    // 陀螺仪定义
    this->sensor_gyro = new Imu(PORT_SENSOR_IMU);

    // 定位轮和传感器定义
    /** @class TrackingWheel @note 定位轮, 2.75in直径
     *
     * @note 第二个参数 @param wheelDiameter
     * 填写定位轮的直径
     * Omniwheel--全向轮(目前没找到牵引轮和麦克纳姆轮的定义)
     * 轮子直径对应的数值参见 `https://kb.vex.com/hc/en-us/articles/38888566734612-Understanding-VEX-V5-Wheels`
     *
     * @note 第三个参数 @param distance
     * 填定位轮与旋转中心的偏移值
     * 垂直定位轮的偏移值只与左右相关, 偏左填负值/偏右填正值
     * 水平定位轮的偏移值只与前后有关, 靠后填负值/靠前填正值
     * 单位为 inch (1cm = 0.394in)
     */
#ifdef PORT_SENSOR_ENC_VERTICAL
    this->verticalEnc = new Rotation(PORT_SENSOR_ENC_VERTICAL);
    this->verticalTrackingWheel = new TrackingWheel(
       this->verticalEnc,
       Omniwheel::NEW_275,
       0.847 // 2.15cm
   );
#else
    this->verticalEnc = nullptr;
    this->verticalTrackingWheel = nullptr;
#endif
#ifdef PORT_SENSOR_ENC_HORIZONTAL
    this->horizontalEnc = new Rotation(PORT_SENSOR_ENC_HORIZONTAL);
    this->horizontalTrackingWheel = new TrackingWheel(
        this->horizontalEnc,
        Omniwheel::NEW_275,
        -1.024, // -2.5cm
        -1.0
    );
#else
    this->horizontalEnc = nullptr;
    this->horizontalTrackingWheel = nullptr;
#endif

    /** @class Drivetrain @note 传动系统/底盘
     *
     * @note 第三个参数 @param trackWidth
     * 填写底盘 `左侧轮中心` 和 `右侧轮中心` 的间距
     * 单位为 inch (1cm = 0.394in)
     *
     * @note 第四个参数 @param wheelDiameter
     * 填底盘轮子的直径 3.25in
     *
     * @note 第五个参数 @param rpm
     * 填底盘轮子的转速
     * 蓝芯电机为600rpm, 如有传动计算传动比
     *
     * @note 第六个参数 @param horizontalDrift
     * 表示水平漂移值, 更高的数值会让底盘跑的更快, 但是会产生横向滑行漂移
     * 官方建议: 如底盘中使用了牵引轮(胶皮轮), 则该值填8; 如未使用则填2
     */
    this->drivetrain = new Drivetrain(
        this->leftMotors,
        this->rightMotors,
        10.756,
        Omniwheel::NEW_325,
        400,
        HOR_DRIFT
    );

    // 直线控制器
    this->linearController = new ControllerSettings(
        LINEAR_KP,   // 比例增益 (kP) 5.9  8.9
        LINEAR_KI,  // 积分增益 (kI) 0.04  0.1
        LINEAR_KD,    // 微分增益 (kD) 3.2 2.3
        10,      // 抗饱和 (如果误差超出此范围，积分将被设置为0)
        
        // {误差值小于'小误差范围'达到'小误差范围超时'时间后, 退出}
        1.5,             // 小误差范围 (单位为inch)
        100,    // 小误差范围超时 (单位为毫秒)

        // {误差值小于'大误差范围'达到'大误差范围超时'时间后, 退出}
        3.5,             // 大误差范围 (单位为inch)
        300,    // 大误差范围超时 (单位为毫秒)

        60      // 最大加速度 (10ms对应的加速度值) // 防止启动时打滑和停止时倾翻
    );

    this->angularController = new ControllerSettings(
        ANGULAR_KP,  // proportional gain (kP)  1.0 //1.15
        ANGULAR_KI, // integral gain (kI)      0.02
        ANGULAR_KD, // derivative gain (kD)     2.8 //2.5
        10,    // anti windup
        1,    // small error range, in degrees
        100,  // small error range timeout, in milliseconds
        3,    // large error range, in degrees
        300,  // large error range timeout, in milliseconds
        5     // maximum acceleration (slew)
    );

    this->sensors = new OdomSensors(
        this->verticalTrackingWheel,
        nullptr,
        this->horizontalTrackingWheel,
        nullptr,
        this->sensor_gyro
    );

    // 摇杆曲线控制 `https://www.vexforum.com/t/expo-drive-lemlibs-implementation/123337`
    this->throttleCurve = new ExpoDriveCurve(
        3,    // 摇杆死区 (绝对值小于此数值将输出0功率)
        10,   // 最小输出功率 (能够驱动底盘的电机最小功率)
        1 // 曲线增益, 如不需使用填1, 需要使用请参照 `https://www.desmos.com/calculator/umicbymbnl?lang=zh-CN`
    );

    this->steerCurve = new ExpoDriveCurve(
        3,    // joystick deadband out of 127
        10,   // minimum output where drivetrain will move out of 127
        1 // expo curve gain
    );

    this->chassis = new Chassis(
        *this->drivetrain,
        *this->linearController,
        *this->angularController,
        *this->sensors,
        this->throttleCurve,
        this->steerCurve
    );
}

Kernel::~Kernel() {
    if (this->chassis != nullptr) {
        delete this->chassis;
        this->chassis = nullptr;
    }
    if (this->steerCurve != nullptr) {
        delete this->steerCurve;
        this->steerCurve = nullptr;
    }
    if (this->throttleCurve != nullptr) {
        delete this->throttleCurve;
        this->throttleCurve = nullptr;
    }
    if (this->sensors != nullptr) {
        delete this->sensors;
        this->sensors = nullptr;
    }
    if (this->angularController != nullptr) {
        delete this->angularController;
        this->angularController = nullptr;
    }
    if (this->linearController != nullptr) {
        delete this->linearController;
        this->linearController = nullptr;
    }
    if (this->drivetrain != nullptr) {
        delete this->drivetrain;
        this->drivetrain = nullptr;
    }
    if (this->horizontalTrackingWheel != nullptr) {
        delete this->horizontalTrackingWheel;
        this->horizontalTrackingWheel = nullptr;
    }
    if (this->verticalTrackingWheel != nullptr) {
        delete this->verticalTrackingWheel;
        this->verticalTrackingWheel = nullptr;
    }
    if (this->horizontalEnc != nullptr) {
        delete this->horizontalEnc;
        this->horizontalEnc = nullptr;
    }
    if (this->verticalEnc != nullptr) {
        delete this->verticalEnc;
        this->verticalEnc = nullptr;
    }
    if (this->sensor_gyro != nullptr) {
        delete this->sensor_gyro;
        this->sensor_gyro = nullptr;
    }
    if (this->leftMotors != nullptr) {
        delete this->leftMotors;
        this->leftMotors = nullptr;
    }
    if (this->rightMotors != nullptr) {
        delete this->rightMotors;
        this->rightMotors = nullptr;
    }
}

Chassis* Kernel::getChassis() {
    return this->chassis;
}

void Kernel::reset() {
    this->leftMotors->tare_position_all();
    this->rightMotors->tare_position_all();
}

void Kernel::brake(motor_brake_mode_e brakeMode) {
    this->leftMotors->set_brake_mode_all(brakeMode);
    this->rightMotors->set_brake_mode_all(brakeMode);
    this->leftMotors->brake();
    this->rightMotors->brake();
}

int Kernel::getPosition() {
    return this->rightMotors->get_position(0) * 1000;
}

int Kernel::getActualVelocity() {
    return this->rightMotors->get_actual_velocity(0);
}

std::string Kernel::selfCheck() {
    std::string msg = "";
    if (this->steerCurve == nullptr) {
        msg = msg + "sC ";
    }
    if (this->throttleCurve == nullptr) {
        msg = msg + "tC ";
    }
    if (this->sensors == nullptr) {
        msg = msg + "s ";
    }
    if (this->angularController == nullptr) {
        msg = msg + "aC ";
    }
    if (this->linearController == nullptr) {
        msg = msg + "lC ";
    }
    if (this->drivetrain == nullptr) {
        msg = msg + "dt ";
    }
    if (this->horizontalTrackingWheel == nullptr) {
        msg = msg + "hTW ";
    }
    if (this->verticalTrackingWheel == nullptr) {
        msg = msg + "vTW ";
    }
    if (this->horizontalEnc == nullptr) {
        msg = msg + "hE ";
    }
    if (this->verticalEnc == nullptr) {
        msg = msg + "vE ";
    }
    if (this->sensor_gyro == nullptr) {
        msg = msg + "sg ";
    }
    if (this->leftMotors == nullptr) {
        msg = msg + "lM ";
    }
    if (this->rightMotors == nullptr) {
        msg = msg + "rM ";
    }
    return msg;
}

