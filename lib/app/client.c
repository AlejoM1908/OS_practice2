#include <sys/socket.h>
#include <string.h>
#include "utils/network_connection.c"
#include "utils/console_outputs.c"

void startClient(){
    bool exitVar = false;
    char data[] = "", buffer[10];

    // Start server connection
    int socket = clientConnection();

    display_welcome();
    while (!exitVar){
        // Ask for data to search
        strcat(data, input_source_ID());
        strcat(data, "-");
        strcat(data, input_destiny_ID());
        strcat(data, "-");
        strcat(data, input_daytime());

        // Clear and ask for server response
        system("clear");
        printf("procesando, espere un momento ...\n");
        send(socket, data, sizeof(data), 0);
        read(socket, buffer, 10);

        // Output result
        system("clear");
        output_mean_time(atoi(buffer));
        exitVar = exitProgram();
    }
    
}