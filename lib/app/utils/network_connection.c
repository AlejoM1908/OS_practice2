#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "search_data.c"
#define PORT 3535
#define BACKLOG 8

// Program constants
int SOCKET_DOMAIN = AF_INET, SOCKET_TYPE = SOCK_STREAM , SOCKET_PROTOCOL = 0;
bool flags [32] = {false};
HashTable *table;

/**
 * the function sendError is used to interrupt the program when something goes wrong
 * @param message to be printed in the error explanation
*/
void sendError(const char message[]){
    perror(message);
    exit(EXIT_FAILURE);
}

void check(int exp, const char message[]){
    if (exp < 0) sendError(message);
}

void * handleConnection(void * data){
    int clientfd;
    char buffer[20];

    clientfd = *((int *) data);

    printf("hilo iniciado con el socket %d\n", clientfd);

    processQuery(table, clientfd);
}

void acceptClients(int serverfd){
    struct sockaddr_in clientAddress;
    int clientfd, client = 0;
    pthread_t pid;
    socklen_t clientSize = sizeof(clientAddress);

    // Recive all client connections
    printf("Esperando conexiones ...\n");
    while (true){
        // Check if thread ended client communication
        pthread_join(pid, NULL);

        // Accept new client connection
        clientfd = accept(serverfd, (struct sockaddr *) &clientAddress, &clientSize);
        check (clientfd, "error en conexión con cliente");

        printf("se ha aceptado una conexión correctamente\n");

        // create new thread for attend new client
        check(pid = pthread_create(&(pid), NULL, (void *) handleConnection, (void *) &clientfd), 
            "ha ocurrido un error al crear el hilo para la conexión");
        printf("se ha creado correctamente un hilo para el cliente \n");

        if (client == 31) client = 0;
        else client++;
    }
}

/**
 * the function serverConnection is used to generate a new active listening port for server
 * @return the id of the server socket to read queries
*/
void serverConnection(){
    struct sockaddr_in address;
    int opt = 1;

    // Loading hashtable for searching process
    table = initHash();
    loadHash(table);

    // Creating socket file descriptor
    int serverfd = socket(SOCKET_DOMAIN, SOCKET_TYPE, SOCKET_PROTOCOL);
    if (serverfd < 0) sendError("error en creación socket - server");

    address.sin_family = SOCKET_DOMAIN;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bzero(address.sin_zero,8);

    // Setting socket options
    check(setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)),
        "error en opciones del socket");
    
    // Binding socket
    check(bind(serverfd, (struct sockaddr*) &address, sizeof(struct sockaddr)),
        "error en bind");

    // Activating listening mode in socket
    check(listen(serverfd, BACKLOG), "error en listen");

    // accept all 32 clients
    acceptClients(serverfd);
}

/**
 * the function clientConnection is used to generate a new connection to a server by a client
 * @return the id of the client socket to send queries
*/
int clientConnection(){
    struct sockaddr_in serverAddress;

    // Creating socket file descriptor
    int clientfd = socket(SOCKET_DOMAIN, SOCKET_TYPE, SOCKET_PROTOCOL);
    if (clientfd < 0) sendError("error en creación socket - client");

    serverAddress.sin_family = SOCKET_DOMAIN;
    serverAddress.sin_port = htons(PORT);

    // Converting server ip to binary
    if (inet_aton("127.0.0.1", &serverAddress.sin_addr) <= 0)
        sendError("Ip ingresada invalida o no soportada");

    // Connecting to server
    if (connect(clientfd, (struct sockaddr*) &serverAddress, (socklen_t) sizeof(struct sockaddr)) < 0)
        sendError("error en la conexión con el servidor");

    return clientfd;
}
 