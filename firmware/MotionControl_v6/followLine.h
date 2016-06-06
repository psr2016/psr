#ifndef		__FOLLOWLINE_H
#define 	__FOLLOWLINE_H


#include "position_control.h"

class FollowLine: public PositionControl {
  public:
    FollowLine(Kinematics & kinem, SpeedControlTask & speed_ctrl, Point & target, Point & pointForLine):
      PositionControl(kinem, speed_ctrl), m_line(target, pointForLine), m_target(target), m_target_reached(false)
      { m_verse = evaluateVerse(target, kinem.pose()); };
    bool target_reached() { return m_target_reached; }
    void run();
    void on() { m_on = true; };//TODO
    void off() { m_on = false; };//TODO
    float evaluateLinearSpeed(Point & target, Pose & current_pose, float current_speed);
    float evaluateAngularSpeed();
    float evaluateVerse(Point & target, Pose & current_pose);
    float calcGamma();
    float normalizeAngle(float x);
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
    float m_tolerance;
    bool  m_target_reached;
    bool  m_on;
};



#endif
