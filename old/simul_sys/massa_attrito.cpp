/*
 * massa_attrito.cpp
 */


#include "massa_attrito.h"

MassaAttrito::MassaAttrito(float m, float b, float delta_t)
  : DynamicSystem(delta_t)
{
  m_massa = m;  m_b = b;  // impostazione attributi
  v_k = 0;  p_k = 0;      // stato iniziale
}

float MassaAttrito::evaluate(float input)
{
  float y = v_k;                       // calcolo uscita
  float v_k1 = (-m_b / m_massa * m_delta_t + 1) * v_k
               + m_delta_t / m_massa * input;  // aggiornamento v_k
  p_k = v_k * m_delta_t + p_k;         // aggiornamento p_k
  v_k = v_k1;
  return y;
}
