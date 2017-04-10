/*
 * massa_molla.cpp
 */

#include "massa_molla.h"


MassaMolla::MassaMolla(float m, float b, float k, float delta_t)
  : DynamicSystem(delta_t)
{
    m_massa = m;  m_b = b; m_k = k;   // impostazione attributi
    v_k = 0;  p_k = 0;                // stato iniziale
}


float MassaMolla::evaluate(float input)
{
    float y = p_k;                       // calcolo uscita
    p_k = v_k * m_delta_t + p_k;         // aggiornamento p_k
    v_k = (- m_k * m_delta_t / m_massa) * p_k
        + (- m_b / m_massa * m_delta_t + 1) * v_k
        + m_delta_t / m_massa * input;  // aggiornamento v_k
    return y;
}

