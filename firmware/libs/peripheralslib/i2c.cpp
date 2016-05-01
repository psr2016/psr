/*
 * i2c.cpp
 */

#include "defines.h"

#include <p33FJ128MC802.h>
#include <libpic30++.h>
#include <stdio.h>

#include "i2c.h"

#define ACK         0
#define NOT_ACK     1

#define i2c_relax() { __delay_us(50); }

#define i2c_start() {                           \
	I2C1CONbits.SEN = 1;                    \
	while (I2C1CONbits.SEN == 1) ;          \
    }

#define i2c_stop() {                            \
	I2C1CONbits.PEN = 1;                    \
	while (I2C1CONbits.PEN == 1) ;          \
    }

#define i2c_restart() {                                         \
        I2C1CONbits.RSEN = 1;                                   \
        while (I2C1CONbits.RSEN);                               \
    }



// flag = 1 --> error in communication
#define i2c_write(data,flag){                       \
        I2C1TRN = (data);                           \
	while(I2C1STATbits.TBF == 1) ;               \
	while(I2C1STATbits.TRSTAT == 1) ;            \
	flag = I2C1STATbits.ACKSTAT;              \
    }

#define i2c_read(data, ack){                        \
    I2C1CONbits.RCEN = 1;                           \
    Nop();                                          \
    while (I2C1STATbits.RBF == 0) ;                 \
    (data) = I2C1RCV;                               \
    I2C1CONbits.ACKDT = ack;                        \
    I2C1CONbits.ACKEN = 1;                          \
    while (I2C1CONbits.ACKEN == 1) ;                \
    I2C1CONbits.ACKDT = 0;                \
}

#define i2c_idle(){                             \
	while(I2C1STATbits.TRSTAT) ;            \
    }

#define i2c_ack_status(){                       \
	flag = I2C1STATbits.ACKSTAT;            \
    }


#define i2c_error() {                           \
    i2c_idle();                                 \
    i2c_stop();                                 \
    if (m_err_handler != NULL) m_err_handler(); \
    }


I2C::I2C(int f)
    : m_freq(f), m_err_handler(NULL)
{ };

void I2C::open(void)
{
    I2C1BRG = m_freq;
    I2C1CON = 0x1200;
    I2C1RCV = 0;
    I2C1TRN = 0;
    I2C1CON = 0x9200;
}

int I2C::read_register(short address, short location, short * data)
{
    int flag;

    i2c_relax();

    i2c_idle();
    i2c_start();

    i2c_write(address,flag);
    if (flag == 1) {
        i2c_stop();
        i2c_error();
        return 0;
    }
    i2c_idle();

    i2c_write(location,flag);
    if (flag == 1) {
        i2c_stop();
        i2c_error();
        return 0;
    }
    i2c_idle();

    i2c_restart();
    i2c_write(address | 0x01,flag);
    if (flag == 1) {
        i2c_stop();
        i2c_error();
        return 0;
    }
    i2c_idle();

    i2c_read( *data, NOT_ACK );

    i2c_stop();

    return 1;	
}

int I2C::read_16_bit_register(short address, short location, int * data)
{
    int flag;
    int h, l;

    i2c_relax();

    i2c_idle();
    i2c_start();

    i2c_write(address,flag);
    if (flag == 1) {
        i2c_stop();
        i2c_error();
        return 0;
    }
    i2c_idle();

    i2c_write(location,flag);
    if (flag == 1) {
        i2c_stop();
        i2c_error();
        return 0;
    }
    i2c_idle();

    i2c_restart();
    i2c_write(address | 0x01,flag);
    if (flag == 1) {
        i2c_stop();
        i2c_error();
        return 0;
    }
    i2c_idle();

    i2c_read( h, ACK );

    i2c_read( l, NOT_ACK );

    i2c_stop();

    *data = (h << 8) | l;

    return 1;	
}

int I2C::read_buffer(short address, short location, unsigned char * data, int data_len)
{
    int flag;
    int i;

    i2c_relax();

    i2c_idle();
    i2c_start();

    i2c_write(address,flag);
    if (flag == 1) {
        i2c_stop();
        i2c_error();
        return 0;
    }
    i2c_idle();

    i2c_write(location,flag);
    if (flag == 1) {
        i2c_stop();
        i2c_error();
        return 0;
    }
    i2c_idle();

    i2c_restart();
    i2c_write(address | 0x01,flag);
    if (flag == 1) {
        i2c_stop();
        i2c_error();
        return 0;
    }
    i2c_idle();

    for (i = 0; i < data_len;i++) {
        if (i == (data_len - 1)) {
            i2c_read( data[i], NOT_ACK );
        }
        else {
            i2c_read( data[i], ACK );
        }
    }

    i2c_stop();

    return 1;	
}

int I2C::write_register(short address,short location,short data)
{
    int flag;

    i2c_relax();

    i2c_idle();
    i2c_start();

    i2c_write(address,flag);
    if (flag == 1) {
        i2c_stop();
        i2c_error();
        return 0;
    }
    i2c_idle();

    i2c_write(location,flag);
    if (flag == 1) {
        i2c_stop();
        i2c_error();
        return 0;
    }
    i2c_idle();

    i2c_write(data,flag);
    if (flag == 1) {
        i2c_stop();
        i2c_error();
        return 0;
    }
    i2c_idle();

    i2c_stop();

    return 1;
}

int I2C::write_buffer(short address, short location, unsigned char * data, int len)
{
    int flag;

    i2c_relax();

    i2c_idle();
    i2c_start();

    i2c_write(address,flag);
    if (flag == 1) {
        i2c_stop();
        i2c_error();
        return 0;
    }
    i2c_idle();

    i2c_write(location,flag);
    if (flag == 1) {
        i2c_stop();
        i2c_error();
        return 0;
    }
    i2c_idle();

    while (len >= 0) {
        i2c_write(*data,flag);
        if (flag == 1) {
            i2c_stop();
            i2c_error();
            return 0;
        }
        i2c_idle();
        ++data;
        --len;
    }

    i2c_stop();

    return 1;
}

int I2C::read_buffer_raw(short address, unsigned char * data, int data_len)
{
    int flag;
    int i;

    i2c_relax();

    i2c_idle();
    i2c_start();

    i2c_write(address | 0x01,flag);
    if (flag == 1) {
        i2c_stop();
        i2c_error();
        return 0;
    }
    i2c_idle();

    for (i = 0; i < data_len;i++) {
        if (i == (data_len - 1)) {
            i2c_read( data[i], NOT_ACK );
        }
        else {
            i2c_read( data[i], ACK );
        }
    }

    i2c_stop();

    return 1;	
}

