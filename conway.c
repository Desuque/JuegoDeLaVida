#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include "pbmlib.h"

#define TAM_DATOS 5
#define ENCENDIDO 1
#define APAGADO 0

void help() {
	printf("Uso:\n");
	printf("conway -h\n");
	printf("conway -V\n");
	printf("conway i M N inputfile [-o outputprefix]\n\n");
	printf("Opciones:\n");
	printf("-h, --help\n");
	printf("Imprime este mensaje.\n");
	printf("-V, --version Da la versión del programa.\n");
	printf("-o Prefijo de los archivos de salida.\n\n");
	printf("Ejemplos:\n");
	printf("conway 10 20 20 glider -o estado\n");
	printf("Representa 10 iteraciones del Juego de la Vida en "
			"una matriz de 20x20, con un estado inicial tomado "
			"del archivo ‘‘glider’’. Los archivos de salida se "
			"llamarán estado_n.pbm. Si no se da un prefijo para"
			" los archivos de salida, el prefijo será el nombre "
			"del archivo de entrada.\n");
}

void version() {
	printf("El Juego de la Vida - Version 0.1\n");
	printf("Programado para IBM por Leandro Desuque y Pepenacho.\n");
	printf("Todos los derechos reservados, el logo de PlayStation"
			"y el de Coca-Cola aparecen por cortesia de"
			"Microsoft.");
}

void cargarMatriz(int** matriz, int fila, int columna) {
	matriz[fila][columna] = ENCENDIDO;
}

bool infoValida(int fila, int columna, int tam_M, int tam_N) {
	if(fila>tam_M || columna>tam_N) {
		fprintf(stderr, "Las coordenadas ingresadas no son validas.\n");
		return false;
	}
	return true;
}
/**
 * NOTA PARA LEANDRO: creo que con fscanf queda más lindo, 
 * no me acuerdo ahora cómo se usaba, queda el tip (?)
 * */
int procesarArchivo(int** matriz, char* fileName, int tam_M, int tam_N) {
	FILE *archivo;
	char string[TAM_DATOS];
  	memset(&string[0], 0, TAM_DATOS);
	archivo = fopen(fileName,"r");
	if(archivo == NULL) {
		fprintf(stderr, "Error al abrir el archivo.\n");
	} else {
		printf("Leyendo estado inicial...\n");
		while(fgets(string, sizeof(string), archivo) != NULL) {
			unsigned int fila = atoi(&string[0]);
			unsigned int columna = atoi(&string[2]);
			if (infoValida(fila, columna, tam_M, tam_N)) {
				cargarMatriz(matriz, fila, columna);
			} else {
				return -1;
			}
	    }
	}
	fclose (archivo);
	return 0;
}

/**
 * Asigno memoria dinamica para la matriz de datos
 * y dejo todos los casilleros marcados como APAGADOS
 */
int** inicializarMatriz(unsigned int filas, unsigned int columnas) {
	int **matriz = (int **)malloc(filas * sizeof(int*));
	for (unsigned int i=0; i<filas; i++) {
		matriz[i] = (int *)malloc(columnas * sizeof(int));
	}
	for(unsigned int i=0; i<filas; i++) {
		for(unsigned int j=0; j<columnas; j++) {
			matriz[i][j] = APAGADO;
		}
	}
	return matriz;
}
unsigned int vecinos(int** matriz, 
		unsigned int i, unsigned int j,
		unsigned int m, unsigned int n){
			
		int	fOffsets[]={  1,  1,  1,  0, -1, -1, -1,  0};
		int cOffsets[]={ -1,  0,  1,  1,  1,  0, -1, -1};
		
		unsigned int seleccionado;
		unsigned int encontrados = 0;
		
		for( seleccionado = 0, seleccionado < 8, seleccionado += 1){
			int f = fOffsets[seleccionado] + i;
			int c = cOffsets[seleccionado] + j;
			
			if (f < 0 ) f += m
			if (c < 0 ) c += n
			
			if ( matriz[f][c] == ENCENDIDO ) encontrados += 1
		}
		
		return encontrados
}
void recalcularMatriz(int** matriz, unsigned int filas, unsigned int columnas) {
	unsigned int f,c;
	for( f = 0; f < filas; f += 1 ){
		for c = 0; c < columnas; c += 1 ){
			unsigned int vecs = vecinos(matriz,f,c,filas,columnas);
			if( vecs < 2 || vecs > 3 ){
				//si una celda tiene menos de dos o más de tres vecinos, 
				//su siguiente estado es apagado
				matriz[f][c] = APAGADO
			}else if (matriz[f][c] == APAGADO && vecs == 3){
				//si una celda apagada tiene exactamente 3 vecin9os encendidos,
				//su siguiente estado es encendido
				matriz[f][c] = ENCENDIDO
			}
			//si una celda encendida tiene dos o tres vecinos encendidos,
			//su siguiente estado es encendido (no hace falta programar esto)
		}
	}
}

void grabarEstado(int** matriz, filas, columnas) {
	//TODO PBM LIB
}

void avanzarEstados(int** matriz, unsigned int iteraciones, char* estado, unsigned int filas, unsigned int columnas) {
	for(unsigned int i=0; i<iteraciones; i++) {
		printf("Grabando %s_%d.pbm\n", estado, i+1);
		grabarEstado(matriz, filas, columnas);
		recalcularMatriz(matriz, filas, columnas);
	}
	printf("Listo\n");
}

void liberarRecursos(int** matriz, unsigned int filas) {
	/**
	* Libero la memoria utilizada por la matriz
	*/
	for (unsigned int i=0; i<filas; i++) {
		free(matriz[i]);
	}
	free(matriz);
}

int main (int argc, char *argv[]) {
	printf("hola!");
	int **matriz;
	int c;
	while (1) {
		static struct option long_options[] = {
				{"help", no_argument, 0, 'h'},
				{"version",  no_argument,       0, 'V'},
				{0, 0, 0, 0}
		};
		int option_index = 0;
		c = getopt_long (argc, argv, "hVo:",
                       long_options, &option_index);
		if (c == -1)
        break;
		switch (c) {
			case 'h':
				help();
				break;
			case 'V':
				version();
				break;
			case 'o':
				int filas = atoi(argv[2])
				int columnas = atoi(argv[3])
				matriz = inicializarMatriz(filas, columnas);
				if (procesarArchivo(matriz, argv[4], filas, columnas) != -1) {
					avanzarEstados(matriz, atoi(argv[1]), optarg, filas, columnas);
				}
				liberarRecursos(matriz, filas);
				break;
			case '?':
				break;
			default:
				abort();
		}
	}
	return 0;
}
