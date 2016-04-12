#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>
#include "DBLinkedList.h"
#include "socket.h"

#define POLL_SIZE 32

struct _Client
{
    int fd;
};

typedef struct _Client Client;

void free_client(void *client);
int find_data(void *data);
void send_message(struct pollfd *poll_set, int got_message_client_fd, int numfds, int m_len, char* buf);

int tmp_fd;

void print_packet(char *buf, int size) {
    int i;
    for (i = 0; i < size; i++) {
        printf("%02X ", buf[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    struct sockaddr_un addr;
    char buf[2];
    Client *new_client;
    Client *tmp;
    int server_fd, client_fd;
    int numfds = 0;
    struct pollfd poll_set[POLL_SIZE];
    int fd_index;
    DList *list;
    int length;
    int rc;
    list = NULL;

    unlink(SOCKET_NAME);
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM,0)) == -1) {
        perror("socket error");
        close(server_fd);
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

    poll_set[0].fd = server_fd;
    poll_set[0].events = POLLIN;
    numfds++;

    while (1) {
        if (poll(poll_set, numfds, 100000) > 0) {
            if (poll_set[0].revents & POLLIN) {
                client_fd = accept(server_fd, NULL, NULL);
                new_client = (Client*) malloc(sizeof(Client));
                new_client->fd = client_fd;
                poll_set[numfds].fd = client_fd;
                poll_set[numfds].events = POLLIN;
                poll_set[numfds].revents = 0;
                numfds++;
                list = d_list_append(list, (void*)new_client);
                printf("Adding client on fd %d\n", client_fd);
                length = d_list_length(list);
                printf("client number:%d\n", length);

            }
            for (fd_index = 1; fd_index < numfds; fd_index++) {
                if (poll_set[fd_index].revents & POLLHUP) {
                    tmp_fd = poll_set[fd_index].fd;
                    tmp = (Client *) d_list_find_data(list, find_data);
                    list = d_list_remove_nth_with_data(list, tmp, free_client);
                    printf("poll_set[fd_index]:%d\n", poll_set[fd_index].fd);
                    numfds--;
                    poll_set[fd_index] = poll_set[numfds];
                } else if (poll_set[fd_index].revents & POLLIN) {
                    int len;
                    int tmp_len;
                    char *send = NULL;
                    int dest = 0;
                    int n = 0;
                    rc = read(poll_set[fd_index].fd, &len, 4);
                    printf("len = %d, rc=%d\n", len, rc);
                    rc = 0;
                    tmp_len = len;
                    while((rc = read(poll_set[fd_index].fd, buf, sizeof(buf))) > 0) {
                        dest = n;
                        n = n + rc;
                        printf("tmp_len = %d, rc=%d n=%d\n", tmp_len, rc, n);
                        tmp_len = tmp_len - rc;
                        printf("size of char:%zu\n", sizeof(char));
                        send = (char*)realloc(send, sizeof(char)*n);
                        printf("size of send:%zu\n", sizeof(send));
                        strncpy(send + dest, buf, n);
                        if (tmp_len == 0) {
                            printf("read all message\n");
                            break;
                        }
                    }
                    printf("size of buf: %zu\n", sizeof(send));
                    print_packet(send, sizeof(send));
                    send_message(poll_set, poll_set[fd_index].fd, numfds, len, send);
                    printf("called pollin numfds:%d\n", numfds);
                }
            }
        }
    }
    return 0;
}

void free_client(void *client) {
    Client *remove = (Client*)client;
    free(remove);
}

int find_data(void *data) {
    Client *client = (Client *)data;
    if (client->fd == tmp_fd) {
        return 1;
    } else {
        return 0;
    }
}

void send_message(struct pollfd *poll_set, int got_message_client_fd, int numfds, int m_len, char* buf) {
    int i;
    int write_num;
    void *send;
    send = malloc(sizeof(char)*(m_len) + 4);
    *((int*)(send)) = m_len;
    strncpy(send + 4, buf, m_len);
    printf("send:%s", ((char*)(send + 4)));

    for(i = 1; i < numfds; i++) {
        if (poll_set[i].fd == got_message_client_fd) {
            continue;
        }
        write_num = write(poll_set[i].fd, send, m_len);
        printf("write_num:%d poll_set[i].fd:%d write buf:%s", write_num, poll_set[i].fd, buf);
    }
    free(send);
}
