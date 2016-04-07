#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include "DBLinkedList.h"

char *socket_path = "";

int main(int argc, char *argv[]) {
    struct sockaddr_un addr;
    int servfd,cl,rc;
    int option = 1;
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
        close(servfd);
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

    close(servfd);
}
