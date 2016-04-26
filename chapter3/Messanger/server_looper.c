#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include "socket.h" 
#include "server_controler.h"
#include "server_looper.h"

struct _Looper {
    struct pollfd pollfd_array[POLL_SIZE];
    int numfds;
    int (*handle_accept_event)(Server *server, int fd);
    int (*remove_client)(Server *server, int remove_fd);
    void (*handle_event)(Server *server, int client_fd, int msg_fd);
};

Looper* looper_new() {
    Looper *new_looper = NULL;
    new_looper = (Looper*) malloc(sizeof(Looper));
    if (!new_looper) {
        printf("Can't make looper data\n");
    }
    new_looper->numfds = 0;

    return new_looper;
}

void add_accept_event_call_back(Looper* looper, int (*server_handle_accept_event)(Server *server, int fd)) {
    if (!looper) {
        printf("Can't add accept_event_call_back\n");
        return;
    }
    looper->handle_accept_event = server_handle_accept_event;
}

void add_remove_client_call_back(Looper* looper, int (*server_remove_client)(Server *server, int remove_fd)) {
    if (!looper) {
        printf("Can't add remove_client_call_back\n");
        return;
    } 
    looper->remove_client = server_remove_client;
}

void add_handle_event_call_back(Looper* looper, void (*server_handle_event)(Server *server, int client_fd, int msg_fd)) {
    if (!looper) {
        printf("Can't add handle_event_call_back\n");
        return;
    }
    looper->handle_event = server_handle_event;
}

static void add_and_set_fd_event(Looper *looper, int client_fd) {
    int fd_number;
    struct pollfd *pollfd_pointer;
    if (!looper) {
        printf("Can't set event\n");
    }

    pollfd_pointer = looper->pollfd_array;
    fd_number = looper->numfds;
    if (fd_number < 0) {
        printf("Can't set and add fd event\n");
    }

    pollfd_pointer[fd_number].fd = client_fd;
    pollfd_pointer[fd_number].events = POLLIN;
    pollfd_pointer[fd_number].revents = 0;

    fd_number++;
    looper->numfds = fd_number;
}

static void remove_client_fd(Looper *looper, Server *server, int fd_index) {
    int remove_fd;
    int state;
    struct pollfd *pollfd_pointer;
    int fd_number;

    fd_number = looper->numfds;

    if (!looper || fd_number < 0) {
        printf("Can't remove event\n");
    }

    pollfd_pointer = looper->pollfd_array;
    remove_fd = pollfd_pointer[fd_index].fd;
    state = looper->remove_client(server, remove_fd);

    if (state == -1) {
        printf("Failed to remove client\n");
    } else {
        fd_number--;
        pollfd_pointer[fd_index] = pollfd_pointer[fd_number];
    }

    looper->numfds = fd_number;
}

void run(Looper *looper, Server *server) {
    struct pollfd *pollfd_pointer;
    struct sockaddr_un addr;
    int server_fd, client_fd, msg_fd;
    int fd_number, fd_index;

    msg_fd = open(MESSAGE_PATH, O_RDWR | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);

    if (msg_fd == -1) {
        printf("Failed to make message file\n");
        return;
    }

    unlink(SOCKET_NAME);
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM,0)) == -1) {
        perror("socket error");
        exit(-1);
    }

    pollfd_pointer = looper->pollfd_array;
    fd_number = looper->numfds;

    pollfd_pointer[0].fd = server_fd;
    pollfd_pointer[0].events = POLLIN;
    fd_number++;

    looper->numfds = fd_number;

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path)-1);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind error");
        close(server_fd);
        exit(-1);
    }

    if (listen(server_fd, 0) == -1) {
        perror("listen error");
        close(server_fd);
        exit(-1);
    }

    while(1) {
        fd_number = looper->numfds;

        if (poll(pollfd_pointer, fd_number, 10000) > 0) {
            if (pollfd_pointer[0].revents & POLLIN) {
                client_fd = looper->handle_accept_event(server, pollfd_pointer[0].fd);
                printf("happend event pollin client_fd:%d\n", client_fd);
                add_and_set_fd_event(looper, client_fd);
            }

            for (fd_index = 1; fd_index < fd_number; fd_index++) {
                if (pollfd_pointer[fd_index].revents & POLLHUP) {
                    remove_client_fd(looper, server, fd_index);
                } else if (pollfd_pointer[fd_index].revents & POLLIN) {
                    looper->handle_event(server, pollfd_pointer[fd_index].fd, msg_fd);
                }
            }
        }
    }
}
