#include "gimbal_task.h"



//*****************************************************Global Value************************************************************


Gimbal_Node Global_gimbal_node(&Global_msg_center,
                                GIMBAL_MAX_TOPIC_LENGTH,
                                &Global_pitch_motor,
                                &Global_yaw_motor);


void gimbal_run()
{

    Global_gimbal_node.run();
}

void gimbal_update_second()
{
    Global_gimbal_node.update_time_second();
}
void gimbal_debug_run()
{
    Global_gimbal_node.debug_rpm_run();
}

//*******************************************************Init*******************************************

void Gimbal_Node::init_local_data()
{

    this->plocal_data.cur_time.minute = 0;
    this->plocal_data.cur_time.second = 0;
    this->plocal_data.cur_time.second_frac_4= 0;


    this->plocal_data.target_time.minute = 0;
    this->plocal_data.target_time.second = 0;
    this->plocal_data.target_time.second_frac_4 = 0;


    this->plocal_data.pitch_motor_data.cur_angle_radians =0.2618;
    this->plocal_data.pitch_motor_data.target_angle_radians = 0;

    this->plocal_data.yaw_motor_data.cur_angle_radians = -0.2618;
    this->plocal_data.yaw_motor_data.target_angle_radians = 0;

    this->plocal_data.setting_voltage_or_rpm = 10;

}






//**********************************************Gimbal_Node Public Methods*******************************************************

/**
 * For tim
*/
void Gimbal_Node::run()
{

    this->subscribe_gimbal_control();

    this->publish_gimbal_pos();
    this->publish_present_time();
    //syn must after publish present time
   // this->subscribe_syn_time();

    this->gimbal_pitch_control();
    this->gimbal_yaw_control(); 

}

void Gimbal_Node::debug_rpm_run()
{
    this->subscribe_gimbal_control();
    this->ppitch_motor->debug_flag = RPM_DEBUG;
    
    
    if (this->ppitch_motor->to_rpm(this->plocal_data.setting_voltage_or_rpm) != ACTION_SUCESS)
    {
        this->gimbal_control_action_wrong_error_handler();
    }
    if (this->ppitch_motor->get_cur_radians(&this->plocal_data.pitch_motor_data.cur_angle_radians) != ACTION_SUCESS)
    {
        this->gimbal_control_action_wrong_error_handler();
    }
    if (this->ppitch_motor->get_cur_rpm(&this->plocal_data.present_debug_value) != ACTION_SUCESS)
    {
        this->gimbal_control_action_wrong_error_handler();
    }
    
    this->publish_gimbal_pos();
    this->publish_present_time();
    
}

void Gimbal_Node::update_time_second()
{
    this->plocal_data.cur_time.second++;
    this->plocal_data.cur_time.second = RCLAMP(this->plocal_data.cur_time.second,0,59);

}



//************************************************Gimbal_Node Private Methods**************************************

void Gimbal_Node::subscribe_gimbal_control()
{
         
    if ((this->feedback_gimbal_control = this->receive_msg_from_center(&Global_Topic_gimbal_control)) == SEND_SUCCESS)
    {
        Global_Topic_gimbal_control.decode(this->prx_buffer,
                                           this->plocal_data.pitch_motor_data.target_angle_radians,
                                           this->plocal_data.yaw_motor_data.target_angle_radians,
                                           this->plocal_data.target_time.minute,
                                           this->plocal_data.target_time.second,
                                           this->plocal_data.target_time.second_frac_4,
                                           this->plocal_data.setting_voltage_or_rpm
                                           );
    }
    else
    {
        this->receive_from_center_error_handler();

    }
}

void Gimbal_Node::subscribe_syn_time()
{

    if ((this->feedback_syn_time = this->receive_msg_from_center(&Global_Topic_Syn_time)) == SEND_SUCCESS)
    {
        Global_Topic_Syn_time.decode(    this->prx_buffer,
                                         this->plocal_data.cur_time.minute,
                                         this->plocal_data.cur_time.second,
                                         this->plocal_data.cur_time.second_frac_4
                                         );
    }
    else
    {
        this->receive_from_center_error_handler();
    }

}

void Gimbal_Node::publish_gimbal_pos()
{
    Global_Topic_gimbal_pos.encode(this->ptx_buffer,
                                   this->plocal_data.pitch_motor_data.cur_angle_radians,
                                   this->plocal_data.yaw_motor_data.cur_angle_radians,
                                   this->plocal_data.present_debug_value
                                   );

    if ((this->feedback_gimbal_pos = this->send_msg_to_center(&Global_Topic_gimbal_pos)) != SEND_SUCCESS)
    {
        this->send_to_center_error_handler();
    }
}

void Gimbal_Node::publish_present_time()
{
    Global_Topic_Present_time.encode(this->ptx_buffer,
                                     this->plocal_data.cur_time.minute,
                                     this->plocal_data.cur_time.second,
                                     this->plocal_data.cur_time.second_frac_4);
    if ((this->feedback_present_time = this->send_msg_to_center(&Global_Topic_Present_time)) != SEND_SUCCESS)
    {
        this->send_to_center_error_handler();
    }
    
}





void Gimbal_Node::gimbal_pitch_control()
{
    this->ppitch_motor->debug_flag = NO_DEBUG;
    if (this->ppitch_motor->to_radians( this->plocal_data.pitch_motor_data.target_angle_radians,
                                        this->plocal_data.target_time.minute,
                                        this->plocal_data.target_time.second,
                                        this->plocal_data.target_time.second_frac_4) != ACTION_SUCESS)
    {
        this->gimbal_control_action_wrong_error_handler();
    }
    if (this->ppitch_motor->get_cur_radians(&this->plocal_data.pitch_motor_data.cur_angle_radians) != ACTION_SUCESS)
    {
        this->gimbal_control_action_wrong_error_handler();
    }
  
}


void Gimbal_Node::gimbal_yaw_control()
{
    this->pyaw_motor->debug_flag = NO_DEBUG;
    if (this->pyaw_motor->to_radians(   this->plocal_data.yaw_motor_data.target_angle_radians,
                                        this->plocal_data.target_time.minute,
                                        this->plocal_data.target_time.second,
                                        this->plocal_data.target_time.second_frac_4) != ACTION_SUCESS)
    {
        this->gimbal_control_action_wrong_error_handler();
    }
    if (this->pyaw_motor->get_cur_radians(&this->plocal_data.yaw_motor_data.cur_angle_radians) != ACTION_SUCESS)
    {
        this->gimbal_control_action_wrong_error_handler();
    }
    if (this->pyaw_motor->get_cur_code(&this->plocal_data.present_debug_value) != ACTION_SUCESS)
    {
        this->gimbal_control_action_wrong_error_handler();
    } 
}


