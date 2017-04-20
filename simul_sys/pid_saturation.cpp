/*
 * pid_saturation.cpp
 */


#include "pid_saturation.h"

#define USE_ANTI_WIND_UP

PID_Saturation::PID_Saturation(float kp, float ki, float kd, float saturation, float delta_t)
    : DynamicSystem(delta_t),
      m_kp(kp), m_ki(ki), m_kd(kd),
      m_saturation(saturation),
      m_out_i(0), m_prev_input(0),
      m_saturation_flag(false)
{
}


float PID_Saturation::evaluate(float input)
{
    float deriv = (input - m_prev_input) / m_delta_t;
    m_prev_input = input;

#ifdef USE_ANTI_WIND_UP
    // if ANTIWINDUP is active, do not integrate when the system is in saturation
    if (!m_saturation_flag)
        m_out_i = m_out_i + m_ki * input * m_delta_t;
#else
    m_out_i = m_out_i + m_ki * input * m_delta_t;
#endif

    float output = input * m_kp + m_out_i + deriv * m_kd;

    if (output > m_saturation) {
        output = m_saturation;
        m_saturation_flag = true;
    }
    else if (output < - m_saturation) {
        output = - m_saturation;
        m_saturation_flag = true;
    }
    else
        m_saturation_flag = false;

    return output;
}

