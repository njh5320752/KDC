#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "client_controler.h"
#include "DBLinkedList.h"

struct _Client_Msg
{
    DList *message_list;
};

Client_Msg* new_Client_Msg() {
    Client_Msg *client_msg;
    client_msg = (Client_Msg*) malloc(sizeof(Client_Msg));

    if(!client_msg) {
        printf("Failed to make client msg space\n");
    }
    return;
}

void client_request_all_message(int fd) {
    int n_byte;
    n_byte = write(fd, &op_code, sizeof(op_code));

    if (n_byte < OP_CODE_MEMORY_SIZE) {
        printf("Failed to write op_code\n");
    }
    return;
}

void client_send_message(int fd, char *read_msg) {
    char * packet;
    time_t current_time;
    int strlen, msg_size;
    int dest;

    dest = 0;

    current_time = time(NULL);
    printf("called client_send_message\n");

    if (current_time == ((time_t) - 1)) {
        printf("Failure to obtain the current time.\n");
        return;
    }

    printf("read_msg:%s\n", read_msg);
    strlen = strlen(read_msg);
    printf("read msg len:%d\n", strlen);

    if (len == 0) {
        printf("There is no data in your command\n");
        return;
    }
    msg_size = sizeof(short) + sizeof(long int) + sizeof(int) + len + 1;
    packet = make_packet_space(msg_size);

    if (!packet) {
        printf("Failed to make space\n");
    }

    printf("Put op code\n");
    dest = write_op_code_to_packet(packet, SND_MSG);
    dest += write_time_to_packet(packet + dest, current_time);
    dest += write_strlen_to_packet(packet + dest, strlen);
    dest += write_str_to_packet(pacekt + dest, read_msg, strlen);

    if (dest < msg_size) {
        printf("Failed to write op_code message\n");
        return;
    }

    write(fd, packet, msg_size);
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

int client_receive_all_messages(Client_Msg *client_msg, int fd) {
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

    return 0;
}

int client_receive_message(Client_Msg *client_msg, int fd) {
    long int time;
    int strlen;
    char *str;
    Message *new_msg;

    time = get_time_with_fd(fd);
    strlen = get_strlen_with_fd(fd);
    str = (char*) malloc(strlen + 1);
    get_str_with_fd(fd, str, strlen + 1);

    new_msg = new_message(time, strlen, str);
    client_msg->message_list = d_list_append(client_list->message_list, (void*)new_msg);

    return 0;
}
