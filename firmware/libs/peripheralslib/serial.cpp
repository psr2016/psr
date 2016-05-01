/*
 * serial.cpp
 */

#include "defines.h"

#include <p33FJ128MC802.h>
#include <stdio.h>

#include "serial.h"

static UART_1 * uart1_ptr;
static UART_2 * uart2_ptr;

static void uart1_input_map(unsigned int RB_RX)
{
    RPINR18bits.U1RXR = RB_RX;
}

static void uart1_output_map(unsigned int RB_TX)
{
    switch (RB_TX) {
    case 0 : RPOR0bits.RP0R=0b00011;break;
    case 1 : RPOR0bits.RP1R=0b00011;break;
    case 2 : RPOR1bits.RP2R=0b00011;break;
    case 3 : RPOR1bits.RP3R=0b00011;break;
    case 4 : RPOR2bits.RP4R=0b00011;break;
    case 5 : RPOR2bits.RP5R=0b00011;break;
    case 6 : RPOR3bits.RP6R=0b00011;break;
    case 7 : RPOR3bits.RP7R=0b00011;break;
    case 8 : RPOR4bits.RP8R=0b00011;break;
    case 9 : RPOR4bits.RP9R=0b00011;break;
    case 10: RPOR5bits.RP10R=0b00011;break;
    case 11: RPOR5bits.RP11R=0b00011;break;
    case 12: RPOR6bits.RP12R=0b00011;break;
    case 13: RPOR6bits.RP13R=0b00011;break;
    case 14: RPOR7bits.RP14R=0b00011;break;
    case 15: RPOR7bits.RP15R=0b00011;break;
    }
}


UART_BASE::UART_BASE()
    : m_rx_handler(NULL)
{
    tx_buffer.max_len = SERIAL_BUFFER_SIZE;
    tx_buffer.head = tx_buffer.tail = tx_buffer.size = 0;
}

void UART_BASE::put_buffer(unsigned char * p, int len)
{
    while (len > 0) {
        put_char(*p);
        ++p; --len;
    }
}


void UART_BASE::put_string(char * p)
{
    while (*p != 0) {
        put_char(*p);
        ++p;
    }
}


// ------------------------------------------------ UART 1 ---------------------------------------

UART_1::UART_1()
    : UART_BASE()
{
    uart1_ptr = this;
}


void UART_1::open(int baud_rate, int RB_TX, int RB_RX, bool tx_open_drain, bool tx_interrupt, bool rx_interrupt)
{
    m_use_tx_interrupt = tx_interrupt;
    m_use_rx_interrupt = rx_interrupt;

    // CONFIGURE PINS
    //set tx_pin as output
    TRISB &= ~(unsigned int)(1 << RB_TX);

    //set rx_pin as input
    TRISB |= (1 << RB_RX);

    uart1_input_map(RB_RX);
    uart1_output_map(RB_TX);

    RPINR18bits.U1CTSR = 0b11111; // To VSS

    if (tx_open_drain) {
        //set in open drain configuration
        ODCB |= (1 << RB_TX); //Tx
    }

    //SET SERIAL
    U1MODEbits.STSEL = 0; 	  // 1-stop bit
    U1MODEbits.PDSEL = 0; 	  // No Parity, 8-data bits
    U1MODEbits.ABAUD = 0;	  // Auto-Baud Disabled
    U1MODEbits.BRGH  = 0; 	  // Low Speed mode
    U1BRG = baud_rate; 		  // BAUD Rate Setting
	
    U1MODEbits.UEN     = 0b00;
    U1STAbits.URXISEL  = 0b00;

    U1STAbits.UTXISEL0 = 0;
    U1STAbits.UTXISEL1 = 0;

    IFS0bits.U1RXIF = 0 ; // clear the interrupt
    IPC2bits.U1RXIP = 2 ; // priority 2
    if (rx_interrupt)
        IEC0bits.U1RXIE = 1 ; // turn on the interrupt
    else
        IEC0bits.U1RXIE = 0 ; // turn off the interrupt

    IFS0bits.U1TXIF = 0 ; // clear the interrupt
    IPC3bits.U1TXIP = 1 ; // priority 1
    if (tx_interrupt)
        IEC0bits.U1TXIE = 1 ; // turn on the interrupt
    else
        IEC0bits.U1TXIE = 0 ; // turn off the interrupt

    U1MODEbits.UARTEN  = 1;     // Enable UART
    U1STAbits.UTXEN    = 1; 	// Enable UART Tx
}


