#include"BlueeTooth.h"
#include"main.h"
#include "stm32f1xx_hal_uart.h"
#include"usart.h"
#include "motor.h"
#include "sevro.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
uint16_t angle1;
uint16_t angle2;
uint8_t rx_byte;
char mes[20]="";
uint16_t speed;
uint16_t speed1;
CarState_t car_state;

void simple_parse_with_atoi(const char* str, char* command_char, uint16_t* value) {
  // --- 1. 提取命令字符 (不变) ---
  *command_char = str[0];

  // --- 2. 手动提取数字部分 ---
  // 计算数字部分的长度 (总长度 - 1，因为第一个字符是命令)
  int num_len = strlen(str) - 1;

  // 检查长度是否有效，防止内存问题
  if (num_len <= 0) {
    printf("Error: No number part found.\n");
    *value = 0; // 给一个默认值
    return;
  }

  // 创建一个足够大的临时缓冲区来存放数字字符串
  char temp_num_str[20]; // 假设数字不会超过20位

  // 使用 strncpy 将数字部分复制过来 (更安全)
  // str + 1 等价于 &str[1]，指向数字开始的位置
  strncpy(temp_num_str, str + 1, num_len);

  // !!! 关键步骤：手动添加字符串结束符 !!!
  temp_num_str[num_len] = '\0';

  // --- 3. 使用 atoi 进行转换 ---
  int temp_value = atoi(temp_num_str);

  // --- 4. 手动进行范围检查 ---
  // atoi 不检查范围，所以我们需要自己来
  if (temp_value < 0 || temp_value > 65535) {
    printf("Warning: atoi result %d is out of uint16_t range. Clamping or setting to 0.\n", temp_value);
    *value = 0; // 或者根据你的需求进行钳制 (clamp)
  } else {
    *value = (uint16_t)temp_value;
  }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
  HAL_UART_Transmit(&huart1, mes, sizeof(mes), 1000);
   HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *)mes, sizeof(mes));
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
  simple_parse_with_atoi(mes, &rx_byte, &speed);
  if(huart->Instance == USART1)  // 判断是哪个串口触发的中断
  {
    switch(rx_byte)
    {
      case 'F':  //修改状态变成前进
        car_state = CAR_FORWARD;
        speed1=speed;
        break;
      case 'B':  // 修改状态变成后退
        car_state = CAR_BACKWARD;
        speed1=speed;
        break;
      case 'L':  // 修改此时状态为左转
        car_state = CAR_LEFT;
        break;
      case 'R':  // 修改此时状态为右转
        car_state = CAR_RIGHT;
        break;
      case 'H':  // 停止
        car_state = CAR_STOP;
        break;
      case 'O':
      HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
        car_state = CAR_OPEN;//打开舵机
      if (speed>88){angle1=88;}
      else{angle1=speed;}
        break;
      case 'C':
        car_state = CAR_CLOSE;//关闭舵机
        break;
      case 'P':
        if (car_state == CAR_PRESS){
          car_state = CAR_RELEASE;
        }
        else {
          car_state = CAR_PRESS;
          if (speed>88){angle2=88;}
          else{angle2=speed;}
        }


        break;
    case 'X':
        car_state = CAR_ROTATE_LEFT;//原地左转
        break;
      case 'Y':
        car_state = CAR_ROTATE_RIGHT;//原地右转
        break;
      default:
        break;
    }

  }
  // HAL_UART_Transmit_IT(&huart1, mes, 5);
  // HAL_UART_Receive_IT(&huart1, mes, 5);  // 重新开启串口接收中断
}