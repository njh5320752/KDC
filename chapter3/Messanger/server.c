#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include "socket.h"
#include "sm_manager.h"
#include "DBLinkedList.h"
#include "msg_manager.h"

struct _Client
{
    int fd;
};

struct _Server
{
    DList *message_list;
    DList *client_list;
};

int  server_add_client(Server *server, int client_fd) {
    Client *new_client = NULL;
    DList *list = NULL;
    new_client = (Client*) malloc(sizeof(Client));

    if (!new_client || !server) {
        printf("Can't add client\n");
        return 0;
    }

    new_client->fd = client_fd;
    list = d_list_append(server->client_list, (void*) new_client);

    if (!list) {
        printf("Can't add client\n");
        return 0;
    }

    server->client_list = list;
    return 1;
}

int server_remove_client(Server *server, int remove_fd) {
    Client * remove_client;

    if (!server || !(remove_client < 0)) {
        printf("Can't free client\n");
        return 0;
    }

    remove_client = server_find_client(server, &remove_fd);
    if (!remove_client) {
        printf("There is no client to remove\n");
        return 0;
    }

    server->client_list = d_list_remove_nth_with_data(server->client_list, (void*) remove_client, free_client);
    return 1;
}

void free_client(void *client) {
    Client *remove = (Client*)client;
    free(remove);
}

Client* server_find_client(Server *server, void *client_data) {
    Client *client = NULL;
    client = (Client*) d_list_find_data(server->client_list, find_client_data, client_data);

    if (client == NULL) {
        printf("Can't find this data:%d\n", *((int*) client_data));
        return NULL;
    }
    return client;
}

int find_client_data(void *data, void *client_data) {
    Client *client = (Client *)data;

    printf("client data:%d\n", client->fd);
    if (client->fd == *((int*) client_data)) {
        return 1;
    } else {
        return 0;
    }
}

int server_get_res_all_msg_packet_with_fd(int fd, Server *server) {
    int all_msg_size, msg_len;
    char *packet;

    msg_len = d_list_length(server->message_list);
    if (!msg_len) {
        printf("There is no message\n");
        return -1;
    }

    all_msg_size = server_get_all_msg_size(server);
    if (!all_msg_size) {
        printf("Can't make packet\n");
        return -1;
    }

    packet = make_packet_space(OP_CODE_MEMORY_SIZE + MSG_NUM_MEMORY_SIZE + all_msg_size);
    if (!packet) {
        printf("Can't write data to packet\n");
        return -1;
    }
}

int server_get_msg_list_size(Server *server) {
    int msg_len, all_msg_size;
    int strlen = 0;
    DList* message_node = server->message_list;

    if (!message_list) {
        printf("There is no message\n");
        return -1;
    }

    msg_len = d_list_length(server->message_list);

    while (message_node) {
        strlen += get_strlen_with_msg(message_list);
        message_node = d_list_next(message_node);
    }

    all_msg_size = msg_len*(TIME_MEMORY_SIZE + STR_LENGTH_MEMORY_SIZE) + strlen;
    printf("all_msg_size:%d\n", all_msg_size);

    return all_msg_size;
}

void server_send_all_message(int fd, Server *server) {
    printf("called server_send_all_message\n");
    Message *msg = NULL;
    DList *msg_list;
    short op_code;
    char *packet;
    int op_code_size, packet_size, msg_len, msg_len_data_size;

    msg_list = server->message_list;
    msg_len = d_list_length(msg_list);
    printf("msg_len:%d\n", msg_len);

    if (!msg_len) {
        printf("There is no message\n");
        return;
    }
    msg_len_data_size = sizeof(msg_len_data_size);

    op_code_size = sizeof(short);
    op_code = OP_CODE_2;
    packet = (char*) malloc(op_code_size + msg_len_data_size + );
    *((short*)packet) = op_code;
    packet_size = op_code_size;

    packet_size += msg_len_data_size;
    packet = (char*) realloc(packet, packet_size);
    *((int*)(packet + op_code_size)) = msg_len;
    print_packet(packet, packet_size);

    while (msg_list) {
        msg = (Message*) d_list_get_data(msg_list);
        packet_size = pack_msg_with_msg(msg, packet, packet_size);
        msg_list = d_list_next(msg_list);
    }

    printf("Made op_code 2 binary\n");
    print_packet(packet, packet_size);
    write(fd, packet, packet_size);
}

DList* server_send_message(int fd, DList *client_list, DList *msg_list) {
    printf("called server_send_message\n");
    Client *client = NULL;
    Message *new_msg = NULL;
    char *packet_msg;
    int client_num, client_fd;

    client = server_find_client(client_list, &fd);

    packet_msg = pack_msg_with_fd(fd);

    new_msg = server_new_message(packet_msg);
    msg_list = d_list_append(msg_list, (void*) new_msg);

    client_num = d_list_length(client_list);
    printf("client_num:%d\n", client_num);

    while (client_list) {
        client = (Client*) d_list_get_data(client_list);
        client_fd = client->fd;
        if (client_fd != fd) {
            server_write_msg(new_msg, client_fd);
        }
        client_list = d_list_next(client_list);
    }

    return msg_list;
}

Message* server_new_message(char *packet_msg) {
    Message *new_msg = NULL;
    long int time;
    int str_len;
    char *str;

    new_msg = (Message*) malloc(sizeof(Message));

    time = get_time_with_msg(packet_msg);
    new_msg->time = time;
    printf("time:%ld\n", time);

    str_len = get_str_len_with_msg(packet_msg);
    new_msg->length = str_len;
    printf("str_len:%d\n", str_len);

    str = get_str_with_msg(packet_msg);
    new_msg->message = str;
    printf("str:%s\n", str);

    return new_msg;
}

void server_write_msg(Message *msg, int client_fd) {
    printf("server_write_msg\n");
    short op_code;
    char *packet;
    int op_code_size, packet_size;

    op_code_size = sizeof(short);
    op_code = OP_CODE_4;

    packet = (char*) malloc(op_code_size);
    *((short*)packet) = op_code;

    packet_size = op_code_size;

    packet_size = pack_msg_with_msg(msg, packet, packet_size);
    printf("packet_size=%d\n", packet_size);

    print_packet(packet, packet_size);

    write(client_fd, packet, packet_size);
}

Server* server_new() {
    Server *new_server = (Server*) malloc(sizeof(Server));
    if (!new_server) {
        printf("Can't make server data\n");
    }
    new_server->message_list = NULL;
    new_server->client_list = NULL;

    return new_server;
}
