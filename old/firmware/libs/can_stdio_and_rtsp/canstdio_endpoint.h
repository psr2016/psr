#ifndef CANSTDIO_ENDPOINT_H
#define CANSTDIO_ENDPOINT_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* Gestore frame CAN */
void canstdio_endpoint_process_can_frame(const uint8_t *data, unsigned int len, void *user_ptr);

/* Controlla se c'è almeno un client connesso */
bool canstdio_is_connected();

/* Riceve carattere da stdin (bloccante / non bloccante) */
char canstdio_getcharacter(bool blocking);

/* Controlla presenza di almeno un carattere da stdin (non bloccante) */
bool canstdio_is_input_available();

/* Invia uno o più caratteri a stdout */
void canstdio_send(const char *data, int data_len);

#ifdef __cplusplus
}
#endif

#endif /* CANSTDIO_ENDPOINT_H */
