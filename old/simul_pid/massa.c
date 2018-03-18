/*
 * massa.c
 */


#include <stdio.h>
#include <stdlib.h>

float v_k = 0;

float T = 0.01;
float M = 1;
float b = 0.5;

float fdt(float f_k)
{
    float v_k1 = (T/M)*f_k + (M - b*T) / M * v_k;
    v_k = v_k1;
    return v_k1;
}

int main(int argc, char *argv[])
{
    int i;
    for (i = 0; i < 10*100;i++) { // 10 secondi di simulazione
        printf("%f\n", fdt(2)); // gradino di 2N
    }
}

