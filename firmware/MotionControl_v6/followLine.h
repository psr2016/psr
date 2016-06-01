#ifndef		__FOLLOWLINE_H
#define 	__FOLLOWLINE_H


#include "position_control.h"

class FollowLine: public PositionControl {
  public:
    FollowLine(Kinematics & kinem, SpeedControlTask & speed_ctrl, Point & target, Point & pointForLine):
      PositionControl(kinem, speed_ctrl), m_line(target, pointForLine), m_target(target) { };
    bool target_reached();
    void run();
    void on() { m_on = true; };//TODO
    void off() { m_on = false; };//TODO
    float evaluateLinearSpeed(Point & target, Pose & current_pose, float current_speed);
    float evaluateAngularSpeed();
    void evaluateVerse(Point & target, Pose & current_pose);
    float calcGamma();
private:
    Line m_line;
    Point m_target;
    float kd, kh;
    float m_accel; //TODO da chiedere al prof se esistono gia' questi parametri da qualche altra parte
    float m_accel_step;
    float m_vmax;
    float m_decel;
    float m_dt;
    float m_next_speed;
    float m_decel_distance;
    float m_verse;
    bool  m_target_reached;
    bool  m_on;
};



#endif
