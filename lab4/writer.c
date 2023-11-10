#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_FILE "/dev/my_dev"
char *seg_for_c[27] = {
    "1111001100010001", // A
    "0000011100000101", // b
    "1100111100000000", // C
    "0000011001000101", // d
    "1000011100000001", // E
    "1000001100000001", // F
    "1001111100010000", // G
    "0011001100010001", // H
    "1100110001000100", // I
    "1100010001000100", // J
    "0000000001101100", // K
    "0000111100000000", // L
    "0011001110100000", // M
    "0011001110001000", // N
    "1111111100000000", // O
    "1000001101000001", // P
    "0111000001010000", //q
    "1110001100011001", //R
    "1101110100010001", //S
    "1100000001000100", //T
    "0011111100000000", //U
    "0000001100100010", //V
    "0011001100001010", //W
    "0000000010101010", //X
    "0000000010100100", //Y
    "1100110000100010", //Z
    "0000000000000000"
};

int main(int argc, char* argv[]) {
    int fd;
    char *name = argv[1];
    char write_buffer[17] ;
    char read_buffer[17];
    
    // 打开字符设备文件
    fd = open(DEVICE_FILE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device file");
        return -1;
    }
    
    for (int i =0; i<strlen(name);i++ ){
        strcpy(write_buffer, seg_for_c[name[i] - 'A']);
        // write_buffer = seg_for_c[name[i]-'A'];
        write(fd, write_buffer, strlen(write_buffer));

        // 重新定位文件偏移以便读取数据

        // 从字符设备读取数据
        read(fd, read_buffer, sizeof(read_buffer));
        printf("Read from device: %s\n", read_buffer);
        sleep(1);
    }
    write(fd, seg_for_c[26], strlen(seg_for_c[26]));
    read(fd, read_buffer, sizeof(read_buffer));
    printf("Read from device: %s\n", read_buffer);
    
    // 关闭字符设备文件
    close(fd);
    
    return 0;
}