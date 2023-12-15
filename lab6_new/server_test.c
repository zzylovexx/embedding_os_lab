#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#define MAX_CLIENTS 5
struct Transaction {
    char operation[20];
    int amount;
};
int account_balance = 0;
sem_t semaphore;

void *handle_client(void *client_socket) {
    int client_sock = *(int *)client_socket;
    while(1){
        // 接收客戶端要求
    
    struct Transaction transaction;
    // recv(client_sock, &transaction, sizeof(struct Transaction), 0);
    ssize_t bytes_received = recv(client_sock, &transaction, sizeof(struct Transaction), 0);

    if (bytes_received <= 0) {
        // Handle disconnection or error
        // perror("Error receiving data");
        break;  // Exit the loop on error or disconnection
    }

    // printf("%s\n",transaction.operation);
    int amount = transaction.amount;
    sem_wait(&semaphore);
    if (strcmp(transaction.operation, "deposit") == 0) {
        // 存款
        // 假設每次存款 1 元
        account_balance += amount;
        printf("After deposit: %d\n", account_balance);
    } else if (strcmp(transaction.operation, "withdraw") == 0) {
        // 提款
        // sem_wait(&semaphore);
        account_balance -= amount;
        // sem_post(&semaphore)
        printf("After withdraw: %d\n", account_balance);
    }
    sem_post(&semaphore);

    
    // free(client_socket);

    }
    close(client_sock);
    
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    sem_init(&semaphore, 0, 1);  // 初始化全域信號量

    int server_sock, client_sock, port;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    pthread_t thread_id;

    // 套接字设置
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    port = atoi(argv[1]);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding socket");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, 5) == -1) {
        perror("Error listening on socket");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", port);

    while (1) {
        char buffer[10];

        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size);
        if (client_sock == -1) {
            perror("Error accepting connection");
            continue;
        }
        // recv(client_sock, &transaction, sizeof(struct Transaction), 0);
        // printf("%s\n", transaction.operation);
        // printf("%d\n",transaction.amount);
        // 為每個客戶端創建一個專用的執行緒

        // Create a client context
        int *client_sock_ptr = (int *)malloc(sizeof(int));
        *client_sock_ptr = client_sock;

        if (pthread_create(&thread_id, NULL, handle_client, (void *)client_sock_ptr) < 0) {
            perror("Error creating thread");
            close(client_sock);
            free(client_sock_ptr);
            continue;
        }
        pthread_detach(thread_id);
    }

    sem_destroy(&semaphore);  // 清除 semaphore
    close(server_sock);

    return 0;
}