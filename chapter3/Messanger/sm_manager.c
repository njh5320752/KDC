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

void server_send_message(int fd, DList *list) {
    printf("called server_send_message\n");
    Client *client = NULL;
    char *send_msg;
    int client_num;

    client = server_find_client(list, &fd);
    send_msg = pack_msg(fd);
    client_num = d_list_length(list);
    printf("client_num:%d\n", client_num);
    return;
}
