/*
 * circ_rotation.cpp
 */

#include "circ_rotation.h"
#include "geometry.h"
#include <math.h>

CircularRotation::CircularRotation(Kinematics & kinem, SpeedControlTask & speed_ctrl,
                                   float linear_accel, float linear_vmax, float linear_decel,
                                   float linear_vmin,float angular_target_range)
    : PositionControl(kinem, speed_ctrl)
{
    //Set acc e vel
    m_linear_accel=linear_accel;
    m_linear_decel=linear_decel;
    m_linear_vmax=linear_vmax;
    m_linear_vmin=linear_vmin;

    //Set grado di tolleranza
    m_angular_target_range = TO_RADIANS(angular_target_range);
}

void CircularRotation::set_rotation_target(float angular_target, float radius)
{
	m_radius=radius;
	
	//conversione da gradi a radianti
    m_angular_target = TO_RADIANS(angular_target);
    
    //Metà della distanza tra le ruote
    float half_wheelbase = m_kinematics.wheelbase() / 2;

    //Set raggi
    m_radius_left = fabs(radius - half_wheelbase);
    m_radius_right = fabs(radius + half_wheelbase);
	
    //Trasformazione da lineare ad angolare
    m_angular_accel = m_linear_accel / fabs(radius); //linear_accel = accelerazione punto centrale
    m_angular_decel = m_linear_decel / fabs(radius); //linear_decel = decelerazione punto centrale
    m_angular_vmax = m_linear_vmax / fabs(radius); //linear_vmax = velocità massima punto centrale

    //Step
    m_angular_accel_step = m_angular_accel * m_real_time_period;
    m_angular_decel_distance = (m_angular_vmax * m_angular_vmax) / (2 * m_angular_decel);
    
    //Distanza angolare iniziale posta a 0
    m_kinematics.set_angular_distance(0);
    m_target_reached = false;
}

void CircularRotation::run()
{
	if (m_target_reached) return;

	/*distance = Distanza angolare corrente dal target
	 * m_angular_target = Distanza angolare da raggiungere
	 * m_kinematics.angular_distance() = Distanza attualmente percorsa
    */
    
    float angular_fixed_distance = m_kinematics.angular_distance();
    if (m_radius<0) angular_fixed_distance = -angular_fixed_distance;
    
    float distance = m_angular_target - angular_fixed_distance;
    float s;

    if (distance < 0) {
        s = -1;
        distance = -distance;
    }
    else
        s = 1;
        
    //Distanza da raggiungere minore della soglia
    if (distance <= m_angular_target_range) {
        m_speed_control.set_targets(0, 0); //Ferma il robot
        m_target_reached = true;
        return;
    }

    float angular_speed = m_kinematics.angular_speed();

    if (distance < m_angular_decel_distance) {
        //Fase 3
        //Velocità attesa della frenata
        float expected_angular_speed = sqrt(m_angular_vmax * m_angular_vmax - 2 * m_angular_decel * (m_angular_decel_distance - distance));
        if (expected_angular_speed > fabs(angular_speed)) {
            // La vel da raggiungere risulta MAGGIORE della vel corrente
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
        //Fase 1
        if (m_angular_next_speed < m_angular_vmax) {
            m_angular_next_speed += m_angular_accel_step;
            //Fase 2
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

    //Sotto m_linear_vmin la ruota non gira
    if (wheel_speed_left < m_linear_vmin) wheel_speed_left = m_linear_vmin;
	if (wheel_speed_right < m_linear_vmin) wheel_speed_right = m_linear_vmin;

	//Correzione senso di rotazione
    wheel_speed_left = s * wheel_speed_left;
    wheel_speed_right = s * wheel_speed_right;

    //Verso di rotazione antiorario
    m_speed_control.set_targets(wheel_speed_left, wheel_speed_right); //velocità ruota sinistra e ruota destra

}
