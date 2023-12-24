#include "bsp_can.h"
#include "motor.h"

//*************************************Global Value***********************************************

Can Global_can1(
                CAN1_MAIL_BOX, 
                CAN1_FIFO,
                &hcan1);














//********************************************************Official Function*********************************************************





void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan == &hcan1)
    {
        if (Global_can1.prx_callback_func != nullptr)
        {
            Global_can1.prx_callback_func();
        }
        Global_pitch_motor.get_feedback();
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






//*********************************************************Custom Function********************************************


Can::Can(   uint32_t mail_box,
            uint32_t rx_fifo,
            CAN_HandleTypeDef* hcan):
            Mail_box(mail_box),
            Rx_fifo(rx_fifo),
            hcan(hcan)
{
  
}

Can::~Can()
{

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


void Can::set_rx_callback(pvoid_void_func pfunc)
{
    this->prx_callback_func = pfunc;
}

void Can::set_tx_callback(pvoid_void_func pfunc)
{
    this->ptx_callback_func = pfunc;
}


void Can::send_data(CAN_TxHeaderTypeDef* ptx_header,uint8_t* ptx_buffer)
{

    if (HAL_CAN_AddTxMessage(this->hcan,ptx_header,ptx_buffer,&this->Mail_box)!= HAL_OK)
    {
        Error_Handler();
    } 
}

/**
 * @warning GPT said that this function may block until pendingmsg_callback is called if there is no new msg in FIFO!!!
*/
void Can::receive_data(CAN_RxHeaderTypeDef* prx_header,uint8_t* prx_buffer)
{
    if (HAL_CAN_GetRxMessage(this->hcan,this->Rx_fifo,prx_header,prx_buffer) != HAL_OK)
    {
        Error_Handler();
    }
}
