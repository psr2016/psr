/*
 * pi_controller.cpp
 */


#include "pi_controller.h"

PI_Controller::PI_Controller(float kp, float ki, float delta_t)
  : DynamicSystem(delta_t), m_kp(kp), m_ki(ki), m_out_i(0)
{
}


float PI_Controller::evaluate(float input)
{
  m_out_i = m_out_i + m_ki * input * m_delta_t;
  return input * m_kp + m_out_i;
}
