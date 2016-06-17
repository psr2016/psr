/*
 * followLine.cpp
 */

#include "followLine.h"
#include <math.h>

FollowLine::FollowLine(Kinematics & kinem, SpeedControlTask & speed_ctrl)
      : PositionControl(kinem, speed_ctrl),
	kd(1.0),
	kh(75.0),
	ks(100.0),
	m_accel(600),
	m_vmax(600),
	m_decel(600),
	m_next_speed(0),
	m_tolerance(30),
	m_target_reached(false),
	m_two_step(false)
{
    m_accel_step = m_accel * m_real_time_period;
    m_decel_distance = (m_vmax * m_vmax) / (2 * m_decel);
}

void FollowLine::set_target(float xT, float yT, float xS, float yS)
{
    m_target = Point(xT, yT);

    if ( isTooClose() ) {
	Point m_startLine(m_kinematics.pose().x(), m_kinematics.pose().y());
	m_line.set_line(m_target, m_startLine);
	m_two_step = true;
    } else {
	Point m_startLine(xS, yS);
	m_line.set_line(m_target, m_startLine);
	m_two_step = false;
    }

    m_direction = evaluateDirection();
    m_target_reached = false;
}

bool FollowLine::isTooClose() {

    float angularError = fabs(evaluateAngularSpeed());
    float distance = m_target.getDistance(m_kinematics.pose().x(), m_kinematics.pose().y());

    if ( angularError > 10 && distance < 10 ) { //TODO settare un parametro opportuno, verificare sul campo
	return true;
    }

    return false;
}

void FollowLine::run()
{
    if (m_target_reached) return;

    if ( (fabs( m_target.x() - m_kinematics.pose().x() ) < m_tolerance) && (fabs( m_target.y() - m_kinematics.pose().y() ) < m_tolerance) ) {
	m_speed_control.set_targets(0, 0);
	m_target_reached = true;
	return;
    }

    if ( ( m_direction * evaluateDirection() ) < 0 ) {//controllo per vedere se abbiamo passato il punto
	m_direction = -m_direction;
    }

    //TODO Controllare tutti i segni;
    float v_target_right = evaluateAngularSpeed();
    float v_target_left = -v_target_right;

    if ( !m_two_step || fabs(v_target_left) < 10 ) { //TODO settare un parametro opportuno, verificare sul campo

      float v_target = evaluateLinearSpeed();
      //TODO controllare se la somma va in saturazione
      v_target_left += v_target;
      v_target_right += v_target;

    }
    m_speed_control.set_targets(v_target_left, v_target_right);
}

float FollowLine::evaluateAngularSpeed()
{
    float distanceFromLine = m_line.getDistance(m_kinematics.pose().x(), m_kinematics.pose().y());
    float distanceFromTarget = m_target.getDistance(m_kinematics.pose().x(), m_kinematics.pose().y());
    
    if ( distanceFromTarget < 70 ) {
	distanceFromTarget = distanceFromTarget / ks;
    } else {
	distanceFromTarget = 1;
    }
      
    if (m_direction > 0)
        return (kd * distanceFromLine) + (distanceFromTarget * kh * normalizeAngle(m_line.getDTheta() - m_kinematics.pose().theta()));
    else
        return (kd * distanceFromLine) + (distanceFromTarget * kh * normalizeAngle(m_line.getDTheta() - (m_kinematics.pose().theta() + PI)));
}

float FollowLine::evaluateDirection()
{
    float dTheta = atan2 ( (m_target.y() - m_kinematics.pose().y()), (m_target.x() - m_kinematics.pose().x()) );
    
    if(fabs(dTheta - m_kinematics.pose().theta()) < PI/2)
	 return 1;
    else
	 return -1;
}

float FollowLine::evaluateLinearSpeed()
{
    float current_speed = m_kinematics.linear_speed();
    float distance = m_target.getDistance(m_kinematics.pose().x(), m_kinematics.pose().y());

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

    return m_direction * m_next_speed;
}

float FollowLine::normalizeAngle(float a)
{
    if (a > PI)
        a = a - TWO_PI;
    if (a < -PI)
        a = a + TWO_PI;
    return a;
}
