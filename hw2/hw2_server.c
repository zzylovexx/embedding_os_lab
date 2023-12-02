#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
// #define PORT 5001
#define MENU_ITEMS 2

// Define MenuItem before Shop
struct MenuItem {
    char name[50];
    int quantity;
    int price;
};

// Define Shop using MenuItem
struct Shop {
    char name[50];
    int distance;
    struct MenuItem menu[10];
};

// Function declaration
void sendback_message_to_client(int connfd, char* massage){
    char sendback_msg[256];
    memset(sendback_msg, 0, 256);
    strcpy(sendback_msg, massage);   //he strcpy() function copies the string pointed by source (including the null character) to the destination.
    printf("server send to client:\n%s\n", sendback_msg);  //for debug

    int n;
    if ((n = send(connfd, sendback_msg, 256, 0)) == -1){ //or strlen(sendback_msg)+1  (need to add '\0')
        exit(EXIT_FAILURE);
    }
}
void sendShopList(int clientSocket, struct Shop *shops, int shopCount);
int handle_order(int clientSocket, char *order, char *currentOrder, char *currentRestaurant, int shopCount, struct Shop *shops);
int check_order(int clientSocket,struct Shop* shop,  char * currentRestaurant);
int main(int argc, char* argv[]) {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(struct sockaddr_in);

    // Example shop and menu data
    
    // printf("cookie price:%lf\n",shops[0].menu[0].price);
    // Create server socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    int PORT = atoi(argv[1]);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specified port
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Binding failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Start listening for client connections
    if (listen(serverSocket, 1) == -1) {  // Allow only one connection
        perror("Listening failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    printf("Server is waiting for client connection...\n");
    while (1)
    {
        struct Shop shops[] = {
        {"Dessert shop", 3, {{"cookie", 0, 60}, {"cake", 0, 80}}},
        {"Beverage shop", 5, {{"tea", 0, 40}, {"boba", 0, 70}}},
        {"Diner", 8, {{"fried-rice", 0, 120}, {"Egg-drop-soup", 0, 50}}}
        };
        int shopCount = sizeof(shops) / sizeof(shops[0]);
        if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrSize)) == -1) {
            perror("Acceptance failed");
            close(serverSocket);
            exit(EXIT_FAILURE);
        }
        printf("Client connected successfully.\n");
        char currentOrder[256] = "";  // Current order for the client
       
        char currentRestaurant[50] = "";  // Current selected restaurant
        memset(currentOrder, 0, sizeof(currentOrder));
        memset(currentRestaurant, 0, sizeof(currentRestaurant));
    
    
    // Accept client connection
    

        int break_status = 0;
        
    
    while (1)
    {
        char clientData[256];
        memset(clientData, 0 ,sizeof(clientData));
        ssize_t bytesRead;
        bytesRead = recv(clientSocket, clientData, sizeof(clientData) , 0);
        // if (bytesRead <= 0) {
        // // Connection closed or error occurred
        //     printf("Client disconnected or an error occurred. Exiting loop.\n");
        //     break;
        // }
        // clientData[bytesRead] = '\0';
        printf("Received from client: %s\n", clientData);
        break_status=handle_order(clientSocket, clientData, currentOrder, currentRestaurant ,shopCount, shops);
        if (break_status){
            close(clientSocket);
            break;
        }
        // send(clientSocket, "Message received by the server.\n", strlen("Message received by the server.\n"), 0);
    }
    }
    
    // Close the connection
    // close(clientSocket);
    

    return 0;
}
int  handle_order(int clientSocket, char *order, char *currentOrder, char *currentRestaurant, int shopCount,struct Shop *shops) {
    char response[256];
    memset(response, 0 ,sizeof(response));
    // Parse the order
    char item[50];
    int quantity;
    char *restaurant;
    if (sscanf(order, "order %s %d", item, &quantity) == 2){
    
    // sscanf(order, "order %s", restaurant);
    // printf("%s %d\n",item, quantity);
    for (int i = 0; i < shopCount; ++i) {
        for (int j = 0; j < MENU_ITEMS ; ++j) {
            if ( strcmp(shops[i].menu[j].name, item) ==0){
                shops[i].menu[j].quantity += quantity;
                restaurant = shops[i].name ;
                if (strcmp(currentRestaurant, "") == 0 || strcmp(currentRestaurant, restaurant) == 0) {  
                    // Check if the order is from a different restaurant
                    // Update the current restaurant
                    strcpy(currentRestaurant, restaurant);

                    // Update the current order
                    //no item print the first one 
                    //one item print the  
                    //from the fir to print
                    if (strcmp(currentOrder, "") == 0) {
                       
                        sprintf(response, "%s %d\n", item, shops[i].menu[j].quantity);
                        
                    }
                    else if (shops[i].menu[0].quantity!=0 && shops[i].menu[1].quantity!=0){
                        
                        // strcat(currentOrder, "|");
                        sprintf(response, "%s %d|%s %d\n", shops[i].menu[0].name, shops[i].menu[0].quantity, shops[i].menu[1].name, shops[i].menu[1].quantity);
                        // sprintf(response, "%s %d", item, shops[i].menu[0].quantity);
                        // strcat(currentOrder, response);
                    }
                    else{
                        sprintf(response, "%s %d\n", item, shops[i].menu[j].quantity);
                    }

                    
                    strcpy(currentOrder, response);
                    printf("%s\n",currentOrder);
                    // printf("%s\n",currentOrder);
                    // Send the updated order back to the client
                    send(clientSocket, currentOrder, 256, 0);
                } else {
                    // Order from a different restaurant
                    // sprintf(response, "%s %d|%s %d", shops[i].menu[0].name, shops[i].menu[0].quantity, shops[i].menu[1].name, shops[i].menu[1].quantity);
                    printf("%s\n",currentOrder);
                    send(clientSocket, currentOrder, 256, 0);
                    
                }
            }
            
        }
    }
    }
    else if (strcmp(order, "confirm") == 0){
        int confirm_break_order=0 ;
        confirm_break_order = check_order(clientSocket , shops, currentRestaurant);
        if (confirm_break_order){
            return 1;
        }
            
       
    }
    else if (strcmp(order, "shop list")==0)
    {
        // sendback_message_to_client(clientSocket, "Dessert shop:3km\n- cookie:$60|cake:$80\nBeverage shop:5km\n- tea:$40|boba:$70\nDiner:8km\n- fried-rice:$120|Egg-drop-soup:$50\n");
        sendShopList(clientSocket, shops, shopCount);
    }
    else if (strcmp (order , "cancel")==0){
        return 1;
    }
    
    
    
    return 0;
    
    // printf("restaurant name:%s\n", restaurant);
    // Parse the restaurant from the order
    

   
    
}
// Send shop list to the client
int check_order(int clientSocket,struct Shop* shop,  char * currentRestaurant){
    char distance_buffer[256];
    char price_buffer[256];
    memset(distance_buffer, 0, sizeof(distance_buffer));  // Initialize the buffer
    memset(price_buffer, 0, sizeof(price_buffer));      
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < MENU_ITEMS ; ++j) {
            if ( strcmp(shop[i].name, currentRestaurant)==0){
                int distance = shop[i].distance;
                int total_price =0;
                total_price = shop[i].menu[0].price*shop[i].menu[0].quantity + shop[i].menu[1].price*shop[i].menu[1].quantity;
                sprintf(distance_buffer, "Please wait a few minutes...\n");
                sleep(distance);
                sprintf(price_buffer,"Delivery has arrived and you need to pay %d$\n",total_price);
                // sprintf(price_buffer,"Delivery has arrived and you need to pay 140$\n");

                send(clientSocket, distance_buffer, 256, 0);
                send(clientSocket, price_buffer, 256, 0);
                printf("%s",distance_buffer);
                printf("%s",price_buffer);
                return 1; //stop 
                

                // printf("%s",distance_buffer);
                // printf("%s",price_buffer);
                
            }
        }
    }
    send(clientSocket, "Please order some meals\n",256,0);
    return 0; //continue
    // send(clientSocket, "Egg-drop-soup 100\n",256,0);
}
void sendShopList(int clientSocket, struct Shop *shops, int shopCount) {
    char shopbuffer[256];

    // memset(buffer, 0, sizeof(buffer));
    memset(shopbuffer, 0, sizeof(shopbuffer));// sprintf(buffer + strlen(buffer), "- $%d",shops[0].menu[0].price);
    for (int i = 0; i < shopCount; ++i) {
        sprintf(shopbuffer + strlen(shopbuffer), "%s:%dkm\n", shops[i].name, shops[i].distance);
        
            sprintf(shopbuffer + strlen(shopbuffer), "- %s:$%d", shops[i].menu[0].name, shops[i].menu[0].price);
            sprintf(shopbuffer + strlen(shopbuffer), "|%s:$%d\n", shops[i].menu[1].name, shops[i].menu[1].price);
        }

    // Send the shop list
    send(clientSocket, shopbuffer, 256, 0);
    
}