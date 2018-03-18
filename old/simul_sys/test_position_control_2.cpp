/*
 * test_position_control_2.cpp
 */

#include <fstream>
#include <math.h>
#include <stdlib.h>
#include "dynamic_system.h"
#include "massa_attrito.h"
#include "pid_saturation.h"
#include "position_controller.h"


int main(int argc, char **argv)
{
    MassaAttrito m(1.0, 0.5, 0.01);
    // Massa = 1 kg
    // Coeff.Attrito = 0.5
    // Delta T = 10 ms

    float v_target, p_target;
    float v_real = 0, p_real = 0;

    p_target = 4; // 4 meters of distance

    PID_Saturation speed_pid(6.0, 6.0, 0, 30, 0.01);
    // saturation at 30N

    PositionController position_controller(0.8, 2, 0.8, 0.01);
    // acceleration 0.8 m/s^2
    // vmax 2 m/s
    // deceleration 0.8 m/s^2

    std::ofstream output_file("data.txt");
    // output file dei dati

    // 12 secs di simulazione
    for (int i = 0; i < 12 / 0.01;i++) {

        float v_target = position_controller.evaluate(p_target, p_real, v_real);

        float v_error = v_target - v_real;
        float speed_pid_output = speed_pid.evaluate(v_error);

        v_real = m.evaluate(speed_pid_output);
        p_real = m.get_position();

        output_file << (i*0.01) << " " << v_real << " "  << v_target << " " << p_real << std::endl;

    }

    output_file.close();
}
