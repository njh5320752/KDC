#include <sys/socket.h>
#include <sys/un.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "socket.h"
#include "packet.h"
#include "client_controler.h"

#define POLL_SIZE 2

void client_looper() {
    Client_Msg *client_msg;
    char *read_msg;
    int msg_len;
    int comp_result;
    char cm_num;

    struct sockaddr_un addr;
    struct pollfd poll_set[POLL_SIZE];
    int client_fd;

    if ((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    poll_set[0].fd = STDIN_FILENO;
    poll_set[0].events = POLLIN;

    poll_set[1].fd = client_fd;
    poll_set[1].events = POLLIN;

    if (connect(client_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect error");
        close(client_fd);
        exit(-1);
    }
 
    client_msg = new_Client_Msg();

    while (1) {
        if (poll(poll_set, POLL_SIZE, 100000) > 0) {
            printf("called poll\n");
            if (poll_set[0].revents & POLLIN) {
                read_msg = NULL;
                msg_len = client_read_command(poll_set[0].fd, &read_msg);
                if (msg_len >= 10 && (comp_result = strncasecmp(read_msg, COMMAND, 8) == 0)) {
                    if (read_msg[msg_len -1] == '\n') {
                        read_msg[msg_len -1] = '\0';
                    }
                    cm_num = read_msg[8];
                    switch(cm_num) {
                        case '1':
                            if (msg_len == 10) {
                                printf("command 1\n");
                                client_request_all_message(poll_set[1].fd);
                            } else {
                                printf("Please recommand\n");
                            }
                            break;
                        case '3':
                            if (msg_len >= 11 && (read_msg[9] == ' ')) {
                                printf("command 3\n");
                                client_send_message(poll_set[1].fd, read_msg + 10);
                            } else {
                                printf("Please recommand\n");
                            }
                            break;
                        case '5':
                            if (msg_len == 10) {
                                printf("command 5\n");
                                client_request_last_message_from_first_loaction(poll_set[1].fd);
                            } else {
                                printf("Please recommand\n");
                            }
                            break;
                        case '7':
                            if (msg_len == 10) {
                                printf("command 7\n");
                                client_request_last_message_from_last_location(poll_set[1].fd);
                            } else {
                                printf("Please recommand\n");
                            }
                            break;
                        default:
                            printf("Your command numser is %c Please recommand\n", cm_num);
                    }
                } else {
                    printf("Please recommand\n");
                }
                free(read_msg);
            }

            if (poll_set[1].revents & POLLHUP){
                printf("poll_hup\n");
                close(poll_set[1].fd);
                printf("Server is not connected\n");
                exit(-1);
            }

            if (poll_set[1].revents & POLLIN) {
                short op_code;
                int fd = poll_set[1].fd;
                op_code = get_op_code_with_fd(fd);
                printf("client_fd = %d, op_cde=%2x\n", poll_set[1].fd, op_code);
                switch (op_code) {
                    case RES_ALL_MSG:
                        client_receive_all_messages(client_msg, fd);
                        break;
                    case RCV_MSG:
                        client_receive_message(client_msg, fd);
                        break;
                    default:
                        printf("Wrong opcode:%2x\n", op_code);
                }
            }
        }
    }
}
