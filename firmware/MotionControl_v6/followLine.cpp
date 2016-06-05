/*
 * followLine.cpp
 */

#include "followLine.h"
#include <math.h>

void FollowLine::run()
{
    if (m_target_reached) return;
  
    if ( (fabs( m_target.x() - m_kinematics.pose().x() ) < m_tolerance) && (fabs( m_target.y() - m_kinematics.pose().y() ) < m_tolerance) ) {
	m_speed_control.set_targets(0, 0);
	m_target_reached = true;
	return;
    }
    
    if ( ( m_verse * evaluateVerse(m_target, m_kinematics.pose()) ) < 0 ) {//controllo per vedere se abbiamo passato il punto
	m_verse = -m_verse;
    }
    
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
    if (m_verse > 0)
        return (-kd * distance) + ( kh * (m_line.getDTheta() - m_kinematics.pose().theta()));
    else
        return (-kd * distance) + ( kh * (m_line.getDTheta() - (m_kinematics.pose().theta() + PI)));
        //TODO controllare se e' sufficiente aggiungere PI
        //TODO verificare se bisogna normalizzare l'angolo
}

float FollowLine::evaluateVerse(Point & target, Pose & current_pose)
{
    if(((cos(current_pose.theta())*(target.x()-current_pose.x()))+(sin(current_pose.theta())*(target.y()-current_pose.y()))) > 0)
	return 1;
    else
	return -1;
}

float FollowLine::evaluateLinearSpeed(Point & target, Pose & current_pose, float current_speed)
{
    Point current = Point(current_pose.x(), current_pose.y());
    float distance = target.getDistance(current);

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

    return m_verse * m_next_speed;
}
