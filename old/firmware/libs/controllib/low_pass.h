/*
 * low_pass.h
 */

#ifndef __LOW_PASS_H
#define __LOW_PASS_H

#include "filter.h"

class LowPassFilter : public Filter {
 public:
 LowPassFilter(float dt) : Filter(dt) { };
    void set_cutoff_frequecy(float hz);
    float apply(float v);
 private:
    float m_alpha, m_alpha_1;
};

#endif
