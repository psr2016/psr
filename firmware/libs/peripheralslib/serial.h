/*
 * uart.h
 */

#ifndef __SERIAL_H
#define __SERIAL_H

#include "defines.h"
#include <stdbool.h>

// ------------------------------------------------------------

#ifdef EXTERNAL_CLOCK

#define B115200         20
#define B57600          40

#endif

// ------------------------------------------------------------

#ifdef INTERNAL_CLOCK

#define B115200         20
#define B57600          42
#define B4800           (40*12)

#endif

// ------------------------------------------------------------

#define SERIAL_BUFFER_SIZE   1024

typedef struct {
    int max_len;
    bool transmitting;
    int head, tail, size;
    unsigned char buf[SERIAL_BUFFER_SIZE];
} t_tx_buffer;

typedef void (*t_rx_handler)(char);

class UART_BASE {
 public:
    UART_BASE();
    virtual void open(int baud_rate, int RB_TX, int RB_RX, bool tx_open_drain, bool tx_interrupt, bool rx_interrupt) = 0;
    virtual int kb_hit() = 0;
    virtual char read_char() = 0;
    virtual void read_line(char * s) = 0;
    virtual void put_char(unsigned char c) = 0;
    virtual void put_char_no_fifo(unsigned char c) = 0;
    void put_buffer(unsigned char * p, int len);
    void put_string(char * p);
    void set_rx_handler(t_rx_handler h) { m_rx_handler = h; };

    t_rx_handler m_rx_handler;
    t_tx_buffer tx_buffer;
 protected:
    bool m_use_tx_interrupt, m_use_rx_interrupt;
};


class UART_1 : public UART_BASE {
 public:
    UART_1();
    void open(int baud_rate, int RB_TX, int RB_RX, bool tx_open_drain, bool tx_interrupt, bool rx_interrupt);
    int kb_hit();
    char read_char();
    void read_line(char * s);
    void put_char(unsigned char c);
    void put_char_no_fifo(unsigned char c);
};


class UART_2 : public UART_BASE {
 public:
    UART_2();
    void open(int baud_rate, int RB_TX, int RB_RX, bool tx_open_drain, bool tx_interrupt, bool rx_interrupt);
    int kb_hit();
    char read_char();
    void read_line(char * s);
    void put_char(unsigned char c);
    void put_char_no_fifo(unsigned char c);
};


#endif

