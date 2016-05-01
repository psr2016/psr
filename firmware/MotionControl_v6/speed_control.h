/*
 * speed_control.h
 */

#ifndef __SPEED_CONTROL_H
#define __SPEED_CONTROL_H

#include <stdbool.h>
#include "pid.h"
#include "periodic_task.h"
#include "kinematics.h"
#include "pwm.h"

class SpeedControlTask : public PeriodicTask {
 public:
    SpeedControlTask(Kinematics & kin);
    void run();
    void off(bool stop_pwm = true);
    void set_params(float kp, float ki, float kd);
    void set_targets(float l, float r) { m_target_left = l; m_target_right = r; on(); };
    void set_motors(int l, int r) { off(false); m_pwm_left = l; m_pwm_right = r; set_pwm(l, r); };
    int pwm_left() { return m_pwm_left;};
    int pwm_right() { return m_pwm_right;};
    void get_current_speeds(float & l, float & r) {
        l = m_kinematics.speed_left();
        r = m_kinematics.speed_right();
    };
 private:
    Kinematics & m_kinematics;
    int m_pwm_left, m_pwm_right;
    float m_target_left, m_target_right;
    Pid m_left_pid, m_right_pid;
};

#endif
