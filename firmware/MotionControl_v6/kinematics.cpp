/*
 * kinematics.cpp
 */

#include "kinematics.h"
#include "qei.h"
#include <math.h>

Kinematics::Kinematics(Pose & p, float radius_left, float radius_right, float wheel_base, float ticks)
    : PeriodicTask("kinematics", KINEMATICS_PERIOD, TIME_UNIT, KINEMATICS_JITTER), m_ticks_per_revolution(ticks), robot_pose(p)
{
    set_wheelbase(wheel_base);
    set_radius(radius_left, radius_right);
    m_linear_distance = 0;
    m_angular_distance = 0;
}


void Kinematics::set_radius_left(float radius_left)
{
    m_radius_left = radius_left;
    m_wheel_factor_left = TWO_PI * m_radius_left / m_ticks_per_revolution;
}

void Kinematics::set_radius_right(float radius_right)
{
    m_radius_right = radius_right;
    m_wheel_factor_right = TWO_PI * m_radius_right / m_ticks_per_revolution;
}

void Kinematics::run()
{
    int poscount_L = qei_position_1(); //POS1CNT;
    int poscount_R = qei_position_2(); //POS2CNT;

    int delta_tick_L = -(poscount_L - m_poscount_left); // il segno meno e' necessario perche' le due ruote sono contrapposte
    int delta_tick_R = poscount_R - m_poscount_right;

    m_poscount_left = poscount_L;
    m_poscount_right = poscount_R;

    float f_delta_tick_L = delta_tick_L * m_wheel_factor_left;
    float f_delta_tick_R = delta_tick_R * m_wheel_factor_right;

    m_speed_left = f_delta_tick_L / m_real_time_period;
    m_speed_right = f_delta_tick_R / m_real_time_period;

    m_linear_speed  = ( m_speed_left + m_speed_right ) / 2.0 ;
    // velocità del punto medio nel caso di moto lineare in mm/s

    m_angular_speed = ( m_speed_right - m_speed_left ) / m_wheelbase ;
    // velocità angolare in rad/s


    float delta_linear  = (f_delta_tick_L + f_delta_tick_R) / 2 ; //  spostamento  lineare interasse    [mm]	
    float delta_angular = (f_delta_tick_R - f_delta_tick_L) / m_wheelbase; //  variazione orientamento interasse [rad]

    m_linear_distance += delta_linear  ; //  spostamento lineare totale interasse [mm] durante ogni singolo comando
    m_angular_distance += delta_angular ; //  variazione totale orientamento interasse [rad] durante ogni singolo comando

    float arg = robot_pose.theta() + delta_angular/2.0;

    //approssimazione di secondo ordine dell'odometria
    float dx = delta_linear * cosf(arg);
    float dy = delta_linear * sinf(arg);
	
    robot_pose.x(robot_pose.x() + dx);// +  robot_pos.K_x * robot_pos.delta_angular * dy ; //compensazione della deriva con Kx
    robot_pose.y(robot_pose.y() + dy);// -  robot_pos.K_y * robot_pos.delta_angular * dx ; //compensazione della deriva con ky

    float new_theta = robot_pose.theta() + delta_angular ;

    if ( new_theta > PI ) new_theta -= TWO_PI;
    if ( new_theta < -PI) new_theta += TWO_PI;
    robot_pose.theta(new_theta);

    if (m_speed_left == m_speed_right)
        m_rotation_radius_infinity = true;
    else {
        m_rotation_radius_infinity = false;
        m_rotation_radius = (m_wheelbase / 2.0) * (m_speed_right + m_speed_left) / (m_speed_right - m_speed_left);
    }
}
