/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MEGA 1000000




#define TIM1_Period "1ms"
#define TIM1_Quantum "0_01ms"
#define TIM2_Period "10ms"
#define TIM2_Quantum "0_1ms"
#define TIM3_Period "100ms"
#define TIM3_Quantum "0_1ms"



#define TIM4_Quantum "0_1ms"
#define TIM4_Period "1000ms"



#define TIM5_Quantum "0_1ms"
#define TIM5_Period "1ms"
#define TIM6_Quantum "0_1ms"
#define TIM6_Period "10ms"
#define TIM7_Quantum "0_1ms"
#define TIM7_Period "100ms"




#define TIM8_Period "1000ms"
#define TIM8_Quantum "0_1ms"


#define DMA_stream3 UART7_Rx
#define DMA_stream1 UART7_Tx
#define DMA_stream6 UART8_Rx
#define DMA_stream0 UART8_Tx


#define LED1_Pin GPIO_PIN_1
#define LED1_GPIO_Port GPIOG
#define LED8_Pin GPIO_PIN_8
#define LED8_GPIO_Port GPIOG
#define LED7_Pin GPIO_PIN_7
#define LED7_GPIO_Port GPIOG
#define LED6_Pin GPIO_PIN_6
#define LED6_GPIO_Port GPIOG
#define LED5_Pin GPIO_PIN_5
#define LED5_GPIO_Port GPIOG
#define LED4_Pin GPIO_PIN_4
#define LED4_GPIO_Port GPIOG
#define LED3_Pin GPIO_PIN_3
#define LED3_GPIO_Port GPIOG
#define LED2_Pin GPIO_PIN_2
#define LED2_GPIO_Port GPIOG




#define KEY0_Pin GPIO_PIN_2
#define KEY0_GPIO_Port GPIOB

#define LED_RED_Pin GPIO_PIN_11
#define LED_RED_GPIO_Port GPIOE
#define LED_GREEN_Pin GPIO_PIN_14
#define LED_GREEN_GPIO_Port GPIOF



#define USER_EXTI0_Pin GPIO_PIN_0
#define USER_EXTI0_GPIO_Port GPIOC
#define USER_EXTI0_EXTI_IRQn EXTI0_IRQn
#define USER_EXTI1_Pin GPIO_PIN_1
#define USER_EXTI1_GPIO_Port GPIOC
#define USER_EXTI1_EXTI_IRQn EXTI1_IRQn
#define USER_EXTI2_Pin GPIO_PIN_2
#define USER_EXTI2_GPIO_Port GPIOC
#define USER_EXTI2_EXTI_IRQn EXTI2_IRQn
//********************************************************Custom Define for SPECIFY BOARD A*****************************************
#define TIM1_CLOCK_FREQUENCY_MHZ 180
#define TIM2_CLOCK_FREQUENCY_MHZ 90
#define TIM3_CLOCK_FREQUENCY_MHZ 90
#define TIM4_CLOCK_FREQUENCY_MHZ 90
#define TIM5_CLOCK_FREQUENCY_MHZ 90
#define TIM6_CLOCK_FREQUENCY_MHZ 90
#define TIM7_CLOCK_FREQUENCY_MHZ 90
#define TIM8_CLOCK_FREQUENCY_MHZ 180





//**********************************************************Init Defination***********************************************
#define TIM1_PRESCALER 1799
#define TIM1_COUNT_NUMS 100

#define TIM2_PRESCALER 8999
#define TIM2_COUNT_NUMS 100

#define TIM3_PRESCALER 8999
#define TIM3_COUNT_NUMS 1000

#define TIM4_PRESCALER 8999
#define TIM4_COUNT_NUMS 10000

#define TIM5_PRESCALER 8999
#define TIM5_COUNT_NUMS 10

#define TIM6_PRESCALER 8999
#define TIM6_COUNT_NUMS 100

#define TIM7_PRESCALER 8999
#define TIM7_COUNT_NUMS 1000

#define TIM8_PRESCALER 17999
#define TIM8_COUNT_NUMS 10000



//*********************************************************TypeDef**************************************************
typedef void (*pvoid_void_func)(void);


//****************************************************Extern**************************************************
extern uint8_t tmpu8[10];
extern uint16_t tmpu16[10];
extern uint32_t tmpu32[10];
extern float tmpfloat[10];
extern uint16_t a;







/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
