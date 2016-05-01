/*
 * timers.c
 */



#include "defines.h"

#include <p33FJ128MC802.h>


/* inizializzazione timer2 per il controllo in velocita' e posizione */

void init_timer2(void)
{

    T2CONbits.TON = 0; // disabilito il timer2

    T2CONbits.TSIDL = 0 ; //non ci fermiamo in idle mode
    T2CONbits.T32 = 0 ; //timer a 16 bit
    T2CONbits.TCS = 0; // clock interno Fosc/2
    T2CONbits.TGATE = 0 ; //disabilito la modalita' gated timer

    T2CONbits.TCKPS = 0b10; // 1:64 prescaler

    TMR2 =  0;
    PR2 = 3023; // periodo_timer(s)*((Fosc)/(2*prescaler)) :abbiamo un timer a circa 5ms


    IPC1bits.T2IP = 1;	// priorita' a 1
    IEC0bits.T2IE = 0;    // disabilito Timer2 interrupt
    IFS0bits.T2IF = 0; 	// reset del dell' interrupt flag

    T2CONbits.TON = 1; 	// abilito il timer2		

}
