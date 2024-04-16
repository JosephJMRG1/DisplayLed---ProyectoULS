#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>
#include <signal.h>

#define FILCOL 8

const int pines_filas[FILCOL] = {24, 25, 8, 7, 12, 16, 20, 21};   // Pines GPIO para las filas
const int pines_columnas[FILCOL] = {26, 19, 13, 6, 5, 11, 9, 10}; // Pines GPIO para las columnas

volatile sig_atomic_t received_signal = 0;

/* función maneja las señales recibidas del usuario */
void signal_handler(int signal)
{
    received_signal = signal;
}

/* Uso de función específica de pausa para mayor control */
void pausa()
{
    usleep(100); // 10^0 = 1 MicroSegundos | 10^3 = MiliSegundos | 10^6 = Segundos
}

/* Dejar en limpio pines del GPIO */
void iniciarGPIO()
{
    for (int i = 0; i < FILCOL; i++)
    {
        gpioSetMode(pines_filas[i], PI_OUTPUT);
        gpioSetMode(pines_columnas[i], PI_OUTPUT);
    }
}

/* Test de Leds */
void encenderLeds()
{
    for (int i = 0; i < FILCOL; i++)
    {
        gpioWrite(pines_columnas[i], PI_LOW);
        gpioWrite(pines_filas[i], PI_HIGH);  
        pausa();
        gpioWrite(pines_filas[i], PI_LOW);    
        gpioWrite(pines_columnas[i], PI_HIGH);
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

    while (fscanf(archivo, "%d", &valor) == 1)
    {
        if (valor == 1)
        {
            gpioWrite(pines_filas[fila], PI_HIGH);
            gpioWrite(pines_columnas[columna], PI_LOW);
        }
        else if (valor == 0)
        {
            gpioWrite(pines_filas[fila], PI_LOW);
            gpioWrite(pines_columnas[columna], PI_HIGH);
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
            iniciarGPIO();   // Reinicia los pines GPIO para evitar problemas de estado
        }
    }

    fclose(archivo);
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
    } else {

        printf("Presiona Enter para Comenzar...");
        getchar();
    }

    iniciarGPIO();
    while (!received_signal)
    {
        encenderLeds();
    }

    gpioTerminate();
    printf("Programa Finalizado\n");

    return 0;
}
