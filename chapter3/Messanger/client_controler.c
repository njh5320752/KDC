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

    if (current_time == ((time_t) - 1)) {
        printf("Failure to obtain the current time.\n");
        return 0;
    }

    len = strlen(read_msg);

    if (len == 0) {
        printf("There is no data in your command\n");
        return 0;
    }
    msg_size = sizeof(short) + sizeof(long int) + sizeof(int) + len;
    packet = make_packet_space(msg_size);

    if (!packet) {
        printf("Failed to make space\n");
        return 0;
    }

    dest = write_op_code_to_packet(packet, SND_MSG);
    dest += write_time_to_packet(packet + dest, current_time);
    dest += write_strlen_to_packet(packet + dest, len);
    dest += write_str_to_packet(packet + dest, read_msg, len);

    if (dest < msg_size) {
        printf("Failed to write op_code message\n");
        return 0;
    }

    print_packet(packet, msg_size);
    n_byte = write(fd, packet, msg_size);

    printf("write size : %d\n", n_byte);

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
    return mem_alloc_number;
}

int client_receive_all_messages(Client_Msg *client_msg, int fd) {
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
    if (time == -1) {
        printf("Failed to get time from fd\n");
        return 0;
    }

    print_data(&time, TIME_MEMORY_SIZE);

    strlen = get_strlen_with_fd(fd);
    if (strlen == -1) {
        printf("Failed to get strlen from fd\n");
        return 0;
    }

    print_data(&strlen, STR_LENGTH_MEMORY_SIZE);

    str = (char*) malloc(strlen);
    if (!str) {
        printf("Failed to make str space\n");
    }

    str = get_str_with_fd(fd, str, strlen);
    if (!str) {
        printf("Failed to get str from fd\n");
    }

    print_packet(str, strlen);

    new_msg = new_message(time, strlen, str);
    client_msg->message_list = d_list_append(client_msg->message_list, (void*)new_msg);

    return 1;
}

int client_request_last_message_from_first_loaction(int fd) {
    int n_byte;
    short op_code;

    op_code = REQ_LAST_MSG_FR_FS;

    n_byte = write(fd, &op_code, OP_CODE_MEMORY_SIZE);

    if (n_byte < OP_CODE_MEMORY_SIZE) {
        printf("Failed to write op_code\n");
        return 0;
    }
    return 1;
}

int client_request_last_message_from_last_location(int fd) {
    int n_byte;
    short op_code;

    op_code = REQ_LAST_MSG_FR_LS;

    n_byte = write(fd, &op_code, OP_CODE_MEMORY_SIZE);

    if (n_byte < OP_CODE_MEMORY_SIZE) {
        printf("Failed to write op_code\n");
        return 0;
    }
    return 1;
}
