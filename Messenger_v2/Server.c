struct _Server {
    int fd;
    DList *client_list;
    Looper *looper;
    int file_fd;
    File_DB *file_db;
}

struct _Client {
    int fd;
    int file_offset;
    int read_state;
}

static void free_client(Client *client) {
    free(client);
}

static int match_client(Client *client, int *fd) {
    printf("client data:%d\n", client->fd);
    if (client->fd == *(fd)) {
        return 1;
    } else {
        return 0;
    }
}

static Client* find_client(Server *server, int fd) {
    Client *client = NULL;
    client = (Client*) d_list_find_data(server->client_list, match_client, &fd);

    if (client == NULL) {
        printf("Can't find this data:%d\n", *((int*) client_data));
        return NULL;
    }
    return client;
}

static void add_client(Server *server, int fd) {
    Client *client;
    DList *list;

    list = server->client_list;
    client = (Client*) malloc(sizeof(Client));

    if (!client) {
        printf("Failed to make Client\n");
        return;
    }

    client->fd = fd;
    client->file_offset = 0;
    client->read_state = 0;

    server->client_list = d_list_append(list, client);
}

static void remove_client(Server *server, int fd) {
    Client *remove_client;
    DList *list;

    list = server->client_list;
    remove_client = find_client(server, fd);

    if (!remove_client) {
        printf("There is no client to remove\n");
    }

    server->client_list = d_list_remove_nth_with_data(list, remove_client, free_client);
}

static void handle_accept_event(Server *server) {
    int client_fd;
    int state;

    client_fd = accept(server->fd, NULL, NULL);
    if (client_fd < 0) {
        printf("Failed to accept socket\n");
        return; 
    }   

    add_client(server, client_fd);
    return;
}

static void handle_disconnect_event(Server *server int fd) {

    if (remove_fd < 0) {
        printf("Can't free client\n");
        return;
    }

    if (fd == server->fd) {
        set_looper_state(server->looper, -1);
    } else {
        remove_client(server, fd);
    }
}

static void handle_req_event(Server *server, int fd) {

}

static void handle_events(Server *server, int fd, int revents) {
    if (revents == POLLHUP) {
         handle_disconnect_event(server, fd);
    } else if (revents == POLLIN) {
        if (fd == server->fd) {
            handle_accept_event(server);
        } else {
            handle_req_event(server, fd);
        }
    }
}

Server* new_server(Looper *looper) {
    Server *server;
    struct sockaddr_un addr;
    int mesg_store_fd;
    int server_fd;
    
    if (!looper) {
        printf("Looper is empty\n");
        return NULL;
    }

    server = (Server*) malloc(sizeof(Server));

    if (!server) {
        printf("Failed to make Server\n");
        return;
    }
    
    mesg_store_fd = open(MESSAGE_PATH, O_RDWR | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);

    if (mesg_store_fd == -1) {
        printf("Failed to make message store\n");
        return;
    }   

    unlink(SOCKET_NAME);
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM,0)) == -1) {
        perror("socket error");
        exit(-1);
    }

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

    server->looper = looper;
    server->fd = server_fd;
    server->client_list = NULL;
    server->file_fd = 0;
    server->file_db  = NULL;

    set_state(server->looper, 1)

    add_watcher(server->looper, handle_events, server, POLLIN);

    return server;
}

void remove_server(Server *server) {
    DList *list;

    if (!server) {
        printf("There is no pointer to Server\n");
        return;
    }

    list = server->client_list;
    d_list_free(list, free_client);

    remove_Looper(server->looper);
    remove_File_DB(server->file_db);

    free(server);
}
