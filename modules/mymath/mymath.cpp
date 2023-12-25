#include "mymath.h"
#include "motor.h"



//**********************************************************Global Value**************************************************


Pid Global_pitch_radians_pid( PITCH_RADIANS_PID_KP,
                              PITCH_RADIANS_PID_KI,
                              PITCH_RADIANS_PID_KD,
                              PITCH_RADIANS_PID_MAX_P,
                              PITCH_RADIANS_PID_MIN_P,
                              PITCH_RADIANS_PID_MAX_I,
                              PITCH_RADIANS_PID_MIN_I,
                              PITCH_RADIANS_PID_MAX_D,
                              PITCH_RADIANS_PID_MIN_D,
                              PITCH_RADIANS_MAX,
                              PITCH_RADIANS_MIN,
                              PITCH_RADIANS_MAX,
                              PITCH_RADIANS_MIN,
                              PITCH_MAX_RPM,
                              PITCH_MIN_RPM
                              );


Pid Global_pitch_rpm_pid( PITCH_RPM_KP,
                          PITCH_RPM_KI,
                          PITCH_RPM_KD,
                          PITCH_RPM_MAX_P,
                          PITCH_RPM_MIN_P,
                          PITCH_RPM_MAX_I,
                          PITCH_RPM_MIN_I,
                          PITCH_RPM_MAX_D,
                          PITCH_RPM_MIN_D,
                          PITCH_MAX_RPM,
                          PITCH_MIN_RPM,
                          PITCH_MAX_RPM,
                          PITCH_MIN_RPM,
                          PITCH_MAX_SETTING_VOLTAGE,
                          PITCH_MIN_SETTING_VOLTAGE
                          );





Pid Global_yaw_radians_pid(   YAW_RADIANS_PID_KP,
                              YAW_RADIANS_PID_KI,
                              YAW_RADIANS_PID_KD,
                              YAW_RADIANS_PID_MAX_P,
                              YAW_RADIANS_PID_MIN_P,
                              YAW_RADIANS_PID_MAX_I,
                              YAW_RADIANS_PID_MIN_I,
                              YAW_RADIANS_PID_MAX_D,
                              YAW_RADIANS_PID_MIN_D,
                              YAW_RADIANS_MAX,
                              YAW_RADIANS_MIN,
                              YAW_RADIANS_MAX,
                              YAW_RADIANS_MIN,
                              YAW_MAX_RPM,
                              YAW_MIN_RPM
                              );

                              
Pid Global_yaw_rpm_pid(   YAW_RPM_KP,
                          YAW_RPM_KI,
                          YAW_RPM_KD,
                          YAW_RPM_MAX_P,
                          YAW_RPM_MIN_P,
                          YAW_RPM_MAX_I,
                          YAW_RPM_MIN_I,
                          YAW_RPM_MAX_D,
                          YAW_RPM_MIN_D,
                          YAW_MAX_RPM,
                          YAW_MIN_RPM,
                          YAW_MAX_RPM,
                          YAW_MIN_RPM,
                          YAW_MAX_SETTING_VOLTAGE,
                          YAW_MIN_SETTING_VOLTAGE
                          );
                          






















//*************************************************Custom Function****************************************************************



/**
 * @brief Use to check crc value of uint8 array 
 * @warning Use default HAL_CRC_Calculate, do not use rev in or out;
 * @warning length_4multi must be multiple of 4, and include CRC bytes 4
 * @warning sender's CRC value is in the last 4 bytes
 * @retval crc_right =1;crc_wrong =0
*/
CRC_STATE check_data4_crc32(uint8_t *pbuffer,uint8_t length_4multi)
{
    uint32_t *p32;
    uint32_t crc_cal = 0;
    
    p32 = (uint32_t*)&pbuffer[0];
    crc_cal = HAL_CRC_Calculate(&hcrc,p32,(uint32_t)(length_4multi/4-1));

    p32 = (uint32_t*)&pbuffer[12];
    if (*p32 == crc_cal)
    {
      return CRC_RIGHT;
    }
    else
    {
      return CRC_WRONG;
    }
}



/**
 * @brief Use to trans float frac_4 to uint16 by multiply 10000
*/
uint16_t trans_frac_uint16(float frac_4)
{
    float tmp = frac_4*10000;
    return (uint16_t)tmp;
}

/**
 * @brief Use to trans float frac_4 to int16 by multiply 10000
*/
int16_t trans_frac_int16(float frac_4)
{
    float tmp = frac_4*10000;
    return (int16_t)tmp;
}



Pid::Pid(float kp,
        float ki, 
        float kd,
        float max_p,
        float min_p,
        float max_i,
        float min_i,
        float max_d,
        float min_d,
        float max_target_value,
        float min_target_value,
        float max_cur_value,
        float min_cur_value,
        float max_output,
        float min_output):
        kp(kp),
        ki(ki),
        kd(kd),
        max_p(max_p),
        min_p(min_p),
        max_i(max_i),
        min_i(min_i),
        max_d(max_d),
        min_d(min_d),
        max_target_value(max_target_value),
        min_target_value(min_target_value),
        max_cur_value(max_cur_value),
        min_cur_value(min_cur_value),
        max_output(max_output),
        min_output(min_output)
{
    this->reset();
}

float Pid::calculate(float goal, float cur)
{
    this->target_value  =  CLAMP(goal,this->min_target_value,this->max_target_value);
    this->cur_value     =  CLAMP(cur,this->min_cur_value,this->max_cur_value);


    this->pre_error = this->cur_error;
    this->cur_error = this->target_value - this->cur_value;


    this->p =  this->kp * this->cur_error;
    this->i += this->ki * this->cur_error;
    this->d =  this->kd * (this->cur_error - this->pre_error);
  
    this->p = CLAMP(this->p,this->min_p,this->max_p);
    this->i = CLAMP(this->i,this->min_i,this->max_i);
    this->d = CLAMP(this->d,this->min_d,this->max_d);


    this->output = this->p + this->i + this->d;
    this->output = CLAMP(this->output,this->min_output,this->max_output);
    return this->output;

    
}



void Pid::reset()
{
  this->cur_error =   0;
  this->pre_error =   0;
  this->p         =   CLAMP(0,this->min_p,this->max_p);
  this->i         =   CLAMP(0,this->min_i,this->max_i);
  this->d         =   CLAMP(0,this->min_d,this->max_d);
  this->target_value =   CLAMP(0,this->min_target_value,this->max_target_value); 
  this->cur_value =   CLAMP(0,this->min_cur_value,this->max_cur_value);
  this->output    =   CLAMP(0,this->min_output,this->max_output);
}
