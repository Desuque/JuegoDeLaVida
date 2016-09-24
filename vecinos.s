#include <mips/regdef.h>
	.data			#Reservo memoria para los offset
	fOffset: .byte 1, 1, 1, 0, -1, -1, -1, 0
	cOffset: .byte -1, 0, 1, 1, 1, 0, -1, -1
	.text
	.align 2

/***
unsigned int vecinos(unsigned char* matriz,
		unsigned int i, unsigned int j,
		unsigned int m, unsigned int n){
***/ 

#$a1 = *matriz[0][0]
#$a2 = i (fila)
#$a3 = j (columna)
#$a4 = m (tam_fila)
#(en la pila) = n (tam_columna) No se usa, ya que la matriz es cuadrada 

/***
En realidad los parametros los tengo que
pasar a registros temporales
ver despues
***/

vecinos_s: .ent vecinos_s
	li $t0, 0			#load inmediate
	li $t6, 0			#encontrados = 0
	la $t1, fOffset 		#apunta a la direccion del primer elemento de fOffset
	la $t2, cOffset		

loop:
	bge $t0, 8, salir		#branch on greater than equal
	addu $t0, $t0, 1
	lh $s0, 0($t1) 			#carga el 1er elemento del vector fOffset en s0
	addu $t3, $s0, $a2		#int f = fOffsets[seleccionado] + i;
	lh $s1, 0($t2)			#carga el 1er elemento del vector cOffset en s1
	addu $t4, $s1, $a3		#int c = cOffsets[seleccionado] + j;

	
	blt $t3, $zero, suma_fila	#if (f < 0 ) f += m;
	bge $t3, $a4, resta_fila	#if (f >= m) f -= m;
	blt $t4, $zero, suma_col	
	blt $t4, $a4, resta_col		#la matriz es cuadrada por definicion (uso $a4)

	mulou $t5, $a4, $t4		# m*c
	subu $t5, $t5, $t4		# (m*c)-c
	addu $t5, $t5, $t3		# ((m*c)-c) + f

	lh $s2, $t5($a1)		#carga el elemento t5 de la matriz en s2
	beq $s2, 1, encontrados		#matriz[posicion] == 1

	blt $t0, 5, loop 		#branch less than
	j salir

encontrados:
	addu $t6, $t6, 1		#encontrados + 1
	j loop
	
suma_fila:
	addu $t3, $t3, $a4		#fila = fila + m
	j loop

resta_fila
	subu $t3, $t3, $a4		#fila = fila - m
	j loop

suma_col:
	addu $t4, $t4, $a4
	j loop

resta_col:
	subu $t4, $t4, $a4
	j loop

salir:
	#return
	addu $v0, $t6, $zero		# return encontrados
	jr ra

.end vecinos_s
.size vecinos_s, .-vecinos_s
