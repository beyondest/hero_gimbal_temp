#include "motor.h"


GMMotor Global_pitch_motor(/*  Fixed Value  */
                           PITCH_MOTOR_ID,
                           PITCH_CODE_ZERO_OFFSET,
                           GM_RX_BUFFER_LENGTH,
                           GM_TX_BUFFER_LENGTH,
                           &Global_can1,
                           /*  Safe Status Init  */
                           PITCH_MAX_SETTING_VOLTAGE,
                           PITCH_MIN_SETTING_VOLTAGE,
                           PITCH_MAX_CODE_VALUE,
                           PITCH_MIN_CODE_VALUE,
                           PITCH_MAX_RPM,
                           PITCH_MIN_RPM,
                           PITCH_MAX_TORQUE_I,
                           PITCH_MAX_TEMPERATURE,
                           /*  Pid Init    */
                           &Global_pitch_radians_pid,
                           &Global_pitch_rpm_pid
                           );

GMMotor Global_yaw_motor(  /*  Fixed Value  */ 
                           YAW_MOTOR_ID,
                           YAW_CODE_ZERO_OFFSET,
                           GM_RX_BUFFER_LENGTH,
                           GM_TX_BUFFER_LENGTH,
                           &Global_can1,
                           /*  Safe Status Init  */
                           YAW_MAX_SETTING_VOLTAGE,
                           YAW_MIN_SETTING_VOLTAGE,
                           YAW_MAX_CODE_VALUE,
                           YAW_MIN_CODE_VALUE,
                           YAW_MAX_RPM,
                           YAW_MIN_RPM,
                           YAW_MAX_TORQUE_I,
                           YAW_MAX_TEMPERATURE,
                           /*  Pid Init    */
                           &Global_yaw_radians_pid,
                           &Global_yaw_rpm_pid
                           );



void pitch_control()
{
    Global_pitch_motor.control();
}
void yaw_control()
{
    Global_yaw_motor.control();
}



//***************************************************GMMotor API****************************************************





ACTION_STATE GMMotor::shutdown()
{
    this->reset();
    if (this->motor_flag == MOTOR_ERROR)
    {
        return ACTION_FAILURE;
    }
    else
    {
        this->motor_flag = MOTOR_SHUTDOWN;
        return ACTION_SUCESS;
    }
}

ACTION_STATE GMMotor::get_cur_radians(float* pcur_radians)
{
    if (this->motor_flag == MOTOR_ERROR)
    {
        LOG_INFO("Motor error");
        return ACTION_FAILURE;
    }
    else
    {
        *pcur_radians = this->cur_radians;
        return ACTION_SUCESS;
    } 
    
}
ACTION_STATE GMMotor::get_cur_rpm(int16_t* prpm)
{
    if (this->motor_flag == MOTOR_ERROR)
    {
        LOG_INFO("Motor error");
        return ACTION_FAILURE;
    }
    else
    {
        *prpm = this->cur_rpm;
        return ACTION_SUCESS;
    } 
}

ACTION_STATE GMMotor::get_cur_code(int16_t* pcode)
{
    if (this->motor_flag == MOTOR_ERROR)
    {
        LOG_INFO("Motor error");
        return ACTION_FAILURE;
    }
    else
    {
        *pcode = this->cur_code;
        return ACTION_SUCESS;
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
        return ACTION_SUCESS;
    }
    else if(this->motor_flag == MOTOR_PREPARING)
    {
        return ACTION_SUCESS;
    }
    else if (this->motor_flag == MOTOR_ERROR)
    {
        return ACTION_FAILURE;
    }
    else
    {
        return ACTION_FAILURE;
    } 
}


/**
 * @note Voltage will be clamped in send_data;
*/
ACTION_STATE GMMotor::to_voltage(int16_t no_clamp_voltage)
{
    if (this->motor_flag == MOTOR_RUNNING && this->motor_flag == MOTOR_PREPARING)
    {
        this->cur_setting_voltage = no_clamp_voltage;

        return ACTION_SUCESS;
    }
    else if (this->motor_flag == MOTOR_ERROR)
    {
        return ACTION_FAILURE;
    }
    else
    {
        return ACTION_FAILURE;
    }
}



ACTION_STATE GMMotor::to_rpm(int16_t rpm)
{
    if (this->motor_flag == MOTOR_RUNNING)
    {
        this->target_rpm = rpm;
        return ACTION_SUCESS;
    }
    else if ( this->motor_flag == MOTOR_PREPARING)
    {
        return ACTION_SUCESS;
    }
    else if (this->motor_flag == MOTOR_ERROR)
    {
        return ACTION_FAILURE;
    }
    else
    {
        return ACTION_FAILURE;
    }
}



/**
 * @warning This is callback function by Tim , DO NOT call it manually;
 * @warning Tim interrupt CANNOT be priorier than CAN rx interrupt!!! 
 * @warning Set Debug flag FIRST if you want to debug;
*/
void GMMotor::control()
{

    if (this->motor_flag == MOTOR_ERROR)
    {
        this->motor_error_handler();
    }
    else if (this->motor_flag == MOTOR_SHUTDOWN)
    {
        this->cur_setting_voltage = GM_MOTOR_SHUTDOWN_VOLTAGE;
        this->send_data();
    }
    else if (this->motor_flag == MOTOR_PREPARING)
    {
        if (this->debug_flag == NO_DEBUG)
        {
            this->update_setting_voltage();
            this->send_data();
        }
        else
        {
            this->cur_setting_voltage = GM_MOTOR_SHUTDOWN_VOLTAGE;
            this->send_data();
        }
    }
    else if (this->motor_flag == MOTOR_RUNNING)
    {
        switch (this->debug_flag)
        {
        case NO_DEBUG:
            this->update_setting_voltage();
            this->send_data();
            break;
        case RPM_DEBUG:
            this->cur_setting_voltage = (int16_t)(this->prpm_pid->calculate((float)this->target_rpm,(float)this->cur_rpm));
            this->send_data();
            break;
        case VOLTAGE_DEBUG:
            this->send_data();
            break;
        default:
            break;
        }
    }
    else
    {
        this->motor_error_handler();
    }
}


