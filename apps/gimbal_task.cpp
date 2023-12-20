#include "gimbal_task.h"






Gimbal_Node Global_gimbal_node(&Global_msg_center,GIMBAL_MAX_TOPIC_LENGTH);












//**********************************************Gimbal task*******************************************************

/**
 * For tim
*/
void Gimbal_Node::run()
{

    this->subscribe_gimbal_control();
    this->subscribe_syn_time();
    this->publish_gimbal_pos();


    this->gimbal_pitch_control();
    this->gimbal_yaw_control();

    


}

void Gimbal_Node::subscribe_gimbal_control()
{
         
    if ((this->feedback_gimbal_control = this->receive_msg_from_center(&Global_Topic_gimbal_control)) == SEND_SUCCESS)
    {
        Global_Topic_gimbal_control.decode(this->prx_buffer,
                                           this->plocal_data->pitch_motor_data.relative_angle_radians,
                                           this->plocal_data->yaw_motor_data.relative_angle_radians,
                                           this->plocal_data->target_time.minute,
                                           this->plocal_data->target_time.second,
                                           this->plocal_data->target_time.second_frac_4
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
        Global_Topic_Syn_time.decode(this->prx_buffer,
                                         this->plocal_data->cur_time.minute,
                                         this->plocal_data->cur_time.second,
                                         this->plocal_data->cur_time.second_frac_4
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
                                   this->plocal_data->pitch_motor_data.cur_angle_radians,
                                   this->plocal_data->yaw_motor_data.cur_angle_radians);

    if ((this->feedback_gimbal_pos = this->send_msg_to_center(&Global_Topic_gimbal_pos)) != SEND_SUCCESS)
    {
        this->send_to_center_error_handler();
    }
}

void Gimbal_Node::publish_present_time()
{
    Global_Topic_Present_time.encode(this->ptx_buffer,
                                     this->plocal_data->cur_time.minute,
                                     this->plocal_data->cur_time.second,
                                     this->plocal_data->cur_time.second_frac_4);
    if ((this->feedback_present_time = this->send_msg_to_center(&Global_Topic_Present_time)) != SEND_SUCCESS)
    {
        this->send_to_center_error_handler();
    }
    

}


void Gimbal_Node::init_local_data()
{

    this->plocal_data->cur_time.minute = 0;
    this->plocal_data->cur_time.second = 0;
    this->plocal_data->cur_time.second_frac_4= 0;


    this->plocal_data->target_time.minute = 0;
    this->plocal_data->target_time.second = 0;
    this->plocal_data->target_time.second_frac_4 = 0;


    this->plocal_data->pitch_motor_data.cur_angle_radians =0;
    this->plocal_data->pitch_motor_data.relative_angle_radians = 0;

    this->plocal_data->yaw_motor_data.cur_angle_radians = 0;
    this->plocal_data->yaw_motor_data.relative_angle_radians = 0;

}



void Gimbal_Node::gimbal_pitch_control()
{

}

void Gimbal_Node::gimbal_yaw_control()
{

}

