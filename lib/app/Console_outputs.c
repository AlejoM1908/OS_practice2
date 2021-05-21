#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/**
 * The function clear_stream is used to clear the pointer when scanf function is used
 * @param in is a pointer to a document
*/
void clear_stream(FILE *in)
{
    int ch;
    
    clearerr(in);
    
    do
        ch = getc(in);
    while (ch != '\n' && ch != EOF);
    
    clearerr(in);
}

/**
 * The function display_welcome is used to show in console the welcome interface
*/
void display_welcome()
{
    printf("##########################################\n");
    printf("### Bienvenido al sistema de busqueda. ###\n");
    printf("##########################################\n\n");
}

/**
 * The function input_source_ID is used to stock the source id and compare if is
 *  valid to search
 * @return the source ID typed by the user
*/
int input_source_ID(){
    bool exit = false;
    int number;

    while (!exit)
    {
        printf("\nPor favor, ingrese la ID de origen:\n");
        
        if (scanf("%d", &number) != 1)
        {
            clear_stream(stdin);
            printf("La ID ingresada no es valida. \n\n");
            fflush(stdout);
        }
        else if (number > 1160 || number < 0)
        {
            printf("El tamaño del id no es el correcto.\n");
        }
        else
        {
            printf("\n");
            exit = true;
        }
    }

    return number;
}

/**
 * The function input_destiny_ID is used to stock the destiny id and compare if is
 * valid to search 
 * @return the destiny ID typed by the user
*/
int input_destiny_ID()
{
    bool exit = false;
    int number;

    while (!exit)
    {
        printf("\nPor favor, ingrese la ID de destino:\n");
        
        if (scanf("%d", &number) != 1)
        {
            clear_stream(stdin);
            printf("La ID ingresada no es valida. \n\n");
            fflush(stdout);
        }
        else if (number > 1160 || number < 0)
        {
            printf("El tamaño del ID no es el correcto.\n");
        }
        else
        {
            printf("\n");
            exit = true;
        }
    }

    return number;
}

/**
 * The function input_daytime is used to store the time in which the trip will be
 * made and compare if is valid to search
 * @return the hour typed by the user
*/
int input_daytime()
{
    bool exit = false;
    int daytime;

    while (!exit)
    {
        printf("\nPor favor, ingrese La hora en que desea tomar el transporte, en formato 24h:\n");
        
        if ((scanf("%d", &daytime) != 1))
        {
            clear_stream(stdin);
            printf("La hora ingresada no es valida. \n\n");
            fflush(stdout);
        }
        else if (daytime < 0 || daytime > 24)
        {
            printf("El formato de la hora es incorrecto.\n");
        }
        else
        {
            printf("\n");
            exit = true;
        }
    }

    return daytime;
}

/**
 * the function output_mean_time is used to showthe average time it will take to
 * make the journey
 * @param time to complete the journey
*/
void output_mean_time(int time){
    if (time >= 0)
        printf("La hora promedio del viaje es: %d\n", time);
    else
        printf("No se pudo encontrar el tiempo de viaje\n");
}

/**
 * the funcition exitProgram is used to show the opctions for exit the program 
 * and compare the options select to user
 * @return the selection of the user
*/
bool exitProgram()
{
    char selection;

    printf("Progama finalizado, ");
    scanf("%c", &selection);
    
    while(true){
        printf("desea volver a consultar?(y/n)\n");
        scanf("%c", &selection);
        
        if (selection != 'y' && selection != 'n'){
            printf("su seleccion no es valida, vuelva a intentar\n");
            continue;
        }
        else if(selection == 'y')
            return false;
        else
            return true;
    }
}
