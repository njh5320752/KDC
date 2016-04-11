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
    char buf[100];
    int fd,rc;

    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        close(fd);
        exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path)-1);

    poll_set[0].fd = STDIN_FILENO;
    poll_set[0].events = POLLIN;

    poll_set[1].fd = fd;
    poll_set[1].events = POLLIN;

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1 ) {
        perror("connect error");
        close(fd);
        exit(-1);
    }
    void *send;
    int test;
    while (1) {
        if (poll(poll_set, POLL_SIZE, 100000) > 0) {
            printf("called poll\n");
            if (poll_set[0].revents & POLLIN) {
                rc = read(poll_set[0].fd, buf, sizeof(buf));
                printf("buf:%s rc:%d\n", buf, rc);
                send = malloc(sizeof(char)*(rc) + 4);
                *((int*)(send)) = rc;
                strncpy(send + 4, buf, rc);
                print_packet(send, rc + 4);
                printf("send:%s", ((char*)(send + 4)));
                test = write(fd, send, rc + 4);
                printf("test:%d\n",test);
                free(send);
            }

            if (poll_set[1].revents & POLLIN) {
                int len;
                int rc;
                rc = read(poll_set[1].fd, &len, 4);
                printf("len = %d, rc=%d\n", len, rc);
                rc = read(poll_set[1].fd, buf, len);
                printf("socket=%d rc=%d, read buf:%s\n", poll_set[1].fd, rc, buf);
            } else if (poll_set[1].revents & POLLHUP){
                printf("poll_hup\n");
            }
        }
    }
    return 0;
}
