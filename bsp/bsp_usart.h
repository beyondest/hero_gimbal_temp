




#ifndef BSP_USART_H
#define BSP_USART_H


//********************************************Include********************************************************************

#include "main.h"
#include "usart.h"
#include "crc.h"
#include "bsp_data_struct.h"
//*********************************************Global Value****************************************************
#define MAX_UART_INDEX 8



extern Usart usart8;
extern Usart usart7;

//********************************************Custom typedef********************************************************
typedef enum:uint8_t
{
    USART_DMA,
    USART_BLOCK,
    USART_IDLE
}USART_MODE;



class Usart
{
private:

    USART_MODE usart_mode;



public:
    void (*prx_callback)(void) = nullptr;
    void (*ptx_callback)(void) = nullptr;
    uint8_t* prx_buffer;
    uint8_t* ptx_buffer;
    uint8_t rx_length;
    uint8_t tx_length;
    UART_HandleTypeDef* huart;


    Usart(UART_HandleTypeDef* huart,
          uint8_t rx_length,
          uint8_t tx_length,
          USART_MODE usart_mode):
          usart_mode(usart_mode),
          rx_length(rx_length),
          tx_length(tx_length),
          huart(huart)
          {
                prx_buffer = new uint8_t[rx_length];
                ptx_buffer = new uint8_t[tx_length];
          };
    ~Usart()
    {
        delete[] prx_buffer;
        delete[] ptx_buffer;
    };
    void set_rx_callback(void (*prx_callbackf)(void))
    {
        this->prx_callback = prx_callbackf;
    };
    void set_tx_callback(void (*ptx_callbackf)(void))
    {
        this->ptx_callback = ptx_callbackf;
    }

    void send_data();
    void enable_receive_data();
    void clear_rx_buffer();
    void clear_tx_buffer();
};


//*********************************************Offical Function************************************************

//**********************************************Custom Function**************************************************


//***********************************************************C API****************************************************************


#ifdef __cplusplus
extern "C"{
#endif

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);



#ifdef __cplusplus
}
#endif


























//*************************************************************C API*********************************************

#ifdef __cplusplus
extern "C"{
#endif

#ifdef __cplusplus
}
#endif



#endif
