/*
    Desarrollado por: Luis Jumbo
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define TAMANIO 1000

void generar_matriz_aleatoria(int matriz[TAMANIO][TAMANIO]);

int main() {
    int proceso, numeroProcesadores;
    int matriz[TAMANIO][TAMANIO];
    int sumaDiagonalPrincipalParcial = 0, sumaDiagonalPrincipalTotal = 0;
    int sumaDiagonalSecundariaParcial = 0, sumaDiagonalSecundariaTotal = 0;
    int valorMáximoDiagonalPrincipal = 0, valorMáximoDiagonalSecundaria = 0;
    int diagonalConValorMasAlto = 0, valorMasAlto = 0;
    int diagonalConSumaMasGrande = 0, sumaMasGrande = 0;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &proceso);
    MPI_Comm_size(MPI_COMM_WORLD, &numeroProcesadores);

    if (proceso == 0) {
        printf(">>> SOY EL PROCESO: %d\n", proceso);
        generar_matriz_aleatoria(matriz);
    }
    // Broadcast de la matriz generada
    MPI_Bcast(matriz, TAMANIO*TAMANIO, MPI_INT, 0, MPI_COMM_WORLD);

    printf(">>> SOY EL PROCESO: %d EN EL PROCESO RAÍZ\n", proceso);
    // Calcular la suma de las diagonales en cada proceso
    for (int i = proceso; i < TAMANIO; i += numeroProcesadores) {
        sumaDiagonalPrincipalParcial += matriz[i][i];
        sumaDiagonalSecundariaParcial += matriz[i][TAMANIO-i-1];
    }
    // Reducción para obtener la suma total de las diagonales
    MPI_Reduce(&sumaDiagonalPrincipalParcial, &sumaDiagonalPrincipalTotal, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&sumaDiagonalSecundariaParcial, &sumaDiagonalSecundariaTotal, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Determinar el valor más alto de la diagonal principal y secundaria
    for (int i = 0; i < TAMANIO; i++) {
        if (matriz[i][i] > valorMáximoDiagonalPrincipal) {
            valorMáximoDiagonalPrincipal = matriz[i][i];
        }
        if (matriz[i][TAMANIO-i-1] > valorMáximoDiagonalSecundaria) {
            valorMáximoDiagonalSecundaria = matriz[i][TAMANIO-i-1];
        }
    }
    
    if (proceso == 0) {
        printf(">>> SOY EL PROCESO: %d\n", proceso);

        // Encontrar la diagonal con el valor más alto
        if (valorMáximoDiagonalPrincipal > valorMáximoDiagonalSecundaria) {
            diagonalConValorMasAlto = 1;
            valorMasAlto = valorMáximoDiagonalPrincipal;
        } else {
            diagonalConValorMasAlto = 2;
            valorMasAlto = valorMáximoDiagonalSecundaria;
        }

        // Encontrar la diagonal con la suma más grande
        if (sumaDiagonalPrincipalTotal > sumaDiagonalSecundariaTotal) {
            diagonalConSumaMasGrande = 1;
            sumaMasGrande = sumaDiagonalPrincipalTotal;
        } else {
            diagonalConSumaMasGrande = 2;
            sumaMasGrande = sumaDiagonalSecundariaTotal;
        }

        printf("La suma de la diagonal principal es: %d\n", sumaDiagonalPrincipalTotal);
        printf("La suma de la diagonal secundaria es: %d\n", sumaDiagonalSecundariaTotal);
        printf("El valor más alto de la diagonal principal es: %d\n", valorMáximoDiagonalPrincipal);
        printf("El valor más alto de la diagonal secundaria es: %d\n", valorMáximoDiagonalSecundaria);
        printf("La diagonal %d posee el valor más alto, el cual es: %d\n", diagonalConValorMasAlto, valorMasAlto);
        printf("La diagonal %d posee la suma más grande, la cual es: %d\n", diagonalConSumaMasGrande, sumaMasGrande);
        printf("*****************************************\n");
        printf("*\tDesarrollado por: Luis Jumbo\t*\n");
        printf("*****************************************\n");
    }
    MPI_Finalize();
    return 0;
}

void generar_matriz_aleatoria(int matriz[TAMANIO][TAMANIO]) {
    srand(time(NULL));
    for (int i = 0; i < TAMANIO; i++) {
        for (int j = 0; j < TAMANIO; j++) {
            matriz[i][j] = rand() % 10000 + 1;
        }
    }
}

