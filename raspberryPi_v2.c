#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <pigpio.h>

#define FILAS 8
#define COLUMNAS 8
#define FILCOL 8

const int pines_filas[FILAS] = {24, 25, 8, 7, 12, 16, 20, 21};      // Pines GPIO para las filas
const int pines_columnas[COLUMNAS] = {26, 19, 13, 6, 5, 11, 9, 10}; // Pines GPIO para las columnas

volatile sig_atomic_t received_signal = 0;

/* función maneja las señales recibidas del usuario */

void signal_handler(int signal)
{
    received_signal = signal;
}

/* Uso de función específica de pausa para mayor control */

void pausa()
{
    usleep(100); // 10^0 = 1 Segundo | 10^-3 = MiliSegundos | 10^-6 = MicroSegundos
}

/* Dejar en limpio pines del GPIO */

void iniciarGPIO()
{
    for (int i = 0; i < FILCOL; i++)
    {
        gpioSetMode(pines_filas[I], PI_OUTPUT);
        gpioSetMode(pines_columnas[j], PI_OUTPUT);
    }
}

/* Test de Leds */

void encenderLeds()
{
    for (int i = 0; i < FILCOL; i++)
    {
        gpioWrite(pines_columnas[FILAS], PI_LOW);
        gpioWrite(pines_columnas[COLUMNAS], PI_HIGH);
        pausa();
        gpioWrite(pines_columnas[FILAS], PI_HiGH);
        gpioWrite(pines_columnas[COLUMNAS], PI_LOW);
    }
}

int main()
{
    // Funciones para que el usuario cancele el programa
    signal(SIGINT, signal_handler);
    printf("Presione CTRL-C para salir\n");
    if (gpioInitialise() == PI_INIT_FAILED)
    {
        printf("ERROR: No fue posible inicializar GPIO\n");
        exit(1);
    }
    else
    {
        int regresiva = 0;
        for (int i = 3; regresiva < i; i--)
        {
            printf("%i\n\n", i);
            usleep(1000000);
        }
    }

    iniciarGPIO();
    while (!received_signal)
    {
        encenderLeds(tablero);
    }

    gpioTerminate();
    printf("Programa Finalizado\n");

    return 0;
}
