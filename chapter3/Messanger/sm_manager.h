#ifndef __SERVER_MESSAGE_MANAGER_H__
#define __SERVER_MESSAGE_MANAGER_H__

#include "DBLinkedList.h"

typedef struct _Client Client;

void server_send_all_message();
DList* server_send_message(int fd, DList *client_list, DList *msg_list);
DList* server_new_client(DList *list, int client_fd);
DList* server_free_client(DList *list, Client *remove_client);
void free_client(void *client);
Client* server_find_client(DList *list, void *client_data);
int find_client_data(void *data, void *client_data);
DList* server_new_message(DList *msg_list, char *packet_msg);
char* server_pack_msg_with_msg(DList *msg_list);

#endif
