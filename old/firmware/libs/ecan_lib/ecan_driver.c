/**********************************************************************
* © 2005 Microchip Technology Inc.
*
* FileName:        main.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       dsPIC33Fxxxx
* Compiler:        MPLAB® C30 v3.00 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and
* intellectual property rights in the code accompanying this message and in all
* derivatives hereto.  You may use this code, and any derivatives created by
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY),
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL,
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and
* determining its suitability.  Microchip has no obligation to modify, test,
* certify, or support the code.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author          	Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Vinaya Skanda 	10/18/06  First release of source file
* Vinaya Skanda		07/25/07  Updates from Joe Supinsky and Jatinder Gharoo incorporated
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* This code is tested on Explorer-16 board with ECAN PICTail Card.
* The device used is dsPIC33FJ256GP710 controller
*************************************************************************************************/



#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpic30.h>

#include "ecan_config.h"

/*
This function configures Acceptance Filter "n"

Inputs:
n-> Filter number [0-15]
identifier-> Bit ordering is given below
Filter Identifier (29-bits) : 0b000f ffff ffff ffff ffff ffff ffff ffff
								   |____________|_____________________|
									  SID10:0           EID17:0


Filter Identifier (11-bits) : 0b0000 0000 0000 0000 0000 0fff ffff ffff
														  |___________|
															  SID10:0
exide -> "0" for standard identifier
		 "1" for Extended identifier

bufPnt -> Message buffer to store filtered message [0-15]
maskSel -> Optinal Masking of identifier bits [0-3]
						
*/

/*seleziono il buffer per il filtri da 0 a 3*/
static void ecan_config_bufpntFilter1(unsigned int selectFilter,unsigned int selectBuf){
     switch(selectFilter){
	 case 0:C1BUFPNT1bits.F0BP=selectBuf;break;
	 case 1:C1BUFPNT1bits.F1BP=selectBuf;break;
	 case 2:C1BUFPNT1bits.F2BP=selectBuf;break;
	 case 3:C1BUFPNT1bits.F3BP=selectBuf;break;
     }
}
/*seleziono il buffer per il filtri da 4 a 7*/
static void ecan_config_bufpntFilter2(unsigned int selectFilter, unsigned int selectBuf){
     selectFilter=selectFilter-4;
     switch(selectFilter){
         case 0:C1BUFPNT2bits.F4BP=selectBuf;break;
	 case 1:C1BUFPNT2bits.F5BP=selectBuf;break;
	 case 2:C1BUFPNT2bits.F6BP=selectBuf;break;
	 case 3:C1BUFPNT2bits.F7BP=selectBuf;break;
     }
}
/*seleziono il buffer per il filtri da 8 a 11*/
static void ecan_config_bufpntFilter3(unsigned int selectFilter, unsigned int selectBuf){
selectFilter=selectFilter-8;
     switch(selectFilter){
         case 0:C1BUFPNT3bits.F8BP=selectBuf;break;
	 case 1:C1BUFPNT3bits.F9BP=selectBuf;break;
	 case 2:C1BUFPNT3bits.F10BP=selectBuf;break;
	 case 3:C1BUFPNT3bits.F11BP=selectBuf;break;
     }
}
/*seleziono il buffer per il filtri da 12 a 15*/

static void ecan_config_bufpntFilter4(unsigned int selectFilter, unsigned int selectBuf){
selectFilter=selectFilter-12;
     switch(selectFilter){
         case 0:C1BUFPNT4bits.F12BP=selectBuf;break;
	 case 1:C1BUFPNT4bits.F13BP=selectBuf;break;
	 case 2:C1BUFPNT4bits.F14BP=selectBuf;break;
	 case 3:C1BUFPNT4bits.F15BP=selectBuf;break;
    }
}


