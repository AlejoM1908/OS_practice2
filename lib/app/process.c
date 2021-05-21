#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "Console_outputs.c"
#include "../Structures/hash_table.c"

/**
 * The function checkErrors is used to verify if the fork and pipes where created
 * correctlly
 * @param processId is the id of the fork
 * @param pipesPointers is the array of both pipes
*/
void checkErrors(int processId, int pipesPointers[2]){
    for (int i=0; i<2; i++){
        if (pipesPointers[i] < 0){
            // Generating the corresponding error message for pipe error
            char message[] = "Error en la generación de la pipe ";
            strcat(message, (char*) &i);
            printf(message);
            exit(1);
        }
    }

    // Generating the corresponding error message for the fork
    if (processId < 0){
        printf("Error en la generación del fork");
        exit(2);
    }
}

/**
 * The function loadHash is used to load the needed information into the 
 * hash table used in the search
 * @param hash where the data is going to be loaded
*/
void loadHash(HashTable* hash){
    // Pointer to the hash data file
    FILE* file = fopen("lib/data/hashData.txt", "r");

    // Creating other needed variables
    int key;
    long int data;
    char buffer[20];
    char* token;

    // Cheking if the file exists
    if (file == NULL){
        printf("No se pudo encontrar el archivo del hash para fopen");
        exit(3);
    }

    // loading the whole document
    for (int i = 0; i<4; i++){
        while(fgets(buffer, 20, file) != NULL){
            token = strtok(buffer,",");
            key = atoi(token);
            token = strtok(NULL,",");
            data = (long int) atoi(token);

            if (!exist(hash,key)){
                insert(hash, key, data);
            }
        }

        fseek(file, 0, SEEK_SET);
    }
}

/**
 * The function searchData is used to traverse the file and get the needed information
 * @param table is the Hash Table to make process faster
 * @param file is the pointer to the data file
 * @param source is the id of the sorce zone of the travel
 * @param dest is the id of the destination zone of the travel
 * @param hour is the hour of the day where the travel start
 * @return the mean time for the given data
*/
int searchData(HashTable* table, FILE* file, int source, int dest, int hour){
    // Creating the needed variables
    int check, src, dst, time, mean_time;
    char buffer[30];
    char* token;
    
    // Extracting the file pointer from the hash table
    long int pointer = get(table, source);
    if (pointer < 0) return -1;

    // Moving the pointer in the file where needed
    check = fseek(file, pointer, SEEK_SET);

    if (check < 0){
        printf("No se pudo desplazar el puntero con fseek");
        exit(6);
    }

    // Traversing the data until source id change or mean_time was finded
    while (fgets(buffer, 30, file) != NULL){
        token = strtok(buffer,",");
        src = atoi(token);
        token = strtok(NULL,",");
        dst = atoi(token);
        token = strtok(NULL,",");
        time = atoi(token);
        token = strtok(NULL,",");
        mean_time = atoi(token);

        if (src != source) return -1;
        else if (dst == dest && time == hour) return mean_time;
    }
}

/**
 * The function searchInFile is used to search the file pointer in the HashTable and
 * find the mean travel time in the file
 * @param table is the Hash Table to make process faster
 * @param source is the id of the sorce zone of the travel
 * @param dest is the id of the destination zone of the travel
 * @param hour is the hour of the day where the travel start
 * @return the mean time for the given data
*/
int searchInFile(HashTable* table, int source, int dest, int hour){
    // Pointer to the data file
    FILE* file = fopen("lib/data/data.txt", "r");

    // Creating other needed variables
    int check, src, dst, time, mean_time;
    char buffer[30];
    char* token;

    // Checking if data file exists
    if (file == NULL){
        printf("No se pudo encontrar el archivo para fopen");
        exit(4);
    }

    // Searching the data in the file
    return searchData(table,file,source,dest,hour);
}

/**
 * The function childrenFunction is used to generate a new process that will search
 * for the average travel time into the document
 * @param pipeWrite where the process will send info to the other process
 * @param pipeRead where the process will read info from the other process
*/
void childrenFunction(int pipeWrite, int pipeRead){
    // Allocating the needed memory
    HashTable* table = initHash();
    int source, destiny, time, check, data;
    bool exitVar = false;

    // Loading the hash data into new Hash Table
    loadHash(table);

    // Generaing a loop for keep the program runing until the user stops it
    while(!exitVar){
        // Receiving the data from the user to search
        read(pipeRead, &source, sizeof(int));
        read(pipeRead, &destiny, sizeof(int));
        read(pipeRead, &time, sizeof(int));

        // Searching for the data in the file
        data = searchInFile(table, source, destiny, time);

        // Sending mean time back to user
        write(pipeWrite, &data, sizeof(data));
        // Reading if the user stops the program
        read (pipeRead, &check, sizeof(int));

        if (check == -1){
            freeHash(table);
            exit(0);
        }
    }
}

/**
 * The function parentFunction is used to call the console application and send the
 * required data to the children process
 * @param pipeWrite where the process will send info to the other process
 * @param pipeRead where the process will read info from the other process
*/
void parentFunction(int pipeWrite, int pipeRead){
    // Allocating the needed memory
    bool exitVar = false;
    int data;
    
    display_welcome();

    // Generaing a loop for keep the program runing until the user stops it
    while (!exitVar){
        // Getting data from user and sending it to searching process
        data = input_source_ID();
        write(pipeWrite, &data, sizeof(data));
        data = input_destiny_ID();
        write(pipeWrite, &data, sizeof(data));
        data = input_daytime();
        write(pipeWrite, &data, sizeof(data));

        // Waiting for responce from the child process
        system("clear");
        printf("procesando, espere un momento ...\n");
        int time = -1;
        read(pipeRead, &time, sizeof(int));

        // Printing the mean time founded or the notFound error
        system("clear");
        output_mean_time(time);

        // Asking if the user want to find another value
        exitVar = exitProgram();
        
        if (exitVar){
            data = -1;
            write(pipeWrite, &data, sizeof(data));
        }
        else{
            data = 0;
            write(pipeWrite, &data, sizeof(data));
        }
    }

    exit(0);
}

/**
 * The function startProram is used to run the whole program, witch compose all
 * the code blocks, the processes and the pipes
*/
void startProgram(){
    // Creating all the memory for the forks and pipes
    int pipesPointers[2], parentPipes[2], childrenPipes[2];
    pid_t processId = 0;
    
    // Creating pipes and forks
    pipesPointers[0] = pipe(parentPipes);
    pipesPointers[1] = pipe(childrenPipes);
    processId = fork();
    checkErrors(processId, pipesPointers);

    // Spliting the code that witch process will execute
    if (processId == 0){
        close(childrenPipes[0]);
        close(parentPipes[1]);
        childrenFunction(childrenPipes[1], parentPipes[0]);
    }
    else if (processId > 0){
        close(childrenPipes[1]);
        close(parentPipes[0]);
        parentFunction(parentPipes[1], childrenPipes[0]);
    }
}
