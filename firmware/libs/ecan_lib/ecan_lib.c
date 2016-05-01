/*
 * ecan_lib.c
 */

#include <xc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpic30.h>

#include "ecan_config.h"
#include "ecan_driver.h"
#include "ecan_lib.h"

#define USE_STANDARD_ID		0
#define USE_EXTENDED_ID		1

#define FIRST_RX_BUFFER		1

typedef struct {
    unsigned int data_length;
    uint8_t data[8];
} t_object_buffer;

typedef struct {
    t_object_buffer buff; // utilizzato solo se flags non è FIFO
    void *user_ptr;
    ecan_message_handler update;
    bool new_data_arrived;
    int flags;
} t_object;

static t_object can_objects[ECAN_MAX_OBJECTS] __attribute__((far));
static int fifo_objnum = -1; // indice dell'oggetto a cui è stata assegnato il flag FIFO

// La FIFO è implementata con una coda circolare
static t_object_buffer fifo_buffer[ECAN_FIFO_SIZE] __attribute__((far));
static int fifo_head, fifo_count;

#if defined(__dsPIC33F__)
static void ecan_input_map(unsigned int RB_RX)
{
    RPINR26bits.C1RXR = RB_RX;
}

static void ecan_output_map(unsigned int RB_TX)
{
    switch (RB_TX) {
    case 0 : RPOR0bits.RP0R=0b10000;break;
    case 1 : RPOR0bits.RP1R=0b10000;break;
    case 2 : RPOR1bits.RP2R=0b10000;break;
    case 3 : RPOR1bits.RP3R=0b10000;break;
    case 4 : RPOR2bits.RP4R=0b10000;break;
    case 5 : RPOR2bits.RP5R=0b10000;break;
    case 6 : RPOR3bits.RP6R=0b10000;break;
    case 7 : RPOR3bits.RP7R=0b10000;break;
    case 8 : RPOR4bits.RP8R=0b10000;break;
    case 9 : RPOR4bits.RP9R=0b10000;break;
    case 10: RPOR5bits.RP10R=0b10000;break;
    case 11: RPOR5bits.RP11R=0b10000;break;
    case 12: RPOR6bits.RP12R=0b10000;break;
    case 13: RPOR6bits.RP13R=0b10000;break;
    case 14: RPOR7bits.RP14R=0b10000;break;
    case 15: RPOR7bits.RP15R=0b10000;break;
    }
}
#elif defined(__dsPIC33E__)
static void ecan_input_map(unsigned int RB_RX)
{
    switch (RB_RX) {
    case 0:  RPINR26bits.C1RXR=0b0100000;break; // RB0/RPI32
    case 1:  RPINR26bits.C1RXR=0b0100001;break; // RB1/RPI33
    case 2:  RPINR26bits.C1RXR=0b0100010;break; // RB2/RPI34
    case 3:  RPINR26bits.C1RXR=0b0100011;break; // RB3/RP35
    case 4:  RPINR26bits.C1RXR=0b0100100;break; // RB4/RP36
    case 5:  RPINR26bits.C1RXR=0b0100101;break; // RB5/RP37
    case 6:  RPINR26bits.C1RXR=0b0100110;break; // RB6/RP38
    case 7:  RPINR26bits.C1RXR=0b0100111;break; // RB7/RP39
    case 8:  RPINR26bits.C1RXR=0b0101000;break; // RB8/RP40
    case 9:  RPINR26bits.C1RXR=0b0101001;break; // RB9/RP41
    case 10: RPINR26bits.C1RXR=0b0101010;break; // RB10/RP42
    case 11: RPINR26bits.C1RXR=0b0101011;break; // RB11/RP43
    case 12: RPINR26bits.C1RXR=0b0101100;break; // RB12/RPI44
    case 13: RPINR26bits.C1RXR=0b0101101;break; // RB13/RPI45
    case 14: RPINR26bits.C1RXR=0b0101110;break; // RB14/RPI46
    case 15: RPINR26bits.C1RXR=0b0101111;break; // RB15/RPI47
    }
}

static void ecan_output_map(unsigned int RB_TX)
{
    switch (RB_TX) {
    case 3:  RPOR0bits.RP35R=0b001110;break; // RB3/RP35
    case 4:  RPOR1bits.RP36R=0b001110;break; // RB4/RP36
    case 5:  RPOR1bits.RP37R=0b001110;break; // RB5/RP37
    case 6:  RPOR2bits.RP38R=0b001110;break; // RB6/RP38
    case 7:  RPOR2bits.RP39R=0b001110;break; // RB7/RP39
    case 8:  RPOR3bits.RP40R=0b001110;break; // RB8/RP40
    case 9:  RPOR3bits.RP41R=0b001110;break; // RB9/RP41
    case 10: RPOR4bits.RP42R=0b001110;break; // RB10/RP42
    case 11: RPOR4bits.RP43R=0b001110;break; // RB11/RP43
    }
}
#endif