/*	Filter Configuration

	ecan1WriteRxAcptFilter(int n, long identifier, unsigned int exide,unsigned int bufPnt,unsigned int maskSel)

	n = 0 to 15 -> Filter number

	identifier -> SID <10:0> : EID <17:0>

	exide = 0 -> Match messages with standard identifier addresses
	exide = 1 -> Match messages with extended identifier addresses

	bufPnt = 0 to 14  -> RX Buffer 0 to 14
	bufPnt = 15 -> RX FIFO Buffer

	maskSel = 0	->	Acceptance Mask 0 register contains mask
	maskSel = 1	->	Acceptance Mask 1 register contains mask
	maskSel = 2	->	Acceptance Mask 2 register contains mask
	maskSel = 3	->	No Mask Selection
	
*/
void ecan_write_rx_acceptance_filter(int n, long identifier, unsigned int exide, unsigned int bufPnt,unsigned int maskSel)
{

    unsigned long sid10_0=0, eid15_0=0, eid17_16=0;
    unsigned int *sidRegAddr,*maskSelRegAddr, *fltEnRegAddr;
    //unsigned int *bufPntRegAddr;
    int aux;
    C1CTRL1bits.WIN=1;

    // Ricavo gli indirizzi dei registri CiRXFnSID, CiBUFPNTn, CiFMSKSELn e CiFEN per il filtro "n"
    sidRegAddr = (unsigned int *)(&C1RXF0SID + (n << 1)); //Registro che specifica il SID per il filtro d'accettazione indicato in n
    //bufPntRegAddr = (unsigned int *)(&C1BUFPNT1 + (n >> 2));//Specifica il buffer dove salvare il messaggio accettato dal filtro
    maskSelRegAddr = (unsigned int *)(&C1FMSKSEL1 + (n >> 3));//Specifica la maschera per i filtri di accettazione
    fltEnRegAddr = (unsigned int *)(&C1FEN1); //Registro che abilita i vari filtri (0 disabilita 1 abilita)


    // Bit-filed manupulation to write to Filter identifier register
    if(exide == 1) { 	// Se usiamo i filtri per l'EID
        eid15_0 = (identifier & 0xFFFF);
        eid17_16= (identifier>>16) & 0x3;
        sid10_0 = (identifier>>18) & 0x7FF;

        *sidRegAddr=(((sid10_0)<<5) + 0x8) + eid17_16;	// Write to CiRXFnSID Register
	    *(sidRegAddr+1)= eid15_0;					// Write to CiRXFnEID Register

    }
    else {			// Se usiamo i filtri per le SID
        sid10_0 = (identifier & 0x7FF);			
        *sidRegAddr=(sid10_0)<<5;					// Write to CiRXFnSID Register
        *(sidRegAddr+1)=0;							// Write to CiRXFnEID Register
    }
    aux = n>>2;
    switch(aux){ // n può andare da 0-15
	case 0:ecan_config_bufpntFilter1(n,bufPnt);break;// seleziono il buffer per il filtri da 0 a 3
	case 1:ecan_config_bufpntFilter2(n,bufPnt);break;// seleziono il buffer per il filtri da 4 a 7
	case 2:ecan_config_bufpntFilter3(n,bufPnt);break;// seleziono il buffer per il filtri da 8 a 11
	case 3:ecan_config_bufpntFilter4(n,bufPnt);break;// seleziono il buffer per il filtri da 12 a 15

    }

    /*Seleziono uno dei 4 buffer indicato dal bufPunt:
      - C1BUFPNT1 Indica il buffer per i messaggi che deve essere usato dai filtri (0-3)
      - C1BUFPNT2 Indica il buffer per i messaggi che deve essere usato dai filtri (4-7)
      - C1BUFPNT3 Indica il buffer per i messaggi che deve essere usato dai filtri (8-11)
      - C1BUFPNT4 Indica il buffer per i messaggi che deve essere usato dai filtri (12-15)*/
    //*bufPntRegAddr = (*bufPntRegAddr) & (0xFFFF - (0xF << (4 *(n & 3)))); // clear nibble
    //*bufPntRegAddr = ((bufPnt << (4 *(n & 3))) | (*bufPntRegAddr));       // Write to C1BUFPNTn Register

    /*Seleziono una delle 2 maschere indicata dal maskSel:
      - C1FMSKSEL1 Seleziona la maschera per i filtri (0-7)
      - C1FMSKSEL2 Seleziona la maschera per i filtri (8-15)*/
    *maskSelRegAddr = (*maskSelRegAddr) & (0xFFFF - (0x3 << ((n & 7) * 2))); // clear 2 bits
    *maskSelRegAddr = ((maskSel << (2 * (n & 7))) | (*maskSelRegAddr));      // Write to C1FMSKSELn Register

    /*Abilita il filtro selezionato*/
    *fltEnRegAddr = ((0x1 << n) | (*fltEnRegAddr)); // Write to C1FEN1 Register

    C1CTRL1bits.WIN=0;
}


