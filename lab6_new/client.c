#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
struct Transaction {
    char operation[20];
    int amount;
};
int main(int argc, char *argv[]) {
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <ip> <port> <operation> <amount> <times>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock, port, times;
    struct sockaddr_in server_addr;
    char *ip, *operation;
    int amount;

    ip = argv[1];
    port = atoi(argv[2]);
    operation = argv[3];
    amount = atoi(argv[4]);
    times = atoi(argv[5]);

    // 套接字设置
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);

    // 连接到服务器
    

    // if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
    //     perror("Connection failed");
    //     close(sock);
    //     exit(EXIT_FAILURE);
    // }
    

    printf("Connected to server\n");

    for (int i = 0; i < times; ++i) {
        struct Transaction transaction;
        
		strcpy(transaction.operation, operation);
		transaction.amount = amount;
        // 傳送操作要求至伺服器
    //     if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    //     perror("Socket creation failed");
    //     exit(EXIT_FAILURE);
    // }
        sock = socket(AF_INET, SOCK_STREAM, 0);
        connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    //     if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
    //     perror("Connection failed");
    //     close(sock);
    //     exit(EXIT_FAILURE);
    // }
        printf("%d\n",i);
        send(sock, &transaction, sizeof(struct Transaction), 0);
        close(sock);
        usleep(1000);
        // sleep(1);
        // 不再需要 sleep，讓 server 自己控制 semaphore
    }

    

    return 0;
}