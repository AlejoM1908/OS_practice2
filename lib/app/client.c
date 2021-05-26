#include <sys/socket.h>
#include <string.h>
#include "utils/network_connection.c"
#include "utils/console_outputs.c"

void startClient(){
    bool exitVar = false;
    char data[20], buffer[10];
    char source, destiny, time;

    // Start server connection
    int socket = clientConnection();

    display_welcome();
    while (!exitVar){
        // Ask for data to search
        source = (char) input_source_ID();
        destiny = (char) input_destiny_ID();
        time = (char) input_daytime();

        strcat(data, &source);
        strcat(data, "-");
        strcat(data, &destiny);
        strcat(data, "-");
        strcat(data, &time);

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