#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int main (){
//     char num_array[10][7] = {
//     {'0', '0', '0', '0', '0', '0', '1'}, // 0
//     {'1', '0', '0', '1', '1', '1', '0'}, // 1
//     {'0', '0', '1', '0', '0', '1', '0'}, // 2
//     {'0', '0', '0', '0', '1', '1', '0'}, // 3
//     {'1', '0', '0', '1', '1', '0', '0'}, // 4
//     {'0', '1', '0', '0', '1', '0', '0'}, // 5
//     {'1', '1', '0', '0', '0', '0', '0'}, // 6
//     {'0', '0', '0', '1', '1', '1', '0'}, // 7
//     {'0', '0', '0', '0', '0', '0', '0'}, // 8
//     {'0', '0', '0', '1', '1', '0', '0'}  // 9
// };
    char *num_array[10] = {
        "1111110", // 0
        "0110000", // 1
        "1101101", // 2
        "1111001", // 3
        "0110011", // 4
        "1011011", // 5
        "0011111", // 6
        "1110001", // 7
        "1111111", // 8
        "1110011"  // 9
    };
    int file_desc ;
    file_desc = open("/dev/etx_device", O_RDWR);
    // char *test_num= "1100110" ;
    char *end_num = "0000000" ;
    printf("%d",file_desc);
    if (file_desc < 0) {
        perror("cannot open file");
        return -1;
    }

    char *student_id = "516520" ;
    int reversedNumber=0;
    int number = atoi(student_id);
    while (number > 0) {
        int digit = number % 10;
        reversedNumber = reversedNumber * 10 + digit; //let student_id  inverse 
        number = number / 10;
    }

    while (reversedNumber > 0) {
        int digit = reversedNumber % 10;
        
        
        reversedNumber = reversedNumber / 10;
        
        printf("%d\n", reversedNumber);
        // if (reversedNumber == 0){
        //     break;
        // }
        write(file_desc, num_array[digit], 7);
        sleep(1);
    }
    // write(file_desc, test_num, 7);
    // sleep(1);
    write(file_desc, end_num,  7);
    
    return 0;
}