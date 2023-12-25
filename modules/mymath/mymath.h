

#ifndef MYMATH_H
#define MYMATH_H

#include "main.h"
#include "crc.h"
//*******************************************************Custom Define**********************************************







#define PITCH_RADIANS_PID_KP 50
#define PITCH_RADIANS_PID_KI 0
#define PITCH_RADIANS_PID_KD 0
#define PITCH_RADIANS_PID_MAX_P MEGA
#define PITCH_RADIANS_PID_MIN_P (-MEGA)
#define PITCH_RADIANS_PID_MAX_I MEGA
#define PITCH_RADIANS_PID_MIN_I (-MEGA)
#define PITCH_RADIANS_PID_MAX_D MEGA
#define PITCH_RADIANS_PID_MIN_D (-MEGA)


#define PITCH_RPM_KP 40
#define PITCH_RPM_KI  3
#define PITCH_RPM_KD  0
#define PITCH_RPM_MAX_P MEGA
#define PITCH_RPM_MIN_P (-MEGA)
#define PITCH_RPM_MAX_I MEGA
#define PITCH_RPM_MIN_I (-MEGA)
#define PITCH_RPM_MAX_D MEGA
#define PITCH_RPM_MIN_D (-MEGA)



#define YAW_RADIANS_PID_KP 50
#define YAW_RADIANS_PID_KI  0
#define YAW_RADIANS_PID_KD  0
#define YAW_RADIANS_PID_MAX_P MEGA
#define YAW_RADIANS_PID_MIN_P (-MEGA)
#define YAW_RADIANS_PID_MAX_I MEGA
#define YAW_RADIANS_PID_MIN_I (-MEGA)
#define YAW_RADIANS_PID_MAX_D MEGA
#define YAW_RADIANS_PID_MIN_D (-MEGA)


#define YAW_RPM_KP 40
#define YAW_RPM_KI  3
#define YAW_RPM_KD  0
#define YAW_RPM_MAX_P MEGA
#define YAW_RPM_MIN_P (-MEGA)
#define YAW_RPM_MAX_I MEGA
#define YAW_RPM_MIN_I (-MEGA)
#define YAW_RPM_MAX_D MEGA
#define YAW_RPM_MIN_D (-MEGA)









//clamp x to be in scope(bottom, upper), difference is that rclamp is circle x from upper to bottom and from bottom to upper;
#define CLAMP(x,bottom,upper) ((x<bottom)?(bottom):((x>upper)?(upper):(x)))

//abs(x+step-upper)  or abs(x-step-bottom) must be 1, or will go wrong
#define RCLAMP(x,bottom,upper) ((x<bottom)?(upper):((x>upper)?(bottom):(x)))


#define PIE 3.1416
#define MEGA 1000000



//*************************************************Custom TypeDef***********************************************

typedef enum:uint8_t
{
    CRC_RIGHT=1,
    CRC_WRONG=0
}CRC_STATE;



//*****************************************Custom Functino***********************************************************




CRC_STATE check_data4_crc32(uint8_t *pbuffer,uint8_t length_4multi);




template <typename T>
T ABS(T x)
{
    return (x<0)?(-x):x;
}



/**
 * @brief Use to divide an integer by 10000 and obtain a float variable with four decimal places.
*/
template <typename T>
float trans_frac_float(T frac_4)
{
    float tmp;
    tmp = (float)frac_4;
    return tmp/10000;
}



int16_t trans_frac_int16(float frac_4);

uint16_t trans_frac_uint16(float frac_4);


class Pid
{
private:
//Const value
    float kp ;
    float ki ;
    float kd ;
    
    float max_p;
    float min_p;
    float max_i;
    float min_i;
    float max_d;
    float min_d;

    float max_target_value;
    float min_target_value;
    float max_cur_value;
    float min_cur_value;
    float max_output;
    float min_output;


//Dynamic value

    float cur_error = 0;
    float pre_error = 0;

    float p = 0;
    float i = 0;
    float d = 0;
    float target_value = 0;
    float cur_value = 0;
    float output = 0;



public:



    Pid(float kp,
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
        float min_output);

    ~Pid(){};

    float calculate(float goal, float cur);
    void reset();
    

};





//**********************************************************C API********************************************************

#ifdef __cplusplus
extern "C"{
#endif



#ifdef __cplusplus
}
#endif

//*****************************************************Extern Value******************************************************

extern Pid Global_pitch_radians_pid;


extern Pid Global_pitch_rpm_pid;





extern Pid Global_yaw_radians_pid;

                              
extern Pid Global_yaw_rpm_pid;
                          


#endif