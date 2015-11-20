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
void collect (double sum, double tempo_inicial, double tempo_final);

int main()
     
{
  /* Este programa bem simples aproxima pi calculando pi = integral 
   * de 0 at� 1 de 4/(1+x*x)dx que � aproximando pela soma de 
   * k=1 at� N de 4 / ((1+[(1/N)*(k-1/2)]**2) e ent�o
   * multiplicando a soma por (1/N). (Esta regra de integra��o num�rica
   * � chamada de "Midpoint rule" e pode ser encontrada em v�rios livros
   * de an�lise num�rica). O �nico dado de entrada necess�rio � N.                                  
   */
  double sum, w;
  int i, N;
  double tempo_inicial, tempo_final;

  /* 
   * A rotina solicita ir� pegar e propagar o valor de N
   */

  N = solicita();

  while (N > 0) {
    tempo_inicial = MPI_Wtime();
    w = 1.0/(double)N;
    sum = 0.0;
    for (i = 1; i <= N; i++)
      sum = sum + f(((double)i-0.5)*w);
    sum = sum * w;
    /*
     * A rotina coleta ir� coletar e imprimir resultados 
     */
    tempo_final = MPI_Wtime();
    coleta (sum, tempo_inicial, tempo_final);
    N = solicita ();
  }

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
void coleta(double sum, double tempo_inicial, double tempo_final)
{
  double err;
  err = sum - pi;
  printf("soma, erro = %7.5f, %10e\n", sum, err);
  printf("tempo decorrido: %f segundos\n", tempo_final - tempo_inicial);
}

