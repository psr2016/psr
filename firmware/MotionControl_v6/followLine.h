#ifndef		__FOLLOWLINE_H
#define 	__FOLLOWLINE_H


#include "position_control.h"

class FollowLine: public PositionControl {
  public:
    FollowLine(Kinematics & kinem, SpeedControlTask & speed_ctrl, Point & target, Point & pointForLine):
      PositionControl(kinem, speed_ctrl), m_line(target, pointForLine) { };
    bool target_reached();
    void run();
    void on() { m_on = true; };//TODO
    void off() { m_on = false; };//TODO
    float evaluate();
    float calcGamma();	
private:
    Line m_line;
    float kd, kh;
    bool m_on;
};



#endif
