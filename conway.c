#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
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

void procesarArchivo(int** matriz, char* fileName) {
	FILE *archivo;
	char string[TAM_DATOS];
  	memset(&string[0], 0, TAM_DATOS);
	archivo = fopen(fileName,"r");
	if (archivo == NULL) {
		fprintf(stderr, "Error al abrir el archivo.\n");
	} else {
		printf("Leyendo estado inicial...\n");
		while(fgets(string, sizeof(string), archivo) != NULL) {
			cargarMatriz(matriz, atoi(&string[0]), atoi(&string[2]));
	    }
	}
	fclose (archivo);
}

/**
 * Asigno memoria dinamica para la matriz de datos
 * y dejo todos los casilleros marcados como APAGADOS
 */
int** inicializarMatriz(int filas, int columnas) {
	int **matriz = (int **)malloc(filas * sizeof(int*));
	for (int i=0; i<filas; i++) {
		matriz[i] = (int *)malloc(columnas * sizeof(int));
	}
	for(int i=0; i<filas; i++) {
		for(int j=0; j<columnas; j++) {
			matriz[i][j] = APAGADO;
		}
	}
	return matriz;
}

void recalcularMatriz(int** matriz) {
	//TODO PEPENACHO
}

void grabarEstado(int** matriz) {
	//TODO PBM LIB
}

void avanzarEstados(int** matriz, int iteraciones, char* estado) {
	for(int i=0; i<iteraciones; i++) {
		printf("Grabando %s_%d.pbm\n", estado, i+1);
		grabarEstado(matriz);
		recalcularMatriz(matriz);
	}
}

void liberarRecursos(int** matriz, int filas) {
	/**
	* Libero la memoria utilizada por la matriz
	*/
	for (int i=0; i<filas; i++) {
		free(matriz[i]);
	}
	free(matriz);
}

int main (int argc, char *argv[]) {
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
				matriz = inicializarMatriz(atoi(argv[2]), atoi(argv[3]));
				procesarArchivo(matriz, argv[4]);
				avanzarEstados(matriz, atoi(argv[1]), optarg);
				liberarRecursos(matriz, atoi(argv[2]));
			break;
			case '?':
				break;
			default:
				abort();
		}
	}
	return 0;
}
