


#ifndef MSG_CENTER_H
#define MSG_CENTER_H
#include "main.h"
#include "topic.h"
#include "logger.h"
#include "bsp_tim.h"
//***************************************************Custom Define***************************************************
#define CENTER_ROTATE_PERIOD_01MS 100
#define EXPIRED_TIME_S 2
#define MAX_TOPIC_CONTAIN_NUMS 1000
#define MAX_BLACK_LIST_NUMS 5

//This is a tradeoff between real-time performance and filtering criteria; the larger this value, the better the real-time performance, but the higher the likelihood of omissions
#define MAX_CHECK_EXPIRED_DATA_RATIO_MINUS1 10


//**********************************************Custom Typedef*****************************************************

typedef enum
{

}task_condition;
typedef enum:uint8_t
{
    CENTER_BUSY,
    CENTER_IDLE
}CENTER_STATE;

typedef enum:uint8_t
{
    SEND_SUCCESS,
    SEND_FAILURE,
    REQUEST_DENIED,
    TOPIC_TOO_LONG,
    NO_MSG,
    SEND_STATE_NO_UPDATE
}SEND_STATE;


//*************************************************************CENTER*************************************************************

class Msg_Center
{
private:
//Memory pointer
    uint8_t* pmsg_pool;     //base of msg_pool_buffer
    uint8_t* pheader_pool;
    uint32_t* ppos_pool; 
    uint8_t* pblack_list_pool;
    uint8_t* psecond_pool;

//CONST values
    uint8_t Max_topic_length;
    uint16_t Max_topic_contain_nums;
    uint8_t Max_black_list_nums;
    uint32_t Pool_memory;
    uint8_t Expired_time_s;

//DYNAMIC values
    uint16_t header_index=0;
    uint32_t buffer_pos_index=0; 

    uint8_t black_list_nums=0;
    uint8_t center_second=0;

//Custom private function

    bool check_if_allowed(char& topic_header);
    void clean_expired_data();
    void rotate_pool();


public:

//Public flag

    CENTER_STATE Center_State = CENTER_IDLE;


//Init 

    
               
    Msg_Center(uint32_t max_topic_length,
               uint16_t max_topic_contain_nums,
               uint8_t max_black_list_nums,
               uint8_t expired_time_s
               );

    ~Msg_Center();
    void init_pool();
    void set_blacklist(Topic* ptopic);


//Main
    void update_center_second();
    void run();

    SEND_STATE receive_msg(Topic* ptopic, uint8_t* tx_buffer);
    SEND_STATE send_msg(Topic* ptopic, uint8_t* rx_buffer);


};


//*********************************************************NODE***********************************************************

class Node
{
protected:

//Const value
    uint32_t R_period_ms=0;
    uint32_t T_period_ms=0;
    Msg_Center* pcenter;
    uint8_t* ptx_buffer;
    uint8_t* prx_buffer;
    uint8_t Max_topic_length;
//Dynamic value

public:
    
//init
    Node(Msg_Center* pcenter,
         uint8_t Max_topic_length
         ):
         pcenter(pcenter),
         Max_topic_length(Max_topic_length)
         {
            ptx_buffer = new uint8_t[Max_topic_length];
            prx_buffer = new uint8_t[Max_topic_length];
         };

    ~Node()
    {
        delete[] ptx_buffer;
        delete[] prx_buffer;
    };
    void set_comms_period(uint32_t r_period_ms,uint32_t t_period_ms)
    {
        this->R_period_ms = r_period_ms;
        this->T_period_ms = t_period_ms;
    };

//Main

    /**
     * Each time after publish, txbuffer will be cleaned if success
    */
    SEND_STATE send_msg_to_center(Topic* ptopic)
    {
        SEND_STATE tmp_send_feedback = pcenter->receive_msg(ptopic,this->ptx_buffer);
        if (tmp_send_feedback == SEND_SUCCESS)
        {
            this->clear_tx_buffer();
            
        }
        return tmp_send_feedback;
    };


    SEND_STATE receive_msg_from_center(Topic* ptopic)
    {
        this->clear_rx_buffer();
        SEND_STATE tmp_receive_feedback = pcenter->send_msg(ptopic,this->prx_buffer);

        return tmp_receive_feedback;
    };

    void clear_tx_buffer()
    {
        set_buffer(this->ptx_buffer,0,(uint32_t)this->Max_topic_length);
    };
    void clear_rx_buffer()
    {
        set_buffer(this->prx_buffer,0,(uint32_t)this->Max_topic_length);
    };

//virtual function
    virtual void run()
    {
        while (1)
        {
        }
        
    };
    
    virtual void init_local_data()
    {
        while (1)
        {
            /* code */
        }
        
    };

    virtual void send_to_center_error_handler()
    {
        while (1)
        {
        }
        
    };
    
    virtual void receive_from_center_error_handler()
    {
        while (1)
        {
        }
        
    };
};


//****************************************************C API**************************************************************
#ifdef __cplusplus
extern "C"{
#endif

void msg_center_run();
void msg_center_update_second();

#ifdef __cplusplus
}
#endif


//****************************************************Extern Value****************************************************


extern Msg_Center Global_msg_center;



#endif