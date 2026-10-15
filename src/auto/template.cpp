#include "haws/auto.h"

void templateAuto() {
    /** @note 一块场地垫的距离为 24inch */
    /** @note 每一行示例语句的执行效果均以(0,0)为起始点位为例 */

    /*****************************************/
    /**************** 重置位置 ****************/
    /*****************************************/
    // 重置底盘位置
    chassis::resetPose();
    // 重置底盘位置, 水平方向、垂直方向位置为0, 朝向为180度
    chassis::resetPose(0, 0, 180);
    

    /*****************************************/
    /**************** 原地转向 ****************/
    /*****************************************/
    // 原地转向, 朝向机器人初始位置方向
    chassis::turnToHeading(0);
    // 原地转向, 朝向机器人初始位置方向的正右方(90°)
    chassis::turnToHeading(90);
    // 原地转向, 朝向机器人初始位置方向的正右方(90°), 最大速度100
    chassis::turnToHeading(90, {.maxSpeed=100});
    // 向左转向(CCW_COUNTERCLOCKWISE), 朝向机器人初始位置方向的正右方(90°), 实际转过-270°
    chassis::turnToHeading(90, {.direction=lemlib::AngularDirection::CCW_COUNTERCLOCKWISE});

    // 原地转向, 朝向点位 (24, 24), 实际转向45°
    chassis::turnToPoint(24, 24);
    // 原地转向, 朝向点位 (24, 24), 实际转向45°, 最大速度100
    chassis::turnToPoint(24, 24, {.maxSpeed=100});
    // 原地转向, 背朝(.forwards=false)点位 (24, 24), 实际转向-135°
    chassis::turnToPoint(24, 24, {.forwards=false});
    // 向左转向(CCW_COUNTERCLOCKWISE), 朝向点位 (24, 24), 实际转向-315°
    chassis::turnToPoint(24, 24, {.direction=lemlib::AngularDirection::CCW_COUNTERCLOCKWISE});


    /*****************************************/
    /*************** 移动至点位 ***************/
    /*****************************************/
    // 向前移动一块场地垫的距离
    chassis::moveToPoint(0, 24);
    // 向前移动一块场地垫的距离, 最大速度100
    chassis::moveToPoint(0, 24, {.maxSpeed=100});
    // 向后移动一块场地垫的距离
    chassis::moveToPoint(0, -24, {.forwards=false});
    
    // 先向右转向, 再前进一块场地垫的距离
    chassis::moveToPoint(24, 0);
    // 先向右转向, 再前进一块场地垫的距离, 最大速度100
    chassis::moveToPoint(24, 0, {.maxSpeed=100});
    // 先向左转向, 再后退一块场地垫的距离
    chassis::moveToPoint(24, 0, {.forwards=false});


    /*****************************************/
    /******* 走弧线移动至点位, 且调整朝向 *******/
    /*****************************************/
    // 先向右转向, 然后向左划弧行驶至一块场地垫的右前方, 方向与初始位置相同
    chassis::moveToPose(24, 24, 0);
    // 向右划弧行驶至一块场地垫的右前方, 朝向初始方向的正右方
    chassis::moveToPose(24, 24, 90);
}