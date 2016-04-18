#ifndef __CLINET_MESSAGE_MENAGER_H__
#define __CLIENT_MESSAGE_MENAGER_H__

#include "DBLinkedList.h"

#define COMMAND "command "

void client_request_all_message(int fd);
void client_send_message(int fd, char *read_msg);
int client_read_command(int fd, char **read_msg);
DList* client_receive_all_messages(DList *msg_list, int fd);
DList* client_receive_message(DList *msg_list, int fd);

#endif
