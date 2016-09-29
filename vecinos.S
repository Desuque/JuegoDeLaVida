#include <mips/regdef.h>
	.data #Reservo memoria para los offset
	fOffset: .word 1, 1, 1, 0, -1, -1, -1, 0
	cOffset: .word -1, 0, 1, 1, 1, 0, -1, -1
	.text
	.align 2
	.globl	vecinos_s
	.ent	vecinos_s

/***
unsigned int vecinos(unsigned char* matriz,
		unsigned int i, unsigned int j,
		unsigned int m, unsigned int n){

Nota: Como la matriz es cuadrada, no se usa el ultimo argumento

***/ 

vecinos_s:
	.frame	$fp,44,ra
	subu	sp, sp, 44 	#Expando el stack
   	sw ra, 16(sp)	//ra en stack
  	sw gp, 20(sp)
	sw $fp, 24(sp)
	sw a0, 32(sp)	//*matriz[0][0]
	sw a1, 36(sp)	//i (fila)
	sw a2, 40(sp)	//j (columna)
	sw a3, 44(sp)	//m (tam_fila)

	li t0, 0			#Inicializo el flag del loop
	li t7, 0			#Inicializo vecinos
	la s0, fOffset		#Cargo la direccion del primer elemento en s0
	la s1, cOffset		#Cargo la direccion del primer elemento en s1

loop:
	lw t3, 0(s0)		#Cargo el 1er elemento del vector fOffset en t3
	addu t3, t3, a1		#int f = fOffsets[seleccionado] + i;
	lw t4, 0(s1)		#carga el 1er elemento del vector cOffset en t4
	addu t4, t4, a2		#int c = cOffsets[seleccionado] + j;

suma_f:
	blt t3, zero, suma_fila
	j resta_f

suma_fila:
	add t3, t3, a3		#fila = fila + m

resta_f:
	slt t6, t3, a3
	beq t6, zero, resta_fila
	j suma_c

resta_fila:
	subu t3, t3, a3		#fila = fila - m

suma_c:
	blt t4, zero, suma_col
	j resta_c

suma_col:
	add t4, t4, a3

resta_c:
	slt t6, t4, a3
	beq t6, zero, resta_col
	j cuentas

resta_col:
	subu t4, t4, a3

cuentas:
	mult a3, t3
	mflo t5
	addu t5, t5, t4				# (m*f)+c
	addu t1, a0, t5 			# Sumo la cantidad de posiciones para posicionarme
	lb t1, (t1)
	beq t1, 1, encontrados		#matriz[posicion] == 1
	j sumar_it

encontrados:
	addu t7, t7, 1		#encontrados + 1

sumar_it:
	addi s0,s0,4	#Incremento la direccion base de fOffset
	addi s1,s1,4	#Incremento la direccion base de cOffset
	addu t0, t0, 1
	blt t0, 8, loop	#Repito hasta completar todos los vecinos
	j salir

salir:
	add v0, t7, zero
	lw ra, 16(sp)
	lw gp, 20(sp)
	lw $fp, 24(sp)

   	addi sp, sp, 4
	jr ra

.end vecinos_s
.size vecinos_s, .-vecinos_s
