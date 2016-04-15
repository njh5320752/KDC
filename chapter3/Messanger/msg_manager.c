#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "msg_manager.h"

void print_packet(char *buf, int size) {
    int i;
    for (i = 0; i < size; i++) {
        printf("%02X ", buf[i]);
    }
    printf("\n");
}

char* pack_msg_with_fd(int fd) {
    char *send_msg = NULL;
    int n_byte;
    int str_len;
    int time_size = sizeof(long int);

    n_byte = pack_time_data(fd, &send_msg, time_size);
    str_len = pack_str_len_data(fd, &send_msg, time_size);
    n_byte = n_byte + pack_str_data(fd, &send_msg, str_len);

    return send_msg;
}

int pack_time_data(int fd, char **msg, int time_size) {
    int n_byte;

    *msg = (char*) realloc(*msg, time_size);
    n_byte = read(fd, *msg, time_size);
    printf("Reaed n_byte:%d\n", n_byte);

    print_packet(*msg, time_size);
    return n_byte;
}

int pack_str_len_data(int fd, char **msg, int time_size) {
    int n_byte;
    int str_len;
    int size;
    int length_size = sizeof(int);
    size = time_size + length_size;

    n_byte = read(fd, &str_len, length_size);
    printf("Reaed n_byte:%d\n", n_byte);
    *msg = (char*) realloc(*msg, size);
    *((int*)(*msg + time_size)) = str_len;

    print_packet(*msg, size);
    return str_len;
}

int pack_str_data(int fd, char **msg, int str_len) {
    int time_size = sizeof(long int);
    int str_len_size = sizeof(int);
    int n_byte;

    *msg = (char*) realloc(*msg, time_size + str_len_size + str_len +1);
    n_byte = read(fd, (*msg + time_size + str_len_size), str_len +1);
    printf("Reaed n_byte:%d\n", n_byte);
    print_packet(*msg, time_size + str_len_size + n_byte);
    return n_byte;
}
long int get_time_with_msg(char *msg) {
    long int time;
    time = *((long int*)(msg));
    return time;
}

int get_str_len_with_msg(char *msg) {
    int time_size = sizeof(long int);
    int str_len;
    str_len = *((int*)(msg + time_size));
    return str_len;
}

char* get_str_with_msg(char *msg) {
    int time_size = sizeof(long int);
    int len_size = sizeof(int);
    return (msg + time_size + len_size);
}

int pack_msg_with_msg(Message* msg, char *packet, int dst) {
    printf("call pack_msg_with_msg()\n");
    long int time = msg->time;
    int length = msg->length;
    char *message = msg->message;

    int packet_size;
    int length_size = sizeof(int);
    int time_size = sizeof(long int);
    packet_size = dst + length_size + time_size + length + 1;

    packet = realloc(packet, packet_size);
    *((long int*)(packet + dst)) = time;
    *((int*)(packet + dst + time_size)) = length;
    strcpy((packet + dst + time_size + length_size), message);
    print_packet(packet, packet_size);

    return packet_size;
}
