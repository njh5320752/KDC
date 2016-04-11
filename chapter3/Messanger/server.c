#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>
#include "DBLinkedList.h"
#include "socket.h"

#define POLL_SIZE 32

struct _Client
{
    int fd;
};

typedef struct _Client Client;

void free_client(void *client);
int find_data(void *data);

int tmp_fd;

int main(int argc, char *argv[]) {
    struct sockaddr_un addr;
    char buf[100];
    Client *new_client;
    Client *tmp;
    int server_fd, client_fd;
    int numfds = 0;
    struct pollfd poll_set[POLL_SIZE];
    int fd_index;
    DList *list;
    int length;
    int rc;
    list = NULL;

    unlink(SOCKET_NAME);
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM,0)) == -1) {
        perror("socket error");
        close(server_fd);
        exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path)-1);

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

    poll_set[0].fd = server_fd;
    poll_set[0].events = POLLIN;
    numfds++;

    while (1) {
        if (poll(poll_set, numfds, 100000) > 0) {
            if (poll_set[0].revents & POLLIN) {
                client_fd = accept(server_fd, NULL, NULL);
                new_client = (Client*) malloc(sizeof(Client));
                new_client->fd = client_fd;
                poll_set[numfds].fd = client_fd;
                poll_set[numfds].events = POLLIN;
                poll_set[numfds].revents = 0;
                numfds++;
                list = d_list_append(list, (void*)new_client);
                printf("Adding client on fd %d\n", client_fd);
                length = d_list_length(list);
                printf("client number:%d\n", length);

            }
            for (fd_index = 1; fd_index < numfds; fd_index++) {
                if (poll_set[fd_index].revents & POLLHUP) {
                    tmp_fd = poll_set[fd_index].fd;
                    tmp = (Client *) d_list_find_data(list, find_data);
                    list = d_list_remove_nth_with_data(list, tmp, free_client);
                    printf("poll_set[fd_index]:%d\n", poll_set[fd_index].fd);
                    numfds--;
                    poll_set[fd_index] = poll_set[numfds];
                    break;
                } else if (poll_set[fd_index].revents & POLLIN) {
                    rc=read(poll_set[fd_index].revents, buf, sizeof(buf));
                    printf("%s\n", buf);
                }
            }
        }
    }
    return 0;
}

void free_client(void *client) {
    Client *remove = (Client*)client;
    free(remove);
}

int find_data(void *data) {
    Client *client = (Client *)data;
    if (client->fd == tmp_fd) {
        return 1;
    } else {
        return 0;
    }
}
