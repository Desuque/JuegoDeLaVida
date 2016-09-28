#include "vecinos.h"

#define ENCENDIDO 1

unsigned int vecinos(unsigned char* matriz,
	unsigned int i, unsigned int j,
	unsigned int m, unsigned int n){

	int fOffsets[]={  1,  1,  1,  0, -1, -1, -1,  0};
	int cOffsets[]={ -1,  0,  1,  1,  1,  0, -1, -1};

	unsigned int seleccionado;
	unsigned int encontrados = 0;
	for( seleccionado = 0; seleccionado < 8; seleccionado += 1){
		int f = fOffsets[seleccionado] + i;
		int c = cOffsets[seleccionado] + j;

		if (f < 0 ) f += m;
		if (f >= m) f -= m;

		if (c < 0 ) c += n;
		if (c >= n) c -= n;

		unsigned int posicion = ((m*c)-c)+f;
		if ( matriz[posicion] == ENCENDIDO ) encontrados += 1;
	}
	return encontrados;
}
