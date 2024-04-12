#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define FIL 8
#define COL 8
#define PI_LOW 0
#define PI_HIGH 1

volatile sig_atomic_t received_signal = 0;

/* Esta función maneja las señales recibidas del usuario */

void signal_handler(int signal)
{
    received_signal = signal;
}

/* Uso de función específica de pausa para mayor control */

void pausa()
{
    usleep(1000000); // 10^0 = 1 Segundo | 10^-3 = MiliSegundos | 10^-6 = MicroSegundos
}

/* Sección dirigida a la simulación de la placa de cobre */

void tab(int tablero[COL][FIL])
{
    for (int i = 0; i < COL; i++)
    {
        for (int j = 0; j < FIL; j++)
        {
            printf("%3d", tablero[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void limpiarTab(int tablero[COL][FIL])
{
    for (int i = 0; i < COL; i++)
    {
        for (int j = 0; j < FIL; j++)
        {
            tablero[i][j] = 0;
        }
    }
}

/* Simulacion del encendido y apagado de los leds */

void recorrerTab(int tablero[COL][FIL])
{
    for (int i = 0; i < COL; i++)
    {
        for (int j = 0; j < FIL; j++)
        {
            tablero[i][j] = PI_HIGH;
            tab(tablero);
            pausa();
            tablero[i][j] = PI_LOW;
        }
        printf("\n");
    }
}

int main()
{

    int tablero[COL][FIL];

    signal(SIGINT, signal_handler);
    printf("Presione CTRL + C para salir...\n\n");

    int regressive = 0;
    for (int i = 3; regressive < i; i--)
    {
        printf("%i\n\n", i);
        usleep(1000000);
    }

    limpiarTab(tablero);

    while (!received_signal)
    {
        recorrerTab(tablero);
    }

    return 0;
}
