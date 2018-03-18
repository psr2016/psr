/*
 * relative_rotation.cpp
 */

#include "relative_rotation.h"

#include <math.h>

RelativeRotation::RelativeRotation(Kinematics & kinem, SpeedControlTask & speed_ctrl,
                                   float linear_accel, float linear_vmax, float linear_decel, float linear_vmin,
                                   float angular_target_range)
    : PositionControl(kinem, speed_ctrl),
    m_linear_accel(linear_accel), m_linear_vmax(linear_vmax), m_linear_decel(linear_decel), m_linear_vmin(linear_vmin),
    m_angular_next_speed(0), m_angular_target(0), m_target_reached(true)
{
    //conversione da gradi a radianti
    m_angular_target_range = angular_target_range*PI/180;
}

void RelativeRotation::set_rotation_target(float angular_target)
{
    //trasformazione da lineare ad angolare
    float wheelbase = m_kinematics.wheelbase();
    m_angular_accel = 2*m_linear_accel / wheelbase; //linear_accel = accelerazione singola ruota
    m_angular_decel = 2*m_linear_decel / wheelbase; //linear_decel = decelerazione singola ruota
    m_angular_vmax = 2*m_linear_vmax / wheelbase; //linear_vmax = velocità massima singola ruota

    m_angular_accel_step = m_angular_accel * m_real_time_period;
    m_angular_decel_distance = (m_angular_vmax * m_angular_vmax) / (2 * m_angular_decel);

    //conversione da gradi a radianti
    m_angular_target = angular_target*PI/180;
    m_kinematics.set_angular_distance(0);
    m_target_reached = false;
}

void RelativeRotation::run()
{
    if (m_target_reached) return;

    float distance = m_angular_target - m_kinematics.angular_distance();
    float s;

    if (distance < 0) {
        s = -1;
        distance = -distance;
    }
    else
        s = 1;

    if (distance <= m_angular_target_range) {
        m_speed_control.set_targets(0, 0);
        m_target_reached = true;
        return;
    }

    float angular_speed = m_kinematics.angular_speed();

    if (distance < m_angular_decel_distance) {
        //fase 3
        //velocità attesa della frenata
        float expected_angular_speed = sqrt(m_angular_vmax * m_angular_vmax - 2 * m_angular_decel * (m_angular_decel_distance - distance));
        if (expected_angular_speed > fabs(angular_speed)) {
            // la vel da raggiungere risulta MAGGIORE della vel corrente
            // questo vuol dire che siamo ancora nella fase di accelerazione
            // pertanto acceleriamo
            m_angular_next_speed += m_angular_accel_step;
            if (m_angular_next_speed > expected_angular_speed) m_angular_next_speed = expected_angular_speed;
            if (m_angular_next_speed > m_angular_vmax) m_angular_next_speed = m_angular_vmax;
        }
        else {
            m_angular_next_speed = expected_angular_speed;
        }
    }
    else {
        //fase 1
        if (m_angular_next_speed < m_angular_vmax) {
            m_angular_next_speed += m_angular_accel_step;
            //fase 2
            if (m_angular_next_speed > m_angular_vmax) m_angular_next_speed = m_angular_vmax;
        }
    }

    //                2*wheel_speed                                angular_speed * wheelbase
    //angular_speed = -------------        =>        wheel_speed = -------------------------
    //                  wheelbase                                              2
    float wheel_speed = m_angular_next_speed*m_kinematics.wheelbase()/2; // wheel_speed = right_speed

    //sotto m_linear_vmin la ruota non gira
    if (wheel_speed < m_linear_vmin) wheel_speed = m_linear_vmin;

    wheel_speed = s*wheel_speed;

    //verso di rotazione antiorario
    m_speed_control.set_targets(-wheel_speed, wheel_speed); //velocità ruota sinistra e ruota destra
}
