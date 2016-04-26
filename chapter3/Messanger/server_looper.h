#ifndef __SERVER_LOOPER_H__
#define __SERVER_LOOPER_H__

#define POLL_SIZE 30

typedef struct _Looper Looper;
Looper* looper_new();
void add_accept_event_call_back(Looper* looper, int (*server_handle_accept_event)(Server *server, int fd));
void add_remove_client_call_back(Looper* looper, int (*server_remove_client)(Server *server, int remove_fd));
void add_handle_event_call_back(Looper* looper, void (*server_handle_event)(Server *server, int client_fd, int msg_fd));
void run(Looper *looper, Server *server);

#endif
