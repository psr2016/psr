/*
 * i2c.h
 */

#ifndef __I2C_H
#define __I2C_H

#define I2C_FREQ_400KHZ        90
#define I2C_FREQ_300KHZ        135
#define I2C_FREQ_200KHZ        180
#define I2C_FREQ_100KHZ        360

typedef void (*i2c_error_handler)();

class I2C {
public:
    I2C(int f);
    void open(void);
    int read_register(short address, short location, short * data);
    int read_16_bit_register(short address, short location, int * data);
    int read_buffer(short address, short location, unsigned char * data, int data_len);
    int write_register(short address,short location,short data);
    int write_buffer(short address, short location, unsigned char * data, int len);
    int read_buffer_raw(short address, unsigned char * data, int data_len);
 private:
    int m_freq;
    i2c_error_handler m_err_handler;
};

#endif
