#include "defines.h"

#include <xc.h>
#include <libpic30.h>
#include <stdio.h>
#include <string.h>

#include "bus_objects.h"
#include "canrtsp_endpoint.h"
#include "canstdio_endpoint.h"
#include "ecan_lib.h"
#include "simulator_interface.h"

static const int id_nodo_locale = CAN_STDIO_AND_RTSP_NODE_ID; /* impostato dal Makefile */
static volatile bool nodo_attivo; // il nodo locale è stato attivato dal coordinatore?
static bool input_buffer_nonempty;
static volatile char seqnum;
static volatile bool clear_to_send = true;

// coda di ricezione
static char input_buffer[16];
static int input_buffer_count;

// N.B.: Questa funzione viene eseguita in contesto di interrupt!
void canstdio_endpoint_process_can_frame(const uint8_t *data, unsigned int len, void *user_ptr)
{
	const t_can_remote_stdio_coord2endpoint *m = (const t_can_remote_stdio_coord2endpoint*)data;
	int i;

	if (m->data_len == -2) // richiesta di passaggio a rtsp
	{
		canrtsp_enter();
	}
	else if (m->data_len == -1) // richiesta di disattivazione stdio
	{
		nodo_attivo = false;
		input_buffer_nonempty = false;
		input_buffer_count = 0;
	}
	else // richiesta di attivazione stdio, opzionalmente con dati in input
	{
		int data_len = m->data_len & 0x7;
		clear_to_send = (m->data_len & 0x80) == 0;
		nodo_attivo = true;
		if (data_len != 0)
		{
			input_buffer_nonempty = true;
			for (i = 0; i < data_len && input_buffer_count != sizeof(input_buffer); i++)
				input_buffer[input_buffer_count++] = m->data[i];
		}
	}

	seqnum = m->seqnum_ack;
}

bool canstdio_is_connected()
{
	return nodo_attivo;
}

char canstdio_getcharacter(bool blocking)
{
	while (blocking && nodo_attivo && !input_buffer_nonempty)
		simulator_relax();

	if (input_buffer_nonempty == false)
		return 0;

	int current_cpu_ipl;
	SET_AND_SAVE_CPU_IPL(current_cpu_ipl, 7); // disattiva interrupt
	char res;

	if (input_buffer_count == 0)
	{
		res = 0;
	}
	else
	{
		res = input_buffer[0];
		memmove(input_buffer, input_buffer + 1, --input_buffer_count);
	}

	input_buffer_nonempty = (input_buffer_count != 0);
	RESTORE_CPU_IPL(current_cpu_ipl); // riattiva interrupt

	return res;
}

bool canstdio_is_input_available()
{
	return input_buffer_nonempty;
}

void canstdio_send(const char *data, int data_len)
{
	int chunk_start, i;

	// invia dati in messaggi CAN di 8 byte ciascuno
	// non fare nulla se l'output non è stato attivato
	for (chunk_start = 0; chunk_start < data_len && nodo_attivo; chunk_start += 7)
	{
		int chunk_len = data_len - chunk_start;
		if (chunk_len > 6) chunk_len = 6;

		t_can_remote_stdio_endpoint2coord m;
		m.seqnum = seqnum + 1;
		m.data_len = chunk_len | (id_nodo_locale << 4);
		memcpy(m.data, data + chunk_start, chunk_len);

		// Attesa massima clear-to-send: circa 1 secondo
		for (i = 0; i < 10000 && clear_to_send == false; i++)
			__delay_us(100);

		if (clear_to_send == false)
		{
			nodo_attivo = false;
			break;
		}

		ecan_send(REMOTE_STDIO_CAN_ID(REMOTE_STDIO_COORD_ID), (void*)&m, 8, 0);

		// Attesa massima: 1 secondo
		for (i = 0; i < 10000 && seqnum != m.seqnum; i++)
			__delay_us(100);

		if (seqnum != m.seqnum) // Niente ACK entro il timeout, assumiamo che il coordinatore sia andato giù e ci disattiviamo
		{
			nodo_attivo = false;
			break;
		}
	}
}
