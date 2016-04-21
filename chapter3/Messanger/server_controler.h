#ifndef __SERVER_H__
#define __SERVER_H__

typedef struct _Client Client;
typedef struct _Server Server;

int server_add_client(Server *server, int client_fd);
int server_remove_client(Server *server, int remove_fd);
void free_client(void *client);
Client* server_find_client(Server *server, void *client_data);
int find_client_data(void *data, void *client_data);
int server_get_res_all_msg_packet(Server *server, char **packet);
int server_get_msg_list_size(Server *server);
int server_get_rcv_msg_packet_with_fd(Server *sever, int fd, char **packet, int msg_fd);
int server_send_message_to_clients(Server *server, int client_fd, char *packet, int packet_size);
Server* server_new();
int server_get_res_last_fr_fs_packet();
int server_get_res_last_fr_ls_packet();

#endif
