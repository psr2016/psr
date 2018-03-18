/*
 * input_sin.h
 */

#ifndef __INPUT_SIN_H
#define __INPUT_SIN_H

#include "dynamic_system.h"

class Sin : public DynamicSystem {
 public:
    Sin(float gain, float omega, float delta_t);
    float evaluate(float input);
 private:
    float m_omega;
    float m_gain;
    float m_time;
};

#endif
