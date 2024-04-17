#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define FILCOL 8
// Funciones que encienden y apagan leds
#define PI_LOW 0
#define PI_HIGH 1

volatile sig_atomic_t received_signal = 0;

/* Esta función maneja las señales recibidas del usuario */
void signal_handler(int signal)
{
    if (signal == SIGINT)
    {
        received_signal = 1;
    }
}

/* Uso de función específica de pausa para mayor control */
void pausa()
{
    usleep(100); // 10^0 = 1 MicroSegundos | 10^3 = MiliSegundos | 10^6 = Segundos
}

/* Sección dirigida a la simulación de la placa de cobre */
void crearTab(int tablero[FILCOL][FILCOL])
{
    for (int i = 0; i < FILCOL; i++)
    {
        for (int j = 0; j < FILCOL; j++)
        {
            printf("%2i", tablero[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void limpiarTab(int tablero[FILCOL][FILCOL])
{
    for (int i = 0; i < FILCOL; i++)
    {
        for (int j = 0; j < FILCOL; j++)
        {
            tablero[i][j] = 0; // Asigna 0 (entero) a cada elemento de la matriz
        }
    }
}

void usarTab(int tablero[FILCOL][FILCOL])
{
    char espacio = ' ';
    for (int i = 0; i < FILCOL; i++)
    {
        for (int j = 0; j < FILCOL; j++)
        {
            if (tablero[i][j] == PI_LOW)
            {
                printf("%c", espacio); // Mostrar espacios en blanco si el valor es PI_LOW
            }
            else
            {
                printf("%3i", tablero[i][j]); // Mostrar el valor si no es PI_LOW
            }
        }
        printf("\n");
    }
    printf("\n");
}

/* Simulación del encendido y apagado de los leds */
void encenderLeds(int tablero[FILCOL][FILCOL])
{
    while (!received_signal)
    {
        for (int i = 0; i < FILCOL; i++)
        {
            for (int j = 0; j < FILCOL; j++)
            {
                tablero[i][j] = PI_HIGH;
                usarTab(tablero); // Mostrar el tablero con PI_HIGH
                pausa();
                tablero[i][j] = PI_LOW;
            }
            printf("\n");
        }
    }
}

void leerArchivo()
{
    FILE *archivo = fopen("matriz.txt", "r");
    if (archivo == NULL)
    {
        printf("Error al abrir el archivo");
        return;
    }

    int valor;
    int fila = 0;
    int columna = 0;
    int tablero[FILCOL][FILCOL];

    while (fscanf(archivo, "%d", &valor) == 1 && !received_signal)
    {
        if (valor == 1)
        {
            tablero[fila][columna] = PI_HIGH;
        }
        else if (valor == 0)
        {
            tablero[fila][columna] = PI_LOW;
        }

        columna++;
        if (columna == FILCOL)
        {
            columna = 0;
            fila++;
        }

        if (fila == FILCOL)
        {
            fila = 0;
            pausa();
        }
    }

    fclose(archivo);
}

void menuDeSeleccion(int tablero[FILCOL][FILCOL])
{
    int opcion;

    printf("\nMenu de Seleccion:\n");
    printf("1. Testeo de LEDs\n");
    printf("2. Leer desde archivo\n");
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);

    if (opcion == 1)
    {
        printf("\nEjecutando Testeo de LEDs...\n");
        encenderLeds(tablero);
        received_signal = 0;
    }
    else if (opcion == 2)
    {
        printf("\nLeyendo desde archivo...\n");
        leerArchivo();
        received_signal = 0;
    }
}

int main()
{
    int tablero[FILCOL][FILCOL];

    // Funciones para que el usuario cancele el programa
    signal(SIGINT, signal_handler);
    printf("Presione CTRL + C para volver al menu...\n\n");

    limpiarTab(tablero);

    while (1)
    {
        menuDeSeleccion(tablero);
    }

    return 0;
}
