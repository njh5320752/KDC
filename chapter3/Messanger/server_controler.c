#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include "socket.h"
#include "server_controler.h"
#include "packet.h"
#include "message.h"
#include "DBLinkedList.h"

struct _Client
{
    int fd;
    int msg_offset;
};

struct _Server
{
    DList *message_list;
    DList *client_list;
};

static int  server_add_client(Server *server, int client_fd) {
    Client *new_client = NULL;
    DList *list = NULL;
    new_client = (Client*) malloc(sizeof(Client));

    if (!new_client || !server) {
        printf("Can't add client\n");
        return -1;
    }

    new_client->fd = client_fd;
    new_client->msg_offset = 0;
    list = d_list_append(server->client_list, (void*) new_client);

    if (!list) {
        printf("Can't add client\n");
        return -1;
    }

    server->client_list = list;
    return 1;
}

static Client* server_find_client(Server *server, void *client_data) {
    Client *client = NULL;
    client = (Client*) d_list_find_data(server->client_list, find_client_data, client_data);

    if (client == NULL) {
        printf("Can't find this data:%d\n", *((int*) client_data));
        return NULL;
    }
    return client;
}

static int server_get_packet_size(int msg_fd, int *msg_num, int end_offset) {
    int offset, strlen;
    int total_strlen;
    int num;
    int packet_size;

    num = 0;

    offset = lseek(msg_fd, TIME_MEMORY_SIZE, SEEK_CUR);
    if (offset < 0) {
        printf("Failed to move\n");
        return -1;
    }
    printf("get packet seek_cur offset:%d\n", offset);
    strlen = get_strlen_with_fd(msg_fd);
    
    if (strlen == -1) {
        return -1;
    }

    total_strlen = strlen;
    printf("strlen:%d\n", strlen);
    num++;

    while (num < 10) {
        offset = lseek(msg_fd, TIME_MEMORY_SIZE + strlen, SEEK_CUR);
        if (offset > end_offset) {
            printf("You move off_set beyond end of file \n");
            break;
        }
        strlen = get_strlen_with_fd(msg_fd);
        if (strlen == -1) {
            break;
        }
        total_strlen += strlen;
        num++;
    }
    
    printf("num:%d total_strlen:%d\n", num, total_strlen);
    packet_size = total_strlen + (num * (TIME_MEMORY_SIZE + STR_LENGTH_MEMORY_SIZE));
    printf("insert num\n");
    *msg_num = num;
    printf("after insert num\n");

    return packet_size;
}

static int server_get_res_last_fr_ls_packet_size_with_msg_file(int msg_fd, int *msg_num, Client *client) {
    int end_offset;
    int set_offset;
    int packet_size;

    end_offset = lseek(msg_fd, 0, SEEK_END);
    printf("end_off_set:%d\n", end_offset);

    if (end_offset == 0) {
        printf("There is no message\n");
        return 0;
    }
    
    set_offset = lseek(msg_fd, client->msg_offset, SEEK_SET);
    printf("seek_set offset:%d\n", set_offset);

    packet_size = server_get_packet_size(msg_fd, msg_num, end_offset);
    printf("after seek packet_size:%d\n", packet_size);
    return packet_size;
}

static int server_get_res_last_fr_packet_size_with_msg_file(int msg_fd, int *msg_num) {
    int end_offset;
    int set_offset;
    int packet_size;
    
    if (msg_fd < 0) {
        printf("Can't read messge from file\n ");
        return -1;
    }

    end_offset = lseek(msg_fd, 0, SEEK_END);
    printf("end_off_set:%d\n", end_offset);

    if (end_offset == 0) {
        printf("There is no message\n");
        return 0;
    }
    
    set_offset = lseek(msg_fd, 0, SEEK_SET);
    printf("seek_set offset:%d\n", set_offset);

    packet_size = server_get_packet_size(msg_fd, msg_num, end_offset);
    return packet_size;
}

