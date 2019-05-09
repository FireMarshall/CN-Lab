#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/stat.h>


#define bool int
#define true 1
#define false 0

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

int main(int argc, char const *argv[]) {

    int client_socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if( client_socket_desc == -1){
        perror("Cannot create socket\n");
        exit(1);
    }

    int port = atoi(argv[1]);

    sockaddr_in server_address;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = port;
    server_address.sin_family = AF_INET;

    int connection_status = connect(client_socket_desc, (sockaddr *)&server_address, sizeof server_address);
    if(connection_status == -1){
        perror("Cannot connect to server\n");
        exit(1);
    }

    printf("1.get\n2.put\n3.quit\nEnter your choice : ");
    int choice;
    scanf("%d", &choice);
    char file_name[256];
    char cmd[256];
    switch (choice){
    case 1:
        printf("Enter the file name: ");
        scanf("%s", file_name);

        strcpy(cmd,"get ");
        strcat(cmd, file_name);
        // printf("cmd: %s\n", cmd);
        send(client_socket_desc, cmd, sizeof cmd, 0);

        int file_size;
        char buffer[65536];
        recv(client_socket_desc, &file_size, sizeof file_size, 0);
        printf("File size : %d", file_size);

        recv(client_socket_desc, buffer,sizeof buffer, 0);
        char final_file_name[256] = "Copy_of_";
        strcat(final_file_name, file_name);
        int file_handle = open(final_file_name, O_CREAT | O_EXCL | O_WRONLY| O_RDONLY, 0666);
        if(file_handle == -1){
            printf("Error opening file.");
            exit(1);
        }

        write(file_handle, buffer, file_size);
        break;
    }


    close(client_socket_desc);
    return 0;
}
