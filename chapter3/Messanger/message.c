#include "message.h"

struct _Message
{
    long int time;
    int strlen;
    char *str;
};

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

int long get_time_with_msg(Message* message) {
    if (!message) {
        printf("Can't find the time\n");
        return -1;
    }

    return message->time;
}

int get_strlen_with_msg(Message *message) {
    if (!message) {
        printf("Can't find the strlen\n");
        return -1
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
