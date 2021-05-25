#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 8080

// Program constants
int SOCKET_DOMAIN = AF_INET, SOCKET_TYPE = SOCK_STREAM , SOCKET_PROTOCOL = 0;

/**
 * the function serverConnection is used to generate a new active listening port for server
 * @return the id of the server socket to read queries
*/
int serverConnection(){
    struct sockaddr_in address;

    // Creating socket file descriptor
    int serverfd = socket(SOCKET_DOMAIN, SOCKET_TYPE, SOCKET_PROTOCOL);
    if (serverfd < 0) sendError("error en creación socket - server");

    address.sin_family = SOCKET_DOMAIN;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Binding socket
    if (bind(serverfd, (struct sockaddr*) &address, sizeof(address)) < 0)
        sendError("error en bind");

    // Activating listening mode in socket
    if (listen(serverfd, 32) < 0) sendError("error en listen");

    // Start reading socket request queue
    int newSocket = accept(serverfd, (struct sockaddr*) &address, (socklen_t*) sizeof(address));
    if (newSocket < 0) sendError("errro en accept");

    return newSocket;
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
    if (inet_pton(SOCKET_DOMAIN, "127.0.0.1", &serverAddress.sin_addr) <= 0)
        sendError("Ip ingresada invalida o no soportada");

    // Connecting to server
    if (connect(clientfd, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0)
        sendError("error en la conexión con el servidor");

    return clientfd;
}

/**
 * the function sendError is used to interrupt the program when something goes wrong
 * @param message to be printed in the error explanation
*/
void sendError(char message[]){
    perror(message);
    exit(EXIT_FAILURE);
}
