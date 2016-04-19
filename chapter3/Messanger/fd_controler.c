#include "fd_controler.h"

int fd_controler_set_event(struct pollfd *poll_set, int fd, int index) {

    if (poll_set == NULL || index < 0) {
        printf("Can't set file descriptor event\n");
        return 0;
    }

    poll_set[index].fd = client_fd;
    poll_set[index].events = POLLIN;
    poll_set[index].revents = 0;
    return 1;
}
