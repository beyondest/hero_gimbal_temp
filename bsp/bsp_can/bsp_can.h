

#ifndef BSP_CAN_H
#define BSP_CAN_H
//**************************************************Include*************************************************
#include "main.h"
#include "can.h"

//**************************************************Custom Define*****************************************************

#define GLOBAL_CAN_FILTER_TYPE CAN_FILTERMODE_IDMASK
#define GLOBAL_CAN_FILTER_SLAVE_START 14
#define GLOBAL_CAN_FILTER_SCALE CAN_FILTERSCALE_16BIT

//this is GM possible ID    1    2     3     4     5     6     7
#define CAN1_ID_YOU_WANT "0x205 0x206 0x207 0x208 0x209 0x20a 0x20b"
#define CAN1_MASK_ID   0x7f0
#define CAN1_FILTER_ID  0x200
#define CAN1_IT_TYPE CAN_IT_RX_FIFO0_MSG_PENDING
#define CAN1_MAIL_BOX CAN_TX_MAILBOX0
#define CAN1_FIFO CAN_RX_FIFO0
#define CAN1_FILTER_INDEX 0






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

    Can(CAN_HandleTypeDef* hcan,
        uint32_t mail_box,
        uint32_t rx_fifo,
        uint32_t filter_id,
        uint32_t mask_id,
        uint32_t rx_it_type,
        uint8_t filter_index);
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



#ifdef __cplusplus
}
#endif

//***************************************************Extern Value****************************************************************
extern Can Global_can1;


#endif