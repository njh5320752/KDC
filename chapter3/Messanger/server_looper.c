#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>
#include "socket.h"
#include "server.h"
#include "fd_controler.h"

#define POLL_SIZE 32

int main(int argc, char *argv[]) {
    struct sockaddr_un addr;
    Client *remove_client;
    int server_fd, client_fd;
    int numfds = 0;
    struct pollfd poll_set[POLL_SIZE];
    int fd_index;
    int length, tmp_fd;
    Server *server;
    server = NULL;

    server = server_new();
    if (server == NULL) {
        printf("Can't add message and client\n");
        return -1;
    }

    unlink(SOCKET_NAME);
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM,0)) == -1) {
        perror("socket error");
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

    int result;

    while (1) {
        if (poll(poll_set, numfds, 100000) > 0) {
            if (poll_set[0].revents & POLLIN) {
                client_fd = accept(server_fd, NULL, NULL);
                if (client_fd < 0 || numfds >= POLL_SIZE) {
                    printf("Can't set a file descriptor event\n");
                    break;
                }
                result = fd_controler_set_event(poll_set, client_fd, numfds);
                if (!result) {
                    printf("Failed to set event\n");
                }
                result = server_add_client(server, client_fd);
                if (!result) {
                    printf("Failed to add client\n");
                }
                numfds++;
                printf("Adding client on fd %d\n", client_fd);
            }
            for (fd_index = 1; fd_index < numfds; fd_index++) 
                if (poll_set[fd_index].revents & POLLHUP) {
                    tmp_fd = poll_set[fd_index].fd;
                    result = server_remove_client(server, tmp_fd);
                    if (!result > 0) {
                        printf("Failed to remove client\n");
                    } else {
                        numfds--;
                        poll_set[fd_index] = poll_set[numfds];
                    }
                } else if (poll_set[fd_index].revents & POLLIN) {
                    char *packet_msg;
                    short op_code;
                    int client_fd;
                    client_fd = poll_set[fd_index].fd;
                    
                    op_code = get_op_code_with_fd(client_fd);

                    switch(op_code) {
                        case OP_CODE_1:
                            server_get_all_msg_packet_with_fd(client_fd, server);
                            server_send_all_message(client_fd, msg_list);
                            break;
                        case OP_CODE_3:
                            msg_list = server_send_message(client_fd, client_list, msg_list);
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
