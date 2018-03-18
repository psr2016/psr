/*
 * input_sin.cpp
 */

#include "input_sin.h"
#include <math.h>

Sin::Sin(float gain, float omega, float delta_t)
    : DynamicSystem(delta_t), m_gain(gain), m_omega(omega)
{
    m_time = 0;
}

float Sin::evaluate(float input)
{
    float y =  m_gain * cos(m_omega * m_time);
    m_time += m_delta_t;
    return y;
}
