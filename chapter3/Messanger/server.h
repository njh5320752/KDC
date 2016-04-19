#ifndef __SERVER_H__
#define __SERVER_H__

typedef struct _Client Client;
typedef struct _Server Server;
typedef struct _Message message;

void server_send_all_message(int fd, DList *msg_list);
DList* server_send_message(int fd, DList *client_list, DList *msg_list);
int server_add_client(Server *server, int client_fd);
int server_remove_client(Server *server, Client *remove_client);
void free_client(void *client);
Client* server_find_client(Server *server, void *client_data);
int find_client_data(void *data, void *client_data);
Message* server_new_message(char *packet_msg);
void server_write_msg(Message *msg, int client_fd);
void server_set_fd_event(struct pollfd *poll_set, int client_fd, int index);

#endif
