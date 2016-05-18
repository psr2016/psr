/*
 * followLine.cpp
 */

#include "followLine.h"
#include <math.h>

bool FollowLine::target_reached() 
{
    return false;//TODO
}
  
void FollowLine::run()
{
    //TODO Controllare tutti i segni;
    float v_target_left = evaluateAngularSpeed();
    float v_target_right = -v_target_left;
    float v_target = evaluateLinearSpeed(m_target, m_kinematics.pose(), m_kinematics.speed_left() - m_kinematics.speed_right());
    //TODO controllare se la somma va in saturazione
    v_target_left += v_target;
    v_target_right += v_target;
    m_speed_control.set_targets(v_target_left, v_target_right);
}

float FollowLine::evaluateAngularSpeed()
{
    float distance = m_line.getDistance(m_kinematics.pose().x(), m_kinematics.pose().y());
    return (-kd * distance) + ( kh * (m_line.getDTheta() - m_kinematics.pose().theta()));
}

float FollowLine::evaluateLinearSpeed(Point & target, Pose & current_pose, float current_speed)
{
    Point current = Point(current_pose.x(), current_pose.y());
    float distance = target.getDistance(current);
    float s;

    if (distance < 0) {
        s = -1;
        distance = -distance;
    }
    else
        s = 1;

    if (distance < m_decel_distance) {
        // fase 3
        float expected_speed = sqrt(m_vmax * m_vmax - 2 * m_decel * (m_decel_distance - distance));
        if (expected_speed > fabs(current_speed)) {
            // la vel da raggiungere risulta MAGGIORE della vel corrente
            // questo vuol dire che siamo ancora nella fase di accelerazione
            // pertanto acceleriamo
            m_next_speed += m_accel_step;
            if (m_next_speed > expected_speed) m_next_speed = expected_speed;
            if (m_next_speed > m_vmax) m_next_speed = m_vmax;
        }
        else {
            m_next_speed = expected_speed;
        }
    }
    else {
        // Fase 1, fase 2
        if (m_next_speed < m_vmax) {
            m_next_speed += m_accel_step;
            if (m_next_speed > m_vmax) m_next_speed = m_vmax;
        }
    }
      
    return s*m_next_speed;
}





