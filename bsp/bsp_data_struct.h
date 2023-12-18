

#ifndef BSP_DATA_STRUCT_H
#define BSP_DATA_STRUCT_H

#include "main.h"





//*****************************************************Time_Data_Struct*******************************************
typedef struct 
{
    uint8_t minute;
    uint8_t second;
    float second_frac_4;

}TIME_DATA;


//******************************************************CV_Data_Struct*************************************************************


typedef struct 
{
    char   sof                ;
    int8_t fire_times         ;
    int16_t relative_pitch    ;
    int16_t relative_yaw      ;
    uint8_t reach_minute      ;
    uint8_t reach_second      ;
    uint16_t reach_second_frac;
    char null_2byte[2]        ;
    uint32_t crc_check        ;

}ACTION_DATA;




typedef struct
{
    char   sof                  ;
    uint8_t time_minute         ;
    uint8_t time_second         ;
    uint16_t time_second_frac   ;
    char null_7byte[7]          ;
    uint32_t crc_check          ;
    
}SYN_DATA;


typedef struct
{

    char sof;
    uint8_t time_minute;
    uint8_t time_second;
    uint16_t time_second_frac;
    int16_t present_pitch;
    int16_t present_yaw;
    char null_3byte[3];
    uint32_t crc_value;

}POS_DATA;

typedef struct 
{
    ACTION_DATA action_data;
    SYN_DATA syn_data;
    POS_DATA pos_data;

}CV_DATA;


//***************************************************Gimbal_Data_Struct************************************************************




typedef struct 
{
    float target_angle_radians;
    float cur_angle_radians;
    

}SINGLE_MOTOR_DATA;


typedef struct 
{
    
    SINGLE_MOTOR_DATA yaw_motor_data;
    SINGLE_MOTOR_DATA pitch_motor_data;
    TIME_DATA cur_time;
    TIME_DATA target_time;

}GIMBAL_DATA;



void copy(uint8_t* des_buffer, uint8_t* ori_buffer, uint32_t length);
void set_buffer(uint8_t* des_buffer, uint8_t des_num,uint32_t length);
void set_buffer32(uint32_t* des_buffer, uint32_t des_num, uint32_t length);





//***************************************************C API***************************************************************

#ifdef __cplusplus
extern "C"{
#endif




#ifdef __cplusplus
}
#endif


#endif