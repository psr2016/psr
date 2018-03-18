/*
 * circ_rotation.h
 */

#ifndef __CIRCULAR_ROTATION_H
#define __CIRCULAR_ROTATION_H

#include <stdbool.h>
#include "position_control.h"
#include "speed_control.h"
#include "kinematics.h"

class CircularRotation : public PositionControl {
 public:
	CircularRotation(Kinematics & kinem, SpeedControlTask & speed_ctrl,
	float linear_accel, float linear_vmax, float linear_decel, float linear_vmin,float angular_target_range);
	
	void set_rotation_target(float angular_target, float radius);
	bool target_reached() { return m_target_reached; };
	void run();
	
 private:
    float m_radius;
	float m_radius_left, m_radius_right;
	float m_angular_accel;
	float m_angular_decel;
	float m_angular_vmax;
	float m_angular_accel_step;
    float m_angular_decel_distance;
    float m_angular_next_speed;
    float m_linear_accel;
    float m_linear_decel;
    float m_linear_vmax;
    float m_linear_vmin;
	
	float m_angular_target;
	float m_angular_target_range;
	bool m_target_reached;
};

extern CircularRotation circular_rotation;

#endif
