#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>
#include "DBLinkedList.h"
#include "socket.h"
#include "sm_manager.h"

#define POLL_SIZE 32

int main(int argc, char *argv[]) {
    struct sockaddr_un addr;
    Client *remove_client;
    int server_fd, client_fd;
    int numfds = 0;
    struct pollfd poll_set[POLL_SIZE];
    int fd_index;
    DList *list;
    int length;
    list = NULL;
    int tmp_fd;

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
                if (client_fd < 0) {
                    printf("File descriptor is wrong\n");
                    break;
                }
                poll_set[numfds].fd = client_fd;
                poll_set[numfds].events = POLLIN;
                poll_set[numfds].revents = 0;
                numfds++;
                list = server_new_client(list, client_fd);
                printf("Adding client on fd %d\n", client_fd);
                length = d_list_length(list);
                printf("client number:%d\n", length);

            }
            for (fd_index = 1; fd_index < numfds; fd_index++) {
                if (poll_set[fd_index].revents & POLLHUP) {
                    tmp_fd = poll_set[fd_index].fd;
                    remove_client = server_find_client(list, &tmp_fd);
                    list = server_free_client(list, remove_client);
                    printf("poll_set[fd_index]:%d\n", poll_set[fd_index].fd);
                    numfds--;
                    poll_set[fd_index] = poll_set[numfds];
                } else if (poll_set[fd_index].revents & POLLIN) {
                    printf("event poll in\n");
                    int n_byte;
                    short op_code;
                    n_byte = read(poll_set[fd_index].fd, &op_code, sizeof(op_code));
                    printf("n_btye:%d op_code:%02x\n", n_byte, op_code);
                    switch(op_code) {
                    case 0x1:
                        server_send_all_message(list);
                        break;
                    case 0x3:
                        server_send_message(poll_set[fd_index].fd, list);
                        break;
                    default:
                        printf("This op_code:%02x is wrong\n", op_code);
                    }
                }
            }
        }
    }
    return 0;
}

