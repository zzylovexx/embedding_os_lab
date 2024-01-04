#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    // 定義主機和埠號
    char *host = "127.0.0.1";
    int port = 12345;

    // 建立socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating server socket");
        exit(EXIT_FAILURE);
    }

    // 給socket綁定主機和埠號
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET; //use ipv4 use in local normally use this setting 
    server_address.sin_addr.s_addr = inet_addr(host); //host means ip address
    server_address.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Error binding socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // 監聽連接
    if (listen(server_socket, 1) == -1) {
        perror("Error listening for connections");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server正在監聽 %s:%d...\n", host, port);

    // 等待客戶端連接
    int client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1) {
        perror("Error accepting connection");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("與客戶端連接成功!\n");

    // 接收數據
    char buffer[1024];
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

    if (bytes_received == -1) {
        perror("Error receiving data");
    } else {
        printf("接收到的數據: %.*s\n", (int)bytes_received, buffer);
    }

    // 關閉連接
    close(client_socket);
    close(server_socket);

    return 0;
}