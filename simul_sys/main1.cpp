/*
 * main1.cpp
 */

#include <fstream>
#include <math.h>
#include "dynamic_system.h"
#include "massa_attrito.h"
#include "massa_molla.h"


int main(int argc, char **argv)
{
    MassaAttrito m(1.0, 0.6, 0.01);
    // Massa = 1 kg
    // Coeff.Attrito = 0.6
    // Delta T = 10 ms

    float input = 3; // spinta di 2 N

    std::ofstream output_file("data.txt");
    // output file dei dati

    // 40 secs di simulazione
    for (int i = 0; i < 40 / 0.01;i++) {
        float output = m.evaluate(input);
        output_file << (i*0.01) << " " << output << std::endl;
    }

    output_file.close();
}
