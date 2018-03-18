/*
 * ecan_config.c
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <libpic30.h>

#include "ecan_config.h"

// Define ECAN Message Buffers
#if defined(__dsPIC33F__)
ECANMSGBUF ecanmsgBuf __attribute__((space(dma),aligned(ECAN_BUFFERS*16)));
#elif defined(__dsPIC33E__)
ECANMSGBUF ecanmsgBuf __attribute__((__far__,aligned(ECAN_BUFFERS*16)));
#endif

/*
 *******************************************************************************************************
  Funzione: ecan_dma_transmit_buffer
 *******************************************************************************************************
 La funzione permette di inizializzare uno degli 8 canali del modulo DMA che consentono il trasferimento
 dei dati da/a le periferiche senza impiegare la CPU.

 In questo caso inizializziamo  il canale tra la DMA RAM (Message Buffer) e la TX DMA Interface
 per effettuare la trasmissione dei dati per il modulo CAN modificando i registri di stato e di controllo.

 - Imposta il DMAxCON=0x2020 per leggere dal DPSRAM address e scrivere al Peripheral Address
   ed abilita Peripheral Indirect Addressing Mode

 - Imposta DMAxPAD=0x0442 per trasmettere con il modulo eCAN1  mentre DMAxPAD=0x0542 per il modulo eCAN2

 - Imposta DMAxCNT=0x0007 indica il numero di elementi da trasferire (0x0001 indica un elemento - 0x0007 indica 8 elementi)

 - Imposta DMAxREQ=0x0046 indicare il servizio richiesto, il primo bit a 0 imposta l'automatic DMA Transfer,
   gli ultimi due bit indica il TX Data Request per il modulo (46 per l' eCAN1 - 47 per l'eCAN2)
*/
void ecan_set_transmit_dma_channel(unsigned int selectChannel)
{
    switch(selectChannel) {
#if defined(__dsPIC33F__)
    case 0:
        DMACS0=0;
        DMA0CON=0x2020;
        DMA0PAD=0x0442;	// ECAN Register (C1TXD)
        DMA0CNT=0x0007;
        DMA0REQ=0x0046;	// IRQ Number = ECAN Transmit
        DMA0STA=  __builtin_dmaoffset(ecanmsgBuf);	
        DMA0CONbits.CHEN=1;
        break;
    case 1:
        DMACS0=0;
        DMA1CON=0x2020;
        DMA1PAD=0x0442;	// ECAN  (C1TXD)
        DMA1CNT=0x0007;
        DMA1REQ=0x0046;	// ECAN  Transmit
        DMA1STA=  __builtin_dmaoffset(ecanmsgBuf);	
        DMA1CONbits.CHEN=1;
        break;
    case 2:
        DMACS0=0;
        DMA2CON=0x2020;
        DMA2PAD=0x0442;	// ECAN  (C1TXD)
        DMA2CNT=0x0007;
        DMA2REQ=0x0046;	// ECAN  Transmit
        DMA2STA=  __builtin_dmaoffset(ecanmsgBuf);	
        DMA2CONbits.CHEN=1;
        break;
    case 3:
        DMACS0=0;
        DMA3CON=0x2020;
        DMA3PAD=0x0442;	// ECAN  (C1TXD)
        DMA3CNT=0x0007;
        DMA3REQ=0x0046;	// ECAN  Transmit
        DMA3STA=  __builtin_dmaoffset(ecanmsgBuf);	
        DMA3CONbits.CHEN=1;
        break;
    case 4:
        DMACS0=0;
        DMA4CON=0x2020;
        DMA4PAD=0x0442;	// ECAN  (C1TXD)
        DMA4CNT=0x0007;
        DMA4REQ=0x0046;	// ECAN  Transmit
        DMA4STA=  __builtin_dmaoffset(ecanmsgBuf);	
        DMA4CONbits.CHEN=1;
        break;
    case 5:
        DMACS0=0;
        DMA5CON=0x2020;
        DMA5PAD=0x0442;	// ECAN  (C1TXD)
        DMA5CNT=0x0007;
        DMA5REQ=0x0046;	// ECAN  Transmit
        DMA5STA=  __builtin_dmaoffset(ecanmsgBuf);	
        DMA5CONbits.CHEN=1;
        break;
    case 6:
        DMACS0=0;
        DMA6CON=0x2020;
        DMA6PAD=0x0442;	// ECAN  (C1TXD)
        DMA6CNT=0x0007;
        DMA6REQ=0x0046;	// ECAN  Transmit
        DMA6STA=  __builtin_dmaoffset(ecanmsgBuf);	
        DMA6CONbits.CHEN=1;
        break;
    case 7:
        DMACS0=0;
        DMA7CON=0x2020;
        DMA7PAD=0x0442;	// ECAN  (C1TXD)
        DMA7CNT=0x0007;
        DMA7REQ=0x0046;	// ECAN  Transmit
        DMA7STA=  __builtin_dmaoffset(ecanmsgBuf);	
        DMA7CONbits.CHEN=1;
        break;
#elif defined(__dsPIC33E__)
    case 0:
        DMARQC=DMAPWC=0;
        DMA0CON=0x2020;
        DMA0PAD=0x0442;	// ECAN  (C1TXD)
        DMA0CNT=0x0007;
        DMA0REQ=0x0046;	// ECAN  Transmit
        DMA0STAL=(unsigned int)ecanmsgBuf;
        DMA0STAH=0;
        DMA0CONbits.CHEN=1;
        break;
    case 1:
        DMARQC=DMAPWC=0;
        DMA1CON=0x2020;
        DMA1PAD=0x0442;	// ECAN  (C1TXD)
        DMA1CNT=0x0007;
        DMA1REQ=0x0046;	// ECAN  Transmit
        DMA1STAL=(unsigned int)ecanmsgBuf;
        DMA1STAH=0;
        DMA1CONbits.CHEN=1;
        break;
    case 2:
        DMARQC=DMAPWC=0;
        DMA2CON=0x2020;
        DMA2PAD=0x0442;	// ECAN  (C1TXD)
        DMA2CNT=0x0007;
        DMA2REQ=0x0046;	// ECAN  Transmit
        DMA2STAL=(unsigned int)ecanmsgBuf;
        DMA2STAH=0;
        DMA2CONbits.CHEN=1;
        break;
    case 3:
        DMARQC=DMAPWC=0;
        DMA3CON=0x2020;
        DMA3PAD=0x0442;	// ECAN  (C1TXD)
        DMA3CNT=0x0007;
        DMA3REQ=0x0046;	// ECAN  Transmit
        DMA3STAL=(unsigned int)ecanmsgBuf;
        DMA3STAH=0;
        DMA3CONbits.CHEN=1;
        break;
#endif
    }
}

