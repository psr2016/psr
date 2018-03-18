/*
 * Gear.h
 */

#ifndef __GEAR_H
#define __GEAR_H

#include <btBulletDynamicsCommon.h>

#include "Body.h"
#include "Input.h"
#include "pid.h"

class Gear : public Body {
 public:
    Gear(QOpenGLFunctions_2_1 * ogl_link, Core::World * world, float sampling_time);
    void init();
    void draw();
    void reshape(int width, int height);
    void preTick(float dt);
    float get_omega_current();
    float get_omega_target() { return m_input->last_value(); };
    float get_torque() { return m_torque; };
 private:
    void make_gear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width,
                   GLint teeth, GLfloat tooth_depth);
    GLint m_gear;
    btCylinderShape * m_gearshape;
    btRigidBody * m_rigidbody;
    Pid * omega_pid;
    float m_omega_target;
    Input * m_input;
    float m_torque;
};

#endif
