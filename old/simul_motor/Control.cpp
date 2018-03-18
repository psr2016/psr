/*
 * Control.cpp
 */

#include "Control.h"

Control::Control(Body * body, World * world, float sampling_time)
    : m_body(body), m_world(world), m_sampling_time(sampling_time)
{
    m_settings = world->settings();
    m_omega_pid = new Pid(m_settings->value("Pid/kp", 0).toFloat(),
                          m_settings->value("Pid/ki", 0).toFloat(),
                          m_settings->value("Pid/kd", 0).toFloat(),
                          0, sampling_time, 0);
}

void Control::preTick(float dt)
{
}

