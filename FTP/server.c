#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sendfile.h>


#define bool int
#define true 1
#define false 0

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;


int main(int argc, char const *argv[]){

    int server_socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket_desc == -1){
        perror("Cannot create socket\n");
        exit(1);
    }

    int port = atoi(argv[1]);

    int client_socket_desc;

    sockaddr_in server_address, client_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = port;
    server_address.sin_addr.s_addr = INADDR_ANY;

    int i_dont_know_why = 1;
    int set_socket_opt_status = setsockopt(server_socket_desc, SOL_SOCKET, SO_REUSEADDR, &i_dont_know_why, sizeof(int));
    if(set_socket_opt_status == -1){
        perror("Cannot set options\n");
        exit(1);
    }

    int bind_status = bind(server_socket_desc, (sockaddr *)&server_address, sizeof server_address);
    if(bind_status == -1){
        perror("Cannot bind\n");
        exit(1);
    }

    listen(server_socket_desc, 10);

    while (true){

        int sizeof_client = sizeof(client_address);

        if(( client_socket_desc = accept(server_socket_desc, (sockaddr *)&client_address, &sizeof_client)) == -1){
            perror("Cannot accept connection\n");
        }

        char cmd[256], tmp[256];
        recv(client_socket_desc, tmp, sizeof tmp, 0);
        // printf("cmd: %s\n", cmd);

        char file_name[265];
        sscanf(tmp, "%s %s", cmd, file_name);

        struct stat file_stat;
        stat(file_name, &file_stat);
        int file_size = file_stat.st_size;

        int file_handle = open(file_name, O_RDONLY);
        if(file_handle == -1){
            file_size = 0;
        }
        send(client_socket_desc, &file_size, sizeof file_size, 0);
        unsigned char buffer[65536];
        // int nbytes_read = read(file_handle, buffer, sizeof buffer);
        sendfile(client_socket_desc, file_handle, NULL, 0);
        close(client_socket_desc);
    }

    close(server_socket_desc);
    return 0;
}
