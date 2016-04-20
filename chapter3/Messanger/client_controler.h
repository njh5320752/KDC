#ifndef __CLINET_MESSAGE_MENAGER_H__
#define __CLIENT_MESSAGE_MENAGER_H__

#define COMMAND "command "

typedef struct _Client_Msg Client_Msg;

Client_Msg* new_Client_Msg();
int client_request_all_message(int fd);
int client_send_message(int fd, char *read_msg);
int client_read_command(int fd, char **read_msg);
int client_receive_all_messages(Client_Msg *client_msg, int fd);
int client_receive_message(Client_Msg *client_msg, int fd);

#endif
