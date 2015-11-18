/*
 * Paraleliza��o
 * Calpi
 * calpi.c
 * Ultima revis�o GPS 18/08/04
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mpi.h"

#define f(x) ((double)(4.0/(1.0+x*x)))
#define pi ((double)(4.0*atan(1.0)))

void startup (void);
int solicit (void);
void collect (double sum);

int main(int argc, char** argv){
	/* Este programa bem simples aproxima pi calculando pi = integral 
	* de 0 at� 1 de 4/(1+x*x)dx que � aproximando pela soma de 
	* k=1 at� N de 4 / ((1+[(1/N)*(k-1/2)]**2) e ent�o
	* multiplicando a soma por (1/N). (Esta regra de integra��o num�rica
	* � chamada de "Midpoint rule" e pode ser encontrada em v�rios livros
	* de an�lise num�rica). O �nico dado de entrada necess�rio � N.                                  
	*/
	double sum, w, total = 0;
	int i, N;
	int rank, p, dest = 0, tag = 50;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	/* 
	* A rotina solicita ir� pegar e propagar o valor de N
	*/

	if(rank == 0) N = solicita();
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

	while (N > 0) {
		w = 1.0/(double)N;
		sum = 0.0;
		for (i = rank; i <= N; i += np) sum = sum + f(((double)i-0.5)*w);
		sum = sum * w;
		/*
		* A rotina coleta ir� coletar e imprimir resultados 
		*/

//ISSO ESTA ERRADO!!!! verificar
		if(rank == 0){
			for(int s = 0; s < np; s++){
				MPI_Recv(&sum, 1, MPI_DOUBLE, s, tag, MPI_COMM_WORLD, &status);
			}
			total += sum;
			coleta(total);
			N = solicita ();
		}
		else{
			MPI_Send(&sum, 1, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();
	return (0);
}


/*  --------------------------------------------------------------  */
int solicita (void)
{
  int N;
  printf ("Entre o n�mero de intervalos para a  aproxima��o:(0 para terminar)\n");
  scanf("%d",&N);
  return (N);
}

/*  --------------------------------------------------------------  */
void coleta(double sum)
{
  double err;
  err = sum - pi;
  printf("soma, erro = %7.5f, %10e\n", sum, err);
}

