#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
void intToBinary4Bits(int file_desc, int num) {
    int binary[4];
    //let number to 4 binary code
    char char_array[4];
    for (int i = 3; i >= 0; i--) {
        binary[i] = num % 2;
        num = num / 2;
    }
    for (int i = 0; i < 4; i++) {
        sprintf(&char_array[i], "%d", binary[i]);
    }
    printf("%s",char_array);
    write(file_desc, char_array, 4);
    sleep(1);
}
int main(int argc, char *argv []) {
    int file_desc;
    
    char option;
    char gpio_state;
    char *student_id = argv[1];
    char *test_num = "0000";
    int number = atoi(student_id);
    file_desc = open("/dev/etx_device", O_RDWR);
    printf("%d",file_desc);
    if (file_desc < 0) {
        perror("cannot open file");
        return -1;
    }
    int reversedNumber = 0; // let number inverse 

    // reverse number 
    while (number > 0) {
        int digit = number % 10;
        reversedNumber = reversedNumber * 10 + digit;
        number = number / 10;
    }
    while (reversedNumber > 0) {
        int digit = reversedNumber % 10;
        intToBinary4Bits(file_desc, digit);
        printf(" "); // 
        reversedNumber = reversedNumber / 10;
    }
    write(file_desc, test_num, 4);
    
    close(file_desc);
    printf("\n");
}
