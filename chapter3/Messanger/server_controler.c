#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include "socket.h"
#include "server_controler.h"
#include "packet.h"
#include "message.h"
#include "DBLinkedList.h"

struct _Client
{
    int fd;
};

struct _Server
{
    DList *message_list;
    DList *client_list;
};

int  server_add_client(Server *server, int client_fd) {
    Client *new_client = NULL;
    DList *list = NULL;
    new_client = (Client*) malloc(sizeof(Client));

    if (!new_client || !server) {
        printf("Can't add client\n");
        return 0;
    }

    new_client->fd = client_fd;
    list = d_list_append(server->client_list, (void*) new_client);

    if (!list) {
        printf("Can't add client\n");
        return 0;
    }

    server->client_list = list;
    return 1;
}

int server_remove_client(Server *server, int remove_fd) {
    Client * remove_client;

    if (!server || (remove_fd < 0)) {
        printf("Can't free client\n");
        return 0;
    }

    remove_client = server_find_client(server, &remove_fd);
    if (!remove_client) {
        printf("There is no client to remove\n");
        return 0;
    }

    server->client_list = d_list_remove_nth_with_data(server->client_list, (void*) remove_client, free_client);
    return 1;
}

void free_client(void *client) {
    Client *remove = (Client*)client;
    free(remove);
}

Client* server_find_client(Server *server, void *client_data) {
    Client *client = NULL;
    client = (Client*) d_list_find_data(server->client_list, find_client_data, client_data);

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

int server_get_res_all_msg_packet(Server *server, char **packet) {
    long int time;
    int strlen;
    char *str;

    int all_msg_size, msg_len;
    int dest = 0;
    Message *msg;

    DList *msg_list;
    msg_list = server->message_list;

    msg_len = d_list_length(server->message_list);
    if (!msg_len) {
        printf("There is no message\n");
        return -1;
    }

    all_msg_size = server_get_msg_list_size(server);
    if (!all_msg_size) {
        printf("Can't make packet\n");
        return -1;
    }

    *packet = make_packet_space(OP_CODE_MEMORY_SIZE + MSG_NUM_MEMORY_SIZE + all_msg_size);
    if (!packet) {
        printf("Can't write data to packet\n");
        return -1;
    }

    dest = write_op_code_to_packet(*packet, RES_ALL_MSG);
    dest += write_msg_num_to_packet((*packet + dest), msg_len);

    while(msg_list) {
        msg = (Message*) d_list_get_data(msg_list);
        time = get_time_with_msg(msg);
        strlen = get_strlen_with_msg(msg);
        str = get_str_with_msg(msg);

        dest += write_time_to_packet((*packet + dest), time);
        dest += write_strlen_to_packet((*packet + dest), strlen);
        dest += write_str_to_packet((*packet + dest), str, strlen);
        msg_list = d_list_next(msg_list);
    }
    return dest;
}

int server_get_msg_list_size(Server *server) {
    int msg_len, all_msg_size;
    int strlen = 0;
    DList* message_node = server->message_list;
    Message* msg;

    if (!message_node) {
        printf("There is no message\n");
        return -1;
    }

    msg_len = d_list_length(server->message_list);

    while (message_node) {
        msg = (Message*) d_list_get_data(message_node);
        strlen += get_strlen_with_msg(msg);
        message_node = d_list_next(message_node);
    }

    all_msg_size = msg_len*(TIME_MEMORY_SIZE + STR_LENGTH_MEMORY_SIZE) + strlen;
    printf("all_msg_size:%d\n", all_msg_size);

    return all_msg_size;
}

int server_get_rcv_msg_packet_with_fd(Server *server, int fd, char **packet, int msg_fd) {
    long int time;
    int strlen;
    char *str;

    int packet_size, dest;
    Message *msg;

    int n_byte;

    dest = 0;

    time = get_time_with_fd(fd);
    strlen = get_strlen_with_fd(fd);
    str = (char*) malloc(strlen);
    str = get_str_with_fd(fd, str, strlen);

    msg = new_message(time, strlen, str);
    server->message_list = d_list_append(server->message_list, (void*) msg);

    packet_size = OP_CODE_MEMORY_SIZE + TIME_MEMORY_SIZE + STR_LENGTH_MEMORY_SIZE + strlen;
    *packet = make_packet_space(packet_size);

    dest = write_op_code_to_packet(*packet, RCV_MSG);
    dest += write_time_to_packet((*packet + dest), time);
    dest += write_strlen_to_packet((*packet + dest), strlen);
    dest += write_str_to_packet((*packet + dest), str, strlen);
    
    n_byte = write_message_to_file(*packet, msg_fd, dest); 
    if(n_byte != dest) {
        printf("Failed to wrtie message to file\n");
        return -1;
    }
    printf("dest:%d\n", dest);
    print_packet(*packet, dest);

    return dest;
}

int server_send_message_to_clients(Server *server, int client_fd, char *packet, int packet_size) {
    Client *client = NULL;
    DList *client_list;
    int n_byte;
    int fd;

    client_list = server->client_list;

    while (client_list) {
        client = (Client*) d_list_get_data(client_list);
        fd = client->fd;
        if (client_fd != fd) {
            n_byte = write(fd, packet, packet_size);
            if (n_byte < packet_size) {
                printf("Failed to write packet to clients\n");
                return 0;
            }
        }
        client_list = d_list_next(client_list);
    }
    return 1;
}

Server* server_new() {
    Server *new_server = (Server*) malloc(sizeof(Server));
    if (!new_server) {
        printf("Can't make server data\n");
    }
    new_server->message_list = NULL;
    new_server->client_list = NULL;

    return new_server;
}
