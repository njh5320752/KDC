#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "packet.h"
#include "client_controler.h"

void client_looper(struct pollfd *poll_set, int poll_size) {
    Client_Msg *client_msg;
    char *read_msg;
    int msg_len;
    int comp_result;
    char cm_num;

    client_msg = new_Client_Msg();

    while (1) {
        if (poll(poll_set, poll_size, 100000) > 0) {
            printf("called poll\n");
            if (poll_set[0].revents & POLLIN) {
                printf("read data\n");
                read_msg = NULL;
                msg_len = client_read_command(poll_set[0].fd, &read_msg);
                if (msg_len >= 10 && (comp_result = strncasecmp(read_msg, COMMAND, 8) == 0)) {
                    if (read_msg[msg_len -1] == '\n') {
                        printf("End of msg is new line\n");
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
