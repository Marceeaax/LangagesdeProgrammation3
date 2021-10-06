#include <stdio.h>
#define FILAS_MATRIZ_A 3
#define COLUMNAS_MATRIZ_A 3
#define FILAS_MATRIZ_B 3
#define COLUMNAS_MATRIZ_B 2
#define MAX_THREAD 2

int matrizA[FILAS_MATRIZ_A][COLUMNAS_MATRIZ_A] = {
 {3, 2, 1},
 {1, 1, 3},
 {0, 2, 1}
};

int matrizB[FILAS_MATRIZ_B][COLUMNAS_MATRIZ_B] = {
 {2, 1},
 {1, 0},
 {3, 2}
};

int producto[FILAS_MATRIZ_B][COLUMNAS_MATRIZ_B];

void aux_multiplicar(int fil, int col) {
	 producto[fil][col] = 0;
	 for (int j = 0; j < COLUMNAS_MATRIZ_A; j++) 
	 {
	 	producto[fil][col] += matrizA[fil][j] * matrizB[j][col];
	 }
}

int main(void) 
{
	pthread_t threads[MAX_THREAD];
	for(i = 0;i < MAX_THREAD;i++)
	{
		int* p;
		
	}
	
	for (int i = 0; i < MAX_THREAD; i++)
        pthread_join(threads[i], NULL);
        
	 for (int col = 0; col < COLUMNAS_MATRIZ_B; col++) {
		 for (int fil = 0; fil < FILAS_MATRIZ_A; fil++) {
		 	aux_multiplicar(fil, col);
		 }
	 }

	 printf("Imprimiendo producto\n");
	 for (int i = 0; i < FILAS_MATRIZ_B; i++) 
	 {
		 for (int j = 0; j < COLUMNAS_MATRIZ_B; j++) 
		 {
		 	printf("%d ", producto[i][j]);
		 }
		 printf("\n");
	 }
}
