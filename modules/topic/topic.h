

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "main.h"
#include "bsp_data_struct.h"

//****************************************************Custom Define******************************************

#define TOPIC_GIMBAL_CONTROL_LENGTH 16
#define TOPIC_GIMBAL_POS_LENGTH 8
#define TOPIC_PRESENT_TIME_LENGTH 6
#define TOPIC_URGENT_MSG_LENGTH 16

#define MAX_TOPIC_LENGTH 16

//*****************************************************************Universal Typedef***************************************************


typedef enum:uint8_t
{
    NODE_WORK_NORMAL,
    NODE_LOSS

}NODE_STATE;



class Topic
{
private:
protected:

public:
    uint8_t topic_length=0 ;
    char topic_header=0;
    Topic(){};
    ~Topic(){};
    
   // virtual void decode(uint8_t* pbuffer,...);
   // virtual void encode(uint8_t* pbuffer,...);
};



//******************************************TOPIC: PRESENT_TIME*************************************************


/**
 *  buffer_size: uint8t * 6
 *  NO.0 (present_time_minute:int , '<B')              |     (0<=x<60)                  |byte0      bytes 1     total 1  
 *  NO.1 (present_time_second:int , '<B')              |     (0<=x<60)                  |byte1      bytes 1     total 2
 *  NO.2 (present_time_second_frac.4*10000:int, '<F')  |     (0<=x<=1)                  |byte2-5    bytes 2     total 6
*/
class TOPIC_Present_Time:public Topic
{
public:
    TOPIC_Present_Time(char topic_head)
    {
        this->topic_header = topic_head;
        this->topic_length = TOPIC_PRESENT_TIME_LENGTH;
    };

    ~TOPIC_Present_Time(){};
    void decode(uint8_t* pbuffer, 
                uint8_t& present_time_minute,
                uint8_t& present_time_second,
                float& present_time_second_frac
    );
    void encode(uint8_t* pbuffer, 
                const uint8_t& present_time_minute,
                const uint8_t& present_time_second,
                const float& present_time_second_frac);

};



//*********************************************TOPIC: GIMBAL_CONTROL************************************************************




/**
 * buffer_size: uint8t * 16
 * NO.0 (relative_pitch.4:float , '<F')               |     (abs(x)<=1.5708)               |byte0-3    bytes 4     total 4
 * NO.1 (relative_yaw.4:float , '<F')                 |     (abs(x)<=3.1416)               |byte4-7    bytes 4     total 8
 * NO.2 (reach_target_time_minute:int , '<B')         |     (0<=x<60)                      |byte8      bytes 1     total 9
 * NO.3 (reach_target_time_second:int , '<B')         |     (0<=x<=60)                     |byte9      bytes 1     total 10
 * NO.4 (reach_target_time_second_frac.4 , '<F')      |     (0<=x<=1)                      |byte10-13  bytes 4     total 14 
 * NO.5 (setting_voltage_or_rpm: int16 ,  '<h')       |     (-30000<=x<=30000)             |byte14-15  bytes 2     total 16   
 * 
*/
class TOPIC_Gimbal_Control:public Topic
{
   
public:
    TOPIC_Gimbal_Control( char topic_head)
    {
        this->topic_header = topic_head;
        this->topic_length = TOPIC_GIMBAL_CONTROL_LENGTH;
    };
    ~TOPIC_Gimbal_Control(){};
    void encode(uint8_t* pbuffer, 
                const float& relative_pitch, 
                const float& relative_yaw,
                const uint8_t& reach_target_minute,
                const uint8_t& reach_target_second,
                const float& reach_target_second_frac,
                const int16_t& setting_voltage_ro_rpm);
    void decode(uint8_t* pbuffer, 
                float& relative_pitch, 
                float& relative_yaw,
                uint8_t& reach_target_minute,
                uint8_t& reach_target_second,
                float& reach_target_second_frac,
                int16_t& setting_voltage_or_rpm);
};

//***************************************TOPIC: GIMBAL_POS****************************************************88


/**
 *  buffer_size: uint8t * 8
 *  NO.0 (present_pitch.4:int , '<F')            |     (abs(x)<=1.5708)            |byte0-3    bytes 4     total 4
 *  NO.1 (present_yaw.4:int , '<F')              |     (abs(x)<=3.1416)            |byte4-7    bytes 4     total 8
*/
class TOPIC_Gimbal_Pos:public Topic
{
public:
    TOPIC_Gimbal_Pos(char topic_head)
    {
        this->topic_header = topic_head;
        this->topic_length = TOPIC_GIMBAL_POS_LENGTH;
    };
    ~TOPIC_Gimbal_Pos(){};
    void decode(uint8_t* pbuffer, 
                float& present_pitch,
                float& present_yaw);
    void encode(uint8_t* pbuffer, 
                const float& present_pitch,
                const float& present_yaw);
};

//***************************************************TOPIC: Urgent_Msg************************************************

/**
 *  buffer_size: uint8t * 2
 *  NO.0 (Node_Flag:uint8t , '<B')           |  byte0   byte1  total 1       
 *  NO.1 (Node_Wrong_Type, uint8t, '<B')     |  byte1   byte1  total 2
*/
class TOPIC_Urgent_Msg:public Topic
{

public:
    TOPIC_Urgent_Msg(char topic_head)
    {
        this->topic_header = topic_head;
        this->topic_length = TOPIC_URGENT_MSG_LENGTH;

    };
    ~TOPIC_Urgent_Msg(){};
    void decode(uint8_t* pbuffer, uint8_t& node_flag, NODE_STATE& node_state);
    void encode(uint8_t* pbuffer, const uint8_t& node_flag,const NODE_STATE& node_state);
};


//**************************************************Global Value******************************************************


extern TOPIC_Gimbal_Control Global_Topic_gimbal_control;
extern TOPIC_Gimbal_Pos Global_Topic_gimbal_pos;
extern TOPIC_Present_Time Global_Topic_Present_time;
extern TOPIC_Present_Time Global_Topic_Syn_time;



//***********************************************************C API****************************************************************
#ifdef __cplusplus
extern "C"{
#endif

#ifdef __cplusplus
}
#endif















#endif