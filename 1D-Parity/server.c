#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define true 1
#define bool int

bool get_parity(int x){
    bool parity = 0;
    while(x){
        parity += (x&1)?1:0;
        x >>= 1;
    }
    return parity%2;
}

int main (int argc, char const *argv[]) {

    int server_socket, client_socket, port;
    int i_dont_know_why = 1;
    struct sockaddr_in server, client;
    port = atoi(argv[1]);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1){
        perror("Cannot create server socket\n");
        exit(1);
    }

    int set_socket_opt_status = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &i_dont_know_why, sizeof(int));

    if(set_socket_opt_status == -1){
        perror("Cannot set socket options\n");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_port = port;
    server.sin_addr.s_addr = INADDR_ANY;

    int bind_status = bind(server_socket, (struct sockaddr *)&server , sizeof server);

    if(bind_status == -1){
        perror("Cannot bind\n");
        exit(1);
    }

    listen(server_socket, 100);

    while(true){
        int sizeof_client = sizeof client;

        client_socket = accept(server_socket, (struct sockaddr*)&client, &sizeof_client);

        if(client_socket == -1){
            perror("Cannot accept\n");
            exit(1);
        }
        
        char buffer[128];

        int x = 12;

        bool parity = get_parity(x);
        char message[128];
        x = 15;
        x = (x * 10 + parity);
        sprintf(message, "%d", x);

        write(client_socket, message, sizeof message);

        close(client_socket);
    }

    close(server_socket);
    return 0;
}
