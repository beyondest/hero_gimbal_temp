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
typedef enum
{
    ACTION_SUCESS,
    ACTION_FAILURE

}ACTION_STATE;   

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



#define CAN1_RX PD0
#define CAN1_TX PD1

#define CAN2_RX PB5
#define CAN2_TX PB6




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

//msg_center_rotate
#define TIM1_PRESCALER 1799
#define TIM1_COUNT_NUMS 100
#define TIM1_PREEMPT_PRIORITY 4
#define TIM1_SUB_PRIORITY 15

//second update
#define TIM2_PRESCALER 8999
#define TIM2_COUNT_NUMS 100
#define TIM2_PREEMPT_PRIORITY 4
#define TIM2_SUB_PRIORITY 0

//cv_node
#define TIM3_PRESCALER 8999
#define TIM3_COUNT_NUMS 1000
#define TIM3_PREEMPT_PRIORITY 4
#define TIM3_SUB_PRIORITY 1

#define TIM4_PRESCALER 8999
#define TIM4_COUNT_NUMS 10000

//gimbal_node
#define TIM5_PRESCALER 8999
#define TIM5_COUNT_NUMS 10
#define TIM5_PREEMPT_PRIORITY 4
#define TIM5_SUB_PRIORITY 1

#define TIM6_PRESCALER 8999
#define TIM6_COUNT_NUMS 100
#define TIM6_PREEMPT_PRIORITY 15
#define TIM6_SUB_PRIORITY 15

//for test
#define TIM7_PRESCALER 8999
#define TIM7_COUNT_NUMS 1000
#define TIM7_PREEMPT_PRIORITY 4
#define TIM7_SUB_PRIORITY 0

#define TIM8_PRESCALER 17999
#define TIM8_COUNT_NUMS 10000

//DMA setting
#define DMA_stream3 UART7_Rx
#define DMA_stream1 UART7_Tx
#define DMA_stream6 UART8_Rx
#define DMA_stream0 UART8_Tx
#define DMA_stream0_PREEMPT_PRIORITY 3
#define DMA_stream0_SUB_PRIORITY 0
#define DMA_stream6_PREEMPT_PRIORITY 3
#define DMA_stream6_SUB_PRIORITY 0


//Usart setting
#define USART8_PREEMPT_PRIORITY 3
#define USART8_SUB_PRIORITY 0
#define USART8_PULL_MODE GPIO_PULLUP
#define USART8_BAUDRATE 9600


//Can setting 
#define GLOBAL_CAN_FILTER_TYPE CAN_FILTERMODE_IDMASK
#define GLOBAL_CAN_FILTER_SLAVE_START 14
#define GLOBAL_CAN_FILTER_SCALE CAN_FILTERSCALE_16BIT

//this is GM possible ID    1    2     3     4     5     6     7
// CAN1_ID_YOU_WANT "0x205 0x206 0x207 0x208 0x209 0x20a 0x20b"
#define CAN1_MASK_ID   0x7f0
#define CAN1_FILTER_ID  0x200
#define CAN1_IT_TYPE CAN_IT_RX_FIFO0_MSG_PENDING
#define CAN1_MAIL_BOX CAN_TX_MAILBOX0
#define CAN1_FIFO CAN_RX_FIFO0
#define CAN1_FILTER_INDEX 0




//*********************************************************TypeDef**************************************************
typedef void (*pvoid_void_func)(void);


//****************************************************Extern**************************************************
extern uint8_t tmpu8[10];
extern uint16_t tmpu16[10];
extern uint32_t tmpu32[10];
extern float tmpfloat[10];






/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
