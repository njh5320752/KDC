#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "socket.h"

#define POLL_SIZE 2

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
    int *send;
    while (1) {
        if (poll(poll_set, POLL_SIZE, 100000) > 0) {
            if (poll_set[0].revents & POLLIN) {
                while ((rc=read(poll_set[0].revents, buf, sizeof(buf))) > 0) {
                    send = (int*)malloc(sizeof(int) * (rc + 1));

                    if (write(fd, buf, rc) != rc) {
                        if (rc > 0) {
                            fprintf(stderr, "partial write");
                        } else {
                            perror("write error");
                            exit(-1);
                        }
                    }
                }
            }

            if (poll_set[1].revents & POLLIN) {

            }

        }
    }
    return 0;
}
