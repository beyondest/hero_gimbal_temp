#include "bsp_data_struct.h"






void copy(uint8_t* des_buffer, uint8_t* ori_buffer, uint32_t length)
{
    for (uint32_t i = 0; i < length; i++)
    {
        *(des_buffer++) = *(ori_buffer++);
    }
    
}


void update_copy(uint8_t* des_buffer, uint8_t* ori_buffer, uint32_t length)
{
    for (uint32_t i = 0; i < length; i++)
    {
        if (ori_buffer[i] != 0)
        {
            des_buffer[i] = ori_buffer[i];
        }
    }
    
}


void set_buffer(uint8_t* des_buffer, uint8_t des_num, uint32_t length)
{
    for (uint32_t i = 0; i < length; i++)
    {
       *(des_buffer++) = des_num;
    }
    
}
void set_buffer32(uint32_t* des_buffer, uint32_t des_num, uint32_t length)
{
    for (uint32_t i = 0; i < length; i++)
    {
       *(des_buffer++) = des_num;
    }
    
}