/*
This function configures Acceptance Filter Mask "m"

Inputs:
m-> Mask number [0-2]
identifier-> Bit ordering is given below
Filter Mask Identifier (29-bits) : 0b000f ffff ffff ffff ffff ffff ffff ffff
								        |____________|_____________________|
									        SID10:0           EID17:0


Filter Mask Identifier (11-bits) : 0b0000 0000 0000 0000 0000 0fff ffff ffff
														       |___________|
															      SID10:0

mide ->  "0"  Match either standard or extended address message if filters match
         "1"  Match only message types that correpond to 'exide' bit in filter
					
*/

/*	Mask Configuration

	ecan1WriteRxAcptMask(int m, long identifierMask, unsigned int mide, unsigned int exide)

	m = 0 to 2 -> Mask Number

	identifier -> SID <10:0> : EID <17:0>

	mide = 0 -> Match either standard or extended address message if filters match
	mide = 1 -> Match only message types that correpond to 'exide' bit in filter

	exide = 0 -> Match messages with standard identifier addresses
	exide = 1 -> Match messages with extended identifier addresses
	
*/
void ecan_write_rx_acceptance_mask(int m, long identifier, unsigned int mide, unsigned int exide)
{

    unsigned long sid10_0=0, eid15_0=0, eid17_16=0;
    unsigned int *maskRegAddr;

    C1CTRL1bits.WIN=1;

    // Ricavo l'indirizzo del registro CiRXMmSID per la maschera numero "m"
    maskRegAddr = (unsigned int *)(&C1RXM0SID + (m << 1)); //Registro che specifica l'ID per la maschera m

    // Bit-filed manupulation to write to Filter Mask register
    if (exide == 1) {
        //Se usiamo le maschere per le EID
        // Ricavo l'EID
        eid15_0 = (identifier & 0xFFFF);
        eid17_16= (identifier>>16) & 0x3;
        sid10_0 = (identifier>>18) & 0x7FF;

        if (mide == 1)  //Accoppia solo gli standard message
            *maskRegAddr=((sid10_0)<<5) + 0x0008 + eid17_16;	// Write to CiRXMnSID Register
        else // Accoppia entrambi
            *maskRegAddr=((sid10_0)<<5) + eid17_16;	// Write to CiRXMnSID Register
        *(maskRegAddr+1)= eid15_0;					// Write to CiRXMnEID Register

    }
    else {
        //Se usiamo i filtri per le SID
        //Ricavo l'SID
        sid10_0 = (identifier & 0x7FF);			
        if (mide == 1)
            *maskRegAddr=((sid10_0)<<5) + 0x0008;					// Write to CiRXMnSID Register
        else
            *maskRegAddr=(sid10_0)<<5;					// Write to CiRXMnSID Register	
		
        *(maskRegAddr+1)=0;							// Write to CiRXMnEID Register
    }

    C1CTRL1bits.WIN=0;
}


