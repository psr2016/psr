/*
 * chebyshev.c
 * Fourth-order Chebyshev type II filter
 */

#ifndef __CHEBYSHEV_H
#define __CHEBYSHEV_H

#include "filter.h"

class Chebyshev4Filter : public Filter {
 public:
    Chebyshev4Filter(float b0, float b1, float b2, float b3, float b4,
                     float a0, float a1, float a2, float a3, float a4,
                     float dt);
    float apply(float v);
 protected:
    float  m_inputTm1,  m_inputTm2,  m_inputTm3,  m_inputTm4;
    float m_outputTm1, m_outputTm2, m_outputTm3, m_outputTm4;
    float m_b[5];
    float m_a[5];
};


#endif
