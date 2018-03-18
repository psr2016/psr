#ifndef __EX1_H
#define __EX1_H

#include "dynamic_system.h"

class G1 : public DynamicSystem {
public:
    G1(float delta_t) : DynamicSystem(delta_t) {
        x1 = 0;
        x2 = 0;
    };

    float evaluate(float input);

private:
    float x1, x2;
};

class G2 : public DynamicSystem {
public:
    G2(float delta_t) : DynamicSystem(delta_t) {
        x1 = 0;
        x2 = 0;
    };

    float evaluate(float input);

private:
    float x1, x2;
};

#endif
