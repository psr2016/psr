#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simple_var_output.h"
#include "bus_objects.h"
#include "ecan_lib.h"

static char debug_output_next_ctr = 0;

// simple var output protocol
void debug_output_flush()
{
	t_can_simple_var_packet p;
	p.type = SIMPLE_VAR_TYPE_FLUSH;
	p.ctr = debug_output_next_ctr;
	debug_output_next_ctr = 0;
	ecan_send(SIMPLE_VAR_OUTPUT_ID, (unsigned char *)&p, 8, 0);
}

void debug_output_char(const char *var_name, char v)
{
	t_can_simple_var_packet p;

        debug_output_string(var_name);

	p.type = SIMPLE_VAR_TYPE_CHAR;
	p.ctr = debug_output_next_ctr++;
	memcpy(p.data, &v, sizeof(v));
	ecan_send(SIMPLE_VAR_OUTPUT_ID, (unsigned char *)&p, 8, 0);
}

void debug_output_int8(const char *var_name, int8_t v)
{
	t_can_simple_var_packet p;

        debug_output_string(var_name);

	p.type = SIMPLE_VAR_TYPE_INT8;
	p.ctr = debug_output_next_ctr++;
	memcpy(p.data, &v, sizeof(v));
	ecan_send(SIMPLE_VAR_OUTPUT_ID, (unsigned char *)&p, 8, 0);
}

void debug_output_uint8(const char *var_name, uint8_t v)
{
	t_can_simple_var_packet p;

        debug_output_string(var_name);

	p.type = SIMPLE_VAR_TYPE_UINT8;
	p.ctr = debug_output_next_ctr++;
	memcpy(p.data, &v, sizeof(v));
	ecan_send(SIMPLE_VAR_OUTPUT_ID, (unsigned char *)&p, 8, 0);
}

void debug_output_bool(const char *var_name, bool v)
{
	t_can_simple_var_packet p;

        debug_output_string(var_name);

	p.type = SIMPLE_VAR_TYPE_BOOL;
	p.ctr = debug_output_next_ctr++;
	memcpy(p.data, &v, sizeof(v));
	ecan_send(SIMPLE_VAR_OUTPUT_ID, (unsigned char *)&p, 8, 0);
}

void debug_output_int16(const char *var_name, int16_t v)
{
	t_can_simple_var_packet p;

        debug_output_string(var_name);

	p.type = SIMPLE_VAR_TYPE_INT16;
	p.ctr = debug_output_next_ctr++;
	memcpy(p.data, &v, sizeof(v));
	ecan_send(SIMPLE_VAR_OUTPUT_ID, (unsigned char *)&p, 8, 0);
}

void debug_output_uint16(const char *var_name, uint16_t v)
{
	t_can_simple_var_packet p;

        debug_output_string(var_name);

	p.type = SIMPLE_VAR_TYPE_UINT16;
	p.ctr = debug_output_next_ctr++;
	memcpy(p.data, &v, sizeof(v));
	ecan_send(SIMPLE_VAR_OUTPUT_ID, (unsigned char *)&p, 8, 0);
}

void debug_output_int32(const char *var_name, int32_t v)
{
	t_can_simple_var_packet p;

        debug_output_string(var_name);

	p.type = SIMPLE_VAR_TYPE_INT32;
	p.ctr = debug_output_next_ctr++;
	memcpy(p.data, &v, sizeof(v));
	ecan_send(SIMPLE_VAR_OUTPUT_ID, (unsigned char *)&p, 8, 0);
}

void debug_output_uint32(const char *var_name, uint32_t v)
{
	t_can_simple_var_packet p;

        debug_output_string(var_name);

	p.type = SIMPLE_VAR_TYPE_UINT32;
	p.ctr = debug_output_next_ctr++;
	memcpy(p.data, &v, sizeof(v));
	ecan_send(SIMPLE_VAR_OUTPUT_ID, (unsigned char *)&p, 8, 0);
}

void debug_output_float(const char *var_name, float v)
{
	t_can_simple_var_packet p;

        debug_output_string(var_name);

	p.type = SIMPLE_VAR_TYPE_FLOAT;
	p.ctr = debug_output_next_ctr++;
	memcpy(p.data, &v, sizeof(v));
	ecan_send(SIMPLE_VAR_OUTPUT_ID, (unsigned char *)&p, 8, 0);
}

void debug_output_string(const char *v)
{
	t_can_simple_var_packet p;
	p.type = SIMPLE_VAR_TYPE_STRING;
	p.ctr = debug_output_next_ctr++;
	strncpy(p.data, v, 6);
	ecan_send(SIMPLE_VAR_OUTPUT_ID, (unsigned char *)&p, 8, 0);
}
 
