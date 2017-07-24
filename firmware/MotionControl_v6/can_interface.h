/*
 * can_interface.h
 */

#ifndef __CAN_INTERFACE_H
#define __CAN_INTERFACE_H

#include "defines.h"
#include <p33FJ128MC802.h>
#include <libpic30++.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "speed_control.h"
#include "kinematics.h"
#include "Controllore.h"

#include "ecan_lib.h"
#include "bus_objects.h"


class CanCommandReceiver {
 public:
    static CanCommandReceiver * instance;
    CanCommandReceiver(Kinematics & kinematics, SpeedControlTask & speed);
    void run();
    void process_command(const t_can_motion_command* cmd);
 private:
    Kinematics & m_kinematics;
    SpeedControlTask & m_speed_controller;
};

class CanPoseSender : public PeriodicTask {
 public:
    CanPoseSender(Pose & robot_pose);
    void run();
 private:
    Pose & m_robot_pose;
};

class CanSpeedSender : public PeriodicTask {
 public:
    CanSpeedSender(SpeedControlTask & speed);
    void run();
 private:
    SpeedControlTask & m_speed_controller;
};

class CanGyroSender : public PeriodicTask {
 public:
    CanGyroSender(Controllore & gyro_ctrl);
    void run();
 private:
    Controllore & m_gyro_controller;
};

// ---------------------------------------------------------------------------------

#define MOTION_COMMAND_OBJECT		0
#define OBSTACLE_AVOIDANCE_OBJECT	1
#define REMOTE_STDIO_RX_OBJECT		2

void init_can_interface(void);


#endif
