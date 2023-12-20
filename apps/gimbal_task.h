

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




//*****************************************************Custom TypeDef********************************************************










//***************************************************Custom Function**********************************************************




class Gimbal_Node:public Node<GIMBAL_DATA>
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

//Error Handler
    void send_to_center_error_handler()override
    {
        while (1)
        {
            /* code */
        }
        
    };
    void receive_from_center_error_handler()override
    {
        while (1)
        {
            /* code */
        }
        
    };
};














//***********************************************************C API****************************************************************


#ifdef __cplusplus
extern "C"{
#endif

#ifdef __cplusplus
}
#endif

//***************************************************Extern Value****************************************************************
extern Gimbal_Node Global_gimbal_node;

#endif