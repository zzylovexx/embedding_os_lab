#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //for open function
#include <unistd.h> //for sleep 
#include <string.h>
// 定義餐廳和菜單
char** createMatrix(int a) ;
void freeMatrix(char** matrix, int rows) ;
void printMatrix(char** matrix, int rows) ;
typedef struct {
  char* name; 
  int distance;
  char* item1;
  int price1;
  char* item2;  
  int price2;
} Restaurant;

char *num_array[10] = {
        "1111110", // 0
        "0110000", // 1
        "1101101", // 2
        "1111001", // 3
        "0110011", // 4
        "1011011", // 5
        "0011111", // 6
        "1110000", // 7
        "1111111", // 8
        "1110011"  // 9
    };
Restaurant restaurants[] = {
  {"Dessert shop", 3, "cookie", 60, "cake", 80},
  {"Beverage shop", 5, "tea", 40, "boba", 70},  
  {"Diner", 8, "fried rice", 120, "egg-drop soup", 50}
};

void initial(){
    int file_desc;
    char end[15] = "000000000000000";
    file_desc = open("/dev/etx_device", O_RDWR);
    if (file_desc < 0) {
        perror("cannot open file");
        return ;
    }
    write(file_desc, end, 15);
}

void light_led(int total_distance,int price){
    char cat [20];
    int digit = 0;
    char str[10]; // 假設數字不超過10位
    sprintf(str, "%d", price);
    char **matrix = createMatrix(total_distance);
    int file_desc;
    file_desc = open("/dev/etx_device", O_RDWR);
    if (file_desc < 0) {
        perror("cannot open file");
        return ;
    }
    for (int i =0 ; i< total_distance*2+1 ; i++){
        strcpy(cat, matrix[i]);
        if (i < strlen(str)) {
            digit = str[i] - '0';
            strcat(cat, num_array[digit]);
        }
        else{
            strcat(cat, num_array[digit]);
        }
        write(file_desc, cat, 15);
        printf("cat: %s\n",cat);
        usleep(500000); // Sleep for 0.5 seconds (500,000 microseconds)

    } 
    freeMatrix(matrix,total_distance);
    
}
// 顯示餐廳列表
void listRestaurants() {
  printf("餐廳列表:\n");
  for(int i=0; i<3; i++) {
    printf("%d: %s (距離 %dkm)\n",i, restaurants[i].name, restaurants[i].distance); 
  }
}

// 點餐功能
void orderFood() {

    int restaurantIndex;
    printf("請選擇餐廳:\n");
    listRestaurants();
    scanf("%d", &restaurantIndex);

    if(restaurantIndex < 0 || restaurantIndex >= 3) {
        printf("錯誤的選擇\n");
        return;
    }

    Restaurant* r = &restaurants[restaurantIndex];
    
    printf("%s 菜單:\n", r->name);
    printf("1. %s ($%d)\n", r->item1, r->price1);
    printf("2. %s ($%d)\n", r->item2, r->price2);  

    int itemIndex, quantity;
    int total=0;
//   printf("請選擇餐點:");
//   scanf("%d", &itemIndex);

    
    printf("請輸入 %s 份量:", r->item1);
    scanf("%d", &quantity);
    total += quantity *r->price1; 
    printf("您已點 %dx %s\n", quantity, r->item1);
    printf("總金額: $%d\n", total); 
    printf("請輸入 %s 份量:", r->item2);
    scanf("%d", &quantity);
    total += quantity *r->price2; 
    printf("您已點 %dx %s\n", quantity, r->item2);
    printf("總金額: $%d\n", total); 
    int confirm;
    printf("1. 確認送出\n");
    printf("2. 取消訂單\n");
    scanf("%d", &confirm);
    if (confirm == 1) {
        printf("謝謝確認,我們將盡快為您送餐\n");
        light_led(r->distance, total);
    } else if (confirm == 2) {
        printf("已取消訂單\n");
        return; 
    }

//   if(itemIndex == 1) {
//     printf("請輸入 %s 份量:", r->item1);
//     scanf("%d", &quantity);
//     printf("您已點 %dx %s\n", quantity, r->item1);
//   } else if(itemIndex == 2) {
//     printf("請輸入 %s 份量:", r->item2);
//     scanf("%d", &quantity);
//     printf("您已點 %dx %s\n", quantity, r->item2);
//   } else {
//     printf("錯誤的選擇\n");
//     return;
//   }

 
//   if(itemIndex == 1) {
//     total = quantity * r->price1; 
//   } else if(itemIndex == 2) {
//     total = quantity * r->price2;
//   }

  // 顯示總金額
  

}

int main() {
  
  int choice;

  do {
    printf("=== 外送系統 ===\n");
    printf("1. 顯示餐廳\n");
    printf("2. 點餐\n");
    printf("0. 結束\n");

    printf("選擇:");
    scanf("%d", &choice);

    switch(choice) {
        case 0:
            initial();
            break;
        case 1:
            listRestaurants();
        case 2:
            orderFood();
        
    }

  } while(choice != 0);

  return 0;
}
char** createMatrix(int a) {
    char** matrix = (char**)malloc((a * 2 + 1) * sizeof(char*));

    // 分配每一行的空間
    for (int i = 0; i <= a * 2; i++) {
        matrix[i] = (char*)malloc(8 * sizeof(char));
    }

    // 初始化矩陣
    for (int i = 0; i < a * 2; i++) {
        for (int j = 0; j < 8; j++) {
            matrix[i][j] = (j >= a - i / 2) ? '0' : '1';
        }
    }

    // 最後一列全部為 0
    for (int j = 0; j < 8; j++) {
        matrix[a * 2][j] = '0';
    }

    return matrix;
}

// 函數：釋放矩陣的動態分配空間
void freeMatrix(char** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
void printMatrix(char** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%c", matrix[i][j]);
        }
        printf("\n");
    }
}