/* Workaround al mancato supporto degli address space identifiers in g++.
 *
 * Dato che gli identificatori di address space utilizzati in libpic30.h
 * (__pack_upper_byte e __eds__) non vengono riconosciuti dal parser C++,
 * facciamo in modo che vengano temporaneamente ignorati.
 *
 * Nota: __pack_upper_byte e __eds__ vengono disattivati solo durante il parsing
 * di libpic30.h, in tutti gli altri casi il parser C++ segnala un errore di
 * compilazione.
 *
 * Con questo accorgimento viene mantenuto il messaggio di errore che, qualora
 * il codice compilato utilizzasse tali parole chiave, avverte che esse non sono
 * utilizzabili in C++ . */

#ifdef __cplusplus
# define __pack_upper_byte
# define __eds__
#  include <libpic30.h>
# undef __pack_upper_byte
# undef __eds__
#else
// se stiamo compilando in C, non c'è bisogno del workaround
# include <libpic30.h>
#endif
