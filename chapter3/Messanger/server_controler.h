#ifndef __SERVER_H__
#define __SERVER_H__

typedef struct _Client Client;
typedef struct _Server Server;

int server_handle_accept_event(Server *server, int fd);
void server_handle_event(Server *server, int client_fd, int msg_fd);
int server_remove_client(Server *server, int remove_fd);
void free_client(void *client);
int find_client_data(void *data, void *client_data);
int server_get_msg_list_size(Server *server);
Server* server_new();

#endif