extern "C" void __attribute__((__interrupt__,__no_auto_psv__)) _U1RXInterrupt(void)
{
    char rxchar ;
    if ( U1STAbits.FERR ) {
        U1STAbits.FERR = 0; // clear framing error if it occurs
    }
    if ( U1STAbits.OERR ) {
        U1STAbits.OERR = 0; // clear overrun if it occurs
    }
    rxchar = U1RXREG ;
    if (uart1_ptr->m_rx_handler != NULL)
        uart1_ptr->m_rx_handler(rxchar);
    IFS0bits.U1RXIF = 0 ; // clear the interrupt
}


int UART_1::kb_hit()
{
    return U1STAbits.URXDA == 1;
}


char UART_1::read_char()
{
    while (U1STAbits.URXDA == 0) {
        if ( U1STAbits.FERR ) {
            U1STAbits.FERR = 0; // clear framing error if it occurs
        }
        if (U1STAbits.OERR == 1) {
            U1STAbits.OERR = 0; // clear overrun if it occurs
        }
    }
    return U1RXREG;
}


void UART_1::read_line(char * s)
{
    char * st = s;
    for (;;) {
        char c;
        c = read_char();
        if (c < ' ') {
            switch (c) {
            case 0x0d:
                putchar(13);
                putchar(10);
                *s = 0;
                return;
            case 0x08:
                if (s != st) {
                    putchar(8);
                    putchar(0x20);
                    putchar(8);
                    --s;
                }
                break;
            }
        }
        else {
            putchar(c);
            *s = c;
            ++s;
        }
    }
}



extern "C" void __attribute__((__interrupt__,__no_auto_psv__)) _U1TXInterrupt(void)
{
    if (uart1_ptr->tx_buffer.size > 0) {
        U1TXREG = uart1_ptr->tx_buffer.buf[uart1_ptr->tx_buffer.head];
        uart1_ptr->tx_buffer.head = (uart1_ptr->tx_buffer.head + 1) % uart1_ptr->tx_buffer.max_len;
        uart1_ptr->tx_buffer.size --;
        uart1_ptr->tx_buffer.transmitting = true;
    }
    else
        uart1_ptr->tx_buffer.transmitting = false;

    IFS0bits.U1TXIF = 0; // clear the interrupt
}


void UART_1::put_char(unsigned char c)
{
    if (m_use_tx_interrupt) {
        if (U1STAbits.UTXBF == 0 && tx_buffer.size == 0) {
                U1TXREG = c; // put char in TXREG
        }
        else {
            tx_buffer.buf[tx_buffer.tail] = c; // put char in tx_buffer
            tx_buffer.tail = (tx_buffer.tail + 1) % tx_buffer.max_len;
            tx_buffer.size ++;
        }
    }
    else {
        //while (U1STAbits.TRMT == 0) {} // wait end of transmission
        while (U1STAbits.UTXBF == 1) {} // wait for not buffer full
        U1TXREG = c;
    }
}


void UART_1::put_char_no_fifo(unsigned char c)
{
    if (!m_use_tx_interrupt) {
        U1TXREG = c;
        while (U1STAbits.TRMT == 0) {} // wait end of transmission
    }
}



// ------------------------------------------------ UART 2 ---------------------------------------


static void uart2_input_map(unsigned int RB_RX)
{
    RPINR19bits.U2RXR = RB_RX;
}

static void uart2_output_map(unsigned int RB_TX)
{
    switch (RB_TX) {
    case 0 : RPOR0bits.RP0R=0b00101;break;
    case 1 : RPOR0bits.RP1R=0b00101;break;
    case 2 : RPOR1bits.RP2R=0b00101;break;
    case 3 : RPOR1bits.RP3R=0b00101;break;
    case 4 : RPOR2bits.RP4R=0b00101;break;
    case 5 : RPOR2bits.RP5R=0b00101;break;
    case 6 : RPOR3bits.RP6R=0b00101;break;
    case 7 : RPOR3bits.RP7R=0b00101;break;
    case 8 : RPOR4bits.RP8R=0b00101;break;
    case 9 : RPOR4bits.RP9R=0b00101;break;
    case 10: RPOR5bits.RP10R=0b00101;break;
    case 11: RPOR5bits.RP11R=0b00101;break;
    case 12: RPOR6bits.RP12R=0b00101;break;
    case 13: RPOR6bits.RP13R=0b00101;break;
    case 14: RPOR7bits.RP14R=0b00101;break;
    case 15: RPOR7bits.RP15R=0b00101;break;
    }
}


