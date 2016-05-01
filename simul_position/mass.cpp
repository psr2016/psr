/*
 * mass.c
 */


#include "mass.h"

Mass::Mass(float M, float b, float dt)
    : m_M(M), m_b(b), m_dt(dt), m_speed(0), m_position(0)
{
}


void Mass::apply_force(float f)
{
    // compute the transfer function
    //           T          M - b*T
    // v(k+1) = --- f(k) + ---------- v(k)
    //           M             M

    // update the speed using the trasfer function
    m_speed = (m_dt/m_M)*f + (m_M - m_b * m_dt) / m_M * m_speed;

    // update the position using the integral
    m_position += m_speed * m_dt;
}

