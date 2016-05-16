#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <sys/un.h>
#include <string.h>
#include <sys/types.h>

#include "server.h"
#include "looper.h"
#include "DBLinkedList.h"
#include "mesg_file_db.h"
#include "mesg_file.h"
#include "socket.h"

struct _Server {
    int fd;
    DList *client_list;
    Looper *looper;
    Mesg_File *mesg_file;
    Mesg_File_DB *mesg_file_db;
};

struct _Client {
    int fd;
    int file_offset;
    int read_state;
};

static void destroy_client(void *client) {
    free((Client*) client);
}

static int match_client(void *client, void *fd) {

    printf("client data:%d\n", ((Client*) client)->fd);
    if (((Client*) client)->fd == *((int*) fd)) {
        return 1;
    } else {
        return 0;
    }
}

static Client* find_client(Server *server, int fd) {
    Client *client = NULL;
    client = (Client*) d_list_find_data(server->client_list, match_client, &fd);

    if (client == NULL) {
        printf("Can't find Client\n");
        return NULL;
    }
    return client;
}

static void add_client(Server *server, int fd) {
    Client *client;
    DList *list;

    list = server->client_list;
    client = (Client*) malloc(sizeof(Client));

    if (!client) {
        printf("Failed to make Client\n");
        return;
    }

    client->fd = fd;
    client->file_offset = 0;
    client->read_state = 0;

    server->client_list = d_list_append(list, client);
}

static void remove_client(Server *server, int fd) {
    Client *remove_client;
    DList *list;

    list = server->client_list;
    remove_client = find_client(server, fd);

    if (!remove_client) {
        printf("There is no client to remove\n");
    }

    server->client_list = d_list_remove_nth_with_data(list, remove_client, destroy_client);
}

static void handle_accept_event(Server *server) {
    int client_fd;
    int state;

    client_fd = accept(server->fd, NULL, NULL);
    if (client_fd < 0) {
        printf("Failed to accept socket\n");
        return;
    }

    add_client(server, client_fd);
    return;
}

static void handle_disconnect_event(Server *server, int fd) {
    if (fd == server->fd) {
        set_state(server->looper, -1);
    } else {
        remove_client(server, fd);
    }
}

static void read_packet(int fd) {
    char buf[MAX_BUF_LEN];
    int n_byte;

    while (n_byte = read(fd, buf, MAX_BUF_LEN)) {
       if (n_byte == 0) {
           printf("Finished read packet\n");
       }
    }
}

static void handle_req_event(Server *server, int fd) {
    read_packet(fd);
}

static void handle_events(int fd, void *user_data, int revents) {
    Server *server = (Server*) user_data;

    if (!server && (fd < 0)) {
        printf("Can't handle events\n");
        return;
    }

    if (revents == POLLHUP) {
         handle_disconnect_event(server, fd);
    } else if (revents == POLLIN) {
        if (fd == server->fd) {
            handle_accept_event(server);
        } else {
            handle_req_event(server, fd);
        }
    }
}

Server* new_server(Looper *looper) {
    Server *server;
    struct sockaddr_un addr;
    int server_fd;
    Mesg_File *mesg_file;
    Mesg_File_DB *mesg_file_db;

    if (!looper) {
        printf("Looper is empty\n");
        return NULL;
    }

    server = (Server*) malloc(sizeof(Server));

    if (!server) {
        printf("Failed to make Server\n");
        return NULL;
    }

    unlink(SOCKET_PATH);
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM,0)) == -1) {
        perror("socket error");
        exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path)-1);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind error");
        close(server_fd);
        exit(-1);
    }

    if (listen(server_fd, 0) == -1) {
        perror("listen error");
        close(server_fd);
        exit(-1);
    }

    mesg_file = new_mesg_file();
    mesg_file_db = new_mesg_file_db();

    if (!mesg_file && !mesg_file_db) {
        printf("There is no a pointer to Mesg_File or Mesg_File_DB\n");
        return NULL;
    }

    server->looper = looper;
    server->fd = server_fd;
    server->client_list = NULL;
    server->mesg_file = mesg_file;
    server->mesg_file_db = mesg_file_db;

    set_state(server->looper, 1);

    add_watcher(server->looper, server_fd, handle_events, server, POLLIN);

    return server;
}

void destroy_server(Server *server) {
    DList *list;

    if (!server) {
        printf("There is no pointer to Server\n");
        return;
    }

    list = server->client_list;
    d_list_free(list, destroy_client);

    destroy_looper(server->looper);
    destroy_mesg_file_db(server->mesg_file_db);

    free(server);
}
