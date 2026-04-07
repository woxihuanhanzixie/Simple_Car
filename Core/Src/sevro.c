#include "sevro.h"
#include "main.h"
#include <stdint.h>
extern TIM_HandleTypeDef htim3;

#define REALEASE_COMPARE 500

void Car_Open(uint16_t angle)//打开舵机，上升高度135°角
{
    uint32_t angle_duty_cycle = (angle *(float)20000*2/(float)5400 )+500;
    // 设置舵机到打开位置的占空比，这个值需要根据实际情况调整
    // uint32_t open_position_duty_cycle = 1500; // 示例值，单位微秒

    // 启动PWM信号输出
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  
    //初始位置设置为0度角
    //__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 500);
    // 设置PWM占空比以控制舵机位置
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, angle_duty_cycle);
 

}

void Car_Close(void)//关闭舵机，下降高度0°角
{
   HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    //初始位置设置为0度角
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 500);
 
}

void Car_Press(uint16_t angle)//夹取物体
{
    // 设置舵机到按下位置的占空比，这个值需要根据实际情况调整
    uint32_t angle_duty_cycle = (angle *(float)20000*2/(float)5400 )+500;

    // 启动PWM信号输出
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

    // 设置PWM占空比以控制舵机位置，转动135度！
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, angle_duty_cycle);
}

void Car_Release()//夹取物体
{
    // 启动PWM信号输出
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

    // 设置PWM占空比以控制舵机位置，转动135度！
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, REALEASE_COMPARE);
}