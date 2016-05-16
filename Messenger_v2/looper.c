#include <poll.h>
#include <stdio.h>
#include <stdlib.h>

#include "DBLinkedList.h"
#include "looper.h"

struct _Looper
{
    DList *watcher_list;
    int state;
};

struct _Watcher
{
    int fd;
    void *user_data;
    void (*handle_events)(int fd, void *user_data, int revents);
    int events;
};

/**
  * set_fd_event:
  * @looper: a pointer to Looper which includes watch_list
  * @fds         : a pointer to pollfd
  *
  * Set events vaule of pollfd 
  **/
static void set_fd_event(Looper *looper, struct pollfd *fds) {
    DList *list;
    DList *next;
    Watcher *watcher;
    int index;

    index = 0;
    list = looper->watcher_list;

    while (list) {
        next = d_list_next(list);
        watcher = d_list_get_data(list);
        
        if (!watcher) {
            printf("There is no a pointer to watcher\n");
            break;
        }

        fds[index].fd = watcher->fd;
        fds[index].events = watcher->events;
        list = next;
        index++;
    }
}

static int match_watcher(void *data, void *client_data) {
    Watcher *watcher = (Watcher*) data;
    int fd = *((int*) client_data);

    if (!watcher) {
        printf("There is no a pointer to Watcher\n");
        return 0;
    }

    if (watcher->fd == fd) {
        return 1;
    } else {
        return 0;
    }
}

static Watcher* find_watcher(Looper *looper, int fd) {
    Watcher *watcher;

    if (!looper) {
        printf("There is no a pointer to Looper\n");
        return NULL;
    }

    watcher = d_list_find_data(looper->watcher_list, match_watcher, &fd);
    return watcher;
}

static void destroy_watcher(void *data) {
    Watcher *watcher = (Watcher*) data;
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
    int n_watcher;
    struct pollfd *fds;
    DList *list;
    Watcher *watcher;

    nfds = 0;

    if (!looper) {
        printf("There is no a pointer to Looper\n");
        return 0;
    }

    list = looper->watcher_list;
    state = looper->state;

    while (looper->state) {
        n_watcher = d_list_length(list);
        if (!n_watcher) {
            printf("There is no watcher\n");
            return 0;
        }

        fds = (struct pollfd*) malloc(sizeof(struct pollfd) * n_watcher);
        set_fd_event(looper, fds);

        nfds = poll(fds, n_watcher, 10000);
        printf("number of pollfd which has nonzero revents:%d\n", nfds);
        
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
        watcher->handle_events(fd, watcher->user_data, revents);
    }

    printf("Looper state:%d\n", state);
    return state;
}

void add_watcher(Looper* looper, int fd, void (*handle_events)(int fd, void *user_data, int revents), void *user_data, int events) {
    DList *list;
    Watcher *watcher;

    watcher = (Watcher*) malloc(sizeof(Watcher));

    if (!watcher) {
        printf("Failed to make Watcher\n");
        return;
    }

    watcher->fd = fd;
    watcher->handle_events = handle_events;
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
        return;
    }

    looper->watcher_list = d_list_remove_nth_with_data(list, watcher, destroy_watcher);
}

void set_state(Looper *looper, int state) {
    if (!looper) {
        printf("Failed to set because There is no pointer to Looper\n");
        return;
    }
    looper->state = state;
}

void destroy_Looper(Looper *looper) {
    DList *list;

    if (!looper) {
        printf("There is no pointer to Looper\n");
        return;
    }

    list = looper->watcher_list;
    d_list_free(list, destroy_watcher);

    free(looper);
}
