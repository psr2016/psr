/*
 * mass.h
 */

#ifndef __MASS_H
#define __MASS_H

class Mass {
 public:
    // M = mass value (Kg)
    // b = friction coefficient
    // dt = sample interval
    Mass(float M, float b, float dt);
    void apply_force(float f);
    float get_speed() { return m_speed; };
    float get_position() { return m_position; };
 private:
    float m_M, m_b;
    float m_dt;
    float m_speed, m_position;
    float m_prev_speed;
};

#endif
