void main() {
    Looper *looper;
    Server *server;

    looper = new_looper();
    server = new_server(looper);
}
