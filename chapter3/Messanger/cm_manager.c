#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cm_manager.h"
#include "msg_manager.h"
#include "DBLinkedList.h"

void client_request_all_message(int fd) {
    printf("called client_request_all_message\n");
    short op_code = 0x1;
    write(fd, &op_code, sizeof(op_code));
    return;
}

void client_send_message(int fd, char *read_msg) {
    char * send_msg;
    time_t current_time;
    int len;
    int msg_size;

    current_time = time(NULL);
    printf("called client_send_message\n");

    if (current_time == ((time_t)-1)) {
        printf("Failure to obtain the current time.\n");
        return;
    }

    printf("read_msg:%s\n", read_msg);
    len = strlen(read_msg);
    printf("read msg len:%d\n", len);
    if (len == 0) {
        printf("There is no data in your command\n");
        return;
    }
    msg_size = sizeof(short) + sizeof(long int) + sizeof(int) + len + 1;
    printf("msg_size:%d\n", msg_size);
    send_msg = (char*) malloc(msg_size);

    printf("Put op code\n");
    *((short*)send_msg) = 0x3;
    print_packet(send_msg, msg_size);

    printf("Put current_time\n");
    *((long int*)(send_msg + sizeof(short))) = current_time;
    print_packet(send_msg, msg_size);

    printf("Put len\n");
    *((int*)(send_msg + sizeof(short) + sizeof(long int))) = len;
    print_packet(send_msg, msg_size);

    printf("Put message\n");
    strcpy(send_msg + sizeof(short) + sizeof(long int) + sizeof(int), read_msg);
    print_packet(send_msg, msg_size);
    write(fd, send_msg, msg_size);
    return;
}

int client_read_command(int fd, char **read_msg) {
    char buf[2];
    int n_byte;
    int copy_dest = 0;
    int mem_alloc_number = 0;
    while ((n_byte = read(fd, buf, sizeof(buf))) > 0) {
        printf("start read command\n");
        printf("n_byte:%d\n", n_byte);
        print_packet(buf, sizeof(buf));
        copy_dest = mem_alloc_number;
        mem_alloc_number = mem_alloc_number + n_byte;
        printf("mem_alloc_number:%d\n", mem_alloc_number);
        *read_msg = realloc((*read_msg), mem_alloc_number);
        strncpy((*read_msg) + copy_dest, buf, n_byte);
        if (buf[n_byte - 1] != '\n') {
            continue;
        }
        if (mem_alloc_number == 1) {
            printf("There is only new line data\n");
            return -1;
        }
        print_packet(*read_msg, mem_alloc_number);
        break;
    }
    printf("end read command\n");
    return mem_alloc_number;
}

DList* client_receive_all_messages(DList *msg_list, int fd) {
    printf("client_receive_all_messages\n");
    int msg_len, size, n_byte, i;
    Message *new_msg;
    size = sizeof(msg_len);

    n_byte = read(fd, &msg_len, size);
    printf("msg_len:%d n_byte:%d\n", msg_len, n_byte);

    for(i = 0; i < msg_len; i++) {
        new_msg = unpack_msg_with_fd(fd);
        msg_list = d_list_append(msg_list, (void*)new_msg);
    }

    return msg_list;
}

DList* client_receive_message(DList *msg_list, int fd) {
    printf("client_receive_message\n");
    Message *new_msg;

    new_msg = unpack_msg_with_fd(fd);
    msg_list = d_list_append(msg_list, (void*)new_msg);
    return msg_list;
}