/*
 *******************************************************************************************************
  Funzione: ecan_set_dma_receive_buffer
 *******************************************************************************************************

 La funzione permette di inizializzare il modulo DMA e di configurare il canale scelto per collegare
 la DMA RAM (Message buffer)e la RX DMA Interface per la ricezione dei dati per il CAN, modificando i
 registri di stato e di controllo.

 - Imposta il DMAxCON=0x0020 per leggere dal Peripheral address e scrivere al DSPRAM Address ed
   abilita Peripheral Indirect Addressing Mode

 - Imposta DMAxPAD=0x0440 per ricevere con il modulo eCAN1 mentre DMAxPAD=0x0540 per il modulo eCAN2

 - Imposta DMAxCNT=0x0007 indica il numero di elementi da trasferire
   (0x0001 indica un elemento - 0x0007 indica 8 elementi)

 - Imposta DMAxREQ=0x0046 indicare il servizio richiesto, il primo bit a 0 imposta l'automatic DMA Transfer,
   gli ultimi due bit indica il RX Data Request per il modulo (22 per l' eCAN1 - 37 per l'eCAN2)

*/
void ecan_set_receive_dma_channel(unsigned int selectChannel)
{
    switch(selectChannel) {
#if defined(__dsPIC33F__)
    case 0:
        DMACS0=0;
        DMA0CON=0x0020;
        DMA0PAD=0x0440;	// ECAN  (C1RXD)
        DMA0CNT=0x0007;
        DMA0REQ=0x0022;	// ECAN  Receive
        DMA0STA= __builtin_dmaoffset(ecanmsgBuf);	
        DMA0CONbits.CHEN=1;
        break;
    case 1:
        DMACS0=0;
        DMA1CON=0x0020;
        DMA1PAD=0x0440;	// ECAN  (C1RXD)
        DMA1CNT=0x0007;
        DMA1REQ=0x0022;	// ECAN  Receive
        DMA1STA= __builtin_dmaoffset(ecanmsgBuf);	
        DMA1CONbits.CHEN=1;
        break;
    case 2:
        DMACS0=0;
        DMA2CON=0x0020;
        DMA2PAD=0x0440;	// ECAN  (C1RXD)
        DMA2CNT=0x0007;
        DMA2REQ=0x0022;	// ECAN  Receive
        DMA2STA= __builtin_dmaoffset(ecanmsgBuf);	
        DMA2CONbits.CHEN=1;
        break;
    case 3:
        DMACS0=0;
        DMA3CON=0x0020;
        DMA3PAD=0x0440;	// ECAN  (C1RXD)
        DMA3CNT=0x0007;
        DMA3REQ=0x0022;	// ECAN  Receive
        DMA3STA= __builtin_dmaoffset(ecanmsgBuf);	
        DMA3CONbits.CHEN=1;
        break;
    case 4:
        DMACS0=0;
        DMA4CON=0x0020;
        DMA4PAD=0x0440;	// ECAN  (C1RXD)
        DMA4CNT=0x0007;
        DMA4REQ=0x0022;	// ECAN  Receive
        DMA4STA= __builtin_dmaoffset(ecanmsgBuf);	
        DMA4CONbits.CHEN=1;
        break;
    case 5:
        DMACS0=0;
        DMA5CON=0x0020;
        DMA5PAD=0x0440;	// ECAN  (C1RXD)
        DMA5CNT=0x0007;
        DMA5REQ=0x0022;	// ECAN  Receive
        DMA5STA= __builtin_dmaoffset(ecanmsgBuf);	
        DMA5CONbits.CHEN=1;
        break;
    case 6:
        DMACS0=0;
        DMA6CON=0x0020;
        DMA6PAD=0x0440;	// ECAN  (C1RXD)
        DMA6CNT=0x0007;
        DMA6REQ=0x0022;	// ECAN  Receive
        DMA6STA= __builtin_dmaoffset(ecanmsgBuf);	
        DMA6CONbits.CHEN=1;
        break;
    case 7:
        DMACS0=0;
        DMA7CON=0x0020;
        DMA7PAD=0x0440;	// ECAN  (C1RXD)
        DMA7CNT=0x0007;
        DMA7REQ=0x0022;	// ECAN  Receive
        DMA7STA= __builtin_dmaoffset(ecanmsgBuf);	
        DMA7CONbits.CHEN=1;
        break;
#elif defined(__dsPIC33E__)
    case 0:
        DMARQC=DMAPWC=0;
        DMA0CON=0x0020;
        DMA0PAD=0x0440;	// ECAN  (C1RXD)
        DMA0CNT=0x0007;
        DMA0REQ=0x0022;	// ECAN  Receive
        DMA0STAL=(unsigned int)ecanmsgBuf;
        DMA0STAH=0;
        DMA0CONbits.CHEN=1;
        break;
    case 1:
        DMARQC=DMAPWC=0;
        DMA1CON=0x0020;
        DMA1PAD=0x0440;	// ECAN  (C1RXD)
        DMA1CNT=0x0007;
        DMA1REQ=0x0022;	// ECAN  Receive
        DMA1STAL=(unsigned int)ecanmsgBuf;
        DMA1STAH=0;
        DMA1CONbits.CHEN=1;
        break;
    case 2:
        DMARQC=DMAPWC=0;
        DMA2CON=0x0020;
        DMA2PAD=0x0440;	// ECAN  (C1RXD)
        DMA2CNT=0x0007;
        DMA2REQ=0x0022;	// ECAN  Receive
        DMA2STAL=(unsigned int)ecanmsgBuf;
        DMA2STAH=0;
        DMA2CONbits.CHEN=1;
        break;
    case 3:
        DMARQC=DMAPWC=0;
        DMA3CON=0x0020;
        DMA3PAD=0x0440;	// ECAN  (C1RXD)
        DMA3CNT=0x0007;
        DMA3REQ=0x0022;	// ECAN  Receive
        DMA3STAL=(unsigned int)ecanmsgBuf;
        DMA3STAH=0;
        DMA3CONbits.CHEN=1;
        break;
#endif
    }
}





