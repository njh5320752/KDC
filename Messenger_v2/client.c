#include <stdio.h>
#include <poll.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>

#include "client.h"
#include "looper.h"
#include "socket.h"

struct _Client
{
    int fd;
    Looper *looper;
};

static void read_packet(int fd) {

}

static void handle_res_events(Client *client, int fd) {
    read_packet(fd);
}

static void handle_disconnect(Client *client, int fd) {

}

static void handle_stdin_event(int fd) {

}

static void handle_events(int fd, void *user_data, int revents) {
    Client *client = (Client*) user_data;

    if (!client) {
        printf("There is no a pointer to Client\n");
        return;
    }

    if (revents == POLLHUP) {
        handle_disconnect(client, fd);
    } else if (revents == POLLIN) {
        if (fd == client->fd) {
            handle_res_events(client, fd);
        } else if (fd == STDIN_FILENO) {
            handle_stdin_event(fd);
        } else {
            printf("There is no fd to handle event\n");
            return;
        }
    } else {
        printf("There is no event to handle\n");
    }
}

Client* new_client(Looper *looper) {
    Client *client;

    struct sockaddr_un addr;
    int client_fd;

    if (!looper) {
        printf("There is no a pointer to Looper\n");
        return NULL;
    }

    client = (Client*) malloc(sizeof(Client));

    if (!client) {
        printf("Failed to make client\n");
        return NULL;
    }

    client->looper = looper;

    if ((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(client_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect error");
        close(client_fd);
        exit(-1);
    }

    client->fd = client_fd;

    add_watcher(looper, STDIN_FILENO, handle_events, client, POLLIN);
    add_watcher(looper, client_fd, handle_events, client, POLLIN);

    return client;
}
