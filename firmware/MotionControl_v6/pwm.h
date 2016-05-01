/*
 * pwm_control.h
 */

#ifndef __PWM_CONTROL
#define __PWM_CONTROL

void init_pwm(void);
void set_pwm(int configS, int configD);

#define MAX_PWM 4095
#define LEFT_DIR    P1OVDCONbits.POUT1L
#define RIGHT_DIR   P1OVDCONbits.POUT2L


#endif
