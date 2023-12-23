#include "motor.h"
#include "mymath.h"
#include "logger.h"
#include "bsp_data_struct.h"

GMMotor Global_pitch_motor(PITCH_MAX_CODE_VALUE,
                           PITCH_MOTOR_ID,
                           GM_RX_BUFFER_LENGTH,
                           GM_TX_BUFFER_LENGTH,
                           &Global_can1);


GMMotor Global_yaw_motor(  YAW_MAX_CODE_VALUE,
                           YAW_MOTOR_ID,
                           GM_RX_BUFFER_LENGTH,
                           GM_TX_BUFFER_LENGTH,
                           &Global_can1);
//***************************************************GMMotor API****************************************************


/**
 * After start must use to radians, or Motor will be stayed in Motor_Preparing
*/
ACTION_STATE GMMotor::start()
{
    this->reset();
    if (this->motor_flag == MOTOR_ERROR)
    {
        return ACTION_FAILURE;
    }
    else if (this->motor_flag == MOTOR_NOINIT)
    {
        LOG_INFO("No Init motor");
        while (1)
        {
            /* code */
        }
        return ACTION_FAILURE;
    }   
    else
    {
        this->target_radians = this->motor_reset_radians;
        return ACTION_SUCESS; 
    }

}         


ACTION_STATE GMMotor::shutdown()
{
    this->reset();
    if (this->motor_flag == MOTOR_ERROR)
    {
        return ACTION_FAILURE;
    }
    else if (this->motor_flag == MOTOR_NOINIT)
    {
        LOG_INFO("No Init motor");
        while (1)
        {
            /* code */
        }
        return ACTION_FAILURE;
    }
    else
    {
        this->motor_flag = MOTOR_SHUTDOWN;
        return ACTION_SUCESS;
    }
}

ACTION_STATE GMMotor::get_cur_radians(float* pfloat)
{
    if (this->motor_flag == MOTOR_RUNNING)
    {
        *pfloat = this->cur_radians;
        return ACTION_SUCESS;
    }
    else if (this->motor_flag == MOTOR_NOINIT)
    {
        LOG_INFO("No Init motor");
        while (1)
        {
            /* code */
        }
        return ACTION_FAILURE;
    }
    else
    {
        return ACTION_FAILURE;
    } 
}



/**
 * Only when motor is running, will to radians; but will return success both when motor already and running
*/
ACTION_STATE GMMotor::to_radians(float target_radians, 
                                 uint8_t target_time_minute,
                                 uint8_t target_time_second,
                                 float target_time_second_frac)
{

    if (this->motor_flag == MOTOR_RUNNING)
    {
        this->target_radians = target_radians;
        this->target_time.minute = target_time_minute;
        this->target_time.second = target_time_second;
        this->target_time.second_frac_4 = target_time_second_frac;
        this->update_setting_voltage();
        this->send_data();
        return ACTION_SUCESS;
    }
    else if(this->motor_flag == MOTOR_PREPARING)
    {
        this->update_setting_voltage();
        this->send_data();
        return ACTION_SUCESS;
    }
    else if (this->motor_flag == MOTOR_NOINIT)
    {
        LOG_INFO("No Init motor");
        while (1)
        {
            /* code */
        }
        return ACTION_FAILURE;      
    }
    else
    {
        return ACTION_FAILURE;
    }
}


/**
 * @warning For safe use, add this function to can msgpending callback and donot call it manually
 * @brief This function will update setting voltage if motor_running autoly
*/
void GMMotor::receive_data()
{
    this->pcan->receive_data(&this->rx_header,this->prx_buffer);
    this->decode();
    this->check_motor_state();
    if (this->motor_flag == MOTOR_PREPARING)
    {   
        this->update_setting_voltage();
        if (ABS((this->cur_radians - this->target_radians)) < MAX_TORLERATE_RADIANS_ERROR_ABS)
        {
            this->motor_flag = MOTOR_RUNNING;
        }
        return;
    }
    else if (this->motor_flag == MOTOR_ERROR)
    {
        this->motor_error_handler();
        return;
    }
    else if (this->motor_flag == MOTOR_SHUTDOWN)
    {
        return;
    }
    else if (this->motor_flag == MOTOR_RUNNING)
    {
        this->update_setting_voltage();
        return;
    }
    else
    {
        return;
    }
}

//*********************************************GMMotor Private Method*******************************************


float GMMotor::trans_code_to_abspos_radians(uint16_t code)
{
    if (this->Code_zero_offset == 10000)
    {
        LOG_INFO("Set motor offset first before doing anything");
        while (1)
        {
            /* code */
        }
    }    

    float tmp_float = 0;
    tmp_float = ((float)code - (float)this->Code_zero_offset)/(float)this->Max_code_value;
    tmp_float = tmp_float * 2 * PIE;
    return tmp_float;
}



void GMMotor::encode()
{

    this->cur_setting_voltage = CLAMP(this->cur_setting_voltage, this->Min_setting_voltage, this->Max_setting_voltage);
    
    uint8_t index = this->Motor_ID-1;
    prx_buffer[index] = (this->cur_setting_voltage >> 8) & 0xff;
    prx_buffer[index+1] = this->cur_setting_voltage & 0xff;

}

