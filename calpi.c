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
int solicit (void);
void collect (double sum, double tempo_inicial, double tempo_final);

int main()
     
{
  /* Este programa bem simples aproxima pi calculando pi = integral 
   * de 0 até 1 de 4/(1+x*x)dx que é aproximando pela soma de 
   * k=1 até N de 4 / ((1+[(1/N)*(k-1/2)]**2) e então
   * multiplicando a soma por (1/N). (Esta regra de integração numérica
   * é chamada de "Midpoint rule" e pode ser encontrada em vários livros
   * de análise numérica). O único dado de entrada necessário é N.                                  
   */
  double sum, w;
  int i, N;
  double tempo_inicial, tempo_final;

  /* 
   * A rotina solicita irá pegar e propagar o valor de N
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
     * A rotina coleta irá coletar e imprimir resultados 
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
  printf ("Entre o número de intervalos para a  aproximação:(0 para terminar)\n");
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

