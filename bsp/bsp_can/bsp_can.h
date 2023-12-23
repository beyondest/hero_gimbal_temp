

#ifndef BSP_CAN_H
#define BSP_CAN_H
//**************************************************Include*************************************************
#include "main.h"
#include "can.h"

//**************************************************Custom Define*****************************************************






//*****************************************************Custom TypeDef********************************************************


class Can
{
private:
//Const 
    uint32_t Mail_box;
    uint32_t Rx_fifo;



public:
    pvoid_void_func prx_callback_func = nullptr;
    pvoid_void_func ptx_callback_func = nullptr;

    CAN_HandleTypeDef* hcan;

    Can(uint32_t mail_box,
        uint32_t rx_fifo,
        CAN_HandleTypeDef* hcan);
    ~Can();

    void set_rx_callback(pvoid_void_func pfunc);
    void set_tx_callback(pvoid_void_func pfunc);

    void start();
    void stop();

    void send_data(CAN_TxHeaderTypeDef* ptx_header,uint8_t* ptx_buffer);
    void receive_data(CAN_RxHeaderTypeDef* prx_header,uint8_t* prx_buffer);


};







//***************************************************Custom Function**********************************************************
















//***********************************************************C API****************************************************************


#ifdef __cplusplus
extern "C"{
#endif


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan);
void User_Can_Init();
void can_user_init(CAN_HandleTypeDef* hcan);


#ifdef __cplusplus
}
#endif

//***************************************************Extern Value****************************************************************
extern Can Global_can1;


#endif