

#ifndef BSP_TIM_H
#define BSP_TIM_H
//**************************************************Include*************************************************
#include "main.h"
#include "tim.h"

//**************************************************Custom Define*****************************************************

//Notice: you must set period bigger than main.h "" period of tim


//center rotater
#define TIM1_PERIOD_MS 10
#define TIM1_MAX_TASK_SUPPORT 4
//second updater
#define TIM2_PERIOD_MS 1000
#define TIM2_MAX_TASK_SUPPORT 4
//cv node
#define TIM3_PERIOD_MS 500
#define TIM3_MAX_TASK_SUPPORT 4

#define TIM4_PERIOD_MS 1000
#define TIM4_MAX_TASK_SUPPORT 4

//gimbal node
#define TIM5_PERIOD_MS 100
#define TIM5_MAX_TASK_SUPPORT 4

//motor_control
#define TIM6_PERIOD_MS 10
#define TIM6_MAX_TASK_SUPPORT 3


#define TIM7_PERIOD_MS 1000
#define TIM7_MAX_TASK_SUPPORT 4

#define TIM8_PERIOD_MS 1000
#define TIM8_MAX_TASK_SUPPORT 4



//*****************************************************Custom TypeDef********************************************************




//*************************************************Official Function*********************************************************
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);




//***************************************************Custom Function**********************************************************



class Tim
{
public:
//const value
    uint16_t period_ms;
    uint8_t Tim_Frequency_Mhz;
    uint8_t Max_task_support;

    pvoid_void_func* ptask;
    uint16_t Count_Max ;

    uint16_t prescaler;
    uint16_t tim_count_nums;

//dynamic value
    uint16_t count = 0;
    uint8_t task_count;

public:
    TIM_HandleTypeDef* htim;

    Tim(uint16_t period_ms, 
        uint8_t tim_frequency_mhz,
        uint8_t max_task_support,
        uint16_t prescaler,
        uint16_t tim_count_nums,
        TIM_HandleTypeDef* htim);

    ~Tim();
    void init_task();

    void elapsed_callback();

    void add_task(const pvoid_void_func prun_func);
    void remove_task(const pvoid_void_func prun_func);



    void start_task();
    void stop_task();
};














//***********************************************************C API****************************************************************


#ifdef __cplusplus
extern "C"{
#endif

#ifdef __cplusplus
}
#endif

//***************************************************Extern Value****************************************************************

extern Tim Global_tim1;

extern Tim Global_tim2;
extern Tim Global_tim3;
extern Tim Global_tim5;
extern Tim Global_tim6;


extern Tim Global_tim7;


#endif