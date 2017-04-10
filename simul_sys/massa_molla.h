/*
 * massa_molla.h
 */

#ifndef __MASSA_MOLLA_H
#define __MASSA_MOLLA_H

#include "dynamic_system.h"

class MassaMolla : public DynamicSystem {

public:
    MassaMolla(float m, float b, float k, float delta_t);
    float evaluate(float input);

 private:
    float m_massa, m_b, m_k;  // costanti del sistema
    float v_k, p_k;      // stato del sistema
};


#endif
