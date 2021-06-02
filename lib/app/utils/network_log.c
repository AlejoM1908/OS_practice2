#include "stdio.h"
#include <string.h>
#include <time.h>

void writeLog(FILE * fp, char * data, char * IP_addr)
{
    struct tm * time_struct;
    char final_data[100] = "";
    char time_char[25] = "";
    time_t current_time;

    //Extracting current time
    time(&current_time);
    time_struct = localtime(&current_time);

    //Formating the date and time:
    strftime(time_char, 25, "[%Y-%m-%d %T]", time_struct);
    
    //File exists?
    if (fp == NULL)
    {
        printf("\n Error leyendo el archivo.\n");
    }

    //Cat all the information
    strcat(final_data, time_char);
    strcat(final_data, "[");
    strcat(final_data,IP_addr);
    strcat(final_data, "][");
    strcat(final_data,data);
    strcat(final_data, "]");

    //Printing the final string
    printf("\nConsulta guardada en log: %s\n",final_data);

    //Writing to file
    fprintf(fp, "%s", final_data);
    fputs("\n",fp);
}