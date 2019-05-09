#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define true 1
#define bool int


bool get_parity(int x){
    bool parity = 0;
    while (x){
        parity += (x&1)?1:0;
        x >>= 1;
    }
    return parity%2;
}

int main(int argc, char const *argv[]){    

    int client_socket, port;

    port = atoi(argv[1]);

    struct sockaddr_in server;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(client_socket == -1){
        perror("Cannot create socket\n");
        exit(1);
    }

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = port;

    int connection_status = connect(client_socket, (struct sockaddr*)&server, sizeof server);

    if(connection_status == -1){
        perror("Cannot create server socket\n");
        exit(1);
    }

    char buffer[128];
    int nbytes_read = read(client_socket, buffer, sizeof buffer);

    printf("R : %s\n",buffer);
    int x, parity;

    x = atoi(buffer);

    parity = x%10;
    x /= 10;
    if( parity == get_parity(x)){
        printf("No error occurred\n");
    } else {
        printf("Error occurred\n");
    }
    close(client_socket);
    return 0;
}
