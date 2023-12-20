

#ifndef CV_COMMUNICATION_H
#define CV_COMMUNICATION_H
//******************************************************Include*********************************************************
#include "msg_center.h"
#include "topic.h"
#include "bsp_usart.h"
#include "mymath.h"
//******************************************************Custom Define********************************************************
#define ACTION_DATA_LENGTH 16
#define SYN_DATA_LENGTH 16
#define CV_MAX_TOPIC_LENGTH 16
//*******************************************************Custom TypeDef**********************************************************
typedef enum:uint8_t
{
    DATA_STATE_ACTION,
    DATA_STATE_POS,
    DATA_STATE_SYN,
    DATA_STATE_WRONG

}DATA_STATE;


//***************************************************Custom Function****************************************************************


class CV_Node:public Node<CV_DATA>
{
private:
    
//custom const value
    Usart* usart;

//custom dynamic value
    float tmp_cur_yaw=0;
    float tmp_cur_pitch=0;
    float tmp_present_second_frac=0;
    
    SEND_STATE feedback_gimbal_control=SEND_SUCCESS;
    SEND_STATE feedback_gimbal_pos=SEND_SUCCESS;
    SEND_STATE feedback_syn_time=SEND_SUCCESS;
    SEND_STATE feedback_present_time = SEND_SUCCESS;

//custom private methods
    void publish_action_data();
    void publish_syn_data();
    void feedback_pos_data();

    DATA_STATE cv_encode_to_uart();
    DATA_STATE cv_decode_from_uart();
    void receive_action_to_data(uint8_t *prx_data ,ACTION_DATA* pout);
    void receive_syn_to_data(uint8_t *prx_data,SYN_DATA* pout);
    void save_pos_data_to_buffer(POS_DATA* pdata_to_send,uint8_t* ptx_buffer);



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