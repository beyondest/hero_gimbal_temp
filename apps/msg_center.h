


#ifndef MSG_CENTER_H
#define MSG_CENTER_H
#include "main.h"
#include "topic.h"
#include "logger.h"

//***************************************************Custom Define***************************************************
#define ROTATE_PERIOD_01MS 100
#define EXPIRED_TIME_S 2
#define MAX_TOPIC_CONTAIN_NUMS 1000
#define MAX_BLACK_LIST_NUMS 5



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
    uint32_t Rotate_period_01ms;
    uint8_t Expired_time_s;

//DYNAMIC values
    uint16_t header_index=0;
    uint32_t buffer_pos_index=0; 

    uint8_t black_list_nums=0;
    uint8_t center_second=0;
    uint32_t rotate_times = 0;      //for calculate center_second

public:

//Public flag

    CENTER_STATE Center_State;


//Init 


    Msg_Center(uint32_t rotate_period_01ms,
               uint16_t max_topic_contain_nums,
               uint8_t max_black_list_nums,
               uint32_t max_topic_length,
               uint8_t expired_time_s
               ):
    Max_topic_length(max_topic_length),
    Max_topic_contain_nums(max_topic_contain_nums),
    Max_black_list_nums(max_black_list_nums),
    Rotate_period_01ms(rotate_period_01ms),
    Expired_time_s(expired_time_s),
    Pool_memory((uint32_t)(max_topic_contain_nums*max_topic_length))
    {
        
        pmsg_pool = new uint8_t[Pool_memory];
        pheader_pool = new uint8_t[Max_topic_contain_nums];
        ppos_pool = new uint32_t[Max_topic_contain_nums];
        pblack_list_pool = new uint8_t[Max_black_list_nums];
        psecond_pool = new uint8_t[Max_topic_contain_nums];
        this->init_pool();

    };

    ~Msg_Center()
    {
        delete[] pmsg_pool;
        delete[] pheader_pool;
        delete[] ppos_pool;
        delete[] pblack_list_pool;
        delete[] psecond_pool;
    };
    void init_pool();
    void set_blacklist(Topic* ptopic);


//Main

    bool check_if_allowed(char& topic_header);
    void rotate_pool();
    SEND_STATE receive_msg(Topic* ptopic, uint8_t* tx_buffer);
    SEND_STATE send_msg(Topic* ptopic, uint8_t* rx_buffer);
    
};


//*********************************************************NODE***********************************************************

template <typename Local_Data_Struct>
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
    Local_Data_Struct* plocal_data;

public:
    
//init
    Node(Msg_Center* pcenter,
         uint8_t Max_topic_length):
         pcenter(pcenter),
         Max_topic_length(Max_topic_length)
         {
            ptx_buffer = new uint8_t[Max_topic_length];
            prx_buffer = new uint8_t[Max_topic_length];
            plocal_data = new Local_Data_Struct[1];

         };

    ~Node()
    {
        delete[] ptx_buffer;
        delete[] prx_buffer;
        delete[] plocal_data;
    };
    void set_comms_period(uint32_t r_period_ms,uint32_t t_period_ms)
    {
        this->R_period_ms = r_period_ms;
        this->T_period_ms = t_period_ms;
    };

//Communication with Center

    SEND_STATE send_msg_to_center(Topic* ptopic)
    {
        if (pcenter->Center_State == CENTER_BUSY)
        {
            return SEND_FAILURE;
        }
        else
        {
            SEND_STATE tmp_send_feedback = pcenter->receive_msg(ptopic,this->ptx_buffer);
            this->clear_tx_buffer();
            return tmp_send_feedback;
        }
    };

    SEND_STATE receive_msg_from_center(Topic* ptopic)
    {
        if (pcenter->Center_State == CENTER_BUSY)
        {
            return SEND_FAILURE;
        }
        else
        {
            this->clear_rx_buffer();
            return pcenter->send_msg(ptopic,this->prx_buffer);
        }
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

#ifdef __cplusplus
}
#endif


//****************************************************Extern Value****************************************************


extern Msg_Center Global_msg_center;




#endif