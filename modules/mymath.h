

#ifndef MYMATH_H
#define MYMATH_H

#include "main.h"
#include "crc.h"

//clamp x to be in scope(bottom, upper), difference is that rclamp is circle x from upper to bottom and from bottom to upper;
#define CLAMP(x,bottom,upper) ((x<bottom)?(bottom):((x>upper)?(upper):(x)))

//abs(x+step-upper)  or abs(x-step-bottom) must be 1, or will go wrong
#define RCLAMP(x,bottom,upper) ((x<bottom)?(upper):((x>upper)?(bottom):(x)))

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
    float kp = 0;
    float ki = 0;
    float kd = 0;

//Dynamic value
    float cur_error = 0;
    float pre_error = 0;
    float p = 0;
    float i = 0;
    float d = 0;



public:



    Pid(/* args */);
    ~Pid();

    float calculate(float goal, float cur_value);
    
};





//**********************************************************C API********************************************************

#ifdef __cplusplus
extern "C"{
#endif



#ifdef __cplusplus
}
#endif



#endif