/*
 *******************************************************************************************************
 Funzione: ecan_init_bit_timing
 *******************************************************************************************************
 La funzione permette di impostare il BitTiming in accordo con le specifiche CAN della BOSCH è diviso in 4 segmenti:

 - il Synchronization Segment, il Propagation Time Segment, il Phase Buffer Segment 1 e il Phase Buffer Segment 2
    ognuno dei quali  si può programmare con uno specifico quanto di tempo Tq.(La lunghezza del Tq rappresenta
    l'unità di tempo base del tempo di bit).

 - SJW Synchronization Segment Width (assume valori tra 1 e 4) imposta il SYNC_SEG che è utilizzato dai vari nodi
   per sincronizzare la fase del clock interno

 - phaseSeg1 Phase Segement 1 (assume valori tra 1 e 8) serve a compensare gli errori di sfasamento tra i nodi,
   viene allungato per effettuare le risincronizzazione

 - phaseSeg2 Phase Segement 2 (assume valori tra 1 e 8) serve a compensare gli errori di sfasamento tra i nodi,
   viene accorciato per effettuare la risincronizzazione

 - propSeg Propagation Segment (assume valori tra 1 e 8) serve a compensare gli eventuali ritardi di propagazione
   sul Bus

 - Indica il bitRate della rete

 - BRP Baud Rate Prescaler (assume valori tra 0 e 63)è il valore che viene utilizzato per dividere la frequenza
   generata dall'oscillatore principale

   Usually invoked as: ecan_init_bit_timing(0,4,8,6,5);

   Bit Time = (Sync Segment + Phase Segment 1 + Phase Segment 2 + Propagation Delay ) =
   Bit Time = (     4       +      8          +        6        +        5          ) = 23*TQ

   Internal Clock CAN Speed
   FCY = 77385000/2
   No CAN Prescaler, so FTQ = FCY / 2 = 19346250
   Bit frequency = FTQ/23 ~= 841141.30 BPS

*/
void ecan_init_bit_timing(int BRP,int SJW, int phaseSeg1, int phaseSeg2, int propSeg)
{
    /* FCAN è selezionata per essere FCY */
    /*Forse non c'è bisogno per nei dsPIC33 non è implementato il CANCK viene dato in automatico FCAN=Fcy*/
    //C1CTRL1bits.CANCKS = 0x1; //FCAN = Fcy Se è 0 FCAN=Fosc

    /* Il Baud Rate Prescaler si calcola così ma per comodità inseriamo direttamente noi il valore per selezionare la velocità di lavoro */
    /*CiCFG1<BRP> =(FCAN /(2 ×N×FBAUD))– 1*/
    //int BRP = ((FCY/(2*20*bitRate))-1);
    /* Synchronization Jump Width è SJW-1 */
    C1CFG1bits.SJW = SJW - 1;
    /* Baud Rate Prescaler */
    C1CFG1bits.BRP = BRP;
    /* Phase Segment 1 time è phaseSeg-1 */
    C1CFG2bits.SEG1PH = phaseSeg1 - 1;
    /* Phase Segment 2 time è impostato su programmabile */
    C1CFG2bits.SEG2PHTS = 0x1;
    /* Phase Segment 2 time è phaseSeg-1*/
    C1CFG2bits.SEG2PH = phaseSeg2 - 1;
    /* Propagation Segment time è propseg-1*/
    C1CFG2bits.PRSEG = propSeg - 1;
    /* La Bus line viene campionata 3 volte */
    C1CFG2bits.SAM = 0x1;
}




