#ifndef		__FOLLOWLINE_H
#define 	__FOLLOWLINE_H


#include "position_control.h"

class FollowLine: public PositionControl {
  public:
    FollowLine(Kinematics & kinem, SpeedControlTask & speed_ctrl);
    bool target_reached() { return m_target_reached; }
    void run();
    float evaluateLinearSpeed(Point & target, Pose & current_pose, float current_speed);
    float evaluateAngularSpeed();
    float evaluateDirection(Point & target, Pose & current_pose);
    float calcGamma();
    float normalizeAngle(float x);
    void set_target(float xT, float yT, float xS, float yS);
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
    float m_direction;
    float m_tolerance;
    bool  m_target_reached;
};



#endif
