

#ifndef MOTOR_H
#define MOTOR_H
//**************************************************Include*************************************************
#include "main.h"
#include "bsp_can.h"
#include "bsp_data_struct.h"
#include "mymath.h"
#include "logger.h"
//**************************************************Custom Define*****************************************************


/* Yaw Init */
#define YAW_MOTOR_ID 1
#define YAW_CODE_ZERO_OFFSET 4096

#define YAW_MAX_CODE_VALUE 8192
#define YAW_MIN_CODE_VALUE 0
#define YAW_MAX_SETTING_VOLTAGE 30000
#define YAW_MIN_SETTING_VOLTAGE -30000
#define YAW_MAX_RPM 100
#define YAW_MIN_RPM -100
#define YAW_MAX_TORQUE_I 1000
#define YAW_MAX_TEMPERATURE 50

/*Pitch Init*/
#define PITCH_MOTOR_ID 3
#define PITCH_CODE_ZERO_OFFSET 4096

#define PITCH_MAX_CODE_VALUE 8192
#define PITCH_MIN_CODE_VALUE 0
#define PITCH_MAX_SETTING_VOLTAGE 30000
#define PITCH_MIN_SETTING_VOLTAGE -30000
#define PITCH_MAX_RPM 100
#define PITCH_MIN_RPM -100
#define PITCH_MAX_TORQUE_I 1000
#define PITCH_MAX_TEMPERATURE 50


/* Total GM Init */

#define MAX_TORLERATE_RADIANS_ERROR_ABS 0.1
#define GM_MOTOR_SHUTDOWN_VOLTAGE 0


#define GM_TX_BUFFER_LENGTH 8
#define GM_RX_BUFFER_LENGTH 8

#define GM_CAN_SEND_ID_BASE 0x1ff
#define GM_CAN_SEND_EXT_ID_BASE 0x2ff
#define GM_CAN_FEEDBACK_ID_BASE 0x204
#define GM_CAN_FRAME_RTR CAN_RTR_DATA
#define GM_CAN_FRAME_IDE CAN_ID_STD
#define GM_CAN_FRAME_DLC 8





//*****************************************************Custom TypeDef********************************************************

/**
 * when MOTOR_PREPARING , radians control cmd will fail, but then it turns into run automatically(do not change motor flag on your own)
*/
typedef enum:uint8_t
{
    MOTOR_RUNNING,
    MOTOR_SHUTDOWN,
    MOTOR_ERROR,
    MOTOR_PREPARING,

}MOTOR_FLAG;


typedef enum:uint8_t
{
    VOLTAGE_DEBUG,
    RPM_DEBUG,
    NO_DEBUG
}MOTOR_DEBUG_FLAG;


class GMMotor
{
private:


//Const

    /*  Init value   */
    uint8_t Motor_ID ;
    uint16_t Code_zero_offset;
    Can* pcan;
    uint8_t rx_buffer_length;
    uint8_t tx_buffer_length;
    
    uint8_t* prx_buffer;
    uint8_t* ptx_buffer;



    CAN_TxHeaderTypeDef tx_header = {
        .StdId = 0,
        .IDE = GM_CAN_FRAME_IDE,
        .RTR = GM_CAN_FRAME_RTR,
        .DLC = GM_CAN_FRAME_DLC
        
    };
    
    CAN_RxHeaderTypeDef rx_header = {
        .StdId = 0,
        .IDE = GM_CAN_FRAME_IDE,
        .RTR = GM_CAN_FRAME_RTR,
        .DLC = GM_CAN_FRAME_DLC
    };

    /*   safe status  */

    int16_t Max_setting_voltage;
    int16_t Min_setting_voltage;
    uint16_t Max_code;
    uint16_t Min_code;
    int16_t Max_rpm;
    int16_t Min_rpm;
    int16_t Max_torque_I;
    uint8_t Max_temperature;

    float motor_reset_radians = 0;

//Dynamic Value

    /* target value */
    float target_radians = 0;
    int16_t target_rpm = 0;

    TIME_DATA target_time = {
        .minute = 0,
        .second = 0,
        .second_frac_4 = 0
    };


    /* cur value */

    float cur_radians = 0;
    uint16_t cur_code = 0;
    int16_t cur_rpm = 0;
    int16_t cur_torque_I = 0;
    uint8_t cur_temperature = 0;
    
    TIME_DATA cur_time = {
        .minute = 0,
        .second = 0,
        .second_frac_4 = 0
    };

    /* setting value */
    int16_t cur_setting_voltage = 0;


//private method

    /*      Communicate     */
    float trans_code_to_abspos_radians(uint16_t code);
    void encode();
    void decode();
    void send_data();
    void receive_data();

    /*      Inner method        */
    void check_motor_state();
    void update_setting_voltage();
    void reset();

public:
//Const

    Pid* pradians_pid = nullptr;
    Pid* prpm_pid = nullptr;
    Pid* mypid = nullptr;

//Dynamic Value
    MOTOR_FLAG motor_flag = MOTOR_SHUTDOWN;
    MOTOR_DEBUG_FLAG debug_flag = NO_DEBUG;

    GMMotor(
            uint8_t motor_id,
            uint16_t code_zero_offset,
            uint8_t rx_buffer_length,
            uint8_t tx_buffer_length,
            Can* pcan,
            int16_t max_setting_voltage,
            int16_t min_setting_voltage,
            uint16_t max_code,
            uint16_t min_code,
            int16_t max_rpm,
            int16_t min_rpm,
            int16_t max_torue_I,
            uint8_t max_temperature,
            Pid* pradians_pid,
            Pid* prpm_pid
            );

    ~GMMotor();

//For Gimbal node app



    ACTION_STATE get_cur_radians(float* pcur_radians);
    ACTION_STATE to_radians(float target_radians, 
                           uint8_t target_time_minute,
                           uint8_t target_time_second,
                           float target_time_second_frac);
    
    ACTION_STATE start();            
    ACTION_STATE shutdown();

    ACTION_STATE to_voltage(int16_t no_clamp_voltage);
    ACTION_STATE to_rpm(int16_t rpm);


//Callback Function


    void get_feedback();
    void control();



//Error Handler

    void motor_error_handler()
    {
        this->cur_setting_voltage = GM_MOTOR_SHUTDOWN_VOLTAGE;
        while (1)
        {
           /*if ("Problem solved")
            {
                this->motor_flag = MOTOR_PREPARING;
            }*/ 
        }
        
    }
};







//***************************************************Custom Function**********************************************************
















//***********************************************************C API****************************************************************


#ifdef __cplusplus
extern "C"{
#endif

#ifdef __cplusplus
}
#endif

//***************************************************Extern Value****************************************************************
extern GMMotor Global_pitch_motor;
extern GMMotor Global_yaw_motor;


#endif