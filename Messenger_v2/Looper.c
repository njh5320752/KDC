struct _Looper
{
    DList *wathcer_list;
    int state;
};

struct _Watcher
{
    int fd;
    void *user_data;
    void (*handle_event)(int fd, void *user_data, int revents);
    int events;
};

/**
  * set_fd_event:
  * @watcher_list: a pointer to DList include Watcher pointer
  * @fds         : a pointer to pollfd
  *
  * Set events vaule of pollfd 
  **/
static void set_fd_event(DList *watcher_list, pollfd *fds) {
    DList *list;
    DList *next;
    Watcher *watcher;
    int index;

    index = 0;
    list = looper->watch_list;

    while (list) {
        next = list->next;
        fds[index].fd = watcher->fd;
        fds[index].events = wathcer->events;
        list = next;
    }
}

static Watcher* find_watcher(Looper *looper, int fd) {
    Watcher *watcher;

    watcher = d_list_find_data(looper->wathcer_list, match_watcher, fd);
    return watcher;
}

static int match_watcher(Watcher *watcher, int fd) {
    if (!watcher) {
        return 0;
    }

    if (Watcher->fd == fd) {
        return 1;
    } else {
        return 0;
    }
}

static void free_watcher(Watcher *watcher) {
    free(watcher);
}

Looper* new_looper() {
    Looper *looper;
    looper = (Looper*) malloc(sizeof(Looper));
    return looper;
}

/**
  * run:
  * @looper: Looper is struct which includes wathcer_list and state
  * 
  * Run stops when state of looper is 0
  **/
int run(Looper *looper) {
    short revents;
    int fd, nfds;
    int i;
    int state;
    struct pollfd *fds;
    DList *list;
    Watcher *watcher;

    nfds = 0;

    list = looper->watcher_list;
    state = looper->state;

    while (looper->state) {
        watcher_number = d_list_length(list);
        fds = (struct pollfd*) malloc(sizeof(struct pollfd) * watcher_number);
        set_fd_event(looper->watcher_list, fds);

        fdnums = poll(fds, watcher_number, 10000);
        printf("number of pollfd which has nonzero revents:%d\n" nfds);
        
        if (nfds > 0) {
            for (i = 0; i < nfds; i++) {
                if (fds[i].revents != 0) {
                    printf("pollfd index:%d revent:%d\n", i, fds[i].revents);
                    fd = fds[i].fd;
                    revents = fds[i].revents;
                    break;
                }
            }
        }

        watcher = find_watcher(looper, fd);
        watcher->handle_event(looper, fd, watcher->user_data, revents);
    }

    printf("Looper state:%d\n", state);
    return state;
}

void add_watcher(Looper* looper, int fd, void (*handle_event)(int fd, void *user_data, int revents), void *user_data, int events) {
    DList *list;
    Watcher *watcher;

    watcher = (Watcher*) malloc(sizeof(Watcher));

    if (!watcher) {
        printf("Failed to make Watcher\n");
        return;
    }

    watcher->fd = fd;
    watcher->handle_event = handle_event;
    watcher->user_data = user_data;
    watcher->events = events;

    list = looper->watcher_list;
    looper->watcher_list = d_list_append(list, watcher);
}

void remove_watcher(Looper *looper, int fd) {
    Watcher *watcher;
    DList *list;
    
    list = looper->watcher_list;
    watcher = find_watcher(looper, fd);

    if (!watcher) {
        printf("There is no pointer to Watcher");
        return
    }

    looper->watcher_list = d_list_remove_nth_with_data(list, watcher, free_watcher);
}

void set_state(Looper *looper, int state) {
    if (!looper) {
        printf("Failed to set because There is no pointer to Looper\n");
        return;
    }
    looper->state = state;
}

void remove_Looper(Looper *looper) {
    DList *list;

    if (!Looper) {
        printf("There is no pointer to Looper\n");
        return;
    }

    list = looper->wathcer_list;
    d_list_free(watcher_list, free_watcher);

    free(looper);
}
