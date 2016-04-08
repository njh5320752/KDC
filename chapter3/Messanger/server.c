#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>
#include "DBLinkedList.h"
#include "socket.h"

#define POLL_SIZE 32

int main(int argc, char *argv[]) {
    struct sockaddr_un addr;
    int server_fd, client_fd;
    char buf[100];
    int option = 1;
    int numfds = 0;
    struct pollfd poll_set[POLL_SIZE];
    int fd_index;
    DList *list;
    int length;
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
                poll_set[numfds].fd = client_fd;
                poll_set[numfds].events = POLLIN;
                numfds++;
                list = d_list_append(list, (void*)client_fd);
                printf("Adding client on fd %d\n", client_fd);
                length = d_list_length(list);
                printf("client number:%d\n", length);

            }
            for (fd_index = 1; fd_index < numfds; fd_index++) {
                if (poll_set[fd_index].revents & POLLHUP) {
                    d_list_remove_nth_with_data(list, (void*)poll_set[fd_index].fd, NULL);
                    printf("poll_set[fd_index]:%d\n", poll_set[fd_index].fd);
                    numfds--;
                    poll_set[fd_index] = poll_set[numfds];
                    break;
                }
            }
        }
    }
    return 0;
}
