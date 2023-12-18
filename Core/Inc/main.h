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
#define TIM3_Period "100ms"
#define TIM4_Quantum "0_1ms"
#define TIM2_Period "10ms"
#define TIM4_Period "1000ms"
#define TIM3_Quantum "0_1ms"
#define TIM5_Quantum "0_1ms"
#define TIM5_Period "1ms"
#define TIM1_Period "1ms"
#define TIM2_Quantum "0_1ms"
#define TIM6_Period "10ms"
#define TIM7_Period "100ms"
#define TIM8_Period "1000ms"
#define TIM8_Quantum "0_1ms"
#define TIM6_Quantum "0_1ms"
#define TIM7_Quantum "0_1ms"
#define TIM1_Quantum "0_01ms"
#define DMA_stream3 UART7_Rx
#define DMA_stream1 UART7_Tx
#define DMA_stream6 UART8_Rx
#define DMA_stream0 UART8_Tx
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
#define USER_EXTI0_Pin GPIO_PIN_0
#define USER_EXTI0_GPIO_Port GPIOC
#define USER_EXTI0_EXTI_IRQn EXTI0_IRQn
#define USER_EXTI1_Pin GPIO_PIN_1
#define USER_EXTI1_GPIO_Port GPIOC
#define USER_EXTI1_EXTI_IRQn EXTI1_IRQn
#define USER_EXTI2_Pin GPIO_PIN_2
#define USER_EXTI2_GPIO_Port GPIOC
#define USER_EXTI2_EXTI_IRQn EXTI2_IRQn
#define KEY0_Pin GPIO_PIN_2
#define KEY0_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_1
#define LED1_GPIO_Port GPIOG
#define LED_RED_Pin GPIO_PIN_11
#define LED_RED_GPIO_Port GPIOE
#define RED_GREEN_Pin GPIO_PIN_14
#define RED_GREEN_GPIO_Port GPIOF

















/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
