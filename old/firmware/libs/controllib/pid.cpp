/*
 * pid.cpp
 */

#include "pid.h"

Pid::Pid(float kp, float ki, float kd, float kff, float dt, float saturation)
    : m_Kp(kp), m_Ki(ki), m_Kd(kd), m_Kff(kff), m_dt(dt), m_saturation(saturation)

{
    m_derivative_low_pass = new LowPassFilter(dt);
    reset();
}

Pid::Pid()
    : m_Kp(0), m_Ki(0), m_Kd(0), m_Kff(0), m_dt(0), m_saturation(0)
{
    m_derivative_low_pass = new LowPassFilter(0);
    reset();
}

Pid::~Pid()
{
    delete m_derivative_low_pass;
}


void Pid::init(float kp, float ki, float kd, float kff, float dt, float saturation)
{

    m_Kp = kp;
    m_Ki = ki;
    m_Kd = kd;
    m_Kff = kff;
    m_dt = dt;
    m_derivative_low_pass->set_dt(dt);
    m_saturation = saturation;
    reset();
}

void Pid::set_params(float kp, float ki, float kd, float kff)
{

    m_Kp = kp;
    m_Ki = ki;
    m_Kd = kd;
    m_Kff = kff;
    reset();
}

void Pid::set_derivative_low_pass(float cutoff_hz)
{
    m_derivative_low_pass->set_cutoff_frequecy(cutoff_hz);
}

void Pid::reset()
{
    m_u_i = 0;
    m_error_old = 0;
    m_old_process_var = 0;
    m_windup = false;
}

float Pid::derivative_low_pass(float val)
{
    return m_derivative_low_pass->apply(val);
}

float Pid::derivative(float error, float process_var)
{
    return (error - m_error_old);
}


float Pid::evaluate(float target, float process_var)
{
    float error = target - process_var;
    return evaluate_using_error(error, target, process_var);
}

float Pid::evaluate_using_error(float error, float target, float process_var)
{
    float u_p, u_d;

    u_p  = m_Kp * error;

    if (m_Kd != 0.0)
        u_d  = m_Kd *  derivative_low_pass(derivative(error, process_var)) / m_dt;
    else
        u_d = 0.0;

    m_old_process_var = process_var;
    m_error_old = error;

    if (!m_windup)
        m_u_i += m_Ki * (((error + m_error_old) / 2.0) * m_dt);

    m_out = u_p + m_u_i + u_d + target * m_Kff;

    if (m_out >= m_saturation) {
        m_out = m_saturation;
        m_windup = true;
    }
    else if (m_out <= (-1.0 * m_saturation)) {
        m_out = -1.0 * m_saturation;
        m_windup = true;
    }
    else
        m_windup = false;

    return m_out;
}


float PidDerivateOutput::derivative(float error, float process_var)
{
    return - (process_var - m_old_process_var);
}


