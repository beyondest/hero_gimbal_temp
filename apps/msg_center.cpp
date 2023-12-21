#include "msg_center.h"
#include "mymath.h"



//*********************************************Global Value********************************************

Msg_Center Global_msg_center(
        MAX_TOPIC_LENGTH,
        MAX_TOPIC_CONTAIN_NUMS,
        MAX_BLACK_LIST_NUMS,
        EXPIRED_TIME_S
);


void msg_center_run()
{
    Global_msg_center.run();
}

void msg_center_update_second()
{
    Global_msg_center.update_center_second();
}



//*****************************************************Msg_Center Public Methods********************************************************************************




/**
 * For Tim
*/
void Msg_Center::run()
{
    if (this->Center_State == CENTER_BUSY)
    {
        LOG_INFO("rotate_pool failed, center busy");
        return;
    }
    else
    {
        this->Center_State = CENTER_BUSY;
        this->rotate_pool();
        this->clean_expired_data();
        this->Center_State = CENTER_IDLE;
    
    }
}



void Msg_Center::update_center_second()
{
    if (this->Center_State == CENTER_BUSY)
    {
        LOG_INFO("failed to update center second, center busy");
        return;
    }
    this->Center_State = CENTER_BUSY;
    this->center_second++;
    this->center_second = RCLAMP(center_second,0,59);
    this->Center_State = CENTER_IDLE;
}


//******************************************************Msg_Center Private Methods*******************************************
Msg_Center::Msg_Center(uint32_t max_topic_length,
               uint16_t max_topic_contain_nums,
               uint8_t max_black_list_nums,
               uint8_t expired_time_s
               ):
    Max_topic_length(max_topic_length),
    Max_topic_contain_nums(max_topic_contain_nums),
    Max_black_list_nums(max_black_list_nums),
    Pool_memory((uint32_t)(max_topic_contain_nums*max_topic_length)),
    Expired_time_s(expired_time_s)
    {
        
        pmsg_pool = new uint8_t[Pool_memory];
        pheader_pool = new uint8_t[Max_topic_contain_nums];
        ppos_pool = new uint32_t[Max_topic_contain_nums];
        pblack_list_pool = new uint8_t[Max_black_list_nums];
        psecond_pool = new uint8_t[Max_topic_contain_nums];
        this->init_pool();

    };

Msg_Center:: ~Msg_Center()
    {
        delete[] pmsg_pool;
        delete[] pheader_pool;
        delete[] ppos_pool;
        delete[] pblack_list_pool;
        delete[] psecond_pool;
    };


/**
 * When a node wants to subscribe, call this function in node receive_msg_from_center
*/
SEND_STATE Msg_Center::send_msg(Topic* ptopic, uint8_t* prx_buffer)
{
    if (this->Center_State == CENTER_BUSY)
    {
        LOG_INFO("subscribe failed, center busy");

        return SEND_FAILURE;
    }
    else if (ptopic->topic_length>this->Max_topic_length)
    {
        LOG_INFO("subscribe failed, topic too long");
        return TOPIC_TOO_LONG;
    }
    else if (!check_if_allowed(ptopic->topic_header))
    {
        LOG_INFO("subscribe failed, request denied");
        return REQUEST_DENIED;
    } 
    else
    {
        this->Center_State = CENTER_BUSY;


        uint16_t target_index = this->header_index;
        uint8_t* ptarget_buffer;

        for (uint16_t i = 0; i < this->Max_topic_contain_nums; i++)
        {
            if (pheader_pool[target_index] == ptopic->topic_header)
            {
                ptarget_buffer = &(this->pmsg_pool[this->ppos_pool[target_index]]);
                copy(prx_buffer,ptarget_buffer,ptopic->topic_length);
                this->Center_State = CENTER_IDLE;
                return SEND_SUCCESS;
            }
            target_index--;
            target_index = RCLAMP(target_index,0,this->Max_topic_contain_nums-1);
        }

        this->Center_State = CENTER_IDLE;
        
        LOG_INFO("subscribe failed, no topic you want");
        return NO_MSG;
    }
    
};

