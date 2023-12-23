#include "logger.h"


void LOG_INFO(const char* loginfo)
{
    uint8_t log_length = 0;
    for ( ;log_length < MAX_LOG_LENGTH; log_length++)
    {
        if (loginfo[log_length] == '\n')
        {
            break;
        }    
    }
}