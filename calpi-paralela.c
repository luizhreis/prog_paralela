/*
 * Paralelização
 * Calpi
 * calpi.c
 * Ultima revisão GPS 18/08/04
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mpi.h"

#define f(x) ((double)(4.0/(1.0+x*x)))
#define pi ((double)(4.0*atan(1.0)))

void startup (void);
int solicita (void);
void coleta (double, double, double);

int main(int argc, char** argv){
	/* Este programa bem simples aproxima pi calculando pi = integral 
	* de 0 até 1 de 4/(1+x*x)dx que é aproximando pela soma de 
	* k=1 até N de 4 / ((1+[(1/N)*(k-1/2)]**2) e então
	* multiplicando a soma por (1/N). (Esta regra de integração numérica
	* é chamada de "Midpoint rule" e pode ser encontrada em vários livros
	* de análise numérica). O único dado de entrada necessário é N.                                  
	*/
	double sum, w, total = 0;
	int i, N;
	int rank, np, dest = 0, tag = 50, iniciado, finalizado;
	double tempo_inicial, tempo_final;
	MPI_Status status;
	MPI_Initialized(&iniciado);
	if(!iniciado) MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	/* 
	* A rotina solicita irá pegar e propagar o valor de N
	*/

	if(rank == 0) N = solicita();
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

	//while (N > 0) {
		tempo_inicial = MPI_Wtime();
		w = 1.0/(double)N;
		sum = 0.0;
		total = 0.0;
		for (i = rank; i <= N; i += np) sum = sum + f(((double)i-0.5)*w);
		//sum = sum * w;
		/*
		* A rotina coleta irá coletar e imprimir resultados 
		*/

		if(rank == 0){
			total += sum;
			for(int s = 1; s < np; s++){
				MPI_Recv(&sum, 1, MPI_DOUBLE, s, tag, MPI_COMM_WORLD, &status);
				total += sum;
			}
			total = total * w;
			tempo_final = MPI_Wtime();
			coleta(total, tempo_inicial, tempo_final);
			//N = solicita ();
			//MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
		}
		else{
			MPI_Send(&sum, 1, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
		}
	//}
	MPI_Finalized(&finalizado);
	if(!finalizado) MPI_Finalize();
	return (0);
}


/*  --------------------------------------------------------------  */
int solicita (void)
{
  int N;
  printf ("Entre o número de intervalos para a  aproximação:(0 para terminar)\n");
  scanf("%d",&N);
  return (N);
}

/*  --------------------------------------------------------------  */
void coleta(double total, double tempo_inicial, double tempo_final)
{
	double err;
	err = total - pi;
	printf("soma, erro = %7.5f, %10e\n", total, err);
	printf("tempo decorrido: %f segundos\n", tempo_final - tempo_inicial);
}

