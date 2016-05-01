/*
 * filter.h
 */

#ifndef __FILTER_H
#define __FILTER_H

class Filter {
 public:
    Filter(float dt) : m_dt(dt), m_prev_value(0) { };
    virtual float apply(float v) = 0;
    void set_dt(float dt) { m_dt = dt; };
 protected:
    float m_dt;
    float m_prev_value;
};


#endif
