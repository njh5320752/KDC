#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "socket.h"
#include "server_controler.h"
#include "packet.h"
#include "fd_controler.h"

#define POLL_SIZE 32

int main(int argc, char *argv[]) {
    struct sockaddr_un addr;
    int server_fd, client_fd;
    int numfds = 0;
    struct pollfd poll_set[POLL_SIZE];
    int fd_index;
    int tmp_fd;
    char *packet;
    short op_code;
    int packet_size;
    int result, n_byte;
    int msg_fd;

    msg_fd = open(MESSAGE_PATH, O_RDWR | O_APPEND | O_CREAT, S_IRWXU);

    if (msg_fd == -1) {
        printf("Failed to make message file\n");
        return -1;
    }

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
                    client_fd = poll_set[fd_index].fd;
                    op_code = get_op_code_with_fd(poll_set[fd_index].fd);
                    printf("CLIENT FD: %d %d\n", poll_set[fd_index].fd, fd_index);

                    switch(op_code) {
                    case REQ_ALL_MSG:
                        packet_size = server_get_res_all_msg_packet(server, &packet);
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
                        packet_size = server_get_rcv_msg_packet_with_fd(server, client_fd, &packet, msg_fd);
                        if (packet_size == -1) {
                            printf("Failed to make rcv_msg_packet\n");
                        } else {
                            result = server_send_message_to_clients(server, client_fd, packet, packet_size);
                            if (!result) {
                                printf("Failed to send message to clients\n");
                            }
                            free(packet);
                        }
                        break;
                    case REQ_LAST_MSG_FR_FS:
                        packet_size = server_get_res_last_fr_fs_packet(server, client_fd, msg_fd, &packet);

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
                    case REQ_LAST_MSG_FR_LS:
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
        }
    }

    return 0;
}
