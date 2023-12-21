#include "mymath.h"

/**
 * @brief Use to check crc value of uint8 array 
 * @warning Use default HAL_CRC_Calculate, do not use rev in or out;
 * @warning length_4multi must be multiple of 4, and include CRC bytes 4
 * @warning sender's CRC value is in the last 4 bytes
 * @retval crc_right =1;crc_wrong =0
*/
CRC_STATE check_data4_crc32(uint8_t *pbuffer,uint8_t length_4multi)
{
    uint32_t *p32;
    uint32_t crc_cal = 0;
    
    p32 = (uint32_t*)&pbuffer[0];
    crc_cal = HAL_CRC_Calculate(&hcrc,p32,(uint32_t)(length_4multi/4-1));

    p32 = (uint32_t*)&pbuffer[12];
    if (*p32 == crc_cal)
    {
      return CRC_RIGHT;
    }
    else
    {
      return CRC_WRONG;
    }
}



/**
 * @brief Use to trans float frac_4 to uint16 by multiply 10000
*/
uint16_t trans_frac_uint16(float frac_4)
{
    float tmp = frac_4*10000;
    return (uint16_t)tmp;
}

/**
 * @brief Use to trans float frac_4 to int16 by multiply 10000
*/
int16_t trans_frac_int16(float frac_4)
{
    float tmp = frac_4*10000;
    return (int16_t)tmp;
}


