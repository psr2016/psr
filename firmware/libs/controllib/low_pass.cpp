/*
 * low_pass.cpp
 */

#include "low_pass.h"

#define TWO_PI   6.28318530717959

void LowPassFilter::set_cutoff_frequecy(float hz)
{
    float rc = 1.0 / (TWO_PI * hz);
    m_alpha = m_dt / (rc + m_dt);
    m_alpha_1 = 1 - m_alpha;
}


float LowPassFilter::apply(float val)
{
    float v = val * m_alpha + m_prev_value * m_alpha_1;
    m_prev_value = v;
    return v;
}

