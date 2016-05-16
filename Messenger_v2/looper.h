#ifndef __LOOPER_H__
#define __LOOPER_H__

typedef struct _Looper Looper;
typedef struct _Watcher Watcher;

Looper* new_looper();
int run(Looper *looper);
void add_watcher(Looper* looper, int fd, void (*handle_events)(int fd, void *user_data, int revents), void *user_data, int events);
void remove_watcher(Looper *looper, int fd);
void set_state(Looper *looper, int state);
void destroy_Looper(Looper *looper);

#endif
