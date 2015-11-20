#include <stdio.h>
#include <stdlib.h>

void gera_vetor( long *, long );
void print_vetor( long *, long );
long get_qtd_menor( long *, long, long );

int main( long argc, char** argv ){
	long *v1, *v2, tam, temp;
	printf("Informe o tamanho do vetor: ");
	scanf("%ld", &tam);
	v1 = ( long * ) malloc( sizeof( long ) * tam );
	v2 = ( long * ) malloc( sizeof( long ) * tam );
	if( v1 == NULL || v2 == NULL ){
		printf("[x] ERRO: memoria não foi alocada para o array...\n");
		return 1;
	}
	printf("[*] INFO: preenchendo vetor...\n");
	gera_vetor( v1, tam );
	printf("[*] INFO: exibindo vetor criado...\n" );
	print_vetor( v1, tam );
	printf("[*] INFO: ordenando elementos...\n" );
	for ( int i = 0; i < tam; ++i ){
		temp = get_qtd_menor( v1, tam, i );
		v2[temp] = v1[i];
	}
	printf("[*] INFO: exibindo vetor ordenado...\n" );
	print_vetor( v2, tam );
	free( v1 );
	free( v2 );
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
