/*
 * massa_attrito.h
 */

#ifndef __MASSA_ATTRITO_H
#define __MASSA_ATTRITO_H

#include "dynamic_system.h"

class MassaAttrito : public DynamicSystem {

public:
  MassaAttrito(float m, float b, float delta_t);
  float evaluate(float input);

private:
  float m_massa, m_b;  // costanti del sistema
  float v_k, p_k;      // stato del sistema
};


#endif
