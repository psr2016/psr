/*
 * dynamic_system.h
 */

#ifndef __DYNAMIC_SYSTEM
#define __DYNAMIC_SYSTEM

class DynamicSystem {

public:
  DynamicSystem(float delta_t) { m_delta_t = delta_t; };
  virtual float evaluate(float input) = 0;

protected:
  float m_delta_t;
};

#endif

