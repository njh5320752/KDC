#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>

char *socket_path = "";

int main(int argc, char *argv[]) {
    struct sockaddr_un addr;
    char buf[100];
    int servfd,cl,rc;
    
    if (argc > 1) {
        socket_path = argv[1];
    }

    if ((fd = socket(AF_UNIX, SOCK_STREA,0)) == -1) {
        perror("socket error");
        exit(-1);
    }

    if (fd < 0) {
        error(1, errno, "Unix Domain Socket Error()");
    }
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "socket", sizeof(addr.sun_path)-1);
    bind(fd, (struct sockaddr*)&addr, sizeof(addr));
}
