/*
 * Control.h
 */

#ifndef __CONTROL_H
#define __CONTROL_H

#include "Body.h"
#include "World.h"
#include "pid.h"
#include <QSettings>

class Control {
 public:
    Control(Body * body, World * world, float sampling_time);
    ~Control() {};
    void preTick(float dt);
 private:
    Body * m_body;
    World * m_world;
    float m_sampling_time;
    Pid * m_omega_pid;
};


#endif
