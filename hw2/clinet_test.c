#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5001

// 函数声明
void receiveShopList(int serverSocket);
void recieve_response(int serverSocket);
int main() {
    int serverSocket;
    struct sockaddr_in serverAddr;

    // 创建客户端套接字
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 尝试连接到服务器
    if (connect(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connection failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server.\n");

    // 接收商店清单
    
    char temp_buf[256];
    while (1)
    {
        printf("Enter a message (type 'exit' to quit): ");
        if (fgets(temp_buf, sizeof(temp_buf), stdin) == NULL) {
            perror("Error reading input");
            break;
        }
        temp_buf[strcspn(temp_buf, "\n")] = '\0'; //let final is \0
        // if (strcmp(temp_buf , "shop list")==0){
        //     receiveShopList(serverSocket);
        // }
        // printf("temp_buf size:%ld\n",sizeof(temp_buf));
        if (strcmp(temp_buf, "exit") == 0) {
            printf("Exiting...\n");
            close(serverSocket);
            break;
        }
        
        send(serverSocket, temp_buf, strlen(temp_buf), 0);
        recieve_response(serverSocket);
    }

    
    // 关闭连接
    close(serverSocket);

    return 0;
}
void recieve_response(int serverSocket){
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    // Receive data from the server
    ssize_t bytesRead = recv(serverSocket, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead <= 0) {
        perror("Error receiving response");
    } else {
        // Null-terminate the received data
        buffer[bytesRead] = '\0';

        // Display the server's response
        printf("Server's response: %s\n", buffer);
    }
}
// 接收商店清单
void receiveShopList(int serverSocket) {
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    // 接收数据
    recv(serverSocket, buffer, sizeof(buffer), 0);

    // 显示商店清单
    printf("Shop List:\n%s", buffer);
}