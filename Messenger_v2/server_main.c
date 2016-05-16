#include "looper.h"
#include "server.h"

int main() {
    Looper *looper;
    Server *server;
    int state;

    looper = new_looper();
    server = new_server(looper);

    state = run(looper);

    return 0;
}
