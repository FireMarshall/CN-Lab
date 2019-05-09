#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define bool int
#define true 1
#define false 0

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;


// output 1010000011

char data[32];
char gen[] = "1001";

void crc(){
    char tmp[20] = {0};
    const int n = strlen(gen);
    const int size = strlen(data);

    int i = 0;
    for(i = size; i < n+size-1; ++i){
        data[i] = '0';
    }
    for(i = 0; i < n; ++i){
        tmp[i] = data[i];
    }
    int j = 0;
    while(i < n+size){
        if(tmp[0] == '1'){
            for(int k = 0; k < n; ++k){
                tmp[k] = (tmp[k] == gen[k])? '0':'1';
            }
        }
        for(j = 0; j < n-1; ++j){
            tmp[j] = tmp[j+1];
        }
        tmp[j] = data[i++];
    }
    for(i = size; i < n+size-1; ++i){
        data[i] = tmp[i-size];
    }
}


int main(int argc, char const *argv[]) {

    int client_socket_desc;;
    if((client_socket_desc = socket(AF_INET, SOCK_STREAM, 0))== -1){
        perror("Cannot create socket\n");
        exit(1);
    }

    int port = atoi(argv[1]);

    sockaddr_in server_address;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = port;
    server_address.sin_family = AF_INET;

    if((connect(client_socket_desc, (sockaddr *)&server_address, sizeof server_address))== -1){
        perror("Cannot connect to server\n");
        exit(1);
    }

    // char buffer[4096];
    printf("Enter text to send: ");
    scanf("%s", data);
    crc();
    data[4] = (data[] == '1') ? '0':'1';
    printf("DATA SENT : %s\n", data);
    write(client_socket_desc, data, sizeof data);

    close(client_socket_desc);
    return 0;
}

