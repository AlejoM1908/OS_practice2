#include <sys/socket.h>
#include <string.h>
#include "utils/network_connection.c"
#include "utils/console_outputs.c"

void startClient(){
    bool exitVar = false, serverOff = false;
    char data[13] = "", buffer[13], source[5], destiny[5], time[3];

    // Start server connection
    int socket = clientConnection();

    display_welcome();
    while (!exitVar){
        bzero(data, 13);
        bzero(buffer, 12);
        // Ask for data to search
        snprintf(source, 5, "%d", input_source_ID());
        snprintf(destiny, 5, "%d", input_destiny_ID());
        snprintf(time, 3, "%d", input_daytime());

        strcat(data, source);
        strcat(data, "-");
        strcat(data, destiny);
        strcat(data, "-");
        strcat(data, time);

        // Clear and ask for server response
        system("clear");
        printf("procesando, espere un momento ...\n");
        send(socket, data, sizeof(data), 0);
        read(socket, buffer, 13);

        // Output result
        system("clear");
        output_mean_time(atoi(buffer));
        exitVar = exitProgram();
        if (exitVar) serverOff = exitServer();

        if (serverOff) send(socket, "exit", 4, 0);
        if (exitVar) send(socket, "2exit", 5, 0);
    }
    
}