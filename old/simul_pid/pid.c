/*
 * pid.c
 */


#include <stdio.h>
#include <stdlib.h>

float f_1 = 0, v_1 = 0;

float T = 0.01;
float M = 1;
float b = 0.5;

// f(t) = M a(t) + b v(t)
// M = mass
// b = friction constant

float mass(float f)
{
    float v = (T/M)*f_1 + (M - b*T) / M * v_1;
    f_1 = f;
    v_1 = v;
    return v;
}

float Kp = 0;
float Ki = 0;
float Kd = 0;
float SATURATION_VALUE = 10;
float integral_factor = 0;
float error_1 = 0;
int saturated = 0;

float PID(float target, float current)
{
    float error = target - current;
    float out_P = Kp * error;
    float out_I = integral_factor + Ki * error * T;
    float out_D = Kd * (error - error_1) / T ;
    float output = out_P + out_I + out_D;

    integral_factor = out_I;

    return output;
}

float PID_SAT(float target, float current)
{
    float error = target - current;
    float out_P = Kp * error;

    float out_I;
    if (!saturated) {
        out_I = integral_factor + Ki * error * T;
        integral_factor = out_I;
    }
    else
        out_I = 0;	

    float out_D = Kd * (error - error_1) / T ;
    float output = out_P + out_I + out_D;

    if (output > SATURATION_VALUE) {
        output = SATURATION_VALUE;
        saturated = 1;
    }
    else if (output < -SATURATION_VALUE) {
        output = -SATURATION_VALUE;
        saturated = 1;
    }
    else
        saturated = 0;

    return output;
}

float climb(int i)
{
    if (i > 7*100) {
        return -3;
    }
    else
        return 0;
}

int main(int argc, char *argv[])
{
    int i;
    float current_speed;
    float target_speed = 8; // 8 m/s

    Kp = atof(argv[1]);
    Ki = atof(argv[2]);
    Kd = atof(argv[3]);

    current_speed = 0;
    for (i = 0; i < 10*100;i++) { // 10 secondi di simulazione
        float pi_output = PID_SAT(target_speed, current_speed);
        //printf("%f\n", target_speed - current_speed);
        //printf("%f\n", pi_output);
        current_speed = mass(pi_output);
        printf("%f\n", current_speed);
    }
}