/**
 * When a node wants to publish , call this function in node send_msg_to_center
*/
SEND_STATE Msg_Center::receive_msg(Topic* ptopic, uint8_t* ptx_buffer)
{
    if (this->Center_State == CENTER_BUSY)
    {
        LOG_INFO("publish failed, center busy");
        return SEND_FAILURE;

    }
    else if (ptopic->topic_length>this->Max_topic_length)
    {

        LOG_INFO("publish failed, topic too long");
        return TOPIC_TOO_LONG;
    }
    else if (!check_if_allowed(ptopic->topic_header))
    {
        LOG_INFO("publish failed, request denied");
        return REQUEST_DENIED;
    } 
    else
    {
        this->Center_State = CENTER_BUSY;

        uint8_t* cur_pmsg_pool=&(this->pmsg_pool[this->buffer_pos_index]);
        uint8_t* cur_pheader_pool= &(this->pheader_pool[this->header_index]); 
        uint32_t* cur_ppos_pool= &(this->ppos_pool[this->header_index]);
        uint8_t* cur_psecond_pool = &(this->psecond_pool[this->header_index]);

        
        copy(cur_pmsg_pool,ptx_buffer,ptopic->topic_length);
        *cur_pheader_pool = ptopic->topic_header;
        *cur_ppos_pool = this->buffer_pos_index;
        *cur_psecond_pool = this->center_second;

        this->rotate_pool();

        this->Center_State = CENTER_IDLE;
        return SEND_SUCCESS;
    }
}


bool Msg_Center::check_if_allowed(char& topic_header)
{
    for (size_t i = 0; i < this->black_list_nums; i++)
    {
        if (topic_header == this->pblack_list_pool[i])
        {
            return false;
        }  
    }
    return true;
    
};


/**
 * @brief Only called in rotate pool, one rotate one call; only check 1/10 pool
*/
void Msg_Center::clean_expired_data()
{
    uint16_t target_index = this->header_index;
    uint8_t diff_time_s = 0;
    uint16_t max_check_nums = (uint16_t)(this->Max_topic_contain_nums/MAX_CHECK_EXPIRED_DATA_RATIO_MINUS1);
    //
    for (uint16_t i = 0; i < max_check_nums; i++)
    {
        diff_time_s =(uint8_t)ABS((int8_t)this->psecond_pool[target_index]-(int8_t)this->center_second);
        if (diff_time_s>this->Expired_time_s)
        {

            uint8_t* cur_pheader_pool= &(this->pheader_pool[target_index]); 
            uint8_t* cur_psecond_pool = &(this->psecond_pool[target_index]);
            *cur_pheader_pool = 0;
            *cur_psecond_pool = this->center_second;
        }  
        target_index--;
        target_index = RCLAMP(target_index,0,this->Max_topic_contain_nums);
    }
    

}

/**
 * 4 pointers ++ together in once run
 * msg_pool ++ max_topic_length
 * header_pool ++
 * pos_pool ++
 * time_pool ++
 * 
*/
void Msg_Center::rotate_pool()
{
    this->buffer_pos_index+=this->Max_topic_length;
    this->header_index+=1;
    this->buffer_pos_index = RCLAMP(this->buffer_pos_index,0,Pool_memory-1);
    this->header_index = RCLAMP(this->header_index,0,this->Max_topic_contain_nums-1);
}


void Msg_Center::set_blacklist(Topic* ptopic)
{

    if (this->black_list_nums>this->Max_black_list_nums)
    {

        LOG_INFO("set_blacklist failed, black_list_nums > Max balck_list_nums");
        return;
    }
            
    this->pblack_list_pool[this->black_list_nums++] =ptopic->topic_header;
};


void Msg_Center::init_pool()
{
    set_buffer(this->pmsg_pool,0,this->Pool_memory);
    set_buffer(this->pblack_list_pool,0,(uint32_t)this->Max_black_list_nums);

    set_buffer(this->pheader_pool,0,(uint32_t)this->Max_topic_contain_nums);
    set_buffer32(this->ppos_pool,0,(uint32_t)this->Max_topic_contain_nums);
    set_buffer(this->psecond_pool,this->center_second,(uint32_t)this->Max_topic_contain_nums);
}












