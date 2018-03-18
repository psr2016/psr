/*
 * pid.cpp
 */

#include "pid.h"

Pid::Pid(float kp, float ki, float kd, float dt, float saturation)
    : m_Kp(kp), m_Ki(ki), m_Kd(kd), m_dt(dt), m_saturation(saturation)

{
    reset();
}

Pid::Pid()
    : m_Kp(0), m_Ki(0), m_Kd(0), m_dt(0), m_saturation(0)
{
    reset();
}

Pid::~Pid()
{
}


void Pid::init(float kp, float ki, float kd, float dt, float saturation)
{
    m_Kp = kp;
    m_Ki = ki;
    m_Kd = kd;
    m_dt = dt;
    m_saturation = saturation;
    reset();
}

void Pid::set_params(float kp, float ki, float kd)
{

    m_Kp = kp;
    m_Ki = ki;
    m_Kd = kd;
    reset();
}

void Pid::reset()
{
    m_u_i = 0;
    m_error_old = 0;
    m_old_process_var = 0;
    m_windup = false;
}


float Pid::evaluate(float target, float process_var)
{
    float error = target - process_var;
    float u_p, u_d;

    u_p  = m_Kp * error; // Proportional term

    u_d  = m_Kd *  (error - m_error_old) / m_dt; // Derivative term
    m_error_old = error;

    if (!m_windup)
        m_u_i += m_Ki * (((error + m_error_old) / 2.0) * m_dt); // Integral term

    m_out = u_p + m_u_i + u_d;

    if (m_out > m_saturation) {
        m_windup = true;
        m_out = m_saturation;
    }
    else if (m_out < -m_saturation) {
        m_windup = true;
        m_out = -m_saturation;
    }
    else
        m_windup = false;

    return m_out;
}


