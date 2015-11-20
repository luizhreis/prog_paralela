#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void gera_vetor( long *, long );
long get_tam();
void print_vetor( long *, long );
long get_qtd_menor( long *, long, long );

int main( int argc, char** argv ){
	int rank, np, dest = 0, tag = 50, iniciado, finalizado;
	long *v1, *v2, resp[2], tam, temp, mod;
	double tempo_inicial, tempo_final;
	MPI_Status status;
	MPI_Comm com=MPI_COMM_WORLD;
	MPI_Initialized(&iniciado);
	if(!iniciado) MPI_Init(&argc, &argv);
	MPI_Comm_rank(com, &rank);
	MPI_Comm_size(com, &np);

	if (rank == 0) tam = get_tam();
	MPI_Bcast(&tam, 1, MPI_LONG, 0, com);
	v1 = ( long * ) malloc( sizeof( long ) * tam );
	if( v1 == NULL ){
		printf("{%2d} [x] ERRO: memoria não foi alocada para o array...\n", rank);
		return 1;
	}
	if( rank == 0 ){
		v2 = ( long * ) malloc( sizeof( long ) * tam );
		if( v2 == NULL ){
			printf("{%2d} [x] ERRO: memoria não foi alocada para o array...\n", rank);
			return 1;
		}
		printf("{%2d} [*] INFO: preenchendo vetor...\n", rank);
		gera_vetor( v1, tam );
		printf("{%2d} [*] INFO: exibindo vetor criado...\n", rank );
		print_vetor( v1, tam );
	}
	tempo_inicial = MPI_Wtime();
	MPI_Bcast( v1, tam, MPI_LONG, 0, com);
	//print_vetor( v1, tam );
	MPI_Barrier( com );
	printf("{%2d} [*] INFO: ordenando elementos...\n", rank );
	for ( int i = rank; i < tam; i += np ){
		temp = get_qtd_menor( v1, tam, i );
		if( rank == 0){
			v2[temp] = v1[i];
			if( (tam - i) < np ){
				mod = tam - i;
			}
			else{
				mod = 0;
			}
			for(int s = 1; s < np - mod; s++){
				MPI_Recv(&resp, 2, MPI_LONG, s, tag, com, &status);
				v2[ resp[1] ] = resp[0];
			}
		}
		else{
			resp[0] = v1[i];
			resp[1] = temp;
			MPI_Send(&resp, 2, MPI_LONG, dest, tag, com);
		}
		
	}
	if( rank == 0 ){
		tempo_final = MPI_Wtime();
		printf("{%2d} [*] INFO: exibindo vetor ordenado...\n", rank );
		print_vetor( v2, tam );
		printf("tempo decorrido: %f segundos\n", tempo_final - tempo_inicial);
	}
	free( v1 );
	free( v2 );
	MPI_Finalized(&finalizado);
	if(!finalizado) MPI_Finalize();
	return 0;
}

long get_qtd_menor( long *v, long tam, long indice){
	long i, qtd = 0;
	for ( int i = 0; i < tam; ++i ){
		if ( i == indice ) continue;
		if ( v[i] < v[indice] ) qtd++;
	}
	return qtd;
}

long get_tam(){
	long tam;
	printf("Informe o tamanho do vetor: \n");
	scanf("%ld", &tam);
	return tam;
}

void gera_vetor( long *v, long tam ){
	long i, j;
	int flag;
	for( i = 0; i < tam; i++ ){
		flag = 1;
		while( flag ){
			flag = 0;
			v[i] = rand() % ( tam * 10 );
			for( j = 0; j < i; j++ ){
				if( v[j] == v[i] ){
					flag = 1;
					break;
				}
			}
		}
	}
}

void print_vetor( long *v, long tam ){
	long i;
	for( i = 0; i < tam; i++ ){
		printf( "%ld, ", v[i] );
	}
	printf( "\n" );
}