void ecan_initialize(unsigned int RB_RX, unsigned int RB_TX, bool TX_OPEN_DRAIN, bool use_interrupt)
{
    // ECAN1 Initialisation

    // CONFIGURE PINS
    //set tx_pin as output
    TRISB &= ~(unsigned int)(1 << RB_TX);

    //set rx_pin as input
    TRISB |= (1 << RB_RX);

    ecan_input_map(RB_RX);
    ecan_output_map(RB_TX);

    if (TX_OPEN_DRAIN) {
        //set in open drain configuration
        ODCB |= (1 << RB_TX); //Tx
    }

    // configure bit timing
    ecan_config();

    /* eCAN Interrupts */

    if (use_interrupt) {
        IEC2bits.C1IE = 1;      // CAN Event interrupt
        C1INTEbits.RBIE = 1;    //rx buffer interrupt
        C1INTEbits.TBIE = 1;    //tx buffer interrupt
    }
    else {
        IEC2bits.C1IE = 0;      // CAN Event interrupt
        C1INTEbits.RBIE = 0;    //rx buffer interrupt
        C1INTEbits.TBIE = 0;    //tx buffer interrupt
    }

    C1INTFbits.RBIF = 0;    //rx buffer interrupt

    IFS2bits.C1RXIF = 0;        // clear interrupt flag
    IEC2bits.C1RXIE = 0;    // RX CAN Interrupt off

    /* DMA Interrupts */
    IEC0bits.DMA0IE = 0;
    IEC0bits.DMA1IE = 0;
    IEC1bits.DMA2IE = 0;
    IEC2bits.DMA3IE = 0;
    IFS0bits.DMA0IF = 0;
    IFS0bits.DMA1IF = 0;
    IFS1bits.DMA2IF = 0;
    IFS2bits.DMA3IF = 0;

#if defined(__dsPIC33F__)
    IEC2bits.DMA4IE = 0;
    IEC3bits.DMA5IE = 0;
    IEC4bits.DMA6IE = 0;
    IEC4bits.DMA7IE = 0;
    IFS2bits.DMA4IF = 0;
    IFS3bits.DMA5IF = 0;
    IFS4bits.DMA6IF = 0;
    IFS4bits.DMA7IF = 0;
#endif

    // Configure buffer #0 for sending outgoing messages
    ecan_configure_tx_buffer(0, ECAN_LOWEST_PRIORITY);
}

static void invoke_update_handler(t_object * obj)
{
    t_object_buffer *src_buffer;
    unsigned int data_length;
    uint8_t buffer[8];

    int current_cpu_ipl;
    SET_AND_SAVE_CPU_IPL(current_cpu_ipl, 7); // disattiva interrupt

    if ((obj->flags & ECAN_RX_FLAG_FIFO))
    {
        src_buffer = &fifo_buffer[fifo_head++];
        if (fifo_head == ECAN_FIFO_SIZE)
            fifo_head = 0;
        if (--fifo_count != 0)
            obj->new_data_arrived = true;
        else
            obj->new_data_arrived = false;
    }
    else
    {
        src_buffer = &obj->buff;
        obj->new_data_arrived = false;
    }

    data_length = src_buffer->data_length;
    memcpy(buffer, src_buffer->data, data_length);

    RESTORE_CPU_IPL(current_cpu_ipl); // ripristina interrupt

    obj->update(buffer, data_length, obj->user_ptr);
}

static void download_dma_buffer(t_object_buffer *dest, unsigned int *src)
{
    dest->data_length = (unsigned char)(src[2] & 0x000F);
    memcpy(dest->data, &src[3], dest->data_length);
}

static void perform_object_update_nonfifo(int objnum, int buffnum)
{
    t_object * obj;

    obj = &can_objects[objnum];

    download_dma_buffer(&obj->buff, ecan_get_buffer_ptr(buffnum));
    ecan_clear_rxful(buffnum);

    if ((obj->flags & ECAN_RX_FLAG_ASYNC) != 0)
        invoke_update_handler(obj);
    else
        obj->new_data_arrived = true;
}

static void perform_object_update_fifo(int objnum, int buffnum)
{
    t_object * obj;

    obj = &can_objects[objnum];

    if (fifo_count == ECAN_FIFO_SIZE) // scarta un vecchio messaggio se la FIFO è già piena
    {
        if (++fifo_head == ECAN_FIFO_SIZE)
            fifo_head = 0;
        fifo_count = ECAN_FIFO_SIZE - 1;
    }

    download_dma_buffer(&fifo_buffer[(fifo_head + fifo_count++) % ECAN_FIFO_SIZE], ecan_get_buffer_ptr(buffnum));
    ecan_clear_rxful(buffnum);

    if ((obj->flags & ECAN_RX_FLAG_ASYNC) != 0)
        invoke_update_handler(obj);
    else
        obj->new_data_arrived = true;
}

