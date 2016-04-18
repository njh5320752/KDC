#ifndef __SERVER_CONTROLER_H__
#define __SERVER_CONTROLER_H__

#include "DBLinkedList.h"
#include "msg_manager.h"

typedef struct _Client Client;
typedef struct _Message Message;
typedef struct _Server Server;

void server_send_all_message(int fd, DList *msg_list);
DList* server_send_message(int fd, DList *client_list, DList *msg_list);
Server* server_add_client(DList *list, int client_fd);
DList* server_free_client(DList *list, Client *remove_client);
void free_client(void *client);
Client* server_find_client(DList *list, void *client_data);
int find_client_data(void *data, void *client_data);
Message* server_new_message(char *packet_msg);
void server_write_msg(Message *msg, int client_fd);
void server_set_fd_event(struct pollfd *poll_set, int client_fd, int index);

#endif
