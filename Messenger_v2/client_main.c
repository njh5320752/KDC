#include "looper.h"
#include "client.h"

int main() {
    Looper *looper;
    Client *client;
    int state;

    looper = new_looper();
    client = new_client(looper);

    state = run(looper);

    destroy_client(client);
    destroy_looper(looper);

    return 0;
}
