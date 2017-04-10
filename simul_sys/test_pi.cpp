/*
 * main1.cpp
 */

#include <fstream>
#include <math.h>
#include <stdlib.h>
#include "dynamic_system.h"
#include "massa_attrito.h"
#include "second_order.h"
#include "pi_controller.h"
#include "pid_controller.h"


int main(int argc, char **argv)
{
    //MassaAttrito m(1.0, 0.6, 0.01);
    // Massa = 1 kg
    // Coeff.Attrito = 0.6
    // Delta T = 10 ms

    SecondOrder m(1.0, 3, 2, 0.01);
    //SecondOrder m(1.0, 1, 1, 0.01);

    float target = 5;
    float output = 0;
    float d = 0;

    PI_Controller c(5.0, 2.0, 0.01);

    std::ofstream output_file("data.txt");
    // output file dei dati

    // 40 secs di simulazione
    for (int i = 0; i < 20 / 0.01;i++) {

      float error = target - output;
      float controller_output = c.evaluate(error);

      if (controller_output > 100) controller_output = 100; // saturation at 100N
      if (controller_output < -100) controller_output = - 100;

      output = m.evaluate(controller_output);

      //if (i > 600 && i < 1200) output -= 2;
      // if (i % 200 == 0) d = rand()*0.25/RAND_MAX - 0.125;
      // output += d;

      output_file << (i*0.01) << " " << output << std::endl;
    }

    output_file.close();
}
