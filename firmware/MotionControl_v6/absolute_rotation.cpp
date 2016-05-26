/*
 * absolute_rotation.cpp
 */

#include "absolute_rotation.h"

#include <math.h>

AbsoluteRotation::AbsoluteRotation(Kinematics & kinem, SpeedControlTask & speed_ctrl,
float lin_accel, float lin_vmax, float lin_decel, float lin_vmin, float angular_range)
    : PositionControl(kinem, speed_ctrl)
{
	//settiamo accel e velocità 
    m_lin_accel = lin_accel;
    m_lin_vmax = lin_vmax;
    m_lin_decel = lin_decel;
    m_lin_vmin = lin_vmin;
    
    
    
    m_angular_range = angular_range*PI/180; //soglia 
    
}

void AbsoluteRotation::evaluate_absolute_rotation(float target_angle)
{
	
    //da lineare ad angolare
    float wheelbase = m_kinematics.wheelbase(); //metodo implementato da Russo-Portelli
    m_angular_accel = 2*m_lin_accel / wheelbase;  // alpha= at/L/2 => alpha = 2at/L dove alpha= accel ang e at=accel lin
    m_angular_decel = 2*m_lin_decel / wheelbase; 
    m_angular_vmax = 2*m_lin_vmax / wheelbase; 
    
    m_angular_accel_increment = m_angular_accel * m_real_time_period; //step
    m_angular_decel_distance = (m_angular_vmax * m_angular_vmax) / (2 * m_angular_decel);
	
	
    
    //da gradi a radianti
    m_target_angle = target_angle*PI/180;
    m_target_reached = false;
    
    
}

void AbsoluteRotation::run()
{
	
	if (m_target_reached) return;

    float distance = m_target_angle - m_kinematics.pose().theta();
    float s;

    if (distance < 0) {
        s = -1;
        distance = -distance;
    }
    else
        s = 1;
        
    //controllo target_reached raggiunto
    if(distance <= m_angular_range){
		
		m_speed_control.set_targets(0, 0);
        m_target_reached = true;
        return;
    }
    
    
    
    
    float current_angular_speed = fabs(m_kinematics.angular_speed());
    
    if (distance < m_angular_decel_distance) {
        //fase 3
        //velocità attesa della frenata
        float expected_angular_speed = sqrt(m_angular_vmax * m_angular_vmax - 2 * m_angular_decel * (m_angular_decel_distance - distance));
        if (expected_angular_speed > current_angular_speed) {
            // la vel da raggiungere risulta MAGGIORE della vel corrente
            // questo vuol dire che siamo ancora nella fase di accelerazione
            // pertanto acceleriamo
            m_angular_next_speed += m_angular_accel_increment;
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
            m_angular_next_speed += m_angular_accel_increment;
            //fase 2
            if (m_angular_next_speed > m_angular_vmax) m_angular_next_speed = m_angular_vmax;
        }
    }
    
    //da angolare a lineare
    // angular_next_speed = wheel_speed/L/2 => angular_next_speed = 2*wheel_speed/L 
    //ricaviamo la formula inversa
                                                
    float wheel_speed = m_angular_next_speed*m_kinematics.wheelbase()/2; // velocità ruota destra
    

    //sotto m_lin_vmin la ruota non gira
    
  
    
    if (wheel_speed < m_lin_vmin) wheel_speed = m_lin_vmin;
    
    wheel_speed = s*wheel_speed;
    
    
    //verso di rotazione antiorario
    m_speed_control.set_targets(-wheel_speed, wheel_speed); //settiamo le velocità
}
      
	
    
    
	
