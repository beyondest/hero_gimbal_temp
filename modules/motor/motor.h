

#ifndef MOTOR_H
#define MOTOR_H
//**************************************************Include*************************************************
#include "main.h"
#include "bsp_can.h"
//**************************************************Custom Define*****************************************************


#define MAX_TORLERATE_RADIANS_ERROR_ABS 0.5

#define YAW_MOTOR_ID 1
#define YAW_MAX_CODE_VALUE 8192
#define YAW_MAX_SETTING_VOLTAGE 30000
#define YAW_MIN_SETTING_VOLTAGE -30000

#define PITCH_MOTOR_ID 3
#define PITCH_MAX_CODE_VALUE 8192
#define PITCH_MAX_SETTING_VOLTAGE 30000
#define PITCH_MIN_SETTING_VOLTAGE -30000

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
    MOTOR_NOINIT

}MOTOR_FLAG;


class GMMotor
{
private:


//Const

    /*  Init value   */
    uint16_t Max_code_value;
    uint8_t Motor_ID ;
    uint16_t Code_zero_offset = 10000;
    Can* pcan;
    uint8_t rx_buffer_length;
    uint8_t tx_buffer_length;
    
    uint8_t* prx_buffer;
    uint8_t* ptx_buffer;

    float motor_reset_radians = 0;


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
    int16_t Max_torque_I;
    int16_t Max_temperature;


//Dynamic Value

    /* target value */
    float target_radians = 0;
    float target_rpm = 0;

    TIME_DATA target_time= {
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
    void set_motor_offset(uint16_t code_zero_offset);
    void set_motor_radians_pid(Pid* pradians_pid);
    void set_motor_rpm_pid(Pid* prmp_pid);
    void set_motor_safe_status( int16_t Max_setting_voltage,
                                int16_t Min_setting_voltage,
                                uint16_t Max_code,
                                uint16_t Min_code,
                                int16_t Max_rpm,
                                int16_t Max_torque_I,
                                int16_t Max_temperature
                               );
    void set_motor_reset_ridians(float radians);

    float trans_code_to_abspos_radians(uint16_t code);
    
    void encode();
    void decode();
    void send_data();
    void receive_data();

    void check_motor_state();

    void update_setting_voltage();
    void reset();
public:
//Const

    Pid* pradians_pid = nullptr;
    Pid* prpm_pid = nullptr;
    Pid* mypid = nullptr;

//Dynamic Value
    MOTOR_FLAG motor_flag = MOTOR_NOINIT;
    
    GMMotor(uint16_t max_code_value,
            uint8_t motor_id,
            uint8_t rx_buffer_length,
            uint8_t tx_buffer_length,
            Can* pcan)
            :Max_code_value(max_code_value),
            Motor_ID(motor_id),
            pcan(pcan),
            rx_buffer_length(rx_buffer_length),
            tx_buffer_length(tx_buffer_length)
            {

                this->prx_buffer = new uint8_t[rx_buffer_length];
                this->ptx_buffer = new uint8_t[tx_buffer_length];
                set_buffer(this->ptx_buffer,0,this->rx_buffer_length);
                set_buffer(this->prx_buffer,0,this->rx_buffer_length);

                if (this->Motor_ID > 4)
                {
                    this->tx_header.StdId = GM_CAN_SEND_EXT_ID_BASE ;

                }
                else
                {
                    this->tx_header.StdId = GM_CAN_SEND_ID_BASE;
                }

            }

    ~GMMotor()
    {
        delete[] prx_buffer;
        delete[] ptx_buffer;

    };

//Init

    void init(
            int16_t max_setting_voltage,
            int16_t min_setting_voltage,
            uint16_t max_code,
            uint16_t min_code,
            int16_t max_rpm,
            int16_t max_torque_I,
            int16_t max_temperature,
            float reset_radians,
            Pid* ppid_radians,
            Pid* ppid_rpm,
            uint16_t code_zero_offset
    );


//Main



    ACTION_STATE get_cur_radians(float* pfloat);

    //for app
    ACTION_STATE to_radians(float target_radians, 
                           uint8_t target_time_minute,
                           uint8_t target_time_second,
                           float target_time_second_frac);
    
    ACTION_STATE start();            
    ACTION_STATE shutdown();






//Error Handler
    void motor_error_handler()
    {
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