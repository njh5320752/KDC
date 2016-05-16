#include "looper.h"
#include "client.h"

int main() {
    Looper *looper;
    Client *client;
    int state;

    looper = new_looper();
    client = new_client(looper);

    state = run(looper);

    return 0;
}
