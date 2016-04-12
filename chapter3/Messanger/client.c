#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>
#include "socket.h"

#define POLL_SIZE 2

void print_packet(char *buf, int size) {
    int i;
    for (i = 0; i < size; i++) {
        printf("%02X ", buf[i]);
    }
    printf("\n");
}

int main(void) {
    struct sockaddr_un addr;
    struct pollfd poll_set[POLL_SIZE];
    char buf[2];
    int client_fd,rc;

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

    int test;
    void *send;
    int n = 0;
    int dest = 0;
    while (1) {
        if (poll(poll_set, POLL_SIZE, 100000) > 0) {
            printf("called poll\n");
            if (poll_set[0].revents & POLLIN) {
                printf("read data\n");
                send = NULL;
                while ((rc = read(poll_set[0].fd, buf, sizeof(buf))) > 0) {
                    printf("buf:%s rc:%d\n", buf, rc);
                    dest = n;
                    n = n + rc;
                    printf("n:%d\n", n);
                    send = realloc(send, sizeof(char)*n + sizeof(int));
                    printf("size of send:%zu\n", sizeof(send));
                    strncpy(send + sizeof(int) + dest, buf, n);
                    if (buf[rc -1] != '\n') {
                        printf("buf\n");
                        continue;
                    }
                    if (n == 1) {
                        printf("There is only new line data\n");
                        break;
                    }
                    n = n - 1;
                    *((int*)(send)) = n;
                    print_packet(send, n + 4);
                    printf("send:%s", ((char*)(send + 4)));
                    test = write(client_fd, send, n + 4);
                    printf("test:%d\n",test);
                    break;
                }
                n = 0;
                dest = 0;
                rc = 0;
                free(send);
                printf("end read data\n");
            }

            if (poll_set[1].revents & POLLIN) {
                int len;
                int rc;
                rc = read(poll_set[1].fd, &len, 4);
                printf("len = %d, rc=%d\n", len, rc);
                rc = read(poll_set[1].fd, buf, len);
                print_packet(buf, n);
                printf("socket=%d rc=%d, read buf:%s\n", poll_set[1].fd, rc, buf);
            }

            if (poll_set[1].revents & POLLHUP){
                printf("poll_hup\n");
                close(poll_set[1].fd);
                printf("Server is not connected\n");
                exit(-1);
            }
        }
    }
    return 0;
}
