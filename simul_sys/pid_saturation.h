/*
 * pid_saturation.h
 */

#ifndef __PID_SATURATION_H
#define __PID_SATURATION_H

#include "dynamic_system.h"
#include <stdbool.h>

class PID_Saturation : public DynamicSystem {

public:
    PID_Saturation(float kp, float ki, float kd, float saturation, float delta_t);
    float evaluate(float input);

private:
    float m_kp, m_ki, m_kd, m_saturation, m_out_i, m_prev_input;
    bool m_saturation_flag;
};

#endif
