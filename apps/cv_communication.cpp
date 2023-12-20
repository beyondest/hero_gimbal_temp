#include "cv_communication.h"
#include "bsp_usart.h"


//*********************************************************Global Value**********************************************
CV_Node Global_cv_node(

    &Global_msg_center,
    CV_MAX_TOPIC_LENGTH,
    &usart8
);










//******************************************************CV_Node function********************************************



/**
 * For tim to call
*/
void CV_Node::run()
{
    switch (this->cv_decode_from_uart())
    {
    case DATA_STATE_WRONG:
        this->cv_usart_error_handler();
        break;
    case DATA_STATE_ACTION:
        this->publish_action_data();
    case DATA_STATE_SYN:
        this->publish_syn_data();
    default:
        break;
    }
    this->feedback_pos_data();

}



void CV_Node::publish_action_data()
{

    Global_Topic_gimbal_control.encode(this->ptx_buffer,
                                        trans_frac_float(this->plocal_data->action_data.relative_pitch),
                                        trans_frac_float(this->plocal_data->action_data.relative_yaw),
                                        this->plocal_data->action_data.reach_minute,
                                        this->plocal_data->action_data.reach_second,
                                        trans_frac_float(this->plocal_data->action_data.reach_second_frac)
                                        );

    if ((this->feedback_gimbal_control = this->send_msg_to_center(&Global_Topic_gimbal_control)) != SEND_SUCCESS)
    {
        this->send_to_center_error_handler();
    }

}


void CV_Node::publish_syn_data()
{

    Global_Topic_Syn_time.encode(this->ptx_buffer,
                                 this->plocal_data->syn_data.time_minute,
                                 this->plocal_data->syn_data.time_second,
                                 trans_frac_float(this->plocal_data->syn_data.time_second_frac)
                                 );
    
    if ((this->feedback_syn_time = this->send_msg_to_center(&Global_Topic_Syn_time)) != SEND_SUCCESS)
    {
        this->send_to_center_error_handler();
    }

}

/**
 * @brief Only when subscribe Pos and Present time is both SUCCESS, then will feedback to upper PC
 * 
*/
void CV_Node::feedback_pos_data()
{
//subscribe gimbal pos

    if ((this->feedback_gimbal_pos = this->receive_msg_from_center(&Global_Topic_gimbal_pos))== SEND_SUCCESS)
    {
 
        Global_Topic_gimbal_pos.decode(this->prx_buffer,
                                        this->tmp_cur_pitch,
                                        this->tmp_cur_yaw
                                        );
        this->plocal_data->pos_data.present_pitch = trans_frac_int16(this->tmp_cur_pitch);
        this->plocal_data->pos_data.present_yaw = trans_frac_int16(this->tmp_cur_yaw);
    }
    else
    {
        this->receive_from_center_error_handler();
    }

//subscribe present time, Only CV_node need to trans_frac_uint16 here
    if ((this->feedback_present_time = this->receive_msg_from_center(&Global_Topic_Present_time)) == SEND_SUCCESS)
    {
        Global_Topic_Present_time.decode(this->prx_buffer,
                                            this->plocal_data->pos_data.time_minute,
                                            this->plocal_data->pos_data.time_second,
                                            this->tmp_present_second_frac
                                            );
        this->plocal_data->pos_data.time_second_frac = trans_frac_uint16(this->tmp_present_second_frac);
    }
    else
    {
        this->receive_from_center_error_handler();
    }
    
//send to upper pos data if everything OK

    if (this->feedback_gimbal_pos == SEND_SUCCESS && this->feedback_present_time == SEND_SUCCESS)
    {
        this->cv_encode_to_uart();
        this->usart->send_data();
    }
}


