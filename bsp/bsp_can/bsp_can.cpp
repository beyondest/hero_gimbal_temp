#include "bsp_can.h"


//*************************************Global Value***********************************************

Can Global_can1(
                CAN1_MAIL_BOX,
                
                CAN1_FIFO,
                &hcan1,
                CAN1_FILTER_ID,
                CAN1_MASK_ID,
                CAN1_IT_TYPE,
                CAN1_FILTER_INDEX);














//********************************************************Official Function*********************************************************





void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan == &hcan1)
    {
        if (Global_can1.prx_callback_func != nullptr)
        {
            Global_can1.prx_callback_func();
        }
        
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
            CAN_HandleTypeDef* hcan,
            uint32_t filter_id,
            uint32_t mask_id,
            uint32_t rx_it_type,
            uint8_t filter_index):
            Mail_box(mail_box),
            Rx_fifo(rx_fifo),
            hcan(hcan)
{
    CAN_FilterTypeDef can_filter;
    can_filter.SlaveStartFilterBank = GLOBAL_CAN_FILTER_SLAVE_START;
    can_filter.FilterBank = filter_index;
    can_filter.FilterMode = GLOBAL_CAN_FILTER_TYPE;
    can_filter.FilterScale = GLOBAL_CAN_FILTER_SCALE;
    //init
    can_filter.FilterIdHigh = 0x0000;
    can_filter.FilterIdLow = 0x0000;
    can_filter.FilterMaskIdHigh = 0x0000;
    can_filter.FilterMaskIdLow = 0x0000;

    can_filter.FilterFIFOAssignment = this->Rx_fifo;
    can_filter.FilterActivation = ENABLE;

    can_filter.FilterIdLow = filter_id;
    can_filter.FilterMaskIdLow = mask_id;
    
    if (HAL_CAN_ConfigFilter(this->hcan,&can_filter) != HAL_OK)
    {
        Error_Handler();
    }
    
    if (HAL_CAN_ActivateNotification(&hcan1,rx_it_type) != HAL_OK)
    {
        Error_Handler();
    }
     
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
