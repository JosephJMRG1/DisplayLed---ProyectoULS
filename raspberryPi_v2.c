#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <pigpio.h>

#define FILAS 8
#define COLUMNAS 8

const int pines_filas[FILAS] = {2, 3, 4, 5, 6, 7, 8, 9};               // Pines GPIO para las filas
const int pines_columnas[COLUMNAS] = {10, 11, 12, 13, 14, 15, 16, 17}; // Pines GPIO para las columnas

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
    for (int i = 0; i < COLUMNAS; i++)
    {
        for (int j = 0; j < FILAS; j++)
        {
            gpioSetMode(pines_columnas[j], PI_OUTPUT);
        }
    }
}

/* Test de Leds */

void encenderLeds()
{
    for (int i = 0; i < COLUMNAS; i++)
    {
        for (int j = 0; j < count; j++)
        {
            gpioWrite(pines_columnas[FILAS], PI_LOW);
            gpioWrite(pines_columnas[COLUMNAS], PI_HIGH);
            pausa();
            gpioWrite(pines_columnas[FILAS], PI_HiGH);
            gpioWrite(pines_columnas[COLUMNAS], PI_LOW);
        }
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

    int regressive = 0;
    for (int i = 3; regressive < i; i--)
    {
        printf("%i\n\n", i);
        usleep(1000000);
    }

    iniciarGPIO();
    encenderLeds();

    gpioTerminate();
    printf("Programa Finalizado\n");

    return 0;
}
