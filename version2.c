#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h> // Incluir para SetConsoleTextAttribute
#define WALL_WIDTH 3
#define CELL_WIDTH 2
#define SPACE ' '
#define MAX_SIZE 10 // Ajusta este valor según tus necesidades
#define MAX_COLORS 10 // Máximo de colores disponibles

void inicializarTablero(int board[MAX_SIZE][MAX_SIZE], int sector[MAX_SIZE][MAX_SIZE], int N) {
    int i, j; // Declaración de variables de los bucles
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            board[i][j] = 0;   // Espacio vacío
            sector[i][j] = 0;  // Sin paredes
        }
    }
}

void leerZonas(char zonas[MAX_SIZE][MAX_SIZE], int N, FILE *file) {
    int i, j; // Declaración de variables de los bucles
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            fscanf(file, " %c", &zonas[i][j]); // Lee cada carácter
        }
    }
}

void printZonas(char zonas[MAX_SIZE][MAX_SIZE], int N) {
    int i, j; // Declaración de variables de los bucles
    printf("Matriz de zonas:\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%c ", zonas[i][j]);
        }
        printf("\n");
    }
}

void setColor(int color) {
    // Cambiar el color del texto en la consola
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void resetColor() {
    // Resetear el color a blanco (o el color por defecto)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // 7 es el color blanco
}

void printBoard(int board[MAX_SIZE][MAX_SIZE], int sector[MAX_SIZE][MAX_SIZE], char zonas[MAX_SIZE][MAX_SIZE], int N) {
    int i, j, k; // Declaración de variables de los bucles
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            // Pared superior
            printf("+");
            for (k = 0; k < WALL_WIDTH; k++) {
                printf("%c", (sector[i][j] & 1) ? '-' : SPACE);
            }
        }
        printf("+\n");

        // Contenido de la celda y paredes laterales
        for (j = 0; j < N; j++) {
            // Pared izquierda
            printf("%c", (sector[i][j] & 4) ? '|' : SPACE);

            // Espacio adicional si hay una pared izquierda o si es la primera columna
            if ((sector[i][j] & 4) || j == 0) {
                printf("%c", SPACE);
            }

            // Pintar las estrellas según la matriz de zonas
            if (board[i][j] == 1) {
                int color = (zonas[i][j] - '0') % MAX_COLORS + 1; // Convertir a color (0-9) y evitar color 0
                setColor(color);
                printf("%*c", CELL_WIDTH, '*');
                resetColor();
            } else {
                printf("%*c", CELL_WIDTH, SPACE);
            }

            // Pared derecha
            printf("%c", (sector[i][j] & 2) ? '|' : SPACE);
        }
        printf("\n");

        // Pared inferior
        for (j = 0; j < N; j++) {
            printf("+");
            for (k = 0; k < WALL_WIDTH; k++) {
                printf("%c", (sector[i][j] & 8) ? '-' : SPACE);
            }
        }
        printf("+\n");
    }
}

void fillBoardWithStars(int board[MAX_SIZE][MAX_SIZE], int N) {
    int i, j; // Declaración de variables de los bucles
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            board[i][j] = 1; // Set each cell to 1 (star)
        }
    }
}

// Verificar si se puede colocar una estrella en la posición (row, col)
int esSeguro(int board[MAX_SIZE][MAX_SIZE], int row, int col, int N) {
    int i, r, c; // Declaración de variables de los bucles
    
    // Verificar la fila y la columna
    for (i = 0; i < N; i++) {
        if (board[row][i] == 1 || board[i][col] == 1) {
            return 0; // No se puede colocar
        }
    }
    
    // Verificar sectores y las celdas vecinas
    int sectorRowStart = (row / 2) * 2; // Cambia esto según la división de sectores
    int sectorColStart = (col / 2) * 2; // Cambia esto según la división de sectores

    for (i = sectorRowStart; i < sectorRowStart + 2; i++) {
        for (int j = sectorColStart; j < sectorColStart + 2; j++) {
            if (board[i][j] == 1) {
                return 0; // No se puede colocar
            }
        }
    }

    // Verificar las celdas vecinas
    for (r = row - 1; r <= row + 1; r++) {
        for (c = col - 1; c <= col + 1; c++) {
            if (r >= 0 && r < N && c >= 0 && c < N && board[r][c] == 1) {
                return 0; // Vecino tiene estrella
            }
        }
    }

    return 1; // Se puede colocar
}

// Función de backtracking para resolver el problema
int resolverEstrellas(int board[MAX_SIZE][MAX_SIZE], int N, int row, int col) {
    if (row == N) {
        return 1; // Solución encontrada
    }

    if (col == N) {
        return resolverEstrellas(board, N, row + 1, 0); // Pasar a la siguiente fila
    }

    // Omitir la celda si ya hay una estrella
    if (board[row][col] == 1) {
        return resolverEstrellas(board, N, row, col + 1); // Continuar con la siguiente celda
    }

    // Intentar colocar una estrella en la celda actual
    if (esSeguro(board, row, col, N)) {
        board[row][col] = 1; // Colocar estrella

        // Continuar con la siguiente celda
        if (resolverEstrellas(board, N, row, col + 1)) {
            return 1; // Solución encontrada
        }

        // Deshacer el movimiento
        board[row][col] = 0; // Retirar estrella
    }

    // Intentar la siguiente celda
    return resolverEstrellas(board, N, row, col + 1);
}

int main() {
    int N, i, j;
    int board[MAX_SIZE][MAX_SIZE];
    int sector[MAX_SIZE][MAX_SIZE];
    char zonas[MAX_SIZE][MAX_SIZE];

    // Abrir el archivo de input
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
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            fscanf(file, "%d", &board[i][j]);
        }
    }

    // Leer la matriz de sectores
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            fscanf(file, "%d", &sector[i][j]);
        }
    }

    // Leer y mostrar la matriz de zonas
    leerZonas(zonas, N, file);
    printZonas(zonas, N);

    // Cerrar el archivo
    fclose(file);

    // Imprimir el tablero inicial
    printf("Tablero inicial:\n");
    printBoard(board, sector, zonas, N);

  if (resolverEstrellas(board, N, 0, 0)) {
        printf("Solución encontrada:\n");
        printBoard(board, sector, zonas, N);
    } else {
        printf("No se encontró solución.\n");
    }

    return 0;
}
