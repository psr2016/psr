/*
 * time_defines
 */

#ifndef __TIME_DEFINES
#define __TIME_DEFINES

#include "timers.h"

#define TIME_UNIT          5 // milliseconds (see timers.h)

// tasks at 5ms
#define KINEMATICS_PERIOD    1
#define KINEMATICS_JITTER    0

// tasks at 10ms
#define SPEED_PERIOD         2
#define SPEED_JITTER         1

// tasks at 40ms
#define SPEED_SENDER_PERIOD      8
#define SPEED_SENDER_JITTER      2

// tasks at 250ms
#define POSE_SENDER_PERIOD       50
#define POSE_SENDER_JITTER       10

#endif
