/*
 * absolute_rotation.h
 */


#ifndef __ABSOLUTE_ROTATION_H
#define __ABSOLUTE_ROTATION_H

#include <stdbool.h>

#include "position_control.h"

class AbsoluteRotation : public PositionControl {
 public: 
	AbsoluteRotation(Kinematics & kinem, SpeedControlTask & speed_ctrl, float lin_accel, float lin_vmax, float lin_decel, float lin_vmin, float target_angle, float angular_range);
	
	bool target_reached() { return m_target_reached; };
	void evaluate_absolute_rotation(float target_angle); 
	void run();
	
	
protected:
    float m_lin_accel;
    float m_lin_vmax;
    float m_lin_decel;
    float m_lin_vmin;
    

    float m_angular_accel;
    float m_angular_accel_increment; //incremento step
    float m_angular_vmax;
    float m_angular_decel;
    float m_angular_decel_distance; //distanza percorsa durante decelerazione
    float m_angular_next_speed;

    float m_target_angle; 
    float m_angular_range;
    
    bool m_target_reached;
	




};
extern AbsoluteRotation absolute_rotation;

#endif

