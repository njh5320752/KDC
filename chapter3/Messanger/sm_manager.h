#ifndef __SERVER_MESSAGE_MANAGER_H__
#define __SERVER_MESSAGE_MANAGER_H__

#include "DBLinkedList.h"

typedef struct _Client Client;

void server_send_all_message();
void server_send_message(int fd, DList *list);
DList* server_new_client(DList *list, int client_fd);
DList* server_free_client(DList *list, Client *remove_client);
void free_client(void *client);
Client* server_find_client(DList *list, void *client_data);
int find_client_data(void *data, void *client_data);

#endif