static int get_packet_to_res_all_msg(Server *server, char **packet) {
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

static int get_packet_to_rcv_msg(Server *server, int fd, char **packet, int msg_fd) {
    long int time;
    int strlen;
    char *str;

    int packet_size, dest;
    Message *msg;

    int n_byte;
    int msg_size;

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

    msg_size = dest - OP_CODE_MEMORY_SIZE;

    n_byte = write_message_to_file(*packet + OP_CODE_MEMORY_SIZE, msg_fd, msg_size); 
    if(n_byte != msg_size) {
        printf("Failed to wrtie message to file\n");
        return -1;
    }
    printf("dest:%d\n", dest);
    print_packet(*packet, dest);

    return dest;
}

static int send_message_to_clients(Server *server, int client_fd, char *packet, int packet_size) {
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

/**
 * get_packet_for_res_last_fr_fs:
 *
 * Write ten messages from first file offset of the message file to the packet
 * Return size of packet;
 **/

static int get_packet_for_res_last_fr_fs(Server* server, int client_fd, int msg_fd, char **packet) {
    int packet_size;
    int msg_num;
    int dest;
    long int time;
    int strlen;
    int i;
    int offset;
    Client *client = NULL;

    msg_num = 0;

    client = server_find_client(server, &client_fd);
    if (!client) {
        printf("Can't find client\n");
    }

    packet_size = server_get_res_last_fr_packet_size_with_msg_file(msg_fd, &msg_num);    
    printf("packet_size:%d msg_num:%d\n", packet_size, msg_num); 

    if (!packet_size | !msg_num) {
        return -1;
    }

    packet_size += OP_CODE_MEMORY_SIZE + MSG_NUM_MEMORY_SIZE;
    
    *packet = make_packet_space(packet_size);
    if (!packet) {
        return -1;
    }

    offset = lseek(msg_fd, 0, SEEK_SET);
    printf("seek_set off_set:%d\n", offset);
    dest = write_op_code_to_packet(*packet, RES_FIRST_MSG);
    dest += write_msg_num_to_packet((*packet + dest), msg_num);

    for (i = 0; i < msg_num; i++) {
        time = get_time_with_fd(msg_fd);
        if (time == -1) {
            return -1;
        }
        dest += write_time_to_packet((*packet + dest), time);
        strlen = get_strlen_with_fd(msg_fd);
        if (strlen == -1) {
            return -1;
        }
        dest += write_strlen_to_packet((*packet + dest), strlen);
        get_str_with_fd(msg_fd, (*packet + dest), strlen);
        dest += strlen;
    }
    print_packet(*packet, dest);
    offset = lseek(msg_fd, 0, SEEK_CUR);
    client->msg_offset = offset;
    printf("seek_cur offset:%d\n", offset);
 
    return packet_size;
}

int server_handle_accept_event(Server *server, int fd) {
    int client_fd;
    int state;

    client_fd = accept(fd, NULL, NULL);
    if (client_fd < 0) {
        printf("Failed to accept socket\n");
        return -1;
    }
    state = server_add_client(server, client_fd);
    if (state == -1) {
        printf("Failed to add client\n");
        return -1;
    }
    return client_fd;
}

void server_handle_event(Server *server, int client_fd, int msg_fd) {
    short op_code;
    int packet_size;
    char *packet;
    int result;
    int n_byte;

    op_code = get_op_code_with_fd(client_fd);
    printf("CLIENT FD: %d\n", client_fd);

    switch (op_code) {
    case REQ_ALL_MSG:
        packet_size = get_packet_to_res_all_msg(server, &packet);
        if (packet_size == -1) {
            printf("Failed to make res_all_msg packet\n");
        } else {
            n_byte = write(client_fd, packet, packet_size);
            if (n_byte != packet_size) {
                printf("Failed to send all message to client\n");
            }
            free(packet);
        }
        break;
    case SND_MSG:
        packet_size = get_packet_to_rcv_msg(server, client_fd, &packet, msg_fd);
        if (packet_size == -1) {
            printf("Failed to make rcv_msg_packet\n");
        } else {
            result = send_message_to_clients(server, client_fd, packet, packet_size);
            if (!result) {
                printf("Failed to send message to clients\n");
            }
            free(packet);
        }
        break;
    case REQ_FIRST_MSG:
        packet_size = get_packet_for_res_last_fr_fs(server, client_fd, msg_fd, &packet);

        if (packet_size == -1) {
            printf("Failed to make res_last_fr_fs packet\n");
        } else {
            n_byte = write(client_fd, packet, packet_size);
            if (n_byte != packet_size) {
                printf("Failed to send message to client\n");
            }
            free(packet);
        }
        break;
    case REQ_LAST_MSG:
        packet_size = server_get_res_last_fr_ls_packet(server, client_fd, msg_fd, &packet);
        if (packet_size == -1) {
            printf("Failed to make res_last_fr_fs packet\n");
        } else {
            n_byte = write(client_fd, packet, packet_size);
            if (n_byte != packet_size) {
                printf("Failed to send message to client\n");
            }
            free(packet);
        }
        break;
    default:
        printf("This op_code:%02x is wrong\n", op_code);
        break;
    }

}

int server_remove_client(Server *server, int remove_fd) {
    Client * remove_client;

    if (!server || (remove_fd < 0)) {
        printf("Can't free client\n");
        return -1;
    }

    remove_client = server_find_client(server, &remove_fd);
    if (!remove_client) {
        printf("There is no client to remove\n");
        return -1;
    }

    server->client_list = d_list_remove_nth_with_data(server->client_list, (void*) remove_client, free_client);
    return 1;
}

void free_client(void *client) {
    Client *remove = (Client*)client;
    free(remove);
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

Server* server_new() {
    Server *new_server = (Server*) malloc(sizeof(Server));
    if (!new_server) {
        printf("Can't make server data\n");
        return NULL;
    }
    new_server->message_list = NULL;
    new_server->client_list = NULL;

    return new_server;
}



int server_get_res_last_fr_ls_packet(Server *server, int client_fd, int msg_fd, char **packet) {
    int packet_size;
    int msg_num;
    int dest;
    long int time;
    int strlen;
    int i;
    int offset;
    Client *client = NULL;

    msg_num = 0;

    client = server_find_client(server, &client_fd);
    if (!client) {
        printf("Can't find client\n");
    }

    packet_size = server_get_res_last_fr_ls_packet_size_with_msg_file(msg_fd, &msg_num, client);    
    printf("packet_size:%d msg_num:%d\n", packet_size, msg_num); 

    if (!packet_size | !msg_num) {
        return -1;
    }

    packet_size += OP_CODE_MEMORY_SIZE + MSG_NUM_MEMORY_SIZE;
    
    *packet = make_packet_space(packet_size);
    if (!packet) {
        return -1;
    }

    offset = lseek(msg_fd, client->msg_offset, SEEK_SET);
    printf("seek_set off_set:%d\n", offset);
    dest = write_op_code_to_packet(*packet, RES_LAST_MSG);
    dest += write_msg_num_to_packet((*packet + dest), msg_num);

    for (i = 0; i < msg_num; i++) {
        time = get_time_with_fd(msg_fd);
        if (time == -1) {
            return -1;
            }
        dest += write_time_to_packet((*packet + dest), time);
        strlen = get_strlen_with_fd(msg_fd);
        if (strlen == -1) {
            return -1;
        }
        dest += write_strlen_to_packet((*packet + dest), strlen);
        get_str_with_fd(msg_fd, (*packet + dest), strlen);
        dest += strlen;
    }
    print_packet(*packet, dest);
    offset = lseek(msg_fd, 0, SEEK_CUR);
    client->msg_offset = offset;
    printf("seek_cur offset:%d\n", offset);
 
    return packet_size;
}
