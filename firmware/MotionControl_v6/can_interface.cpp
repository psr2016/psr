/*
 * can_interface.cpp
 */

#include "defines.h"
#include "can_interface.h"
#include "time_defines.h"
#include "canstdio_endpoint.h"
#include "path_control.h"
#include "absolute_rotation.h"
#include "gpio.h"

extern CanPoseSender  can_pose_sender;
extern CanSpeedSender  can_speed_sender;

CanCommandReceiver * CanCommandReceiver::instance = 0;

CanCommandReceiver::CanCommandReceiver(Kinematics & kinematics, SpeedControlTask & speed)
    : m_kinematics(kinematics), m_speed_controller(speed)
{
    instance = this;
}

void CanCommandReceiver::run()
{
    ecan_update_object(MOTION_COMMAND_OBJECT);
    //ecan_update_object(OBSTACLE_AVOIDANCE_OBJECT);
}

#define M_E(m,e)   (float)m * pow(10,e)

void CanCommandReceiver::process_command(const t_can_motion_command* m)
{
    switch (m->_cmd) {

    case MOTION_COMMAND_SET_WHEEL_RADIUS_LEFT:
        {
            t_command_set_wheel_radius * p = (t_command_set_wheel_radius *)m;
            m_kinematics.set_radius_left(p->wheel_radius);
        }
        break;

    case MOTION_COMMAND_SET_WHEEL_RADIUS_RIGHT:
        {
            t_command_set_wheel_radius * p = (t_command_set_wheel_radius *)m;
            m_kinematics.set_radius_right(p->wheel_radius);
        }
        break;

    case MOTION_COMMAND_SET_WHEEL_DISTANCE:
        {
            t_command_set_wheel_distance * p = (t_command_set_wheel_distance *)m;
            m_kinematics.set_wheelbase(p->wheel_distance);
        }
        break;

    case MOTION_COMMAND_SPEED_PID:
        {
            float kp, ki, kd;
            t_command_speed_pid * p = (t_command_speed_pid *)m;
				
            kp = M_E(p->kp_m, p->kp_e);
            ki = M_E(p->ki_m, p->ki_e);
            kd = M_E(p->kd_m, p->kd_e);

            m_speed_controller.set_params(kp, ki, kd);
            m_speed_controller.off();
        }
        break;

    case MOTION_COMMAND_SET_PMW:
        {
            t_command_set_pwm_speed * p = (t_command_set_pwm_speed *)m;

            //robot_pos.position_control_state = CONTROL_OFF;

            m_speed_controller.set_motors(p->left, p->right);
        }
        break;

    case MOTION_COMMAND_SET_SPEED:
        // if (obstacle_detected == false)
        {
            t_command_set_pwm_speed * p = (t_command_set_pwm_speed *)m;

            m_speed_controller.set_targets(p->left, p->right);

            // robot_pos.motor_locked = 0;
            // robot_pos.motor_locked_count = 0;

            // robot_pos.position_control_state = CONTROL_OFF;
        }
        break;

    case MOTION_COMMAND_STOP_AND_FREE:
        m_speed_controller.off();
        //path_control.abort_and_getNext();
        // if (obstacle_detected == false)
        // {
        //     robot_pos.position_control_state = CONTROL_OFF;

        //     reset_velocity_controllers();

        //     flagsR.pid_on   = 0;
        //     flagsR.motor_en = 0;
        //     flagsR.frenata  = 0;

        //     flagsL.pid_on   = 0;
        //     flagsL.motor_en = 0;
        //     flagsL.frenata  = 0;

        //     path.elements = 0;
        //     path.current = 0;
        //     path.stopped = 1;
        //     robot_pos.path_done = 1;

        //     set_pwm(0, 0);

        //     //clear_smoothing_filter(&filter_left);
        //     //clear_smoothing_filter(&filter_right);
        // }
        break;

    case MOTION_COMMAND_SET_TELEMETRY_MODE:
        {
            t_command_set_telemetry_mode * p = (t_command_set_telemetry_mode *)m;

            if (p->speed_telemetry)
                can_speed_sender.on();
            else
                can_speed_sender.off();

            //if (p->odometry_mode) .... // FIXME. TO BE IMPLEMENTED
        }
        break;

    case MOTION_COMMAND_SET_CURRENT_POSITION:
        {
            t_command_set_current_position * p = (t_command_set_current_position *)m;
            m_kinematics.pose().x(p->x);
            m_kinematics.pose().y(p->y);
            m_kinematics.pose().theta(TO_RADIANS(p->deg100 / 100.0));
        }
        break;

    case MOTION_COMMAND_ROTATE_RELATIVE:
        //if (obstacle_detected == false)
        {
            t_command_rotate * p =  (t_command_rotate *)m;
            path_control.addRelRotation(p->degrees);
        }
        break;

    case MOTION_COMMAND_ROTATE_ABSOLUTE:
        //if (obstacle_detected == false)
        {
            t_command_rotate * p =  (t_command_rotate *)m;
            path_control.addAbsRotation(p->degrees);
        }
        break;

    case MOTION_COMMAND_ROTATE_CIRCULAR:
        //if (obstacle_detected == false)
        {
            t_command_rotate_circular * p =  (t_command_rotate_circular *)m;
            path_control.addCircularRotation(p->degrees/10.0, p->x);
        }
        break;
    case MOTION_COMMAND_LINE_TO_POINT:
        {
            t_command_line_to_point * p =  (t_command_line_to_point *)m;
            path_control.addFollowline(p->x, p->y);
        }
        break;
    case MOTION_COMMAND_FORWARD_TO_POINT:
        {
            t_command_forward_to_point * p =  (t_command_forward_to_point *)m;
            path_control.addGoTo_Point(p->x, p->y);
        }
        break;

    }
}

