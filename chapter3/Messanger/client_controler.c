#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "client_controler.h"
#include "message.h"
#include "packet.h"
#include "DBLinkedList.h"

struct _Client_Msg
{
    DList *message_list;
};

Client_Msg* new_Client_Msg() {
    Client_Msg *client_msg;
    client_msg = (Client_Msg*) malloc(sizeof(Client_Msg));
    client_msg->message_list = NULL;

    if(!client_msg) {
        printf("Failed to make client msg space\n");
    }
    return client_msg;
}

int client_request_all_message(int fd) {
    int n_byte;
    short op_code;

    op_code = REQ_ALL_MSG;

    n_byte = write(fd, &op_code, OP_CODE_MEMORY_SIZE);

    if (n_byte < OP_CODE_MEMORY_SIZE) {
        printf("Failed to write op_code\n");
        return 0;
    }
    return 1;
}

int client_send_message(int fd, char *read_msg) {
    char * packet;
    time_t current_time;
    int len, msg_size;
    int dest, n_byte;

    dest = 0;

    current_time = time(NULL);
    printf("called client_send_message\n");

    if (current_time == ((time_t) - 1)) {
        printf("Failure to obtain the current time.\n");
        return 0;
    }

    printf("read_msg:%s\n", read_msg);
    len = strlen(read_msg);
    printf("read msg len:%d\n", len);

    if (len == 0) {
        printf("There is no data in your command\n");
        return 0;
    }
    msg_size = sizeof(short) + sizeof(long int) + sizeof(int) + len + 1;
    packet = make_packet_space(msg_size);

    if (!packet) {
        printf("Failed to make space\n");
        return 0;
    }

    printf("Put op code\n");
    dest = write_op_code_to_packet(packet, SND_MSG);
    dest += write_time_to_packet(packet + dest, current_time);
    dest += write_strlen_to_packet(packet + dest, len);
    dest += write_str_to_packet(packet + dest, read_msg, len);

    if (dest < msg_size) {
        printf("Failed to write op_code message\n");
        return 0;
    }

    n_byte = write(fd, packet, msg_size);

    if (n_byte < msg_size) {
        printf("Failed to write\n");
        return 0;
    }

    return 1;
}

int client_read_command(int fd, char **read_msg) {
    char buf[2];
    int n_byte;
    int copy_dest = 0;
    int mem_alloc_number = 0;
    printf("start read command\n");
    while ((n_byte = read(fd, buf, sizeof(buf))) > 0) {
        copy_dest = mem_alloc_number;
        mem_alloc_number = mem_alloc_number + n_byte;
        *read_msg = realloc((*read_msg), mem_alloc_number);
        strncpy((*read_msg) + copy_dest, buf, n_byte);
        if (buf[n_byte - 1] != '\n') {
            continue;
        }
        if (mem_alloc_number == 1) {
            printf("There is only new line data\n");
            return -1;
        }
        break;
    }
    printf("end read command\n");
    return mem_alloc_number;
}

int client_receive_all_messages(Client_Msg *client_msg, int fd) {
    printf("client_receive_all_messages\n");
    int msg_num, i;
    msg_num = get_message_num_with_fd(fd);
    printf("msg_num:%d\n", msg_num);

    for (i = 0; i < msg_num; i++) {
        client_receive_message(client_msg, fd);
    }

    return 1;
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
    client_msg->message_list = d_list_append(client_msg->message_list, (void*)new_msg);

    return 1;
}
