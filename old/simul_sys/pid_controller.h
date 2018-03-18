/*
 * pid_controller.h
 */

#ifndef __PID_CONTROLLER_H
#define __PID_CONTROLLER_H

#include "dynamic_system.h"

class PID_Controller : public DynamicSystem {

public:
    PID_Controller(float kp, float ki, float kd, float delta_t);
    float evaluate(float input);

private:
  float m_kp, m_ki, m_kd, m_out_i, m_prev_input;
};


#endif
