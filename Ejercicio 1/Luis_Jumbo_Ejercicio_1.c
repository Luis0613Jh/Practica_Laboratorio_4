/*
    Desarrollado por: Luis Jumbo
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define FILAS 1000
#define COLUMNAS 1000

void generarMatrizAleatoria(int matriz[FILAS][COLUMNAS], int* numero_A_Buscar, int proceso);
int buscarNumeroEnMatriz(int matriz[FILAS][COLUMNAS], int numero_A_Buscar, int proceso, int numeroProcesadores);

int main() {
    int proceso, numeroProcesadores, i, j, numero_A_Buscar, contadorPorProceso, contadorTotal;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &proceso);
    MPI_Comm_size(MPI_COMM_WORLD, &numeroProcesadores);
    
    int matriz[FILAS][COLUMNAS];
    generarMatrizAleatoria(matriz, &numero_A_Buscar, proceso);

    printf(">>> SOY EL PROCESO: %d EN EL PROCESO RAÍZ\n", proceso);
    contadorPorProceso = buscarNumeroEnMatriz(matriz, numero_A_Buscar, proceso, numeroProcesadores);
    MPI_Allreduce(&contadorPorProceso, &contadorTotal, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    
    if (proceso == 0) {
        printf("El número %d aparece %d veces en la matriz.\n", numero_A_Buscar, contadorTotal);
        printf("*****************************************\n");
        printf("*\tDesarrollado por: Luis Jumbo\t*\n");
        printf("*****************************************\n");
    }
    
    MPI_Finalize();
    return 0;
}

void generarMatrizAleatoria(int matriz[FILAS][COLUMNAS], int* numero_A_Buscar, int proceso) {
    if (proceso == 0) {
        printf(">>> SOY EL PROCESO: %d\n", proceso);
        int i, j;

        srand(time(NULL));

        for (i = 0; i < FILAS; i++) {
            for (j = 0; j < COLUMNAS; j++) {
                matriz[i][j] = rand() % 10000 + 1;
            }
        }

        printf("Ingrese el número comprendido entre 1 y 10000 a encontrar en la matriz:\n");
        scanf("%d", numero_A_Buscar);
    }
    MPI_Bcast(numero_A_Buscar, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

int buscarNumeroEnMatriz(int matriz[FILAS][COLUMNAS], int numero_A_Buscar, int proceso, int numeroProcesadores) {
    int contador = 0;
    int i, j;
    for (i = proceso; i < FILAS; i += numeroProcesadores) {
        for (j = 0; j < COLUMNAS; j++) {
            if (matriz[i][j] == numero_A_Buscar) {
                contador++;
            }
        }
    }
    return contador;
}

