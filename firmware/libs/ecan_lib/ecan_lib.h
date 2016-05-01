/*
 * ecan_lib.h
 */

#ifndef __ECAN_LIB
#define __ECAN_LIB

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define ECAN_MAX_OBJECTS	14
typedef void (*ecan_message_handler)(const uint8_t *data, unsigned int len, void *user_ptr);

void ecan_initialize(unsigned int RB_RX, unsigned int RB_TX, bool TX_OPEN_DRAIN, bool use_interrupt);

#define ECAN_RX_FLAG_ASYNC      1 // Esegue handler direttamente in contesto di interrupt
#define ECAN_RX_FLAG_FIFO       2 // N.B.: Può essere configurato un solo filtro in modalità FIFO
void ecan_set_rx_object(int obj_num, int can_id, ecan_message_handler update, void *user_ptr, int flags);
void ecan_unset_rx_object(int obj_num);

#define ECAN_SEND_FLAG_NOWAIT	1
int ecan_send(int can_id, unsigned char * data, unsigned int dataLength, int flags);

/* Se non è stato specificato il flag ECAN_RX_FLAG_ASYNC, gli handler vengono
 * invocati in modo sincrono da questa questa procedura, che deve essere
 * richiamata dal codice client */
bool ecan_update_object(int obj_num);

#ifdef __cplusplus
}
#endif

#endif
