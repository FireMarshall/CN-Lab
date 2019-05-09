#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define bool int
#define true 1
#define false 0

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

char data[100];
char gen[] = "1001";

void crc(){
    int i, j, n = 4 ,size = strlen(data);

    char tmp[50] = {0};

    for(i = 0; i < n; ++i){
        tmp[i] = data[i];
    }
    while(i < n+size){
        if(tmp[0] == '1'){
            for(int k = 0; k < n; ++k){
                tmp[k] = (tmp[k] == gen[k]) ? '0' : '1';
            }
        }
        for(j = 0; j < n-1; ++j){
            tmp[j] = tmp[j+1];
        }
        tmp[j] = data[i++];
    }
    printf("TMP : %s\n", tmp);
}


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

        int sizeof_client = sizeof client_address;
        client_socket_desc = accept(server_socket_desc, (sockaddr *)&client_address, &sizeof_client);

        if(client_socket_desc == -1){
            perror("Cannot accept connection\n");
        }

        char buffer[4096];

        int nbytes_read = read(client_socket_desc, data, sizeof data);
        crc();
        printf("R o C : %s\n", data);
        close(client_socket_desc);
    }
    
    close(server_socket_desc);
    return 0;
}
