/*
 * ex1.cpp
 */

#include "ex1.h"

float G1::evaluate(float input)
{
    float y, x1_tmp, x2_tmp;

    x1_tmp = x1 + x2 * m_delta_t;
    x2_tmp = -360*x1*m_delta_t + (-66 * m_delta_t + 1)*x2 + m_delta_t * input;
    y = 1080 * x1;
    x1 = x1_tmp;
    x2 = x2_tmp;
    return y;
}

float G2::evaluate(float input)
{
    float y, x1_tmp, x2_tmp;

    x1_tmp = x1 + x2 * m_delta_t;
    x2_tmp = -8*x1*m_delta_t + (-2 * m_delta_t + 1)*x2 + m_delta_t * input;
    y = 3*x1 + x2;
    x1 = x1_tmp;
    x2 = x2_tmp;
    return y;
}