void GMMotor::decode()
{
    uint8_t index = this->rx_header.StdId - GM_CAN_FEEDBACK_ID_BASE;
    if (index == this->Motor_ID)
    {
        this->cur_code          =       ((this->prx_buffer[0] << 8) | (this->prx_buffer[1]));
        this->cur_rpm               =       ((this->prx_buffer[2] << 8) | (this->prx_buffer[3])); 
        this->cur_torque_I      =       ((this->prx_buffer[4] << 8) | (this->prx_buffer[5]));
        this->cur_temperature   =       this->prx_buffer[6] ;  
        this->cur_radians = this->trans_code_to_abspos_radians(this->cur_code);
    }    
}


void GMMotor::send_data()
{
    this->encode();
    this->pcan->send_data(&this->tx_header,this->ptx_buffer);
}


void GMMotor::set_motor_reset_ridians(float radians)
{
    this->motor_reset_radians = radians;
}


void GMMotor::set_motor_offset(uint16_t code_zero_offset)
{
    this->Code_zero_offset = code_zero_offset;
}


void GMMotor::set_motor_radians_pid(Pid* ppid)
{
    this->pradians_pid = ppid;
    this->pradians_pid->reset();
}

void GMMotor::set_motor_rpm_pid(Pid* ppid)
{
    this->prpm_pid = ppid;
    this->prpm_pid->reset();
}


void GMMotor::set_motor_safe_status(    int16_t max_setting_voltage,
                                        int16_t min_setting_voltage,
                                        uint16_t max_code,
                                        uint16_t min_code,
                                        int16_t max_rpm,
                                        int16_t max_torque_I,
                                        int16_t max_temperature
                                        )
{
    this->Max_setting_voltage = max_setting_voltage;
    this->Min_setting_voltage = min_setting_voltage;
    this->Max_code            = max_code;
    this->Min_code            = min_code;
    this->Max_rpm             = max_rpm;
    this->Max_torque_I        = max_torque_I;
    this->Max_temperature     = max_temperature;

}



void GMMotor::update_setting_voltage()
{
    this->target_rpm = this->pradians_pid->calculate(this->target_radians,this->cur_radians);
    this->cur_setting_voltage = this->prpm_pid->calculate(this->target_rpm,(float)this->cur_rpm);
}

void GMMotor::init(
                    int16_t max_setting_voltage,
                    int16_t min_setting_voltage,
                    uint16_t max_code,
                    uint16_t min_code,
                    int16_t max_rpm,
                    int16_t max_torque_I,
                    int16_t max_temperature,
                    float reset_radians,
                    Pid* ppid_radians,
                    Pid* ppid_rpm,
                    uint16_t code_zero_offset
)
{
    this->set_motor_safe_status(max_setting_voltage,min_setting_voltage,max_code,min_code,max_rpm,max_torque_I,max_temperature);
    this->set_motor_offset(code_zero_offset);
    this->set_motor_reset_ridians(reset_radians);
    this->set_motor_radians_pid(ppid_radians);
    this->set_motor_rpm_pid(ppid_rpm);
    
    this->motor_flag = MOTOR_SHUTDOWN;
}

void GMMotor::check_motor_state()
{
    if (this->cur_temperature > this->Max_temperature)
    {
        this->motor_flag = MOTOR_ERROR;
    }
    if (this->cur_torque_I > this->Max_torque_I)
    {
        this->motor_flag = MOTOR_ERROR;
    }
    if (this->cur_setting_voltage > this->Max_setting_voltage)
    {
        this->motor_flag = MOTOR_ERROR;
    }
    if (this->cur_code > this->Max_code)
    {
        this->motor_flag = MOTOR_ERROR;
    }
    if (this->cur_code < this->Min_code)
    {
        this->motor_flag = MOTOR_ERROR;
    }
    if (this->cur_rpm > this->Max_rpm)
    {
        this->motor_flag = MOTOR_ERROR;
    }   
}




/**
 * @brief Reset motor will reset pid, reset target to cur, and check if everything all right; if all is well, motor_flag will be set to MOTOR_PREPARING
 * @warning Motor will stay to the present radians after reset
 * @warning Motor will not change motor flag if everything all right
*/
void GMMotor::reset()
{


    this->check_motor_state();


    if (this->motor_flag == MOTOR_ERROR)
    {
        this->motor_error_handler();
    }
    else if (this->motor_flag == MOTOR_NOINIT)
    {
        this->motor_flag = MOTOR_NOINIT;
    }
    else if (this->motor_flag == MOTOR_RUNNING)
    {
        this->motor_flag = MOTOR_PREPARING;
    }
    else if (this->motor_flag == MOTOR_SHUTDOWN)
    {
        this->motor_flag = MOTOR_PREPARING;
    }

    this->pradians_pid->reset();
    this->prpm_pid->reset();

    this->target_radians = this->cur_radians;
    this->target_rpm = this->cur_rpm;

    this->target_time.minute = this->cur_time.minute;
    this->target_time.second = this->cur_time.second;
    this->target_time.second_frac_4 = this->cur_time.second_frac_4;

}









