#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <pigpio.h>

#define FILCOL 8

int pines_filas[FILCOL] = {24, 25, 8, 7, 12, 16, 20, 21};   // Pines GPIO para las filas
int pines_columnas[FILCOL] = {26, 19, 13, 6, 5, 11, 9, 10}; // Pines GPIO para las columnas

volatile sig_atomic_t signalCatch = 0;

/* función maneja las señales recibidas del usuario */
void signal_handler(int signal)
{
    signalCatch = signal;
}

/* Uso de función específica de pausa para mayor control */
void pausa()
{
    usleep(100); // 10^0 = 1 MicroSegundos | 10^3 = MiliSegundos | 10^6 = Segundos
}

/* Preparar los pines del GPIO */
void iniciarGPIO()
{
    for (int i = 0; i < FILCOL; i++)
    {
        for (int j = 0; j < FILCOL; j++)
        {
            gpioSetMode(pines_filas[i], PI_OUTPUT);
            gpioSetMode(pines_columnas[j], PI_OUTPUT);
        }
    }
}

/* Liberar los pines del GPIO */
void finalizarGPIO()
{
    for (int i = 0; i < FILCOL; i++)
    {
        for (int j = 0; j < FILCOL; j++)
        {
            gpioSetMode(pines_filas[i], PI_INPUT);
            gpioSetMode(pines_columnas[j], PI_INPUT);
        }
    }
}

/* Test de Leds */
void encenderLeds()
{
    for (int i = 0; i < FILCOL; i++)
    {
        for (int j = 0; j < FILCOL; j++)
        {
            gpioWrite(pines_columnas[j], PI_LOW);
            gpioWrite(pines_filas[i], PI_HIGH);
            pausa();
            gpioWrite(pines_filas[i], PI_LOW);
            gpioWrite(pines_columnas[j], PI_HIGH);
        }
    }
}

void leerArchivo(int matriz[FILCOL][FILCOL])
{
    FILE *archivo = fopen("matriz.txt", "r");
    if (archivo == NULL)
    {
        printf("Error al abrir el archivo");
        return;
    }

    for (int i = 0; i < FILCOL; i++)
    {
        for (int j = 0; j < FILCOL; j++)
        {
            if (fscanf(archivo, "%d", &matriz[i][j]) != 1)
            {
                printf("Error al leer el archivo");
                fclose(archivo);
                return;
            }
        }
    }

    fclose(archivo);
}

void controlarLeds(int matriz[FILCOL][FILCOL])
{
    for (int i = 0; i < FILCOL; i++)
    {
        for (int j = 0; j < FILCOL; j++)
        {
            if (matriz[i][j] == 1)
            {
                gpioWrite(pines_filas[i], PI_HIGH);
                gpioWrite(pines_columnas[j], PI_LOW);
            }
            else
            {
                gpioWrite(pines_filas[i], PI_LOW);
                gpioWrite(pines_columnas[j], PI_HIGH);
            }

            pausa();
        }
    }
}

void menuDeSeleccion()
{
    int opcion;

    while (!signalCatch) // Loop hasta que el usuario decida cancelar
    {
        printf("\nMenu de Seleccion:\n");
        printf("1. Testeo de LEDs\n");
        printf("2. Leer desde archivo\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        if (opcion == 1)
        {
            printf("\nEjecutando Testeo de LEDs...\n");
            encenderLeds();
        }
        else if (opcion == 2)
        {
            printf("\nLeyendo desde archivo...\n");
            int matriz[FILCOL][FILCOL];
            leerArchivo(matriz);
            printf("\nControlando LEDs segun la matriz...\n");
            controlarLeds(matriz);
        }

        printf("\nPresione ENTER para repetir la opcion o CTRL-C para volver al menu principal...");

        // Limpiar el buffer del teclado
        while (getchar() != '\n')
            ;
    }

    // Si se presiona CTRL-C, limpiar el buffer del teclado
    while (getchar() != '\n')
        ;
}

int main()
{
    // Función para que el usuario cancele el programa
    signal(SIGINT, signal_handler);
    printf("Presione CTRL-C para salir del programa\n");

    if (gpioInitialise() == PI_INIT_FAILED)
    {
        printf("ERROR: No fue posible inicializar GPIO\n");
        return 1;
    }

    iniciarGPIO();
    menuDeSeleccion();
    gpioTerminate();
    printf("Programa Finalizado\n");
    finalizarGPIO();

    return 0;
}
