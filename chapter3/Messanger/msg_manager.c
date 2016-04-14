#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "msg_manager.h"

void print_packet(char *buf, int size) {
    int i;
    for (i = 0; i < size; i++) {
        printf("%02X ", buf[i]);
    }
    printf("\n");
}

char* pack_msg(int fd) {
    char *send_msg = NULL;
    int str_len;
    int n_byte;
    int time_size = sizeof(long int);
    int length_size = sizeof(int);

    send_msg = (char*) realloc(send_msg, time_size);
    printf("Read time\n");
    n_byte = read(fd, send_msg, time_size);
    printf("Reaed n_byte:%d\n", n_byte);
    print_packet(send_msg, time_size);

    n_byte = read(fd, &str_len, length_size);
    printf("Reaed n_byte:%d\n", n_byte);
    send_msg = (char*) realloc(send_msg, time_size + length_size);
    printf("Read len\n");
    *((int*)(send_msg + time_size)) = str_len;
    print_packet(send_msg, time_size + length_size);

    send_msg = (char*) realloc(send_msg, time_size + length_size + str_len + 1);
    printf("Read string\n");
    n_byte = read(fd, (send_msg + time_size + length_size), str_len + 1);
    printf("Reaed n_byte:%d\n", n_byte);
    print_packet(send_msg, time_size + length_size + str_len + 1);
    return send_msg;
}

