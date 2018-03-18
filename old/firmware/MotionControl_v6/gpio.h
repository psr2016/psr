/*
 * gpio.h
 */

#ifndef __GPIO_H
#define __GPIO_H

#include "defines.h"

// BOARD VERSION 2
// LED = RB5, Push-pull output, 0 = LED_ON, 1 = LED_OFF
// BUMP_1 = RB4
// BUMP_2 = RA4

#define init_gpio() {                           \
        TRISBbits.TRISB5 = PORT_OUT;            \
        TRISBbits.TRISB4 = PORT_IN;             \
        TRISAbits.TRISA4 = PORT_IN;             \
                                                \
        ODCBbits.ODCB5 = 0;                     \
                                                \
        LATBbits.LATB5 = 1;                     \
    }


#define led_on()    LATBbits.LATB5 = 0
#define led_off()   LATBbits.LATB5 = 1
#define led_toggle()   LATBbits.LATB5 = !LATBbits.LATB5

#define bumper_1    PORTBbits.RB4
#define bumper_2    PORTAbits.RA4


#endif
