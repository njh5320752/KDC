#ifndef __CLINET_MESSAGE_MENAGER_H__
#define __CLIENT_MESSAGE_MENAGER_H__

#define COMMAND "command"

void print_packet(char *buf, int size);
void client_request_all_message(int fd);
void client_send_message(int fd, char *read_msg);
int client_read_command(int fd, char **read_msg);

#endif
