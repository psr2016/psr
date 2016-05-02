
#ifndef __POSITION_CONTROL_H
#define __POSITION_CONTROL_H

#include <stdbool.h>
#include "periodic_task.h"
#include "speed_control.h"
#include "kinematics.h"

class PositionControl : public PeriodicTask {
public:
    PositionControl(Kinematics & kinem, SpeedControlTask & speed_ctrl) :
        PeriodicTask("PositionControl", POSITION_CONTROL_PERIOD, TIME_UNIT, POSITION_CONTROL_JITTER),
        m_kinematics(kinem),
        m_speed_control(speed_ctrl) { };
    virtual bool target_reached() = 0;
protected:
    Kinematics & m_kinematics;
    SpeedControlTask & m_speed_control;
};

#endif

