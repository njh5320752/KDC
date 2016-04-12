#include "MessageManager.h"

char* readMessage(int fd) {
    char buf[2];
    int rc;
    int dest = 0;
    int n = 0;
    while ((rc = read(fd, buf, sizeof(buf)) > 0)) {
        printf("buf:%s rc:%d\n", buf, rc);
        dest = n;
        n = n + rc;
        printf("n:%n\n", n);
        send = realloc(send, sizeof(char)*n + sizeof(int));
        printf("size of send:%zu\n", sizeof(send));
        strncpy(send + sizeof(int) + dest, buf, n);
        if (buf[rc - 1] != '\n') {
            printf("buf\n");
            continue;
        }
        if (n == 1) {
            printf("There is only new line data\n");
            break;
        }
        n = n - 1;
        *((int*)(send)) = n;
        printf_packet(send, n + 4);
        printf("send:%s", ((char*)(send + 4)));
        break;
    }
    return send;
}