/* ECAN Transmit Message Buffer Configuration

Inputs:
buf	-> Transmit Buffer Number

txIdentifier ->	

Extended Identifier (29-bits) : 0b000f ffff ffff ffff ffff ffff ffff ffff
                                     |____________|_____________________|
                                            SID10:0           EID17:0



Standard Identifier (11-bits) : 0b0000 0000 0000 0000 0000 0fff ffff ffff
                                                            |___________|
                                                                  SID10:0

Standard Message Format:
                                            Word0 : 0b000f ffff ffff ffff
                                                         |____________|||___
                                                             SID10:0   SRR   IDE

                                            Word1 : 0b0000 0000 0000 0000
                                                           |____________|
                                                              EID17:6

                                            Word2 : 0b0000 00f0 0000 ffff
                                                      |_____||           |__|
                                                      EID5:0 RTR         DLC



Extended Message Format:
                                            Word0 : 0b000f ffff ffff ffff
                                                         |____________|||___
                                                             SID10:0   SRR   IDE

                                            Word1 : 0b0000 ffff ffff ffff
                                                           |____________|
                                                              EID17:6

                                            Word2 : 0bffff fff0 0000 ffff
                                                      |_____||           |__|
                                                      EID5:0 RTR         DLC

ide -> "0"  Message will transmit standard identifier
       "1"  Message will transmit extended identifier



remoteTransmit -> "0" Message transmitted is a normal message
                  "1" Message transmitted is a remote message

                Standard Message Format:
                                            Word0 : 0b000f ffff ffff ff1f
                                                         |____________|||___
                                                             SID10:0   SRR   IDE

                                            Word1 : 0b0000 0000 0000 0000
                                                           |____________|
                                                              EID17:6

                                            Word2 : 0b0000 0010 0000 ffff
                                                      |_____||           |__|
                                                      EID5:0 RTR         DLC



                Extended Message Format:
                                            Word0 : 0b000f ffff ffff ff1f
                                                         |____________|||___
                                                             SID10:0   SRR   IDE

                                            Word1 : 0b0000 ffff ffff ffff
                                                           |____________|
                                                              EID17:6

                                            Word2 : 0bffff ff10 0000 ffff
                                                      |_____||           |__|
                                                      EID5:0 RTR         DLC

*/

void ecan_write_tx_message_id(unsigned int buf, long txIdentifier, unsigned int ide, unsigned int remoteTransmit)
{

    unsigned long word0 =0, word1 = 0, word2 = 0;
    unsigned long sid10_0 =0, eid5_0 = 0, eid17_6 = 0;


    if (ide) {	//Imposta l'identificatore per le EID
        eid5_0  = (txIdentifier & 0x3F);
        eid17_6 = (txIdentifier>>6) & 0xFFF;
        sid10_0 = (txIdentifier>>18) & 0x7FF;
        word1 = eid17_6;
    }
    else {
        //Imposta l'identificatore per le SID
        sid10_0 = (txIdentifier & 0x7FF);
    }

    if(remoteTransmit == 1) { 	// Transmit Remote Frame
        word0 = ((sid10_0 << 2) | ide | 0x2);
        word2 = ((eid5_0 << 10)| 0x0200);
    }
    else {
        word0 = ((sid10_0 << 2) | ide);
        word2 = (eid5_0 << 10);
    }
			
// Obtain the Address of Transmit Buffer in DMA RAM for a given Transmit Buffer number

    if(ide)
	ecanmsgBuf[buf][0] = (word0 | 0x0002);
    else
	ecanmsgBuf[buf][0] = word0;

    ecanmsgBuf[buf][1] = word1;
    ecanmsgBuf[buf][2] = word2;
}


/* ECAN Transmit Data

Inputs :
buf -> Transmit Buffer Number

dataLength -> Length of Data in Bytes to be transmitted

data1/data2/data3/data4 ->  Transmit Data Bytes

*/

