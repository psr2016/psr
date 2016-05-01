/*
 * clocking.c
 */

#include <p33FJ128MC802.h>

#include "defines.h"
#include "clocking.h"

#ifdef EXTERNAL_CLOCK

/* Routines for External Clock with XTAL = 11.059200 MHz */

// Select Internal FRC at POR
_FOSCSEL(FNOSC_FRC);
// Enable Clock Switching and Configure Posc in HS mode
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON & POSCMD_HS);

void init_clock()
{
    // Configure PLL prescaler, PLL postscaler, PLL divisor
    PLLFBD = 26; // M = 28
    CLKDIVbits.PLLPOST = 0;// N2 = 2
    CLKDIVbits.PLLPRE = 0; // N1 = 2

    // Initiate Clock Switch to Primary Oscillator with PLL (NOSC = 0b011)
    __builtin_write_OSCCONH(0x03);
    __builtin_write_OSCCONL(0x01);

    // Wait for Clock switch to occur
    while (OSCCONbits.COSC != 0b011);

    // Wait for PLL to lock
    while(OSCCONbits.LOCK!= 1) ;
}
#endif

#ifdef INTERNAL_CLOCK

/* Routines for Internal Clock */

// Select Internal FRC at POR
_FOSCSEL(FNOSC_FRC);
// Enable Clock Switching and Configure
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON);

void init_clock()
{
    OSCTUNbits.TUN = 0;

    // Configure PLL prescaler, PLL postscaler, PLL divisor
    PLLFBD = 40;           // M = 42
    CLKDIVbits.PLLPOST=0; // N2 = 2
    CLKDIVbits.PLLPRE=0;   // N1 = 2

    // Initiate Clock Switch to Internal FRC with PLL (NOSC = 0b001)
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(0x01);

    // Wait for Clock switch to occur
    while (OSCCONbits.COSC != 0b001) ;

    // Wait for PLL to lock
    while (OSCCONbits.LOCK != 1) ;

    //
    // clock = 7.37 * (40 + 2) / ( 2 * 2 ) = 77.385
    //

}

#endif