//------------------------------------------------------------------------------
//    CAN Event interrupt handler
//------------------------------------------------------------------------------
void __attribute__((interrupt, no_auto_psv)) _C1Interrupt(void)
{
    int i;

    C1INTFbits.TBIF = 0;
    IFS2bits.C1IF = 0;

    if (C1INTFbits.RBIF)
    {
        C1INTFbits.RBIF = 0;

        // Gestisci filtri non-FIFO
        for (i = 0; i < ECAN_MAX_OBJECTS; i++)
        {
            if (ecan_get_rxful(i + FIRST_RX_BUFFER))
                perform_object_update_nonfifo(i, i + FIRST_RX_BUFFER);
        }

        // Gestisci filtro FIFO
        if (fifo_objnum != -1)
        {
            while (ecan_get_rxful(C1FIFObits.FNRB))
                perform_object_update_fifo(fifo_objnum, C1FIFObits.FNRB);
        }
    }
}

//------------------------------------------------------------------------------
//    DMA interrupt handlers
//------------------------------------------------------------------------------
void __attribute__((interrupt, no_auto_psv)) _DMA0Interrupt(void)
{
   IFS0bits.DMA0IF = 0;          // Clear the DMA0 Interrupt Flag;
}

void __attribute__((interrupt, no_auto_psv)) _DMA1Interrupt(void)
{
    IFS0bits.DMA1IF = 0;          // Clear the DMA1 Interrupt Flag;
}

void __attribute__((interrupt, no_auto_psv)) _DMA2Interrupt(void)
{
   IFS1bits.DMA2IF = 0;          // Clear the DMA2 Interrupt Flag;
}

void __attribute__((interrupt, no_auto_psv)) _DMA3Interrupt(void)
{
   IFS2bits.DMA3IF = 0;          // Clear the DMA3 Interrupt Flag;
}

#if defined(__dsPIC33F__)
void __attribute__((interrupt, no_auto_psv)) _DMA4Interrupt(void)
{
   IFS2bits.DMA4IF = 0;          // Clear the DMA4 Interrupt Flag;
}

void __attribute__((interrupt, no_auto_psv)) _DMA5Interrupt(void)
{
   IFS3bits.DMA5IF = 0;          // Clear the DMA5 Interrupt Flag;
}

void __attribute__((interrupt, no_auto_psv)) _DMA6Interrupt(void)
{
   IFS4bits.DMA6IF = 0;          // Clear the DMA6 Interrupt Flag;
}

void __attribute__((interrupt, no_auto_psv)) _DMA7Interrupt(void)
{
   IFS4bits.DMA7IF = 0;          // Clear the DMA7 Interrupt Flag;
}
#endif

/*
 * object management
 */

int ecan_send(int can_id, unsigned char * data, unsigned int dataLength, int flags)
{
    const int buf_num = 0; // we send all outgoing messages through buffer #0
    ecan_write_tx_message_id(buf_num, can_id, 0, 0);
    ecan_write_tx_message_data_buf(buf_num, data, dataLength);
    ecan_trigger_tx(buf_num);
    if (flags == ECAN_SEND_FLAG_NOWAIT)
        return 1;
    while (1) {
        int c = ecan_get_tx_condition(buf_num);
        if ((c & 0x0008) == 0) return 1; // TXREQ
        if ((c & 0x0070) != 0) return -1; // bus error, message lost or tx abort error
    }
}

void ecan_set_rx_object(int obj_num, int can_id, ecan_message_handler update, void *user_ptr, int flags)
{
    if (obj_num >= ECAN_MAX_OBJECTS)
        return;

    int buff_num;

    if ((flags & ECAN_RX_FLAG_FIFO) == 0)
    {
        buff_num = obj_num + FIRST_RX_BUFFER;
    }
    else
    {
        buff_num = 15;
        fifo_objnum = obj_num;
    }

    ecan_write_rx_acceptance_filter(obj_num, can_id, USE_STANDARD_ID, buff_num, 0); // standard ID, so use mask 0

    can_objects[obj_num].flags = flags;
    can_objects[obj_num].new_data_arrived = false;
    can_objects[obj_num].update = update;
    can_objects[obj_num].user_ptr = user_ptr;
}

void ecan_unset_rx_object(int obj_num)
{
    ecan_disable_rx_filter(obj_num);

    if (obj_num == fifo_objnum)
    {
        fifo_objnum = -1;

        // Svuota buffer di ricezione
        while (ecan_get_rxful(C1FIFObits.FNRB))
            ecan_clear_rxful(C1FIFObits.FNRB);

        fifo_count = 0;
    }
    else
    {
        // Svuota buffer di ricezione
        ecan_clear_rxful(obj_num + FIRST_RX_BUFFER);
    }

    can_objects[obj_num].new_data_arrived = false;
}

bool ecan_update_object(int obj_num)
{
    t_object *obj = &can_objects[obj_num];

    if (obj->new_data_arrived == true)
    {
        invoke_update_handler(obj);
        return true;
    }

    return false;
}
