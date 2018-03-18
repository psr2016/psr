#ifndef		__FOLLOWLINE_H
#define 	__FOLLOWLINE_H


#include "position_control.h"

class FollowLine: public PositionControl {
  public:
    FollowLine(Kinematics & kinem, SpeedControlTask & speed_ctrl,
               float linear_accel, float linear_vmax, float linear_decel, float linear_vmin);
    bool target_reached() { return m_target_reached; }
    void run();
    float evaluateLinearSpeed();
    float evaluateAngularSpeed();
    float evaluateDirection();
    float calcGamma();
    float normalizeAngle(float x);
    void set_target(float xT, float yT, float xS, float yS);
    bool isTooClose();
    void set_gains(float _kd, float _kh, float _ks) { kd = _kd; kh = _kh; ks = _ks; } ;
private:
    Line m_line;
    Point m_target;
    float kd, kh, ks;
    float m_accel;
    float m_accel_step;
    float m_vmax;
    float m_decel;
    float m_next_speed;
    float m_decel_distance;
    float m_direction;
    float m_tolerance;
    bool m_target_reached;
    bool m_two_step;
};

extern FollowLine follow_line;


#endif
