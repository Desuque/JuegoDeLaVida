#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include "vecinos.h"

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
	printf("El Juego de la Vida - Version 0.2\n");
	printf("Organizacion del computador, FIUBA.\n");
}


void cargarMatriz(char* matriz, unsigned int posicion) {
	matriz[posicion] = ENCENDIDO;
}

bool infoValida(int fila, int columna, int tam_M, int tam_N) {
	if(fila>tam_M || columna>tam_N) {
		fprintf(stderr, "Las coordenadas ingresadas no son validas.\n");
		return false;
	}
	return true;
}

int procesarArchivo(char* matriz, char* fileName, unsigned int filas,
		unsigned int columnas) {
	FILE *archivo;
	char string[TAM_DATOS];
  	memset(&string[0], 0, TAM_DATOS);
	archivo = fopen(fileName,"r");
	if(archivo == NULL) {
		fprintf(stderr, "Error al abrir el archivo.\n");
	} else {
		//printf("Leyendo estado inicial...\n");
		while(fgets(string, sizeof(string), archivo) != NULL) {
			unsigned int fila = atoi(&string[0]);
			unsigned int columna = atoi(&string[2]);
			if (infoValida(fila, columna, filas, columnas)) {
				unsigned int posicion = POSICION(fila, columna, filas, columnas);
				cargarMatriz(matriz, posicion);
			} else {
				return -1;
			}
		}
		fclose (archivo);
	}
	
	return 0;
}

/**
 * Asigno memoria dinamica para la matriz de datos
 * y dejo todos los casilleros marcados como APAGADOS
 */
char* inicializarMatriz(unsigned int filas, unsigned int columnas) {
	char *matriz = (char*)malloc(sizeof(char)*filas*columnas + 1);
	int tamTotal = filas * columnas;
	for(unsigned int i=0; i<tamTotal; i++) {
			matriz[i] = APAGADO;
	}
	return matriz;
}

//devuelve la matriz nueva
char* siguienteMatriz(char* matriz, unsigned int filas, unsigned int columnas, bool salidaPorPantalla) {
	unsigned int f,c;
	char* ret = inicializarMatriz(filas, columnas);
	
	for( f = 0; f < filas; f += 1 ){
		for(c = 0; c < columnas; c += 1 ){
			unsigned int pos = POSICION(f,c, filas, columnas);
			
			unsigned int vecs = vecinos(matriz, f, c, filas, columnas);

			if (salidaPorPantalla == true) {
				printf("%d",vecs);
				if(matriz[pos] == APAGADO){
					printf(" ");
				}else{
					printf("*");
				}
			}
			if( vecs < 2 || vecs > 3 ){
				//si una celda tiene menos de dos o más de tres vecinos, 
				//su siguiente estado es apagado
				ret[pos] = APAGADO;
			}else if (vecs == 3){
				//si una celda apagada tiene exactamente 3 vecin9os encendidos,
				//su siguiente estado es encendido
				ret[pos] = ENCENDIDO;
			}else if ( (vecs == 2 || vecs == 3) && matriz[pos] == ENCENDIDO){
				//si una celda encendida tiene dos o tres vecinos encendidos,
				//su siguiente estado es encendido
				ret[pos] = ENCENDIDO;
			}else{
				ret[pos] = APAGADO;//si no, está apagado
			}
			
		}
		if(salidaPorPantalla == true) {
			printf("\n");
		}
	}
	return ret;
}

void grabarEstado(char* matriz, unsigned int filas, unsigned int columnas,
		char *nombreArchivo) {
	FILE *archivo;
	archivo = fopen(nombreArchivo,"w");
	//escribo numero magico y whitespace
	fprintf(archivo,"P4\n");
	//escribo anchoalto
	fprintf(archivo,"%d %d\n",columnas*8,filas*8);//ancho alto (8 pixel por casillero)
	//rasters
	unsigned int f,c,i;
	for( f = 0; f < filas; f += 1){
		for( i = 0; i < 8; i += 1){//esto es para repetir todo 8 veces
			for( c = 0; c < columnas; c += 1){
				unsigned int posicion = POSICION(f, c, filas, columnas);
				if( matriz[posicion] == ENCENDIDO )
					fprintf(archivo,"%c",0xFF);
				else
					fprintf(archivo,"%c",0x00);
			}
		}
	}
	fclose(archivo);
}

void liberarRecursos(char* matriz) {
	/**
	* Libero la memoria utilizada por la matriz
	*/
	free(matriz);
}

void avanzarEstados(char* matrizInicial, unsigned int iteraciones,
		char* estado, unsigned int filas, unsigned int columnas, bool salidaPorPantalla) {
	char* matrizActual = matrizInicial;
	for(unsigned int i=0; i<iteraciones; i++) {
		if (salidaPorPantalla == false) {
			//generar nombreArchivo
			char nombreArchivo[200];
			sprintf(nombreArchivo,"%s_%d.pbm",estado, i+1);
			printf("Grabando %s\n", nombreArchivo);

			//guardar la matriz actual
			grabarEstado(matrizActual, filas, columnas, nombreArchivo);
		} else {
			printf("Salida %d\n", i+1);
		}
		//generar siguiente matriz
		char* siguiente = siguienteMatriz(matrizActual, filas, columnas, salidaPorPantalla);
		
		//liberar la matriz actual
		if (matrizActual != matrizInicial){
			liberarRecursos(matrizActual);
		}
		//cambiar la actual por la siguiente
		matrizActual = siguiente;
	}
	if (matrizActual != matrizInicial){
		liberarRecursos(matrizActual);
	}
}

void ejecutar(char *argv[], bool salida) {
	int filas  = atoi(argv[2]);
	int columnas  = atoi(argv[3]);
	char* matriz = inicializarMatriz(filas, columnas);
	if (procesarArchivo(matriz, argv[4], filas, columnas) != -1) {
		avanzarEstados(matriz, atoi(argv[1]), optarg, filas, columnas, salida);
	}
	liberarRecursos(matriz);
}

int main (int argc, char *argv[]) {
	char *matriz;
	int c;
	int filas;
	int columnas;
	bool salidaPorPantalla;

	//Si no se ingresa el argumento -o se muestra por pantalla la informacion
	if (argc == TAM_DATOS) {
		salidaPorPantalla = true;
		ejecutar(argv, salidaPorPantalla);
	}
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
				//Si se ingresa el argumento -o se obtiene la informacion en los archivos de salida
				//y se desactiva la informacion por pantalla
				salidaPorPantalla = false;
				ejecutar(argv, salidaPorPantalla);
				break;
			case '?':
				break;
			default:
				abort();

		}
	}
	return 0;
}
