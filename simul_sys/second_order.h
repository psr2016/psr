#ifndef __SECOND_ORDER_H
#define __SECOND_ORDER_H

#include "dynamic_system.h"

class SecondOrder : public DynamicSystem {
 public:
  SecondOrder(float a, float b1, float b2, float delta_t);
  float evaluate(float input);

private:
  float m_a, m_b1, m_b2;
  float x1, x2;
};

#endif
