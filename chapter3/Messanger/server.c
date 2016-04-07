#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <poll.h>
#include "DBLinkedList.h"

#define POLL_SIZE 32

char *socket_path = "";

int main(int argc, char *argv[]) {
    struct sockaddr_un addr;
    int servfd,cl;
    char buf[100];
    int option = 1;
    int numfds = 0;
    struct pollfd poll_set[POLL_SIZE];

    if (argc > 1) {
        socket_path = argv[1];
    }

    if ((servfd = socket(AF_UNIX, SOCK_STREAM,0)) == -1) {
        perror("socket error");
        exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);

    if (setsockopt(servfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) {
        perror("setsockopt failed");
        exit(-1);
    }

    if (bind(servfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind error");
        exit(-1);
    }

   if (listen(servfd, 0) == -1) {
        perror("listen error");
        exit(-1);
    }

    poll_set[0].fd = server_sockfd;
    poll_set[0].event = POLLIN;
    numfds++;
    
    while (1) {
        printf("Watting for client (%d total)... \n", numfds);
        poll(poll_set, numfds, 1000);
        for (fd_index = 0; fd_index < numfds; fd_index++) {
            if (poll_set[fd_index].revents & POLLIN) {
                cl = accept(servfd, NULL, NULL);
                poll_set[numfds].fd = cl;
                poll_set[numfds].events = POLL;
                numfds++;
                printf("Adding client on fd %d\n", cl);
            }
        }
    }
    return 0;
}
