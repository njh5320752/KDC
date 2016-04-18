#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>
#include "socket.h"
#include "cm_manager.h"
#include "msg_manager.h"

#define POLL_SIZE 2

int main(void) {
    struct sockaddr_un addr;
    struct pollfd poll_set[POLL_SIZE];
    int client_fd;
    DList *msg_list;
    msg_list = NULL;

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

    char *read_msg;
    int msg_len;
    int comp_result;
    char cm_num;

    while (1) {
        if (poll(poll_set, POLL_SIZE, 100000) > 0) {
            printf("called poll\n");
            if (poll_set[0].revents & POLLIN) {
                printf("read data\n");
                read_msg = NULL;
                msg_len = client_read_command(poll_set[0].fd, &read_msg);
                if (msg_len >= 10 && (comp_result = strncasecmp(read_msg, COMMAND, 7) == 0)) {
                    if (read_msg[msg_len -1] == '\n') {
                        printf("End of msg is new line\n");
                        read_msg[msg_len -1] = '\0';
                    }
                    cm_num = read_msg[8];
                    switch(cm_num) {
                        case '1':
                            printf("cm_num:%c\n", cm_num);
                            if (msg_len == 10) {
                                printf("command 1\n");
                                client_request_all_message(poll_set[1].fd);
                            } else {
                                printf("Please recommand\n");
                            }
                            break;
                        case '3':
                            printf("cm_num%c\n", cm_num);
                            if (msg_len >= 11 && (read_msg[9] == ' ')) {
                                printf("command 3\n");
                                client_send_message(poll_set[1].fd, read_msg + 10);
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
                printf("end read data\n");
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
                int op_code_size;
                int fd = poll_set[1].fd;
                int n_byte;
                op_code_size = sizeof(op_code);
                n_byte = read(poll_set[1].fd, &op_code, op_code_size);
                printf("client_fd = %d, op_cde=%2x n_byte=%d\n", poll_set[1].fd, op_code, n_byte);
                switch (op_code) {
                    case OP_CODE_2:
                        msg_list = client_receive_all_messages(msg_list, fd);
                        break;
                    case OP_CODE_4:
                        msg_list = client_receive_message(msg_list, fd);
                        break;
                    default:
                        printf("Wrong opcode:%2x\n", op_code);
                }
            }
        }
    }
    return 0;
}
