/*
 * goto_point.h
 */


#ifndef __GOTO_POINT_H
#define __GOTO_POINT_H

#include "position_control.h"

//alfa =rappresenta l'angolo da calcolare per far ruotare il robot verso il punto dato
//rho = in coordinate polari rho rappresenta la distanza tra il robot e il punto

class GoTo_Point : public PositionControl {
 public:
    GoTo_Point(Kinematics & kinem, SpeedControlTask & speed_ctrl,
    float linear_accel, 
    float linear_vmax, 
    float linear_decel, 
    float linear_vmin);

    bool target_reached() { return m_target_reached; };
    void run();
    void set_target(float xT,float yT);
    void setAlpha(float delta_x,float delta_y);
    void setDistance(float delta_x,float delta_y);
    float linear_speed_controller(float rho);
    float angular_speed_controller(float alfa);
    float normalizeAngle(float a);
    
protected:
    
    float m_linear_accel;
    float m_linear_vmax;
    float m_linear_decel;
    float m_linear_vmin;
    bool m_target_reached;
    float m_next_speed;
    float m_angular_next_speed;
    float m_xT;
    float m_yT;
    float kd, kh;
    float m_alfa;
    float m_rho;
    float m_delta_x;
    float m_delta_y;
    float m_angular_accel;
    float m_angular_decel;
    float m_angular_vmax;
    float m_angular_accel_step;
    float m_angular_decel_distance;
    float m_accel_step;
    float m_decel_distance;
    int m_direction;
    float m_linear_speed;
    float m_angular_speed;
    float m_vl;
    float m_vr;
    
    
};

extern GoTo_Point goto_point;

#endif
