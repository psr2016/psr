/*
 * pid.h
 */


#ifndef __PID_H
#define __PID_H

#include <stdbool.h>

class Pid {
 public:
    Pid(float kp, float ki, float kd, float kff, float dt, float saturation);
    Pid();
    ~Pid();
    void init(float kp, float ki, float kd, float kff, float dt, float saturation);
    void set_params(float kp, float ki, float kd, float kff);
    void reset();
    float evaluate(float target, float process_var);
    float get_kp() { return m_Kp; };
    float get_ki() { return m_Ki; };
    float get_kd() { return m_Kd; };
    float get_kff() { return m_Kff; };
    void set_kp(float v) { m_Kp = v; };
    void set_ki(float v) { m_Ki = v; reset(); };
    void set_kd(float v) { m_Kd = v; };
    void set_kff(float v) { m_Kff = v; };
 protected:
    float m_Kp;
    float m_Ki;
    float m_Kd;
    float m_Kff;
    bool m_windup;
    float m_u_i;
    float m_error_old;
    float m_out;
    float m_old_process_var;
    float m_old_derivative_value;
    float m_dt;	
    float m_saturation;
};

#ifndef TWO_PI
#define TWO_PI   6.28318530717959
#endif

#endif
