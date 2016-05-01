/*
 * pwm.cpp
 */


#include "defines.h"
#include <p33FJ128MC802.h>
#include "pwm.h"

/* inizializzazione pwm */

void init_pwm()
{
    /* abilito il modulo pwm*/

    P1TCONbits.PTEN = 1;

    /* il modulo non si ferma quando la cpu è in idle */

    P1TCONbits.PTSIDL = 0;

    /* nessun prescaler ne in ingresso ne in uscita */
	
    P1TCONbits.PTOPS  = 0;
    P1TCONbits.PTCKPS = 0;

    /* modalità free running */

    P1TCONbits.PTMOD = 0;

	
    /* inizializzo a zero il contatore del periodo/duty cycle */

    P1TMRbits.PTMR = 0;
	
    /* imposto il periodo in modo tale da avere una frequenza di 18.8khz*/

    /*
       formula: PTPER = [Fcy/(Fpwm * prescaler factor)] -1 abbiamo 18.8khz
       77385000 / 2
    */
	
    P1TPERbits.PTPER = MAX_PWM / 2; 	

    /* impostiamo le modalità di funzionamento dei pin in uscita */

    PWM1CON1bits.PMOD3 = 0; 	// lo lasciamo a 0 poichè non usiamo la coppia 3
    PWM1CON1bits.PMOD2 = 1;		// lo mettiamo a 1 poichè usiamo la coppia 2 e 1 in modalità indipendente
    PWM1CON1bits.PMOD1 = 1;

    PWM1CON1bits.PEN3H = 0; 	// lo disabilitiamo poichè non lo useremo controllato dal modulo pwm
    PWM1CON1bits.PEN2H = 1;
    PWM1CON1bits.PEN1H = 1;		// li abilitiamo ad essere pilotati dal modulo pwm

    PWM1CON1bits.PEN3L = 0;
    PWM1CON1bits.PEN2L = 1;
    PWM1CON1bits.PEN1L = 1;

    /* impostiamo gli override control register per pilotare i pin l1 ed l2 per la direzione delle ruote */


    P1OVDCONbits.POVD1H = 1;
    P1OVDCONbits.POVD1L = 0;

    P1OVDCONbits.POVD2H = 1;
    P1OVDCONbits.POVD2L = 0;
	
    /* definiamo le uscite iniziali dei pin l1(motore sinistro) ed l2(motore destro) direzione delle ruote*/
    /* stiamo supponendo che il segnale basso faccia andare avanti il robot */


    LEFT_DIR = 0;
    RIGHT_DIR = 0;

	
    /*impostiamo modalità immediata di update del registro di duty cicle */
	
    PWM1CON2bits.IUE  = 1;


    /* impostiamo i duty cycle a 0% di default */

    P1DC1 = 0;
    P1DC2 = 0;
	

    /* configurazione del device */
	
    /*
      FPOR.PWMPIN = 1; // dopo un reset  i pin h ed l sono controllati dai registri di PORT finchè non abilitiamo il modulo PWM

      FPOR.HPOL = 1;
      FPOR.LPOL = 1;  // polarità alta quando è attivo il segnale
    */

      /* Disabilitiamo tutti gli interrupt */


    IFS3bits.PWM1IF  = 0;
    IEC3bits.PWM1IE  = 0;
}


/* funzione per impostare il duty cycle e direzione delle ruote */

void set_pwm(int setL, int setR)
{
    /* configuro duty cycle e direzione della ruota destra */

    if (setR >= 0) {
        RIGHT_DIR = 0;  // se è positivo direzione avanti
        P1DC2 = setR;
    }
    else { 		
        RIGHT_DIR = 1;  // se è negativo andiamo indietro
        P1DC2 = -setR;	
    }

    /* configuro il duty cycle e la direzione della ruota sinistra */

    if (setL >= 0) {
        LEFT_DIR = 1;	//se e' positivo vogliamo  andare avanti, ma per andare avanti dobbiamo andare indietro
        P1DC1 = setL; 	
    }
    else { 		
        LEFT_DIR = 0;	//se e' negativo vogliamo andare indietro, ma per andare indietro dobbiamo andare avanti
        P1DC1 = -setL;
    }
}