void CV_Node::init_local_data()
{
// Action Data   
    this->plocal_data->action_data.sof = 'A';
    this->plocal_data->action_data.fire_times = -1;
    this->plocal_data->action_data.relative_pitch = 0;
    this->plocal_data->action_data.relative_yaw = 0;
    this->plocal_data->action_data.reach_minute = 0;
    this->plocal_data->action_data.reach_second = 0;
    this->plocal_data->action_data.reach_second_frac = 0;
    this->plocal_data->action_data.null_2byte[0] = '1' ;
    this->plocal_data->action_data.null_2byte[1] = '2' ;
    this->plocal_data->action_data.crc_check = 0;

//Syn Data
    this->plocal_data->syn_data.sof = 'S';
    this->plocal_data->syn_data.time_minute = 0;
    this->plocal_data->syn_data.time_second = 0;
    this->plocal_data->syn_data.time_second_frac = 0;
    set_buffer((uint8_t*)&this->plocal_data->syn_data.null_7byte,0,7);
    this->plocal_data->syn_data.crc_check = 0;

//Pos Data  
    this->plocal_data->pos_data.sof = 'P';
    this->plocal_data->pos_data.time_minute = 0;
    this->plocal_data->pos_data.time_second = 0;
    this->plocal_data->pos_data.time_second_frac = 0;
    this->plocal_data->pos_data.present_pitch = 0;
    this->plocal_data->pos_data.present_yaw = 0;
    this->plocal_data->pos_data.null_3byte[0] = '1';
    this->plocal_data->pos_data.null_3byte[1] = '2';
    this->plocal_data->pos_data.null_3byte[2] = '3';
    this->plocal_data->pos_data.crc_value = 0;
}



/** 
 * buffer_size : uint8t * 16
 * more info please see inner funciton defination
*/
DATA_STATE CV_Node::cv_decode_from_uart()
{
    uint8_t* prx_buffer = this->usart->prx_buffer;
    CV_DATA* pcv_data_struct = this->plocal_data;
    switch ((*prx_buffer))
    {
    case 'A':
        if(check_data4_crc32(prx_buffer,ACTION_DATA_LENGTH) != CRC_RIGHT)
        {
            return DATA_STATE_WRONG;
        }
        else
        {
            this->receive_action_to_data(prx_buffer,&(pcv_data_struct->action_data));
            return DATA_STATE_ACTION;
        }
        break;
    case 'S':
        if (check_data4_crc32(prx_buffer,SYN_DATA_LENGTH) != CRC_RIGHT)
        {
            return DATA_STATE_WRONG;
        }
        else
        {
            this->receive_syn_to_data(prx_buffer,&(pcv_data_struct->syn_data));
            return DATA_STATE_SYN;
        }
        break;
    default:
        return DATA_STATE_WRONG;
        break;
    }
}

/** 
 * buffer_size : uint8t * 16
 * more info please see inner funciton defination
*/
DATA_STATE CV_Node::cv_encode_to_uart()
{
    //crc will add to end by this func
    this->save_pos_data_to_buffer(&(this->plocal_data->pos_data),this->usart->ptx_buffer);
    return DATA_STATE_POS;
}


//************************************************CV_UART_Protocol******************************************************



/**
 * BRIEF:  Save rx_data to pout_struct if crc_value is right
 * RETURN: crc_right =1;crc_wrong =0;
 * buffer_size: uint8t * 16
 * NO.0 (SOF:char , '<c')                             |     ('A')                      |byte0      bytes 1     total 1
 * NO.1 (fire_times:int , '<b')                       |     (-1<=x<=100)               |byte1      bytes 1     total 2 (-1:not control;0:control not fire) 
 * NO.2 (relative_pitch.4*10000:int , '<h')           |     (abs(x)<=15708)            |byte2-3    bytes 2     total 4
 * NO.3 (relative_yaw.4*10000:int , '<h')             |     (abs(x)<=31416)            |byte4-5    bytes 2     total 6
 * NO.4 (reach_target_time_minute:int , '<B')         |     (0<=x<60)                  |byte6      bytes 1     total 7
 * NO.5 (reach_target_time_second:int , '<B')         |     (0<=x<=60)                 |byte7      bytes 1     total 8
 * NO.6 (reach_target_time_second_frac.4*10000 , '<H')|     (0<=x<=10000)              |byte8-9    bytes 2     total 10 
 * NO.78 (null bytes: b'12', '<c','<c')  (auto add)   |                                |byte10-11  bytes 2     total 12
 * NO.9(crc_value:int , '<I')   (auto add to end)     |     (return of cal_crc func)   |byte12-15  bytes 4     total 16 
 * PART_CRC: byte2-5
*/
void CV_Node::receive_action_to_data(uint8_t *prx_data ,ACTION_DATA* pout)
{
  pout->sof                 = prx_data[0];
  pout->fire_times          = prx_data[1];
  pout->relative_pitch      = *((int16_t*) &prx_data[2]);
  pout->relative_yaw        = *((int16_t*) &prx_data[4]);
  pout->reach_minute        = prx_data[6];
  pout->reach_second        = prx_data[7];
  pout->reach_second_frac   = *((uint16_t*) &prx_data[8]);
  pout->null_2byte[0]       = prx_data[10];
  pout->null_2byte[1]       = prx_data[11];
  pout->crc_check           = *((uint32_t*) &prx_data[12]);
}

