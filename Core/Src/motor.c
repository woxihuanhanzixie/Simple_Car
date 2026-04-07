//
// Created by linjunjian on 2025/12/19.
//

#include "motor.h"
#include "tim.h"

// 定义左右电机结构体（根据你的硬件连接修改引脚）
Motor_TypeDef LeftMotor = {
    .IN1_Port = LEFT_MOTOR_IN1_GPIO_Port,
    .IN1_Pin = LEFT_MOTOR_IN1_Pin,
    .IN2_Port = LEFT_MOTOR_IN2_GPIO_Port,
    .IN2_Pin = LEFT_MOTOR_IN2_Pin,
    .PWM_TIM = &htim4,  // 替换为实际的定时器句柄
    .PWM_Channel = TIM_CHANNEL_2  // 替换为实际的通道号
};

Motor_TypeDef RightMotor = {
    .IN1_Port = RIGHT_MOTOR_IN1_GPIO_Port,
    .IN1_Pin = RIGHT_MOTOR_IN1_Pin,
    .IN2_Port = RIGHT_MOTOR_IN2_GPIO_Port,
    .IN2_Pin = RIGHT_MOTOR_IN2_Pin,
    .PWM_TIM = &htim4,  // 替换为实际的定时器句柄
    .PWM_Channel = TIM_CHANNEL_3  // 替换为实际的通道号
};

/**
 * @brief  电机初始化
 * @param  无
 * @retval 无
 */
void Motor_Init(void)
{
    // 初始化GPIO和PWM已经在CubeMX中配置完成
    // 这里可以添加额外的初始化代码

    // 初始状态停止所有电机
    Car_Stop();
}

/**
 * @brief  设置单个电机速度和方向
 * @param  motor: 电机结构体指针
 * @param  speed: 速度值，-1000到1000，负数表示反转
 * @retval 无
 */
void Motor_SetSpeed(Motor_TypeDef* motor, int16_t speed)
{
    // 限制速度范围
    if (speed > 1000) speed = 1000;
    if (speed < -1000) speed = -1000;

    // 设置方向和PWM
    if (speed >= 0) {
        // 正转
        HAL_GPIO_WritePin(motor->IN1_Port, motor->IN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(motor->IN2_Port, motor->IN2_Pin, GPIO_PIN_RESET);

        // 设置PWM占空比
        __HAL_TIM_SET_COMPARE(motor->PWM_TIM, motor->PWM_Channel, speed);
    } else {
        // 反转
        HAL_GPIO_WritePin(motor->IN1_Port, motor->IN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(motor->IN2_Port, motor->IN2_Pin, GPIO_PIN_SET);

        // 设置PWM占空比（取绝对值）
        __HAL_TIM_SET_COMPARE(motor->PWM_TIM, motor->PWM_Channel, -speed);
    }
}

/**
 * @brief  小车直线前进
 * @param  speed: 速度值，0-1000
 * @retval 无
 */
void Car_MoveForward(int16_t speed)
{
    if (speed < 0) speed = 0;
    if (speed > 1000) speed = 1000;

    // 两个电机同速正转
    Motor_SetSpeed(&LeftMotor, speed);
    Motor_SetSpeed(&RightMotor, speed);
}

/**
 * @brief  小车直线后退
 * @param  speed: 速度值，0-1000
 * @retval 无
 */
void Car_MoveBackward(int16_t speed)
{
    if (speed < 0) speed = 0;
    if (speed > 1000) speed = 1000;

    // 两个电机同速反转
    Motor_SetSpeed(&LeftMotor, -speed);
    Motor_SetSpeed(&RightMotor, -speed);
}

/**
 * @brief  小车左转（差速转向，右轮速度快于左轮）
 * @param  speed: 基础速度值，0-1000
 * @retval 无
 */
void Car_TurnLeft(int16_t speed)
{
    if (speed < 0) speed = 0;
    if (speed > 1000) speed = 1000;

    // 左轮减速，右轮全速
    Motor_SetSpeed(&LeftMotor, speed / 2);
    Motor_SetSpeed(&RightMotor, speed);
}

/**
 * @brief  小车右转（差速转向，左轮速度快于右轮）
 * @param  speed: 基础速度值，0-1000
 * @retval 无
 */
void Car_TurnRight(int16_t speed)
{
    if (speed < 0) speed = 0;
    if (speed > 1000) speed = 1000;

    // 右轮减速，左轮全速
    Motor_SetSpeed(&LeftMotor, speed);
    Motor_SetSpeed(&RightMotor, speed / 2);
}

/**
 * @brief  小车原地左旋转
 * @param  speed: 旋转速度值，0-1000
 * @retval 无
 */
void Car_RotateLeft(int16_t speed)
{
    if (speed < 0) speed = 0;
    if (speed > 1000) speed = 1000;

    // 左轮反转，右轮正转，实现原地旋转
    Motor_SetSpeed(&LeftMotor, -speed);
    Motor_SetSpeed(&RightMotor, speed);
}

/**
 * @brief  小车原地右旋转
 * @param  speed: 旋转速度值，0-1000
 * @retval 无
 */
void Car_RotateRight(int16_t speed)
{
    if (speed < 0) speed = 0;
    if (speed > 1000) speed = 1000;

    // 左轮正转，右轮反转，实现原地旋转
    Motor_SetSpeed(&LeftMotor, speed);
    Motor_SetSpeed(&RightMotor, -speed);
}

/**
 * @brief  小车停止
 * @param  无
 * @retval 无
 */
void Car_Stop(void)
{
    // 停止所有电机
    Motor_SetSpeed(&LeftMotor, 0);
    Motor_SetSpeed(&RightMotor, 0);
}