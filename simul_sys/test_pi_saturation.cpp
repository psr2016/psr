/*
 * test_pi_saturation.cpp
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

    float target = 8;
    float output = 0;

    PID_Saturation c(6.0, 3.0, 0, 10, 0.01);
    // Kp = 6
    // Ki = 3
    // Kd = 0
    // Saturation = 10N

    std::ofstream output_file("data.txt");
    // output file dei dati

    // 20 secs di simulazione
    for (int i = 0; i < 20 / 0.01;i++) {

      float error = target - output;
      float controller_output = c.evaluate(error);

      output = m.evaluate(controller_output);

      output_file << (i*0.01) << " " << output << std::endl;
    }

    output_file.close();
}
