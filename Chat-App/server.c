#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>


#define true 1
#define false 0

int main(int argc, char const *argv[]){

    int server_socket_desc, client_socket_desc, port;
    int sizeof_client, i_dont_know_why = 1;

    struct sockaddr_in server, client;
    char buffer[100010];
    port = atoi(argv[1]);

    server_socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket_desc == -1){
        perror("Cannot create socket\n");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_port = port;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    int set_socket_opt_status = setsockopt(server_socket_desc, SOL_SOCKET, SO_REUSEADDR, &i_dont_know_why, sizeof(int));

    if(set_socket_opt_status == -1){
        perror("Cannot set socket options\n");
        exit(1);
    }

    int bind_status = bind(server_socket_desc, (struct sockaddr *)&server, sizeof server);

    if(bind_status == -1){
        perror("Cannot bind name to socket\n");
        exit(1);
    }

    listen(server_socket_desc, 10);

    while(true){
        sizeof_client = sizeof client;

        client_socket_desc = accept(server_socket_desc, (struct sockaddr *)&client, &sizeof_client);
        if(client_socket_desc == -1){
            perror("Cannot accept client\n");
            exit(1);
        }

        while (true)
        {
            int nbytes_read = read(client_socket_desc, buffer, sizeof buffer);
            printf("Message recieved from client : %s\n", buffer);
            if(!strcmp(buffer, "q")) break;
            printf("Enter your reply : ");
            scanf("%s", buffer);

            write(client_socket_desc, buffer, nbytes_read);
        }
        close(client_socket_desc);
    }

    close(server_socket_desc);
    return 0;
}