UART_2::UART_2()
    : UART_BASE()
{
    uart2_ptr = this;
}


void UART_2::open(int baud_rate, int RB_TX, int RB_RX, bool tx_open_drain, bool tx_interrupt, bool rx_interrupt)
{
    m_use_tx_interrupt = tx_interrupt;
    m_use_rx_interrupt = rx_interrupt;

    // CONFIGURE PINS
    //set tx_pin as output
    TRISB &= ~(unsigned int)(1 << RB_TX);

    //set rx_pin as input
    TRISB |= (1 << RB_RX);

    uart2_input_map(RB_RX);
    uart2_output_map(RB_TX);

    RPINR19bits.U2CTSR = 0b11111; // To VSS

    if (tx_open_drain) {
        //set in open drain configuration
        ODCB |= (1 << RB_TX); //Tx
    }
    else {
        ODCB &= ~(1 << RB_TX); //Tx
    }

    //SET SERIAL
    U2MODEbits.STSEL = 0; 	  // 1-stop bit
    U2MODEbits.PDSEL = 0; 	  // No Parity, 8-data bits
    U2MODEbits.ABAUD = 0;	  // Auto-Baud Disabled
    U2MODEbits.BRGH  = 0; 	  // Low Speed mode
    U2BRG = baud_rate; 		  // BAUD Rate Setting

    U2MODEbits.UEN     = 0b00;
    U2STAbits.URXISEL  = 0b00;

    U2STAbits.UTXISEL0 = 0;
    U2STAbits.UTXISEL1 = 0;

    IFS1bits.U2RXIF = 0 ; // clear the interrupt
    IPC7bits.U2RXIP = 2 ; // priority 2
    if (rx_interrupt)
        IEC1bits.U2RXIE = 1 ; // turn on the interrupt
    else
        IEC1bits.U2RXIE = 0 ; // turn off the interrupt

    IFS1bits.U2TXIF = 0 ; // clear the interrupt
    IPC7bits.U2TXIP = 1 ; // priority 1
    if (tx_interrupt)
        IEC1bits.U2TXIE = 1 ; // turn on the interrupt
    else
        IEC1bits.U2TXIE = 0 ; // turn off the interrupt

    U2MODEbits.UARTEN  = 1;     // Enable UART
    U2STAbits.UTXEN    = 1; 	// Enable UART Tx
}


int UART_2::kb_hit()
{
    return U2STAbits.URXDA == 1;
}


char UART_2::read_char()
{
    while (U2STAbits.URXDA == 0) {
        if ( U2STAbits.FERR ) {
            U2STAbits.FERR = 0; // clear framing error if it occurs
        }
        if (U2STAbits.OERR == 1) {
            U2STAbits.OERR = 0; // clear overrun if it occurs
        }
    }
    return U2RXREG;
}


void UART_2::read_line(char * s)
{
    char * st = s;
    for (;;) {
        char c;
        c = read_char();
        if (c < ' ') {
            switch (c) {
            case 0x0d:
                putchar(13);
                putchar(10);
                *s = 0;
                return;
            case 0x08:
                if (s != st) {
                    putchar(8);
                    putchar(0x20);
                    putchar(8);
                    --s;
                }
                break;
            }
        }
        else {
            putchar(c);
            *s = c;
            ++s;
        }
    }
}

void UART_2::put_char(unsigned char c)
{
    if (!m_use_tx_interrupt) {
        U2TXREG = c;
        while (U2STAbits.TRMT == 0) {} // wait end of transmission
    }
}

void UART_2::put_char_no_fifo(unsigned char c)
{
    if (!m_use_tx_interrupt) {
        U2TXREG = c;
        while (U2STAbits.TRMT == 0) {} // wait end of transmission
    }
}



