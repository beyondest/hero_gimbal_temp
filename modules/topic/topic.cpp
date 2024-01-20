#include "topic.h"







//*************************************************Used Topic Init*************************************************************



TOPIC_Gimbal_Control Global_Topic_gimbal_control('C');
TOPIC_Gimbal_Pos Global_Topic_gimbal_pos('P');
TOPIC_Present_Time Global_Topic_Present_time('T');

TOPIC_Present_Time Global_Topic_Syn_time('S');








//***********************************************TOPIC: Gimbal_Control***************************************************************

void TOPIC_Gimbal_Control::encode(uint8_t* pbuffer, 
                                const float& relative_pitch, 
                                const float& relative_yaw,
                                const uint8_t& reach_target_minute,
                                const uint8_t& reach_target_second,
                                const float& reach_target_second_frac,
                                const int16_t& setting_voltage_or_rpm)
{
    float* pf;
    int16_t* pi;
    pf = (float*)&pbuffer[0];
    *pf = relative_pitch;
    pf = (float*)&pbuffer[4];
    *pf = relative_yaw;

    pbuffer[8] = reach_target_minute;
    pbuffer[9] = reach_target_second;
    
    pf = (float*)&pbuffer[10];
    *pf = reach_target_second_frac;
    
    pi = (int16_t*)&pbuffer[14];
    *pi = setting_voltage_or_rpm;

}

void TOPIC_Gimbal_Control::decode(uint8_t* pbuffer, 
                                float& relative_pitch, 
                                float& relative_yaw,
                                uint8_t& reach_target_minute,
                                uint8_t& reach_target_second,
                                float& reach_target_second_frac,
                                int16_t& setting_voltage_or_rpm)
{
    float* pf;
    int16_t* pi;
    pf = (float*)&pbuffer[0];
    relative_pitch=*pf;
    pf = (float*)&pbuffer[4];
    relative_yaw=*pf;

    reach_target_minute=pbuffer[8];
    reach_target_second= pbuffer[9];
    
    pf = (float*)&pbuffer[10];
    reach_target_second_frac=*pf;
    pi = (int16_t*)&pbuffer[14];
    setting_voltage_or_rpm = *pi;

}


//***********************************************TOPIC: Gimbal_Pos***************************************************************



void TOPIC_Gimbal_Pos::decode(uint8_t* pbuffer, 
                              float& present_pitch,
                              float& present_yaw,
                              int16_t& present_debug_value)
{
    present_pitch = *(float*)&pbuffer[0];
    present_yaw = *(float*)&pbuffer[4];
    present_debug_value = *(int16_t*)&pbuffer[8];
}
void TOPIC_Gimbal_Pos::encode(uint8_t* pbuffer, 
                            const float& present_pitch,
                            const float& present_yaw,
                            const int16_t& present_debug_value)
{
    float* pf;
    int16_t* pint16;
    pf = (float*)&pbuffer[0];
    *pf = present_pitch;
    pf =(float*)&pbuffer[4];
    *pf = present_yaw;
    pint16 = (int16_t*)&pbuffer[8];
    *pint16 = present_debug_value;
}

//***********************************************TOPIC: Present_Time***************************************************************


void TOPIC_Present_Time::decode(uint8_t* pbuffer, 
                                uint8_t& present_time_minute,
                                uint8_t& present_time_second,
                                float& present_time_second_frac)
{
    present_time_minute = pbuffer[0];
    present_time_second = pbuffer[1];
    present_time_second_frac = *(float*)&pbuffer[2];
}
void TOPIC_Present_Time::encode(uint8_t* pbuffer, 
                                const uint8_t& present_time_minute,
                                const uint8_t& present_time_second,
                                const float& present_time_second_frac)
{
    float* pf;
    pbuffer[0]=present_time_minute;
    pbuffer[1]=present_time_second;
    pf = (float*)&pbuffer[2];
    *pf =present_time_second_frac;
}




//********************************************Topic: Urgent MSG*****************************************************
void TOPIC_Urgent_Msg::decode(uint8_t* pbuffer, uint8_t& node_flag, NODE_STATE& node_state)
{
    node_flag = *pbuffer;
    node_state = (NODE_STATE)pbuffer[1];

}
void TOPIC_Urgent_Msg::encode(uint8_t* pbuffer, const uint8_t& node_flag,const NODE_STATE& node_state)
{
    *pbuffer = node_flag;
    pbuffer[1] = (uint8_t)node_state;
}





