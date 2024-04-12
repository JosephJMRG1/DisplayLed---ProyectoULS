#include <stdio.h>
#include <signal.h>
#include <pigpio.h>
#include <time.h>

#define FILAS 8
#define COLUMNAS 8

const int pines_filas[FILAS] = {2, 3, 4, 5, 6, 7, 8, 9};               // Pines GPIO para las filas
const int pines_columnas[COLUMNAS] = {10, 11, 12, 13, 14, 15, 16, 17}; // Pines GPIO para las columnas

volatile sig_atomic_t recibida_señal = 0;

void manejador_señal(int señal)
{
    recibida_señal = señal;
}

void inicializar_gpio()
{
    if (gpioInitialise() == PI_INIT_FAILED)
    {
        printf("ERROR: No fue posible inicializar GPIO\n");
        exit(1);
    }

    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            gpioSetMode(pines_columnas[j], PI_OUTPUT);
        }
    }
}

void mostrar_matriz_led()
{
    int fila, columna;

    while (!recibida_señal)
    {
        for (fila = 0; fila < FILAS; fila++)
        {
            for (columna = 0; columna < COLUMNAS; columna++)
            {
                // Enciende el LED en la fila y columna actual
                gpioWrite(pines_filas[fila], PI_LOW);
                gpioWrite(pines_columnas[columna], PI_HIGH);
                time_sleep(0.05); // Tiempo para "frecuencia de actualización"
                // Apaga el LED en la fila y columna actual
                gpioWrite(pines_filas[fila], PI_HIGH);
                gpioWrite(pines_columnas[columna], PI_LOW);
            }
        }
    }
}

int main()
{
    signal(SIGINT, manejador_señal);
    printf("Presione CTRL-C para salir\n");

    inicializar_gpio();
    mostrar_matriz_led();

    gpioTerminate();
    printf("Programa Finalizado\n");

    return 0;
}
