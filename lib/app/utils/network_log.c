#include <stdio.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>

void writeLog(char * data, char * IP_addr, sem_t *semaforo){
    sem_wait(semaforo);
    FILE * file = fopen("lib/data/serverQuery.log","a");
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
    if (file == NULL)
    {
        perror("error abrendo el archivo de log");
        exit(1);
    }

    //Cat all the information
    strcat(final_data, time_char);
    strcat(final_data, "[");
    strcat(final_data,IP_addr);
    strcat(final_data, "][");
    strcat(final_data,data);
    strcat(final_data, "]\n");

    //Printing the final string
    printf("\nConsulta guardada en log: %s\n",final_data);

    //Writing to file
    fprintf(file, "%s", final_data);
    fclose(file);
    sem_post(semaforo);
}
