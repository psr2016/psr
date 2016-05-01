#ifndef SIMPLE_VAR_OUTPUT_H
#define SIMPLE_VAR_OUTPUT_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

// simple var output protocol
void debug_output_flush();
void debug_output_char(const char *var_name, char v);
void debug_output_int8(const char *var_name, int8_t v);
void debug_output_uint8(const char *var_name, uint8_t v);
void debug_output_bool(const char *var_name, bool v);
void debug_output_int16(const char *var_name, int16_t v);
void debug_output_uint16(const char *var_name, uint16_t v);
void debug_output_int32(const char *var_name, int32_t v);
void debug_output_uint32(const char *var_name, uint32_t v);
void debug_output_float(const char *var_name, float v);
void debug_output_string(const char *v);

#ifdef __cplusplus
}
#endif

#endif //SIMPLE_VAR_OUTPUT_H