/**
 * @warning This is a callback function called by CAN rx pending msg call back , DO NOT call it manually;
 * @warning Set Debug flag FIRST is you want to debug
*/
void GMMotor::get_feedback()
{

    this->receive_data();
    this->check_motor_state();

    if (this->motor_flag == MOTOR_PREPARING)
    {   
        if (this->debug_flag == NO_DEBUG)
        {
            if (ABS((this->cur_radians - this->target_radians)) < MAX_TORLERATE_RADIANS_ERROR_ABS)
            {
                this->motor_flag = MOTOR_RUNNING;
            }
        }
        else
        {
            this->motor_flag = MOTOR_RUNNING;
        }
    }
    else if (this->motor_flag == MOTOR_ERROR)
    {
        this->motor_error_handler();
    }
    else if (this->motor_flag == MOTOR_SHUTDOWN)
    {
    }
    else if (this->motor_flag == MOTOR_RUNNING)
    {
    }
    else
    {
    }
}

//*********************************************GMMotor Private Method*******************************************



GMMotor::GMMotor(
            uint8_t motor_id,
            uint16_t code_zero_offset,
            uint8_t rx_buffer_length,
            uint8_t tx_buffer_length,
            Can* pcan,
            int16_t max_setting_voltage,
            int16_t min_setting_voltage,
            uint16_t max_code,
            uint16_t min_code,
            int16_t max_rpm,
            int16_t min_rpm,
            int16_t max_torue_I,
            uint8_t max_temperature,
            Pid* pradians_pid,
            Pid* prpm_pid
            )
            :
            Motor_ID(motor_id),
            Code_zero_offset(code_zero_offset),
            pcan(pcan),
            rx_buffer_length(rx_buffer_length),
            tx_buffer_length(tx_buffer_length),
            Max_setting_voltage(max_setting_voltage),
            Min_setting_voltage(min_setting_voltage),
            Max_code(max_code),
            Min_code(min_code),
            Max_rpm(max_rpm),
            Min_rpm(min_rpm),
            Max_torque_I(max_torue_I),
            Max_temperature(max_temperature),
            pradians_pid(pradians_pid),
            prpm_pid(prpm_pid)
{

    this->prx_buffer = new uint8_t[rx_buffer_length];
    this->ptx_buffer = new uint8_t[tx_buffer_length];
    set_buffer(this->ptx_buffer,0,this->rx_buffer_length);
    set_buffer(this->prx_buffer,0,this->rx_buffer_length);

    if (this->Motor_ID > 4)
    {
        this->tx_header.StdId = GM_CAN_SEND_EXT_ID_BASE ;

    }
    else
    {
        this->tx_header.StdId = GM_CAN_SEND_ID_BASE;
    }
    this->start();
}

GMMotor::~GMMotor()
{
    delete[] prx_buffer;
    delete[] ptx_buffer;

};

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
    tmp_float = ((float)code - (float)this->Code_zero_offset)/(float)this->Max_code;
    tmp_float = tmp_float * 2 * PIE;
    return tmp_float;
}



void GMMotor::encode()
{

    this->cur_setting_voltage = CLAMP(this->cur_setting_voltage, this->Min_setting_voltage, this->Max_setting_voltage);
    
    uint8_t index = (uint8_t)((this->Motor_ID-1)*2);
    ptx_buffer[index] = (this->cur_setting_voltage >> 8) & 0xff;
    ptx_buffer[index+1] = this->cur_setting_voltage & 0xff;

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

/**
 * @note Will Clamp setting voltage here, this is the LAST defence
*/
void GMMotor::send_data()
{
    this->cur_setting_voltage = CLAMP(this->cur_setting_voltage,this->Min_setting_voltage,this->Max_setting_voltage);
    set_buffer(this->ptx_buffer,0,8);
    this->encode();
    this->pcan->send_data(&this->tx_header,this->ptx_buffer);
}

void GMMotor::receive_data()
{
    set_buffer(this->prx_buffer,0,8);
    this->pcan->receive_data(&this->rx_header,this->prx_buffer);
    this->decode();
}












void GMMotor::update_setting_voltage()
{

    this->target_rpm = (int16_t)this->pradians_pid->calculate(this->target_radians,this->cur_radians);
    this->cur_setting_voltage = (int16_t)this->prpm_pid->calculate((float)this->target_rpm,(float)this->cur_rpm);

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
    if (this->cur_rpm < this->Min_rpm)
    {
        this->motor_flag = MOTOR_ERROR;
    }


}




/**
 * @brief Reset motor will reset pid, reset target to cur, and check if everything all right; if all is well, motor_flag will be set to MOTOR_PREPARING
 * @warning Motor will go to running state if you do not set to shutdown manually after reset
 *
*/
void GMMotor::reset()
{
    this->check_motor_state();
    if (this->motor_flag == MOTOR_ERROR)
    {
        this->motor_error_handler();
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
    
    this->cur_setting_voltage = GM_MOTOR_SHUTDOWN_VOLTAGE;
}


/**
 * Called in construtor of class, motor will be able to go to running state after receiving first msg of can
*/
void GMMotor::start()
{
    this->reset(); 
    this->target_radians = this->motor_start_radians;
    return;
}         








