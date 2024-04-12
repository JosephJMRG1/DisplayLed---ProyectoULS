#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <pigpio.h>
#include <time.h>

#define FILAS 8
#define COLUMNAS 8

const int pines_filas[FILAS] = {24,25,8,7,12,16,20,21}; // Pines GPIO para las filas
const int pines_columnas[COLUMNAS] = {26,19,13,6,5,11,9,10}; // Pines GPIO para las columnas

volatile sig_atomic_t recibida_señal = 0;

void manejador_señal(int señal) {
    recibida_señal = señal;
}

void inicializar_gpio() {
    if (gpioInitialise() == PI_INIT_FAILED) {
        printf("ERROR: No fue posible inicializar GPIO\n");
        exit(1);
    }

    for (int i = 0; i < FILAS; i++) {
        gpioSetMode(pines_filas[i], PI_OUTPUT);
    }

    for (int j = 0; j < COLUMNAS; j++) {
        gpioSetMode(pines_columnas[j], PI_OUTPUT);
    }
}

void mostrar_matriz_led(int matriz[FILAS][COLUMNAS]) {
    int fila, columna;

    while (!recibida_señal) {
        for (fila = 0; fila < FILAS; fila++) {
            for (columna = 0; columna < COLUMNAS; columna++) {
                if (matriz[fila][columna] == 1) {
                    gpioWrite(pines_filas[fila], PI_LOW); //enciende
                    gpioWrite(pines_columnas[columna], PI_HIGH);
                    time_sleep(0.1);
                    gpioWrite(pines_columnas[columna], PI_LOW); //apaga
                    gpioWrite(pines_filas[fila], PI_HIGH);
                }
            }
            time_sleep(0.1);
        }
    }
}

int main() {
    signal(SIGINT, manejador_señal);
    printf("Presione CTRL-C para salir\n");

    inicializar_gpio();
    
    //diagonal
    int matriz[FILAS][COLUMNAS] = {
        {1, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 1}
    };
    
    mostrar_matriz_led(matriz);

    gpioTerminate();
    printf("Programa Finalizado\n");

    return 0;
}
