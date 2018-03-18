/*
 * pid_controller.cpp
 */


#include "pid_controller.h"

PID_Controller::PID_Controller(float kp, float ki, float kd, float delta_t)
    : DynamicSystem(delta_t), m_kp(kp), m_ki(ki), m_kd(kd), m_out_i(0), m_prev_input(0)
{
}


float PID_Controller::evaluate(float input)
{
    float deriv = (input - m_prev_input) / m_delta_t;
    
    m_out_i = m_out_i + m_ki * input * m_delta_t;

    m_prev_input = input;

    return input * m_kp + m_out_i + deriv * m_kd;
}
