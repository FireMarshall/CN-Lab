#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define true 1


int main(int argc, char const *argv[])
{
    int client_socket_desc, port;

    char buffer[100010];

    struct sockaddr_in server;

    port = atoi(argv[1]);

    client_socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if(client_socket_desc == -1){
        perror("Cannot create socket\n");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_port = port;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    int connect_status = connect(client_socket_desc, (struct sock_addr *)&server, sizeof server);
    if(connect_status == -1){
        perror("Cannot connect to server\n");
        exit(1);
    }
    while(true){
        printf("Enter message to be echoed: ");
        // scanf("%[^\n]", buffer);
        scanf("%s", buffer);

        write(client_socket_desc, buffer, sizeof buffer);
        if(!strcmp(buffer, "q")) break;
        int nbytes_read = read(client_socket_desc, buffer, sizeof buffer);
        printf("Reply from server: %s\n", buffer);
    }

    return 0;
}
