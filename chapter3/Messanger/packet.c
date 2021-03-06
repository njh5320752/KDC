#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "packet.h"
#include "message.h"

void print_packet(char *buf, int size) {
    print_data(buf, size);
    printf("\n");
}

void print_data(void *buf, int size) {
    int i;
    for (i = 0; i < size; i++) {
        printf("%02X ", *((char*)buf + i));
    }
}

short get_op_code_with_fd(int fd) {
    short op_code;
    int n_byte;
    n_byte = read(fd, &op_code, OP_CODE_MEMORY_SIZE);
    printf("OP CODE : %d\n", op_code);
    if (n_byte < OP_CODE_MEMORY_SIZE) {
        printf("Failed to read op_code:%d %d\n",op_code, n_byte);
    }
    return op_code;
}

char* make_packet_space(int size) {
    char *packet_space;
    packet_space = (char*) malloc(size);

    if (!packet_space) {
        printf("Failed to make space\n");
        return NULL;
    }

    return packet_space;
}

int write_op_code_to_packet(char *packet, short op_code) {
    packet = memcpy(packet, &op_code, OP_CODE_MEMORY_SIZE);
    if (!packet) {
        printf("Fail to write op_code\n");
        return 0;
    }
    return OP_CODE_MEMORY_SIZE;
}

int write_msg_num_to_packet(char *packet, int msg_num) {
    packet = memcpy(packet, &msg_num, MSG_NUM_MEMORY_SIZE);
    return MSG_NUM_MEMORY_SIZE;
}

int write_time_to_packet(char *packet, long int time) {
    packet = memcpy(packet, &time, TIME_MEMORY_SIZE);
    return TIME_MEMORY_SIZE;
}

int write_strlen_to_packet(char *packet, int strlen) {
    packet = memcpy(packet, &strlen, STR_LENGTH_MEMORY_SIZE);
    return STR_LENGTH_MEMORY_SIZE;
}

int write_str_to_packet(char *packet, char *str, int strlen) {
    packet = memcpy(packet, str, strlen);
    return strlen;
}
