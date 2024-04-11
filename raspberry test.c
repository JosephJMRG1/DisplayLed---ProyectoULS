#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define FIL 8
#define COL 8
#define PI_LOW 0
#define PI_HIGH 1

volatile sig_atomic_t received_signal = 0;

void signal_handler(int signal)
{
    received_signal = signal;
}

void pausa()
{
    usleep(1000000);
}

void tab(int tablero[FIL][COL])
{
    for (int i = 0; i < FIL; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            printf("%3d", tablero[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void limpiarTab(int tablero[FIL][COL])
{
    for (int i = 0; i < FIL; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            tablero[i][j] = 0;
        }
    }
}

void recorrerTab(int tablero[FIL][COL])
{
    for (int i = 0; i < FIL; i++)
    {
        for (int j = 0; j < COL; j++)
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
    signal(SIGINT, signal_handler);
    int tablero[FIL][COL];

    printf("Presione CTRL + C para salir...\n");

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
