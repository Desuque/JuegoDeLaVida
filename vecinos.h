#ifndef __VECINOS__H_INCLUDED__
#define __VECINOS__H_INCLUDED__

/**
 * Algoritmo implementado en c
 */
unsigned int vecinos(unsigned char* matriz,
		unsigned int i, unsigned int j,
		unsigned int m, unsigned int n);
/**
 * Algoritmo implementado en Assembly
 */
unsigned int vecinos_s(unsigned char* matriz,
		unsigned int i, unsigned int j,
		unsigned int m, unsigned int n);

#endif
