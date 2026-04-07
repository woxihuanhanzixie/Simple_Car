#include"main.h"
#include"usart.h"


extern uint8_t rx_byte;
typedef enum{
  CAR_FORWARD,
  CAR_BACKWARD,
  CAR_LEFT,
  CAR_RIGHT,
  CAR_STOP,
  CAR_OPEN,
  CAR_CLOSE,
  CAR_PRESS,
  CAR_RELEASE,
  CAR_ROTATE_LEFT,
  CAR_ROTATE_RIGHT
}CarState_t;
extern CarState_t car_state;//初始设置为停止状态
extern char mes[20];
extern uint16_t speed;
extern uint16_t angle1;
extern uint16_t angle2;
extern uint16_t speed1;
void simple_parse_with_atoi(const char* str, char* command_char, uint16_t* value);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);