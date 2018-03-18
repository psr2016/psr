/*
 * kinematics.h
 */

#ifndef __KINEMATICS_H
#define __KINEMATICS_H

#include "periodic_task.h"
#include "geometry.h"
#include "time_defines.h"

class Kinematics : public PeriodicTask {
 public:
    Kinematics(Pose & p, float radius_left, float radius_right, float wheel_base, float ticks);
    ~Kinematics() {} ;
    void set_radius(float radius_left, float radius_right) {
        set_radius_left(radius_left);
        set_radius_right(radius_right);
    };
    virtual void set_radius_left(float radius_left);
    virtual void set_radius_right(float radius_right);
    virtual void set_wheelbase(float wheel_base) { m_wheelbase = wheel_base; };
    virtual void run();
    virtual float speed_left() { return m_speed_left; };
    virtual float speed_right() { return m_speed_right; };
    Pose & pose() { return robot_pose;};

    //aggiunti da Massimiliano Portelli e Placido Russo
    virtual float angular_speed() { return m_angular_speed; };
    virtual float wheelbase() { return m_wheelbase; };
    virtual float angular_distance() { return m_angular_distance; };
    virtual float linear_speed() { return m_linear_speed; };
    virtual void set_angular_distance(float angular_distance) { m_angular_distance = angular_distance; };

 protected:
    float m_radius_left, m_radius_right;
    float m_wheel_factor_left, m_wheel_factor_right;
    float m_wheelbase;
    float m_ticks_per_revolution;

    int m_poscount_left, m_poscount_right;

    Pose & robot_pose;
    float m_speed_left, m_speed_right;
    float m_linear_speed, m_angular_speed;
    float m_linear_distance, m_angular_distance;
    bool m_rotation_radius_infinity;
    float m_rotation_radius;
};

#endif