/**
 *   BREIF: Save rx_data to synchronous data struct 
 *   buffer_size: uint8t * 16
 *   NO.0 (SOF:char , '<c')                             |     ('S')                      |byte0      bytes 1     total 1
 *   NO.1 (present_time_minute:int , '<B')              |     (0<=x<60)                  |byte1      bytes 1     total 2   
 *   NO.2 (present_time_second:int , '<B')              |     (0<=x<60)                  |byte2      bytes 1     total 3
 *   NO.3 (present_time_second_frac.4*10000:int, '<H')  |     (0<=x<=10000)              |byte3-4    bytes 2     total 5
 *   NO.4 (null_byte:b'1234567')   (auto add)(must add) |                                |byte5-11   bytes 7     total 12
 *   NO.5 (crc_value:int , '<I')  (auto add to end)     |     (return of cal_crc func)   |byte12-15  bytes 4     total 16
 *   PART_CRC: byte1-4
 * 
*/
void CV_Node::receive_syn_to_data(uint8_t *prx_data,SYN_DATA* pout)
{
  pout->sof = prx_data[0];
  pout->time_minute = *((uint8_t*)&prx_data[1]);
  pout->time_second = *((uint8_t*)&prx_data[2]);
  pout->time_second_frac = *((uint16_t*)&prx_data[3]);
  for (uint8_t i = 0; i < 7; i++)
  {
    pout->null_7byte[i] = prx_data[5+i];
  }  
  pout->crc_check = *((uint32_t*)&prx_data[12]);
}

/**
 *  BRIEF: Send position and present time data to upper
 *  PARAMS: huart|decide to use which uart to send data
 *  PARAMS: pdata_to_send|data struct pointer, which already has data to send
 *  NOTICE: Will calculate crc in this function;Only save to UART8 buffer
 *  buffer_size: uint8t * 16
 *  NO.0 (SOF:char , '<c')                             |     ('P')                      |byte0      bytes 1     total 1
 *  NO.1 (present_time_minute:int , '<B')              |     (0<=x<60)                  |byte1      bytes 1     total 2   
 *  NO.2 (present_time_second:int , '<B')              |     (0<=x<60)                  |byte2      bytes 1     total 3
 *  NO.3 (present_time_second_frac.4*10000:int, '<H')  |     (0<=x<=10000)              |byte3-4    bytes 2     total 5
 *  NO.4 (present_pitch.4*10000:int , '<h')            |     (abs(x)<=15708)            |byte5-6    bytes 2     total 7
 *  NO.5 (present_yaw.4*10000:int , '<h')              |     (abs(x)<=31416)            |byte7-8    bytes 2     total 9
 *  NO.678 (null_byte:b'123','<c','<c','<c')           |                                |byte9-11   bytes 3     total 12
 *  NO.9 (crc_value:int , '<I')                        |     (return of cal_crc func)   |byte12-15  bytes 4     total 16
 *  PART_CRC: byte5-8
 * 
 * 
*/
void CV_Node::save_pos_data_to_buffer(POS_DATA* pdata_to_send,uint8_t* ptx_buffer)
{
    uint16_t *pu16;
    int16_t *pi16;
    uint32_t *p32;
    

    ptx_buffer[0] = pdata_to_send->sof;
    ptx_buffer[1] = pdata_to_send->time_minute;
    ptx_buffer[2] = pdata_to_send->time_second;


    pu16 = (uint16_t*)&ptx_buffer[3];
    *pu16 = pdata_to_send->time_second_frac;

    pi16 = (int16_t*)&ptx_buffer[5];
    *pi16 = pdata_to_send->present_pitch;

    pi16 = (int16_t*)&ptx_buffer[7];
    *pi16 = pdata_to_send->present_yaw;

    ptx_buffer[9] = pdata_to_send->null_3byte[0];
    ptx_buffer[10] = pdata_to_send->null_3byte[1];
    ptx_buffer[11] = pdata_to_send->null_3byte[2];



    p32 = (uint32_t*)&ptx_buffer[0];
    pdata_to_send->crc_value=HAL_CRC_Calculate(&hcrc,p32,3);
    p32 = (uint32_t*)&ptx_buffer[12];
    *p32 = pdata_to_send->crc_value;
  
}


























