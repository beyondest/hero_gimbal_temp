

#ifndef BSP_CAN_H
#define BSP_CAN_H
//**************************************************Include*************************************************
#include "main.h"
#include "can.h"


//**************************************************Custom Define*****************************************************


#define CAN1_RXCALLBACK_PERIOD 10
#define CAN1_MAX_RXCALLBACK_SUPPORT_NUMS 4
#define CAN1_RX_DATA_BUFFER_LENGTH  8
#define CAN1_MAX_CLIENTS_NUMS 8

#define CAN2_RXCALLBACK_PERIOD 10
#define CAN2_MAX_RXCALLBACK_SUPPORT_NUMS 4
#define CAN2_RX_DATA_BUFFER_LENGTH  8
#define CAN2_MAX_CLIENTS_NUMS 8

//*****************************************************Custom TypeDef********************************************************


class Can
{
private:
//Const 
    uint32_t Mail_box;
    uint32_t Rx_fifo;
    uint16_t Rxcallback_period;
    uint8_t  Max_rxcallback_support_nums;
    uint8_t Max_clients_nums;
    uint8_t Rx_buffer_length;
    uint8_t Tx_buffer_length;

    pvoid_void_func* prx_callback_func;

    CAN_RxHeaderTypeDef rx_header = {0};
    CAN_RxHeaderTypeDef** pprx_header;

    uint8_t* prx_buffer;
    uint8_t** pprx_buffer;

    
    

//Dynamic
    uint8_t rx_callback_count = 0;
    uint16_t rx_callback_period_count = 0;
    uint8_t clients_count = 0;
    

public:
    pvoid_void_func ptx_callback_func = nullptr;
    CAN_HandleTypeDef* hcan;


    Can(uint32_t mail_box,
        uint32_t rx_fifo,
        uint16_t rxcallback_period,
        uint8_t max_rxcallback_support_nums,
        uint8_t max_clients_nums,
        uint8_t rx_data_buffer_length,
        CAN_HandleTypeDef* hcan);
    ~Can();

    void add_rx_callback(pvoid_void_func pfunc);
    void set_tx_callback(pvoid_void_func pfunc);

    void start();
    void stop();

    void send_data(CAN_TxHeaderTypeDef* ptx_header,uint8_t* ptx_buffer);
    void receive_data_and_giveout_to_clients();

    void rx_fifo_msg_pending_callback();

    void add_client(uint8_t* prx_buffer, CAN_RxHeaderTypeDef* prx_header);

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
extern Can Global_can2;

#endif