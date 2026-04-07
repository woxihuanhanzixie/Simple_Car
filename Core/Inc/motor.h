//
// Created by linjunjian on 2025/12/19.
//

#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"

// 电机控制相关定义
typedef struct {
    GPIO_TypeDef* IN1_Port;
    uint16_t IN1_Pin;
    GPIO_TypeDef* IN2_Port;
    uint16_t IN2_Pin;
    TIM_HandleTypeDef* PWM_TIM;
    uint32_t PWM_Channel;
} Motor_TypeDef;

// 函数声明
void Motor_Init(void);
void Motor_SetSpeed(Motor_TypeDef* motor, int16_t speed);
void Car_MoveForward(int16_t speed);
void Car_MoveBackward(int16_t speed);
void Car_TurnLeft(int16_t speed);
void Car_TurnRight(int16_t speed);
void Car_RotateLeft(int16_t speed);
void Car_RotateRight(int16_t speed);
void Car_Stop(void);

#endif /* __MOTOR_H */