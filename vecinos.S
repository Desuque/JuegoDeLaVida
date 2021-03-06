#include <mips/regdef.h>
	.data //Reservo memoria para los offset
	fOffset: .word 1, 1, 0, -1, -1, 0, -1,   1
	cOffset: .word -1, 0, 1, 1, 0, -1, -1,   1
	.text
	.align 2
	.globl	vecinos
	.ent	vecinos

//ARGUMENTOS (ABA del caller) (al menos 5 words)
#define A4_columnas 48
#define A3_filas 44
#define A2_fila 40
#define A1_columna 36
#define A0_pmatriz 32
//SRA (4 words)
#define FP 24
#define GP 20
#define RA 16

//y se reservan 4 words como ABA

//por lo tanto el stack de esta función mide 32 bytes / 8 words
#define TAM_STACK 32
/***
unsigned int vecinos(unsigned char* matriz,
		unsigned int i, unsigned int j,
		unsigned int m, unsigned int n){
***/ 
vecinos:
	//construir stack
	subu sp, sp, TAM_STACK
   	sw ra, RA(sp)
  	sw gp, GP(sp)
	sw $fp, FP(sp)

	sw a0, A0_pmatriz(sp)		//a0=unsigned char* matriz
	sw a1, A1_columna(sp)		//a1=unsigned int i
	sw a2, A2_fila(sp)		//a2=unsigned int j
	sw a3, A3_filas(sp)		//a3=unsigned int m
	lw t0, A4_columnas(sp)		//t0=unsigned int n

	addu t1, zero, zero 		// t1 = encontrados = 0
	addu t2, zero, zero 		// t2 = seleccionado = 0
	
	//addu v0, zero, zero//dbg

FOR:	sll t3, t2, 2 			//t3=seleccionado*4

//f y c
L16:	la t4, fOffset			//t4=foffset
	addu t4, t4, t3			//t4 = fOffset + seleccionado *4
	lw t4, 0(t4)			//t4 =*t4
	addu t4, t4, a1			//t4+=i , así, t4=int f 
	
L17:	la t5, cOffset 			//t5 = cOffset
	addu t5, t5, t3			//t5 = cOffset + seleccionado *4
	lw t5, 0(t5)			//t5=*t5
	addu t5, t5, a2			//t5+=j , así, t5=int c


//los if
L19:	bge t4, zero, L20 		//salteo si f>=0
	add t4, t4, a3			// f+= m

L20:	blt t4, a3, L22 		//salteo si f < m
	sub t4, t4, a3			//f-=m

L22:	bge t5, zero, L23 		//salteo si c>=0
	add t5, t5, t0			//c+=n

L23:	blt t5, t0, L25 		//salteo si c < n
	sub t5, t5, t0			//c-=n


//condicion para sumar
L25:	//n*f+c

	//addu v0, t5, zero//dbg

	mult t0, t4			//n*f a lo
	mflo t3				//ya no necesito, piso t3 con lo
	addu t3, t3, t5			//t3=t3+c, quedando unsigned int posicion en t3

L26:	addu t3, a0, t3			//t3+=a0, en t3 queda matriz+posicion (no se multiplica or 4 pq es de chars)
	lb t3, 0(t3)			//t3=*t3, ahora t3 tiene el dato (lb porque char tiene 1 byte)


	
	beq t3, zero, EPI_FOR		//si matriz[posicion]==0, salteo
	addiu t1,t1,1			//encontrados +=1

EPI_FOR:
	addu t2, t2, 1			//seleccionado +=1
	addiu t9, zero, 8		//t3 no lo necesito, lo piso con 8
	bltu t2, t9, FOR		//si seleccionado < 8, sigue la fiesta

L28:	addu v0, t1, zero		//return encontrados
	
	//destruir stack	
	lw ra, RA(sp)
	lw gp, GP(sp)
	lw $fp, FP(sp)

   	addi sp, sp, TAM_STACK
	jr ra
.end vecinos

