#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

typedef struct sockaddr_in  sockaddr_in;
typedef struct sockaddr sockaddr;

int main(int argc, char const *argv[]){

    int server_socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if(server_socket_desc == -1){
        perror("Cannot create socket\n");
        exit(1);
    }

    // int set_socket_option_status = setsockopt(server_socket_desc, SOL_SOCKET, SO_REUSEADDR);

    sockaddr_in server_address;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = atoi(argv[1]);
    server_address.sin_family = AF_INET;

    int bind_status = bind(server_socket_desc, (sockaddr *)&server_address, sizeof server_address);
    if(bind_status == -1){
        perror("Cannot bind\n");
        exit(1);
    }

    listen(server_socket_desc, 10);

    while(1){
        sockaddr_in client_address;
        int sizeof_client = sizeof client_address;
        int client_socket_desc = accept(server_socket_desc, (sockaddr * )&client_address, &sizeof_client);
        if(client_socket_desc == -1){
            perror("Cannot accept\n");
            exit(1);
        }

        int row, col;

        recv(client_socket_desc, &row, sizeof row, 0);
        recv(client_socket_desc, &col, sizeof col, 0);
        int a[row][col];
        recv(client_socket_desc, a, sizeof a, 0);

        int row_errors = 0, col_errors = 0;
        for(int i = 0; i < row; ++i){
            int parity = 0;
            for(int j = 0; j < col-1; ++j){
                parity += a[i][j] ? 1:0;
            }
            if(a[i][col-1] != parity%2){
                ++row_errors;
            }
        }
        for(int i = 0; i < col; ++i){
            int parity = 0;
            for(int j = 0; j < row-1; ++j){
                parity += a[j][i] ? 1:0;
            }
            if(a[row-1][i] != parity%2){
                ++col_errors;
            }
        }

        printf("Error row : %d\nError col : %d\n", row_errors, col_errors);
        printf("Recieved data :\n");
        for(int i = 0; i < row; ++i){
            for(int j = 0; j < col; ++j){
                printf("%d ",a[i][j]);
            }
            printf("\n");
        }


    }

    return 0;
}
