

#ifndef CV_COMMUNICATION_H
#define CV_COMMUNICATION_H

#include "msg_center.h"
#include "topic.h"
#include "bsp_usart.h"
#include "mymath.h"

#define ACTION_DATA_LENGTH 16
#define SYN_DATA_LENGTH 16
#define CV_MAX_TOPIC_LENGTH 16

typedef enum:uint8_t
{
    DATA_STATE_ACTION,
    DATA_STATE_POS,
    DATA_STATE_SYN,
    DATA_STATE_WRONG

}DATA_STATE;


DATA_STATE cv_decode_from_uart(uint8_t* prx_buffer,CV_DATA* pcv_data_struct);
DATA_STATE cv_encode_to_uart(uint8_t* ptx_buffer, CV_DATA* pcv_data_struct);



class CV_Node:public Node<CV_DATA>
{
private:
    
    //custom const value
    Usart* usart;

    //custom dynamic value
    float tmp_cur_yaw;
    float tmp_cur_pitch;
    float tmp_present_second_frac;
    SEND_STATE tp_c_feedback;
    SEND_STATE tp_t_feedback;
    SEND_STATE tp_p_feedback;


public:

//Init

    CV_Node(Msg_Center* pcenter,
            uint8_t max_topic_length,
            Usart* usart):
    Node(pcenter,max_topic_length),
    usart(usart)
    {
        this->init_local_data();
    };
    ~CV_Node()
    {

    };
    void init_local_data()override;

//RUN

    void run()override;
    void run_action_data();
    void run_syn_data();

    DATA_STATE cv_encode_to_uart();
    DATA_STATE cv_decode_from_uart();



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
        
    }
    void cv_usart_error_handler()
    {
        while (1)
        {
            /* code */
        }
        
    };

};






//*************************************************************C API****************************************************

#ifdef __cplusplus
extern "C"{
#endif



#ifdef __cplusplus
}
#endif

//*********************************************************Extern Value**********************************************

extern CV_Node Global_cv_node;


#endif