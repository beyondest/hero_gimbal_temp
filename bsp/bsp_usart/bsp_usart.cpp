#include "bsp_usart.h"

//*************************************************Global Data defination******************************************************//


Usart Global_usart8(&huart8,16,16,USART_DMA);
Usart Global_usart7(&huart7,16,16,USART_DMA);

//*******************************************************Offical Function Defination*******************************************************

/**
 * @brief Occurs when DMA Complete transmit data to rx_buffer If you set HAL_Receive_DMA mode;
 * @warning Occurs when uart receive complet data and raise IT If you set HAL_Receive_IT mode
 * 
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart7)
  {
    if (Global_usart7.prx_callback != nullptr)
    {
      Global_usart7.prx_callback();
    }
    Global_usart7.receive_data();
  }
  else if (huart == &huart8)
  {
    if (Global_usart8.prx_callback != nullptr)
    {
      Global_usart8.prx_callback();
    }
    Global_usart8.receive_data();
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
    if (Global_usart7.ptx_callback != nullptr)
    {
      Global_usart7.ptx_callback();
    }
    
  }
  else if (huart == &huart8)
  {
    if (Global_usart8.ptx_callback != nullptr)
    {
      Global_usart8.ptx_callback();
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
  HAL_StatusTypeDef tmp_status;
  switch (this->usart_mode)
  {
  case USART_DMA:
      tmp_status = HAL_UART_Transmit_DMA(this->huart,this->ptx_buffer,this->tx_length);
      if (tmp_status != HAL_OK )
      {
        Error_Handler();
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


/**
 * @warning Must be called before you want to receive, once call once receive
*/
void Usart::receive_data()
{
  switch (this->usart_mode)
  {
  case USART_DMA:
    if (HAL_UART_Receive_DMA(this->huart,this->prx_buffer,this->rx_length) != HAL_OK)
    {
        Error_Handler();
    }
    
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


void Usart::start()
{
  this->clear_rx_buffer();
  this->clear_tx_buffer();
  this->receive_data();
}


/**
 * Not complete, do not use it
*/
void Usart::stop()
{
  this->clear_rx_buffer();
  this->clear_tx_buffer();
  
}
