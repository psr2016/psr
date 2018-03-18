/*
 * main.cpp
 */

#include <iostream>
#include <string.h>
#include "mass.h"
#include "pid.h"
#include "position_controller.h"

using namespace std;

#define SAMPLE_FREQ  100
#define SAMPLE_INTERVAL  (1.0/SAMPLE_FREQ)

bool show_pos = false;
bool show_speed = false;

// M = 1, b = 0.5
Mass mass(1, 0.5, SAMPLE_INTERVAL);

// kp = 6, ki = 3, kd = 0, saturation = 30N
Pid speed_control(6, 3, 0, SAMPLE_INTERVAL, 30);

void speed_control_simlation()
{
    float v_target = 8;

    if (show_speed) cout << "t v_target v_current" << endl;
    if (show_pos) cout << "t p_target p_current" << endl;

    for (int i = 0; i < 10*SAMPLE_FREQ;i++) {
        float f = speed_control.evaluate(v_target, mass.get_speed());
        mass.apply_force(f);
        if (show_speed) cout << i << " " << v_target << " " << mass.get_speed() << endl;
        if (show_pos) cout << i << " " << 0 << " " << mass.get_position() << endl;
    }
}


// kp = 0.5
// max 2 m/s of speed
Pid position_control(0.5, 0, 0, SAMPLE_INTERVAL, 2);


void position_control_simlation()
{
    float position_target = 10;
    float v_target = 0;

    if (show_speed) cout << "t v_target v_current" << endl;
    if (show_pos) cout << "t p_target p_current" << endl;

    for (int i = 0; i < 10*SAMPLE_FREQ;i++) {
        if ( (i % 5) == 0) {
            // run position control after each 5 runs of speed control
            v_target = position_control.evaluate(position_target, mass.get_position());
        }
        float f = speed_control.evaluate(v_target, mass.get_speed());
        mass.apply_force(f);

        if (show_speed) cout << i << " " << v_target << " " << mass.get_speed() << endl;
        if (show_pos) cout << i << " " << position_target << " " << mass.get_position() << endl;
    }
}



// accel 0.3 m/s^2
// max 1 m/s
// decel 0.2 m/s^2
PositionController position_controller_2(0.3, 1, 0.2, SAMPLE_INTERVAL*5);

void position_control_with_profile_simlation()
{
    float position_target = 8;
    float v_target = 0;

    if (show_speed) cout << "t v_target v_current" << endl;
    if (show_pos) cout << "t p_target p_current" << endl;

    for (int i = 0; i < 16*SAMPLE_FREQ;i++) {
        if ( (i % 5) == 0) {
            // run position control after each 5 runs of speed control
            v_target = position_controller_2.evaluate(position_target, mass.get_position(), mass.get_speed());
        }
        float f = speed_control.evaluate(v_target, mass.get_speed());
        mass.apply_force(f);

        if (show_speed) cout << i << " " << v_target << " " << mass.get_speed() << endl;
        if (show_pos) cout << i << " " << position_target << " " << mass.get_position() << endl;
    }
}



int main(int argc, char ** argv)
{
    if (argc > 1) {
        if (!strcmp(argv[1], "pos")) show_pos = true;
        else if (!strcmp(argv[1], "speed")) show_speed = true;
    }
    //speed_control_simlation();
    //position_control_simlation();
    position_control_with_profile_simlation();
}