void ecan_write_tx_message_data(unsigned int buf, unsigned int dataLength,
                                unsigned int data1, unsigned int data2, unsigned int data3, unsigned int data4)
{
    ecanmsgBuf[buf][2] = ((ecanmsgBuf[buf][2] & 0xFFF0) + dataLength);

    ecanmsgBuf[buf][3] = data1;
    ecanmsgBuf[buf][4] = data2;
    ecanmsgBuf[buf][5] = data3;
    ecanmsgBuf[buf][6] = data4;
}

void ecan_write_tx_message_data_buf(unsigned int buf, unsigned char * data, unsigned int dataLength)
{
    ecanmsgBuf[buf][2] = ((ecanmsgBuf[buf][2] & 0xFFF0) + dataLength);
    memcpy(&ecanmsgBuf[buf][3], data, dataLength);
}


void ecan_trigger_tx(int buf_num)
{
    switch (buf_num) {
    case 0: C1TR01CONbits.TXREQ0 = 1; break;
    case 1: C1TR01CONbits.TXREQ1 = 1; break;
    case 2: C1TR23CONbits.TXREQ2 = 1; break;
    case 3: C1TR23CONbits.TXREQ3 = 1; break;
    case 4: C1TR45CONbits.TXREQ4 = 1; break;
    case 5: C1TR45CONbits.TXREQ5 = 1; break;
    case 6: C1TR67CONbits.TXREQ6 = 1; break;
    case 7: C1TR67CONbits.TXREQ7 = 1; break;
    }
}

int ecan_get_trigger_tx(int buf_num)
{
    switch (buf_num) {
    case 0: return C1TR01CONbits.TXREQ0;
    case 1: return C1TR01CONbits.TXREQ1;
    case 2: return C1TR23CONbits.TXREQ2;
    case 3: return C1TR23CONbits.TXREQ3;
    case 4: return C1TR45CONbits.TXREQ4;
    case 5: return C1TR45CONbits.TXREQ5;
    case 6: return C1TR67CONbits.TXREQ6;
    case 7: return C1TR67CONbits.TXREQ7;
    default: return 0;
    }
}

unsigned int * ecan_get_buffer_ptr(int buf_num)
{
    return &ecanmsgBuf[buf_num][0];
}

int ecan_get_tx_condition(int buf_num)
{
    switch (buf_num) {
    case 0: return (C1TR01CON & 0xff);
    case 1: return (C1TR01CON >> 8);
    case 2: return (C1TR23CON & 0xff);
    case 3: return (C1TR23CON >> 8);
    case 4: return (C1TR45CON & 0xff);
    case 5: return (C1TR45CON >> 8);
    case 6: return (C1TR67CON & 0xff);
    case 7: return (C1TR67CON >> 8);
    default: return 0;
    }
}

bool ecan_get_rxful(int buf_num)
{
    if (buf_num < 16)
        return (C1RXFUL1 & (1 << buf_num)) != 0;
    else
        return (C1RXFUL2 & (1 << (buf_num - 16))) != 0;
}

void ecan_clear_rxful(int buf_num)
{
    if (buf_num < 16)
        C1RXFUL1 &= ~(1 << buf_num);
    else
        C1RXFUL2 &= ~(1 << (buf_num - 16));
}

/*
------------------------------------------------------------------------------
 Disable RX Acceptance Filter
 void ecan1DisableRXFilter(int n)
------------------------------------------------------------------------------

n -> Filter number [0-15]
*/

void ecan_disable_rx_filter(int n)
{
    unsigned int *fltEnRegAddr;
    C1CTRL1bits.WIN=1;
    fltEnRegAddr = (unsigned int *)(&C1FEN1);
    *fltEnRegAddr = (*fltEnRegAddr) & (0xFFFF - (0x1 << n));
    C1CTRL1bits.WIN=0;
}