/*	
 *******************************************************************************************************
 Funzione: ecan_operation_mode
 *******************************************************************************************************

 Questa funzione permette di impostare una tra le Operation Mode,è necessario settare CAN_MODE_CONFIG per:
	-inizializzare il modulo
	-settare la bitRate
	-settare le maschere
	-settare i filtri

*/
void ecan_operation_mode(unsigned int mode)
{
    C1CTRL1bits.REQOP=mode;
    while(C1CTRL1bits.OPMODE!=mode);
}



/*
 *******************************************************************************************************
  Funzione ecan_set_dma_buffer_size
 *******************************************************************************************************

 Se un filtro verifica una corrispondenza con un SID/EID di un messaggio ricevuto,viene generata una
 richiesta di trasferimento al DMA dal modulo eCAN al DMA controllerper copiare automaticamente
 il messaggio ricevuto negli appositi buffer per i messaggi definiti dall'utente nella DMA RAM area.
 Il modulo eCAN supporta fino a 32 buffer nel registro di controllo eCan FIFO CiFCTL<DMABS>
*/

void ecan_set_dma_buffer_size(unsigned int bufferSize)
{
    switch(bufferSize) {
    case 4: C1FCTRLbits.DMABS=0b000;
    case 6: C1FCTRLbits.DMABS=0b001;
    case 8: C1FCTRLbits.DMABS=0b010;
    case 12: C1FCTRLbits.DMABS=0b011;
    case 16: C1FCTRLbits.DMABS=0b100;
    case 24: C1FCTRLbits.DMABS=0b101;
    case 32: C1FCTRLbits.DMABS=0b110;
    }	
}

