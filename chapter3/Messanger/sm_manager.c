#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sm_manager.h"
#include "DBLinkedList.h"
#include "msg_manager.h"

struct _Client
{
    int fd;
};

DList* server_new_client(DList *list, int client_fd) {
    Client *new_client = NULL;
    new_client = (Client*) malloc(sizeof(Client));
    new_client->fd = client_fd;
    list = d_list_append(list, (void*) new_client);
    return list;
}

DList* server_free_client(DList *list, Client *remove_client) {
    list = d_list_remove_nth_with_data(list, (void*) remove_client, free_client);
    return list;
}

void free_client(void *client) {
    Client *remove = (Client*)client;
    free(remove);
}

Client* server_find_client(DList *list, void *client_data) {
    Client *client = NULL;
    client = (Client*) d_list_find_data(list, find_client_data, client_data);
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

void server_send_all_message(DList *list) {
    printf("called server_send_all_message\n");
}

DList* server_send_message(int fd, DList *client_list, DList *msg_list) {
    printf("called server_send_message\n");
    Client *client = NULL;
    char *packet_msg;
    int client_num;
    int i;

    client = server_find_client(client_list, &fd);

    packet_msg = pack_msg_with_fd(fd);
    msg_list = server_new_message(msg_list, packet_msg);

    client_num = d_list_length(client_list);
    printf("client_num:%d\n", client_num);
    for (i = 0; i < client_num; i++) {

    }
    return msg_list;
}

DList* server_new_message(DList *msg_list, char *packet_msg) {
    Message *new_msg = NULL;
    long int time;
    int str_len;
    char *str;
    char *packet;

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

    msg_list = d_list_append(msg_list, (void*) new_msg);
    packet = server_pack_msg_with_msg(msg_list);
    
    return msg_list;
}

char* server_pack_msg_with_msg(DList *msg_list) {
    Message *last_msg = NULL;
    DList *last_node;
    short op_code;
    char *packet;
    int op_code_size;
    int packet_size;
    op_code_size = sizeof(short);
    last_node = d_list_last(msg_list);
    if (!last_node) {
        printf("There is no message\n");
        return NULL;
    }
    last_msg = (Message*) d_list_get_data(last_node);
    op_code = 0x4;
    packet = (char*) malloc(op_code_size);
    *((short*)packet) = op_code;
    packet_size = op_code_size;
    packet_size = pack_msg_with_msg(last_msg, packet, packet_size);
    printf("packet_size=%d\n", packet_size);
    print_packet(packet, packet_size);
    return packet;
}
