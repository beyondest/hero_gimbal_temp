#include "bsp_tim.h"
#include "mymath.h"


Tim Global_tim1(TIM1_PERIOD_MS,
                TIM1_CLOCK_FREQUENCY_MHZ,
                TIM1_MAX_TASK_SUPPORT,
                TIM1_PRESCALER,
                TIM1_COUNT_NUMS,
                &htim1);

Tim Global_tim2(TIM2_PERIOD_MS,
                TIM2_CLOCK_FREQUENCY_MHZ,
                TIM2_MAX_TASK_SUPPORT,
                TIM2_PRESCALER,
                TIM2_COUNT_NUMS,
                &htim2);



Tim Global_tim3(TIM3_PERIOD_MS,
                TIM3_CLOCK_FREQUENCY_MHZ,
                TIM3_MAX_TASK_SUPPORT,
                TIM3_PRESCALER,
                TIM3_COUNT_NUMS,
                &htim3);


Tim Global_tim5(TIM5_PERIOD_MS,
                TIM5_CLOCK_FREQUENCY_MHZ,
                TIM5_MAX_TASK_SUPPORT,
                TIM5_PRESCALER,
                TIM5_COUNT_NUMS,
                &htim5);
                
Tim Global_tim7(TIM7_PERIOD_MS,
                TIM7_CLOCK_FREQUENCY_MHZ,
                TIM7_MAX_TASK_SUPPORT,
                TIM7_PRESCALER,
                TIM7_COUNT_NUMS,
                &htim7);

//******************************************************Official func**************************************************************



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim1)
    {
        Global_tim1.elapsed_callback();
    }
    else if (htim == &htim2)
    {
        Global_tim2.elapsed_callback();
    }
    
    else if (htim == &htim3)
    {
        Global_tim3.elapsed_callback();
    }
    else if (htim == &htim4)
    {
        while (1)
        {
            /* code */
        }
        
    }
    else if (htim == &htim5)
    {
        Global_tim5.elapsed_callback();
    }
    else if (htim == &htim6)
    {
        while (1)
        {
            /* code */
        }
        
    }
    else if (htim == &htim7)
    {
        Global_tim7.elapsed_callback();
    }
    else if (htim == &htim8)
    {
        while (1)
        {
            /* code */
        }
        
    }
    
    
}




//****************************************************Tim**************************************************************

Tim::Tim(uint16_t period_ms, 
        uint8_t tim_frequency_mhz,
        uint8_t max_task_support,
        uint16_t prescaler,
        uint16_t tim_count_nums,
        TIM_HandleTypeDef* htim):
    period_ms(period_ms),
    Tim_Frequency_Mhz(tim_frequency_mhz),
    Max_task_support(max_task_support),
    prescaler(prescaler),
    tim_count_nums(tim_count_nums),
    htim(htim)
    {
        uint32_t tmp_u32 = (uint32_t)this->Tim_Frequency_Mhz*MEGA;
        uint32_t tmp_u322 = (uint32_t)this->prescaler+1;
        
        float tmp_fl = tmp_u32/tmp_u322;
        //This variable represents the time interval at which the timer update interrupt occurs
        //Notice unit is s!!!
        float elapse_period_s =tim_count_nums/tmp_fl;
        
        this->Count_Max = (uint16_t)(this->period_ms/elapse_period_s/1000);


        this->ptask = new pvoid_void_func[this->Max_task_support];
        this->init_task();

    };

Tim::~Tim()
{
    delete[] this->ptask;
};

/**
 * this is true callback function to be set in period_elapsed_callback
*/
void Tim::elapsed_callback()
{
    this->count++;

    if (this->count == this->Count_Max)
    {
        this->count = 0;
        for (uint8_t i = 0; i < this->task_count; i++)
        {
            if (this->ptask[i] != nullptr)
            {
                this->ptask[i]();
            }
        }
        return;
    }
    else
    {
        return ;
    }
}


void Tim::add_task(const pvoid_void_func prun_func)
{
    if (this->task_count == this->Max_task_support)
    {
        return;
    }
    else
    {
        this->ptask[this->task_count] = prun_func;
        this->task_count++;
    }

}   

void Tim::remove_task(const pvoid_void_func prun_func)
{
    for (uint8_t i = 0; i < this->task_count; i++)
    {
        if (this->ptask[i] == prun_func)
        {
            this->ptask[i] = nullptr;
            this->task_count--;
            return;
        }
    }
    
}


void Tim::init_task()
{
    for (uint8_t i = 0; i < this->Max_task_support; i++)
    {
        this->ptask[i] = nullptr;
    }
    
}

/**
 * @brief Must be called to make tim work in IT mode
 * 
*/
void Tim::start_task()
{
    HAL_TIM_Base_Start_IT(this->htim);

}


void Tim::stop_task()
{
    HAL_TIM_Base_Stop_IT(this->htim);
    
}