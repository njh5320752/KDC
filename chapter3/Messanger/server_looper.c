struct Looper {
    struct pollfd poll_set[POLL_SIZE];
    int numfds;
    Looper* *accept_client(Server *looper, int fd);
    int *remove_client(Server *server, int remove_fd);
    void response_client_event();
}

Looper* looper_new() {
    Looper *new_looper = NULL;
    new_looper = (Looper*) malloc(sizeof(Looper));
    looper->poll_set = NULL;
    looper->numfds = 0;

    return new_looper;
}

void add_accept_event_call_back(Looper* looper, int *accept_client_from_server(Server *server, int fd)) {
    looper->accept_client = accept_client_from_server;
}

void add_remove_client_call_back(Looper* looper, int *server_remove_client(Server *server, int remove_fd)) {
    looper->remove_client = server_remove_client;
}


static void add_and_set_fd_event(Looper *looper, int client_fd) {
    int number;
    struct pollfd *pollfd_set;
    if (!looper) {
        printf("Can't set event\n");
    }
    
    pollfd_set = looper->poll_set;
    number = looper->numfds;
    if (number < 0) {
        printf("Can't set and add fd event\n");
    }
    
    pollfd_set[number].fd = client_fd;
    pollfd_set[number].events = POLLIN;
    pollfd_set[number].revents = 0;

    number++;
    looper->numfds = number;
    looper->poll_set = pollfd_set;
}

static void remove(Looper *looper, int fd, Server *server) {
    int remove_fd;
    int state;
    struct pollfd *pollfd_set;
 
    if (!looper) {
        printf("Can't set event\n");
    }

    pollfd_set = looper->poll_set;
    remove_fd = poll_set[fd_index].fd;
    state = server_remove_client(server, remove_fd);
    if (!state > 0) {
        printf("Failed to remove client\n");
    } else {
        numfds--;
        poll_set[fd_index] = poll_set[numfds];
    }

    looper->poll_set = pollfd_set;
}

void run(Looper *looper, Server *server, int server_fd, int msg_fd) {
    int client_fd;
    int number;
    int fd_index;
    struct pollfd *pollfd_set;
    
    pollfd_set = looper->poll_set;
    number = looper->numfds;

    pollfd_set[0].fd = server_fd;
    pollfd_set[0].events = POLLIN;
    number++;

    looper->numfds = number;
    looper->poll_set = pollfd_set;

    while(1) {
        if (poll(looper->poll_set, looper->numfds, 10000) > 0) {
            pollfd_set = looper->poll_set;
            if (pollfd_set[0].revents & POLLIN) {
                client_fd = looper->add_client(server ,poll_set[0].fd);
                add_and_set_fd_event(client_fd);
            }

            for (fd_index = 1; fd_index < numfds; fd_index++) {
                if (poll_set[fd_index].revents & POLLHUP) {
                    remove(looper, server);
                } else if (poll_set[fd_index].revents & POLLIN) {
                    looper->
                }
            }
        }
    }
}
