/*
 * main.cpp
 */

#include <fstream>
#include <math.h>
#include "dynamic_system.h"
#include "massa_attrito.h"
#include "massa_molla.h"
#include "input_sin.h"


int main(int argc, char **argv)
{
    MassaMolla m(0.5, 0.2, 2, 0.01);
    // Massa = 0.5 kg
    // Coeff.Attrito = 0.2
    // k = 2
    // Delta T = 10 ms

    float input = 3; // spinta di 2 N

    Sin s = Sin(10, sqrt(2/0.5), 0.01);

    std::ofstream output_file("data.txt");
    // output file dei dati

    // 40 secs di simulazione
    for (int i = 0; i < 40 / 0.01;i++) {
        float output = m.evaluate(input);
        //float output = m.evaluate(s.evaluate(0));
        output_file << (i*0.01) << " " << output << std::endl;
    }

    output_file.close();
}
