/*
 * main1.cpp
 */

#include <fstream>
#include <math.h>
#include "dynamic_system.h"
#include "ex1.h"


int main(int argc, char **argv)
{
    float input = 1; // spinta di 2 N

    G1 g1 = G1(0.031);
    G2 g2 = G2(0.03);

    std::ofstream output_file("data.txt");
    // output file dei dati

    // 40 secs di simulazione
    for (int i = 0; i < 10 / 0.05;i++) {
        float output = g1.evaluate(input);
        output_file << (i*0.03) << " " << output << std::endl;
    }

    output_file.close();
}
