/*
 * circ_rotation.cpp
 */

#include "circ_rotation.h"
#include "geometry.h"
#include <math.h>

CircularRotation::CircularRotation(Kinematics & kinem, SpeedControlTask & speed_ctrl,
float linear_accel, float linear_vmax, float linear_decel, float linear_vmin, float angular_target_range, float radius)
    : PositionControl(kinem, speed_ctrl), m_linear_vmin(linear_vmin),
    m_angular_next_speed(0), m_angular_target(0), m_target_reached(true)
{
	 //trasformazione da lineare ad angolare
    m_angular_accel = linear_accel / radius; //linear_accel = accelerazione punto centrale
    m_angular_decel = linear_decel / radius; //linear_decel = decelerazione punto centrale
    m_angular_vmax = linear_vmax / radius; //linear_vmax = velocità massima punto centrale

    //conversione da gradi a radianti
    m_angular_target_range = TO_RADIANS(angular_target_range);

	//Istruzioni come in position_controller.cpp
    m_angular_accel_step = m_angular_accel * m_real_time_period;
    m_angular_decel_distance = (m_angular_vmax * m_angular_vmax) / (2 * m_angular_decel);
}

void CircularRotation::set_rotation_target(float angular_target, float radius)
{
	float half_wheelbase = m_kinematics.wheelbase() / 2;
	if (radius < 0)
	{
		m_radius_left = radius - half_wheelbase;
		m_radius_right = radius + half_wheelbase;
	}
	else
	{
		m_radius_left = radius + half_wheelbase;
		m_radius_right = radius - half_wheelbase;
	}
		
	//conversione da gradi a radianti
	m_angular_target = TO_RADIANS(angular_target);
	m_kinematics.set_angular_distance(0);
	m_target_reached = false;
}

void CircularRotation::run()
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

	// Velocità_angolare = Velocità lineare / Raggio => Velocità lineare = Velocità_angolare * Raggio
	
	//RUOTA SINISTRA
    //                			     wheel_speed_left            					               			
    //angular_speed = ------------------------------        =>        wheel_speed_left = angular_speed * (m_radius+wheelbase/2)
    //                			  m_radius+wheelbase/2                                    							          
    
    
    float wheel_speed_left = m_angular_next_speed*(m_radius_left); 

	//RUOTA DESTRA
    //                			     wheel_speed_right            					               			
    //angular_speed = ------------------------------        =>        wheel_speed_left = angular_speed * (m_radius-wheelbase/2) 
    //                			  m_radius-wheelbase/2                                    							          
    
    float wheel_speed_right = m_angular_next_speed*(m_radius_right);

    //sotto m_linear_vmin la ruota non gira
    if (wheel_speed_left < m_linear_vmin) wheel_speed_left = m_linear_vmin;
	if (wheel_speed_right < m_linear_vmin) wheel_speed_right = m_linear_vmin;

	//correzione senso di rotazione
    wheel_speed_left = s * wheel_speed_left;
    wheel_speed_right = s * wheel_speed_right;

    //verso di rotazione antiorario
    m_speed_control.set_targets(wheel_speed_left, wheel_speed_right); //velocità ruota sinistra e ruota destra
    
}
