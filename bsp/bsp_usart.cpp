#include "bsp_usart.h"

//*************************************************Global Data defination******************************************************//


Usart usart8(&huart8,16,16,USART_DMA);
Usart usart7(&huart7,1,1,USART_DMA);

//*******************************************************Offical Function Defination*******************************************************

/**
 * @brief Occurs when DMA Complete transmit data to rx_buffer If you set HAL_Receive_DMA mode;
 * @warning Occurs when uart receive complet data and raise IT If you set HAL_Receive_IT mode
 * 
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

  if (huart == &huart8)
  {
    if (usart8.prx_callback != nullptr)
    {
      usart8.prx_callback();
    }
    usart8.enable_receive_data();
    
  }
  else if (huart == &huart7)
  {
    if (usart7.prx_callback != nullptr)
    {
      usart7.prx_callback();
    }
    usart7.enable_receive_data();
  }
  else
  {
    return;
  }

}


/**
 * @brief HAL Default tx complete call back function
 * 
 * 
*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart7)
  {
    if (usart7.ptx_callback != nullptr)
    {
      usart7.ptx_callback();
    }
    
  }
  else if (huart == &huart8)
  {
    if (usart8.ptx_callback != nullptr)
    {
      usart8.ptx_callback();
    }
  }
  else
  {
    return;
  }
  
    
}




//**********************************************Custom Function Defination***********************************************************





void Usart::send_data()
{
  switch (this->usart_mode)
  {
  case USART_DMA:
      if (HAL_UART_Transmit_DMA(this->huart,this->ptx_buffer,this->tx_length) != HAL_OK)
      {
            Error_Handler();
      }
      else
      {
        this->clear_tx_buffer();
      }
    break;
  case USART_BLOCK:
      break;
  case USART_IDLE:
      break;
  default:
    break;
  }
}

void Usart::enable_receive_data()
{
  this->clear_rx_buffer();
  switch (this->usart_mode)
  {
  case USART_DMA:
    HAL_UART_Receive_DMA(this->huart,this->prx_buffer,this->rx_length);
    break;
  case USART_IDLE:
    break;
  case USART_BLOCK:
    break;
  default:
    break;
  }
}

void Usart::clear_rx_buffer()
{
  set_buffer(this->prx_buffer,0,(uint32_t)this->rx_length);
}
void Usart::clear_tx_buffer()
{
  set_buffer(this->ptx_buffer,0,(uint32_t)this->tx_length);
}

