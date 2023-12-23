#include "bsp_led.h"

void led_red_blink()
{
  HAL_GPIO_TogglePin(LED_RED_GPIO_Port,LED_RED_Pin);

}
void led1_blink()
{
  HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
}

void led2_blink()
{
  HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
}
void led3_blink()
{
  HAL_GPIO_TogglePin(LED3_GPIO_Port,LED3_Pin);
}
void led4_blink()
{
  HAL_GPIO_TogglePin(LED4_GPIO_Port,LED4_Pin);
}
void led5_blink()
{
  HAL_GPIO_TogglePin(LED5_GPIO_Port,LED5_Pin);
}
void led6_blink()
{
  HAL_GPIO_TogglePin(LED6_GPIO_Port,LED6_Pin);
}
void led7_blink()
{
  HAL_GPIO_TogglePin(LED7_GPIO_Port,LED7_Pin);
}
void led8_blink()
{
  HAL_GPIO_TogglePin(LED8_GPIO_Port,LED8_Pin);
}
