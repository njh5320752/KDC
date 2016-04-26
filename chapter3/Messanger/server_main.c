#include <stdio.h>
#include "server_controler.h"
#include "server_looper.h"

int main(int argc, char *argv[]) {
    Looper *looper;
    Server *server;

    server = NULL;
    looper = NULL;

    server = server_new();
    if (!server) {
        printf("Can't add message and client\n");
        return -1;
    }

    looper = looper_new();
    if (!looper) {
        printf("Can't make looper\n");
    }

    add_accept_event_call_back(looper, server_handle_accept_event);
    add_remove_client_call_back(looper, server_remove_client);
    add_handle_event_call_back(looper, server_handle_event);
    run(looper, server);

    return 0;
}
