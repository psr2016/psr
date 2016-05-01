#include <stdlib.h>

// Semplice implementazione degli operatori new e delete basata su malloc e free
void *operator new(size_t size)
{
	return malloc(size);
}

void *operator new[](size_t size)
{
	return malloc(size);
}

void operator delete(void *p)
{
	free(p);
}

void operator delete[](void *p)
{
	free(p);
}

// Semplice implementazione della funzione associata ai metodi pure virtual
// (ie astratti). Nota: è impossibile richiamare questa funzione tramite codice
// C++ valido
extern "C" __attribute__((noreturn, naked)) void __cxa_pure_virtual()
{
	asm("reset");
	while(1);
}

#ifdef __XC16ELF
// Simboli necessari per l'inizializzazione di oggetti allocati staticamente
// se il formato oggetto è ELF. COFF usa un'altra ABI e non ne ha bisogno.
void *__dso_handle = 0;
extern "C" int __cxa_atexit(void (*destructor) (void *), void *arg, void *dso)
{
	return 0;
}
#endif
