/*
 * ecan_config.h
 */

#ifndef __ECAN_CONFIG_H__
#define __ECAN_CONFIG_H__

#include "ecan_driver.h"

/*CAN Operative Mode */
#define CAN_MODE_NORMAL 0		//000 = Set Normal Operation mode
#define CAN_MODE_DISABLE 1		//001 = Set Disable mode
#define CAN_MODE_LOOP 2			//010 = Set Loopback mode
#define CAN_MODE_LISTEN 3		//011 = Set Listen-Only Mode
#define CAN_MODE_CONFIG 4		//100 = Set Configuration mode
#define CAN_MODE_LISTEN_ALL 7		//111 = Set Listen All Messages mode

/* CAN Transmit Message Buffer Priority Levels */
#define ECAN_LOWEST_PRIORITY	0
#define ECAN_INTERLOW_PRIORITY	1
#define ECAN_INTERHIGH_PRIORITY	2
#define ECAN_HIGHEST_PRIORITY	3

/* CAN Baud Rate Configuration 		*/
/* #define FCAN  	38692500 */
/* #define BITRATE 967312.5 */
/* #define NTQ 	20		// 20 Time Quanta in a Bit Time */
/* #define BRP_VAL		((FCAN/(2*NTQ*BITRATE))-1) */

/* CAN Message Buffer Configuration */
#define ECAN_BUFFERS	32
#define ECAN_FIFO_SIZE	16
typedef unsigned int ECANMSGBUF [ECAN_BUFFERS][8];	
#if defined(__dsPIC33F__)
extern ECANMSGBUF ecanmsgBuf __attribute__((space(dma),aligned(ECAN_BUFFERS*16)));
#elif defined(__dsPIC33E__)
extern ECANMSGBUF ecanmsgBuf __attribute__((__far__,aligned(ECAN_BUFFERS*16)));
#endif

/* Prototipi */
void ecan_set_transmit_dma_channel(unsigned int selectChannel);
void ecan_set_receive_dma_channel(unsigned int selectChannel);
void ecan_set_dma_buffer_size(unsigned int bufferSize);
void ecan_set_fifo_start_area(unsigned int buffer_num);
void ecan_config(void);
void ecan_operation_mode(unsigned int mode);
void ecan_init_bit_timing(int BRP,int SJW, int phaseSeg1, int phaseSeg2, int propSeg);
void ecan_configure_tx_buffer(unsigned int buffer_num, unsigned int priority);

#endif