void ecan_set_fifo_start_area(unsigned int buffer_num)
{
    C1FCTRLbits.FSA = buffer_num;
}

/*
 * ecan_configure_tx_buffer
 * Configure a transmission buffer by specifying:
 * - the priority
 */
void ecan_configure_tx_buffer(unsigned int buffer_num, unsigned int priority)
{
    switch(buffer_num) {
    case 0:
        C1TR01CONbits.TXEN0=1;
        C1TR01CONbits.TX0PRI=priority;
        break;
    case 1:
        C1TR01CONbits.TXEN1=1;
        C1TR01CONbits.TX1PRI=priority;
        break;
    case 2:
        C1TR23CONbits.TXEN2=1;
        C1TR23CONbits.TX2PRI=priority;
        break;
    case 3:
        C1TR23CONbits.TXEN3=1;
        C1TR23CONbits.TX3PRI=priority;
        break;
    case 4:
        C1TR45CONbits.TXEN4=1;
        C1TR45CONbits.TX4PRI=priority;
        break;
    case 5:
        C1TR45CONbits.TXEN5=1;
        C1TR45CONbits.TX5PRI=priority;
        break;
    case 6:
        C1TR67CONbits.TXEN6=1;
        C1TR67CONbits.TX6PRI=priority;
        break;
    case 7:
        C1TR67CONbits.TXEN7=1;
        C1TR67CONbits.TX7PRI=priority;
        break;
    }
}

/*
  Questa funzione permette di inizializzare il modulo eCAN1:
  - richiama all'inizio la modalità di configurazione
  - imposta il bitTiming
  - imposta la dimensione per il buffer di ricezione
  - imposta i filtri e le maschere
*/
void ecan_config(void)
{
    /* Configuration Mode */
    ecan_operation_mode(CAN_MODE_CONFIG);
    ecan_init_bit_timing(0,4,8,6,5);
    ecan_set_dma_buffer_size(ECAN_BUFFERS);		/* Set the number of CAN Message Buffers in DMA RAM */
    ecan_set_fifo_start_area(ECAN_BUFFERS - ECAN_FIFO_SIZE);
    ecan_set_transmit_dma_channel(0);
    ecan_set_receive_dma_channel(1);

    ecan_write_rx_acceptance_mask(0,0x1FFFFFFF,1,0);    // mask 0 -> all messages with SID
    ecan_write_rx_acceptance_mask(1,0x1FFFFFFF,1,1);    // mask 1 -> all messages with EID

    /*Resetta i registri di fullstatus (CiRXFUL1,CiRXFUL2)ed di overflow (CiRXOVF1, CiRXOVF2) per i buffer da 0-31 */
    C1RXFUL1 = C1RXFUL2 = C1RXOVF1 = C1RXOVF2 = 0x0000;

    /* Normal Mode */
    ecan_operation_mode(CAN_MODE_NORMAL);
    //ecan_operation_mode(CAN_MODE_LOOP);
}
