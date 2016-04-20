#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "message.h"
#include "packet.h"

struct _Message
{
    long int time;
    int strlen;
    char *str;
};

Message* new_message(long int time, int strlen, char *str) {
    Message* new_msg;
    new_msg = (Message*) malloc(sizeof(Message));
    if (!new_msg) {
        printf("Failed to make message\n");
        return NULL;
    }
    new_msg->time = time;
    new_msg->strlen = strlen;
    new_msg->str = str;

    return new_msg;
}

long int get_time_with_fd(int fd) {
    long int time;
    int n_byte;
    n_byte = read(fd, &time, TIME_MEMORY_SIZE);

    if (n_byte < TIME_MEMORY_SIZE) {
        printf("Failed to read TIME_MEMORY_SIZE\n");
        return -1;
    }

    return time;
}

int get_strlen_with_fd(int fd) {
    int strlen;
    int n_byte;
    n_byte = read(fd, &strlen, STR_LENGTH_MEMORY_SIZE);

    if (n_byte < STR_LENGTH_MEMORY_SIZE) {
        printf("Failed to read STR_LENGTH_MEMORY_SIZE\n");
        return -1;
    }

    return strlen + 1;
}

char* get_str_with_fd(int fd, char *str, int strlen) {
    int n_byte;
    n_byte = read(fd, str, strlen + 1);

    if (n_byte < (strlen + 1)) {
        printf("Failed to read str\n");
        return NULL;
    }

    return str;
}

long int get_time_with_msg(Message* message) {
    if (!message) {
        printf("Can't find the time\n");
        return -1;
    }

    return message->time;
}

int get_strlen_with_msg(Message *message) {
    if (!message) {
        printf("Can't find the strlen\n");
        return -1;
    }

    return (message->strlen + 1);
}

char* get_str_with_msg(Message *message) {
    if (!message) {
        printf("Can't find the str\n");
        return NULL;
    }

    return message->str;
}
