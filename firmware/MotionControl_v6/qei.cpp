/*
 * qei.c
 */

#include "defines.h"

#include <p33FJ128MC802.h>



void init_qei(void)
{
    ADPCFG |= 0xFFFF; // Configure QEI1 and QEI2 pins as digital inputs
	
    //*********************************************************************
    //                      INIZIALIZZAZIONE QEI 1
    //*********************************************************************

    //  Configure QEI pins as digital inputs
    TRISBbits.TRISB0=1;
    TRISBbits.TRISB1=1;

    //	Assign QEA1,QEB1 to the corresponding pins
    RPINR14bits.QEA1R = 0b00000; 	//Rp0
    RPINR14bits.QEB1R = 0b00001; 	//Rp1

    //	Index pin tied to VSS
    RPINR15bits.INDX1R = 0b11111;       // VSS

    MAX1CNT=0xffff;

    QEI1CONbits.QEIM    = 0;	// Disable QEI Module
    QEI1CONbits.CNTERR  = 0; 	// Clear any count errors
    QEI1CONbits.QEISIDL = 0; 	// Continue operation during sleep
    QEI1CONbits.SWPAB   = 0;	// QEA and QEB not swapped
    QEI1CONbits.PCDOUT  = 1; 	// QEI logic controls state of I/O pin
    QEI1CONbits.POSRES  = 0; 	// Index pulse does not reset position counter
    POS1CNT = 0; 				// Reset position counter

    //IPC14bits.QEI1IP = 2;
    IEC3bits.QEI1IE = 0;	// Disable QEI1 interrupts	
    IFS3bits.QEI1IF = 0;	// Clear QEI1 interrupt flag

    QEI1CONbits.QEIM = 7; 	// Quadrature Encoder Interface Mode Select bits :
                                // x4 mode with position counter reset by match MAX1CNT

    //*********************************************************************
    //                      INIZIALIZZAZIONE QEI 2
    //*********************************************************************

    // Configure QEI pins as digital inputs
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB3 = 1;

    //	Assign QEA2,QEB2 to the corresponding pins
    RPINR16bits.QEA2R = 0b00010; 	//Rp2
    RPINR16bits.QEB2R = 0b00011; 	//Rp3

    //	Index pin tied to VSS
    RPINR17bits.INDX2R = 0b11111;       // VSS

    MAX2CNT=0xffff;

    QEI2CONbits.QEIM    = 0; 	// Disable QEI Module
    QEI2CONbits.CNTERR  = 0; 	// Clear any count errors
    QEI2CONbits.QEISIDL = 0; 	// Continue operation during sleep
    QEI2CONbits.SWPAB   = 0; 	// QEA and QEB not swapped
    QEI2CONbits.PCDOUT  = 1; 	// QEI logic controls state of I/O pin
    QEI2CONbits.POSRES  = 0; 	// Index pulse does not reset position counter
    POS2CNT = 0; 				// Reset position counter

    //IPC18bits.QEI2IP = 3;
    IEC4bits.QEI2IE  = 0;    	// Disable QEI2 interrupts
    IFS4bits.QEI2IF  = 0;    	// Clear QEI2 interrupt flag

    QEI2CONbits.QEIM = 7; 	// Quadrature Encoder Interface Mode Select bits :
                                // x4 mode with position counter reset by match MAX1CNT
}


int qei_position_1()
{
    return POS1CNT;
}

int qei_position_2()
{
    return POS2CNT;
}

