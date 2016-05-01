/*
 * position_controller.h
 */


#ifndef __POSITION_CONTROLLER_H
#define __POSITION_CONTROLLER_H

#include <stdbool.h>

class PositionController {
 public:
    PositionController(float accel, float v_max, float decel, float dt);
    float evaluate(float target, float current_pos, float current_speed);
 protected:
    float m_accel;
    float m_accel_step;
    float m_vmax;
    float m_decel;
    float m_dt;	
    float m_next_speed;
    float m_decel_distance;
};

#endif
