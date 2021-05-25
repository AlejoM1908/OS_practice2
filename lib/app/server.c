#include <stdbool.h>
#include <sys/socket.h>
#include "utils/network_connection.c"
#include "../Structures/hash_table.c"

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
    if (file == NULL) sendError("No se pudo encontrar el archivo del hash para fopen");

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

int processQuery(HashTable* table, char query[]){
    int source, destiny, time;

    // Extract data
    source = atoi(strtok(query,"-"));
    destiny = atoi(strtok(NULL, "-"));
    time = atoi(strtok(NULL, "-"));

    return searchInFile(table, source, destiny, time);
}

void startServer(){
    HashTable *table = initHash();
    char buffer [20];
    int socket = serverConnection();

    // Load hashtable
    loadHash(table);

    // Start reading querys
    while (true){
        read(socket, buffer, 20);
        int mean_time = processQuery(table, buffer);

        send(socket, mean_time, sizeof(mean_time), 0);
    }
}