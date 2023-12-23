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
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef* hcan)
{
    uint8_t a =0;
    a = a;
}
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef* hcan)
{
    uint8_t a = 0;
    a = a;
}

void User_Can_Init()
{
  CAN_FilterTypeDef can_filter;
  can_filter.SlaveStartFilterBank = GLOBAL_CAN_FILTER_SLAVE_START;
  can_filter.FilterBank = CAN1_FILTER_INDEX;
  can_filter.FilterMode = GLOBAL_CAN_FILTER_TYPE;
  can_filter.FilterScale = GLOBAL_CAN_FILTER_SCALE;
  //init
  can_filter.FilterIdHigh = 0;
  can_filter.FilterIdLow = 0;
  can_filter.FilterMaskIdHigh = 0;
  can_filter.FilterMaskIdLow = 0;

  can_filter.FilterFIFOAssignment = CAN1_FIFO;
  can_filter.FilterActivation = ENABLE;

  can_filter.FilterIdLow = 0;//CAN1_FILTER_ID
  can_filter.FilterMaskIdLow =0;// CAN1_MASK_ID;
  
  if (HAL_CAN_ConfigFilter(&hcan1,&can_filter) != HAL_OK)
  {
      Error_Handler();
  }
  if (HAL_CAN_Start(&hcan1) != HAL_OK)
  {
      Error_Handler();
  }
  if (HAL_CAN_ActivateNotification(&hcan1,CAN1_IT_TYPE) != HAL_OK)
  {
      Error_Handler();
  }
}

void can_user_init(CAN_HandleTypeDef* hcan)
{
    CAN_FilterTypeDef can_filter;
    
    can_filter.FilterBank = 0;
    can_filter.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filter.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter.FilterIdHigh = 0;
    can_filter.FilterIdLow = 0;
    can_filter.FilterMaskIdHigh = 0;
    can_filter.FilterMaskIdLow = 0;
    can_filter.FilterFIFOAssignment = CAN_RX_FIFO0;
    can_filter.FilterActivation = ENABLE ;
    can_filter.SlaveStartFilterBank = 14;

    HAL_CAN_ConfigFilter(hcan,&can_filter);
    HAL_CAN_Start(&hcan1);
    //enable can1 rx interrupt
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
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
