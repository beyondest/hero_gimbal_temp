#include "bsp_can.h"
#include "motor.h"

//*************************************Global Value***********************************************

Can Global_can1(
                CAN1_MAIL_BOX, 
                CAN1_FIFO,
                CAN1_RXCALLBACK_PERIOD,
                CAN1_MAX_RXCALLBACK_SUPPORT_NUMS,
                CAN1_MAX_CLIENTS_NUMS,
                CAN1_RX_DATA_BUFFER_LENGTH,
                &hcan1);


Can Global_can2(
                CAN2_MAIL_BOX, 
                CAN2_FIFO,
                CAN2_RXCALLBACK_PERIOD,
                CAN2_MAX_RXCALLBACK_SUPPORT_NUMS,
                CAN2_MAX_CLIENTS_NUMS,
                CAN2_RX_DATA_BUFFER_LENGTH,
                &hcan2);













//********************************************************Official Function*********************************************************



                                /* CAN1  */

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan == &hcan1)
    {
        Global_can1.rx_fifo_msg_pending_callback();
        Global_yaw_motor.get_feedback();

    }
}



void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan == &hcan1)
    {
        if (Global_can1.ptx_callback_func != nullptr)
        {
            Global_can1.ptx_callback_func();
        }  
    }
}

                        /* CAN2  */

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan == &hcan2)
    {
        Global_can2.rx_fifo_msg_pending_callback();
        Global_pitch_motor.get_feedback();

    }
}


void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan == &hcan2)
    {
        if (Global_can2.ptx_callback_func != nullptr)
        {
            Global_can2.ptx_callback_func();
        }  
    }
}





//*********************************************************Custom Function********************************************


Can::Can(   uint32_t mail_box,
            uint32_t rx_fifo,
            uint16_t rxcallback_period,
            uint8_t max_rxcallback_support_nums,
            uint8_t max_clients_nums,
            uint8_t rx_data_buffer_length,
            CAN_HandleTypeDef* hcan):
            Mail_box(mail_box),
            Rx_fifo(rx_fifo),
            Rxcallback_period(rxcallback_period),
            Max_rxcallback_support_nums(max_rxcallback_support_nums),
            Max_clients_nums(max_clients_nums),
            Rx_buffer_length(rx_data_buffer_length),
            hcan(hcan)
{

    this->prx_callback_func = new pvoid_void_func[this->Max_rxcallback_support_nums];
    this->prx_buffer = new uint8_t[rx_data_buffer_length];
    this->pprx_buffer = new uint8_t*[max_clients_nums];
    this->pprx_header = new CAN_RxHeaderTypeDef*[max_clients_nums];

    for (uint8_t i = 0; i < this->Max_rxcallback_support_nums; i++)
    {
        this->prx_callback_func[i] = nullptr;
    }
    for (uint8_t i = 0; i < max_clients_nums; i++)
    {
        this->pprx_buffer[i] = nullptr;
        this->pprx_header[i] = nullptr;
    }

    
}

Can::~Can()
{
    delete[] this->prx_callback_func;

    delete[] this->prx_buffer;

    delete[] this->pprx_buffer;
    delete[] this->pprx_header;

}

void Can::start()
{
    if (HAL_CAN_Start(this->hcan)!=HAL_OK)
    {
        Error_Handler();
    }
    
}

void Can::stop()
{
    if(HAL_CAN_Stop(this->hcan)!= HAL_OK)
    {
        Error_Handler();
    }
}


void Can::add_rx_callback(pvoid_void_func pfunc)
{
    if (this->rx_callback_count == this->Max_rxcallback_support_nums)
    {
        return;
    }
    else
    {
        this->prx_callback_func[this->rx_callback_count] = pfunc;
        this->rx_callback_count ++;
    }
    
}


void Can::set_tx_callback(pvoid_void_func pfunc)
{
    this->ptx_callback_func = pfunc;
}


/**
 * @brief This a public method to be called, if one client want to send msg, he just need to call this
*/
void Can::send_data(CAN_TxHeaderTypeDef* ptx_header,uint8_t* ptx_buffer)
{
    if (HAL_CAN_AddTxMessage(this->hcan,ptx_header,ptx_buffer,&this->Mail_box)!= HAL_OK)
    {
        Error_Handler();
    } 
}


/**
 * @warning Called by msg_pending callback, do not call it manually!!!
*/
void Can::receive_data_and_giveout_to_clients()
{   
    if (HAL_CAN_GetRxMessage(this->hcan,this->Rx_fifo,&this->rx_header,this->prx_buffer) != HAL_OK)
    {
        while (1)
        {
            /* code */
        }
        Error_Handler();
    }
    for (uint8_t i = 0; i < this->clients_count; i++)
    {
        if (this->rx_header.StdId == this->pprx_header[i]->StdId)
        {
            copy(this->pprx_buffer[i],this->prx_buffer,(uint32_t)this->Rx_buffer_length);
        }
    } 
}


void Can::rx_fifo_msg_pending_callback()
{
    this->receive_data_and_giveout_to_clients();

    if (this->rx_callback_period_count++ == this->Rxcallback_period)
    {
        this->rx_callback_period_count = 0;
        for (uint8_t i = 0; i < this->rx_callback_count; i++)
        {
            if (this->prx_callback_func[i] != nullptr)
            {
                this->prx_callback_func[i]();
            }
        }    
    }
}



void Can::add_client(uint8_t* prx_buffer, CAN_RxHeaderTypeDef* prx_header)
{
    if (this->clients_count == this->Max_clients_nums)
    {
        return;
    }
    else
    {
        this->pprx_buffer[this->clients_count] = prx_buffer;
        this->pprx_header[this->clients_count] = prx_header;
        this->clients_count ++;
    }
}


