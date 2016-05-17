/*
 * relative_rotation.h
 */


#ifndef __RELATIVE_ROTATION_H
#define __RELATIVE_ROTATION_H

#include <stdbool.h>
#include "position_control.h"

class RelativeRotation : public PositionControl {
 public:
    RelativeRotation(Kinematics & kinem, SpeedControlTask & speed_ctrl,
    float linear_accel, float linear_vmax, float linear_decel, float linear_vmin, float angular_target_range);

    bool target_reached() { return m_target_reached; };
    void set_rotation_target(float angular_target);
    void run();

 protected:
    float m_linear_accel;
    float m_linear_vmax;
    float m_linear_decel;
    float m_linear_vmin;

    float m_angular_accel;
    float m_angular_accel_step;
    float m_angular_vmax;
    float m_angular_decel;
    float m_angular_decel_distance;
    float m_angular_next_speed;

    float m_angular_target; //aggiornato in set_rotation_target()
    float m_angular_target_range;
    bool m_target_reached;
};

extern RelativeRotation relative_rotation;

#endif
