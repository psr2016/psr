/*
 * simulator_interface.h
 */

#ifndef __SIMULATOR_INTERFACE
#define __SIMULATOR_INTERFACE

#include "defines.h"
#include <xc.h>
#include <libpic30.h>

#include <stdbool.h>

#define in_simulator() ((bool)((CORCON & 0x8000) != 0)) // restituisce true nel simulatore, false nella realtà

#define simulator_relax() do { if (in_simulator()) { __delay_us(100); } } while(0)

#endif // __SIMULATOR_INTERFACE
