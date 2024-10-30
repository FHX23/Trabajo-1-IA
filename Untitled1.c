#include <stdio.h>

#define MAX_SIZE 10  // Tamaño máximo de las matrices

void inicializarTablero(int board[MAX_SIZE][MAX_SIZE], int sector[MAX_SIZE][MAX_SIZE], int N) {
    // Inicializar el tablero y el sector
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            board[i][j] = 0;   // Espacio vacío
            sector[i][j] = 0;  // Sin paredes
        }
    }
}

void printBoard(int board[MAX_SIZE][MAX_SIZE], int sector[MAX_SIZE][MAX_SIZE], int N) {
    for (int i = 0; i < N; i++) {
        // Imprimir la parte superior de las celdas
        for (int j = 0; j < N; j++) {
            // Comprobar si hay pared arriba
            if (sector[i][j] & 1) {  // Pared arriba
                printf("+---");
            } else {
                printf("+   ");
            }
        }
        printf("+\n");

        // Imprimir el contenido del tablero
        for (int j = 0; j < N; j++) {
            // Comprobar si hay pared a la izquierda
            if (sector[i][j] & 8) {  // Pared izquierda
                printf("|");
            } else {
                printf(" ");
            }

            // Imprimir el valor del tablero
            if (board[i][j] == 1) {
                printf(" * ");  // Estrella
            } else {
                printf("   ");  // Espacio vacío
            }
        }
        // Imprimir la pared derecha
        if (sector[i][N-1] & 2) {  // Pared derecha
            printf("|");
        } else {
            printf(" ");
        }
        printf("\n");
    }

    // Imprimir la parte inferior de las celdas
    for (int j = 0; j < N; j++) {
        // Comprobar si hay pared abajo
        if (sector[N-1][j] & 4) {  // Pared abajo
            printf("+---");
        } else {
            printf("+   ");
        }
    }
    printf("+\n");
}

int main() {
    int N;
    int board[MAX_SIZE][MAX_SIZE];
    int sector[MAX_SIZE][MAX_SIZE];

    // Abrir el archivo de entrada
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }

    // Leer el tamaño de la matriz
    fscanf(file, "%d", &N);
    // Leer la cantidad de estrellas por sector, fila y columna
    int estrellasPorSector, estrellasPorFila, estrellasPorColumna;
    fscanf(file, "%d", &estrellasPorSector);
    fscanf(file, "%d", &estrellasPorFila);
    fscanf(file, "%d", &estrellasPorColumna);

    // Inicializar las matrices
    inicializarTablero(board, sector, N);

    // Leer la matriz de estrellas
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fscanf(file, "%d", &board[i][j]);
        }
    }

    // Leer la matriz de sectores
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fscanf(file, "%d", &sector[i][j]);
        }
    }

    // Cerrar el archivo
    fclose(file);

    // Imprimir el tablero
    printBoard(board, sector, N);

    return 0;
}

