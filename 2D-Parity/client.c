#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr; 

int main(int argc, char const *argv[]){
    
    int client_socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket_desc == -1){
        perror("Cannot accept\n");
        exit(1);
    }
    sockaddr_in server_address;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = atoi(argv[1]);
    server_address.sin_family = AF_INET;

    int connection_status = connect(client_socket_desc, (sockaddr *) &server_address, sizeof server_address);
    if(connection_status == -1){
        perror("Cannot connect\n");
        exit(1);
    }

    int row = 5, col = 5;
    send(client_socket_desc, &row, sizeof row, 0);
    send(client_socket_desc, &col, sizeof col, 0);
    int a[5][5] = {
        {1, 0, 1, 0, 0},
        {1, 1, 0, 0, 0},
        {1, 0, 1, 1, 0},
        {1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0},
    };

    for(int i = 0; i < row; ++i){
        int parity = 0;
        for(int j = 0; j < col-1; ++j){
            parity += a[i][j] ? 1:0;
        }
        a[i][col-1] = parity%2;
    }
    for(int i = 0; i < col; ++i){
        int parity = 0;
        for(int j = 0; j < row-1; ++j){
            parity += a[j][i] ? 1:0;
        }
        a[row-1][i] = parity%2;
    }
    send(client_socket_desc, a, sizeof a, 0);
    close(client_socket_desc);

    return 0;
}