// ----------------------------------------------------------------------------------------------------

CanPoseSender::CanPoseSender(Pose & robot_pose)
    : PeriodicTask("pose_sender", POSE_SENDER_PERIOD, TIME_UNIT, POSE_SENDER_JITTER),
      m_robot_pose(robot_pose)
{
}

void CanPoseSender::run()
{
    t_can_robot_position pos;
    pos.x = m_robot_pose.x();
    pos.y = m_robot_pose.y();
    pos.deg100 = TO_DEGREES(m_robot_pose.theta()) * 100;
    switch (path_control.get_status()) {
    }
    pos.flags = 0;//robot_pos.path_done | (robot_pos.motor_locked << 1) | (robot_pos.position_valid << 2);
    pos.bumpers = (!bumper_1) | ((!bumper_2) << 1);
    ecan_send(ROBOT_POSITION_CAN_ID, (unsigned char *)&pos, 8, 0);
}


// ----------------------------------------------------------------------------------------------------

CanSpeedSender::CanSpeedSender(SpeedControlTask & speed)
    : PeriodicTask("speed_sender", SPEED_SENDER_PERIOD, TIME_UNIT, SPEED_SENDER_JITTER),
      m_speed_controller(speed)
{
}

void CanSpeedSender::run()
{
    t_can_robot_wheels_velocity vel;
    float l, r;
    m_speed_controller.get_current_speeds(l, r);
    vel.left_speed = (short)l;
    vel.right_speed = (short)r;
    //vel.left_pwm = (short)m_speed_controller.pwm_left();
    //vel.right_pwm = (short)m_speed_controller.pwm_right();
    vel.left_pwm = (short)m_speed_controller.get_target_left();
    vel.right_pwm = (short)m_speed_controller.get_target_right();
    ecan_send(ROBOT_WHEELS_VELOCITY_CAN_ID, (unsigned char *)&vel, 8, 0);
}


// ----------------------------------------------------------------------------------------------------

CanGyroSender::CanGyroSender(Controllore & ctrl)
    : PeriodicTask("gyro_sender", GYRO_SENDER_PERIOD, TIME_UNIT, GYRO_SENDER_JITTER),
      m_gyro_controller(ctrl)
{
}

void CanGyroSender::run()
{
    t_can_gyro_raw_data g;
    short x, y, z;
    m_gyro_controller.ReadRawData(x, y, z);
    g.x = x;
    g.y = y;
    g.z = z;
    ecan_send(GYRO_RAW_DATA_CAN_ID, (unsigned char *)&g, 8, 0);
}


// --------------------------------------------------------------------------------

static void update_motion_command(const uint8_t *data, unsigned int len, void *user_ptr)
{
    const t_can_motion_command * m = (const t_can_motion_command*)data;

    CanCommandReceiver::instance->process_command(m);
}


void init_can_interface(void)
{

#ifdef BOARD_VERSION_2
    // RX is RP6, TX is RP7 in open_drain configuration
    // (CAN_H == A, CAN_L == B)
    ecan_initialize(6, 7, true, true);
#else
    // RX is RP10, TX is RP11 in open_drain configuration
    // (CAN_H == A, CAN_L == B)
    ecan_initialize(10, 11, true, true);
#endif

    ecan_set_rx_object(MOTION_COMMAND_OBJECT, MOTION_COMMAND_CAN_ID, update_motion_command, NULL, 0);
    // ecan_set_rx_object(OBSTACLE_AVOIDANCE_OBJECT, OBSTACLE_AVOIDANCE_CAN_ID, update_obstacle_avoidance, NULL, 0);
    ecan_set_rx_object(REMOTE_STDIO_RX_OBJECT, REMOTE_STDIO_CAN_ID(CAN_STDIO_AND_RTSP_NODE_ID),
                       canstdio_endpoint_process_can_frame, NULL, ECAN_RX_FLAG_ASYNC);
}
