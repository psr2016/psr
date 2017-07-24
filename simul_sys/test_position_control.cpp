/*
 * test_position_control.cpp
 */

#include <fstream>
#include <math.h>
#include <stdlib.h>
#include "dynamic_system.h"
#include "massa_attrito.h"
#include "pid_saturation.h"


int main(int argc, char **argv)
{
    MassaAttrito m(1.0, 0.5, 0.01);
    // Massa = 1 kg
    // Coeff.Attrito = 0.5
    // Delta T = 10 ms

    float v_target, p_target;
    float v_real = 0, p_real = 0;

    p_target = 10; // 10 meters of distance

    PID_Saturation speed_pid(6.0, 3.0, 0, 30, 0.01);
    // saturation at 30N

    PID_Saturation position_pid(0.8, 0, 0, 2, 0.01);
    // saturation at 2 m/s

    std::ofstream output_file("data.txt");
    // output file dei dati

    // 10 secs di simulazione
    for (int i = 0; i < 10 / 0.01;i++) {

      float p_error = p_target - p_real;
      float v_target = position_pid.evaluate(p_error);

      float v_error = v_target - v_real;
      float speed_pid_output = speed_pid.evaluate(v_error);

      v_real = m.evaluate(speed_pid_output);
      p_real = m.get_position();

      output_file << (i*0.01) << " " << p_real << " " << v_real <<
	" " << v_target << std::endl;

    }

    output_file.close();
}
