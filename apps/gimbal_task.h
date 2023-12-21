

#ifndef GIMBAL_TASK_H
#define GIMBAL_TASK_H
//**************************************************Include*************************************************
#include "main.h"
#include "topic.h"
#include "msg_center.h"
#include "mymath.h"
#include "logger.h"

//**************************************************Custom Define*****************************************************


#define GIMBAL_MAX_TOPIC_LENGTH 16

extern SEND_STATE tmpst[4];



//*****************************************************Custom TypeDef********************************************************










//***************************************************Custom Function**********************************************************




class Gimbal_Node:public Node
{
private:
    SEND_STATE feedback_gimbal_control = SEND_SUCCESS;
    SEND_STATE feedback_gimbal_pos = SEND_SUCCESS;
    SEND_STATE feedback_syn_time = SEND_SUCCESS;
    SEND_STATE feedback_present_time = SEND_SUCCESS;


//custom private methods
    void gimbal_yaw_control();
    void gimbal_pitch_control();
    void subscribe_gimbal_control();
    void subscribe_syn_time();
    void publish_gimbal_pos();
    void publish_present_time();

public:
//Init
    GIMBAL_DATA plocal_data;

    Gimbal_Node(Msg_Center* pcenter,uint8_t max_topic_length):
    Node(pcenter,max_topic_length)
    {
        this->init_local_data();
    };
    ~Gimbal_Node()
    {

    };
    void init_local_data()override;

//Run

    void run()override;
    void update_time_second();
//Error Handler
    void send_to_center_error_handler()override
    {

        
        while (1)
        {
            tmpst[0] = this->feedback_gimbal_control;
            tmpst[1] = this->feedback_syn_time;
            tmpst[2] = this->feedback_gimbal_pos;
            tmpst[3] = this->feedback_present_time;
        }
        
    };
    void receive_from_center_error_handler()override
    {
        if (this->feedback_gimbal_control == NO_MSG)
        {
            return;
        }
        else if (this->feedback_syn_time == NO_MSG)
        {
            return;
        }

        
        
        while (1)
        {
            tmpst[0] = this->feedback_gimbal_control;
            tmpst[1] = this->feedback_syn_time;
            tmpst[2] = this->feedback_gimbal_pos;
            tmpst[3] = this->feedback_present_time;
        }
        
    };
};














//***********************************************************C API****************************************************************


#ifdef __cplusplus
extern "C"{
#endif

void gimbal_run();

void gimbal_update_second();
#ifdef __cplusplus
}
#endif

//***************************************************Extern Value****************************************************************
extern Gimbal_Node Global_gimbal_node;

#endif