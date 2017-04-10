/*
 * pi_controller.h
 */

#ifndef __PI_CONTROLLER_H
#define __PI_CONTROLLER_H

#include "dynamic_system.h"

class PI_Controller : public DynamicSystem {

public:
  PI_Controller(float kp, float ki, float delta_t);
  float evaluate(float input);

private:
  float m_kp, m_ki, m_out_i;
};


#endif
