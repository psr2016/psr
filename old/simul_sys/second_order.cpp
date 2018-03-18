/*
 * second_order.cpp
 */

#include "second_order.h"

SecondOrder::SecondOrder(float a, float b1, float b2, float delta_t)
    : DynamicSystem(delta_t), m_a(a), m_b1(b1), m_b2(b2), x1(0), x2(0)
{
}

float SecondOrder::evaluate(float input)
{
    float x1_k = x1 + x2 * m_delta_t;
    float x2_k = (- m_b2 * m_delta_t * x1) + (- m_b1 * m_delta_t + 1) * x2 + input * m_delta_t;
    float y = m_a * x1;

    x1 = x1_k;
    x2 = x2_k;
    return y;
}
