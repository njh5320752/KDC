#ifndef __MSG_MANAGER_H__
#define __MSG_MANAGER_H__

void print_packet(char *buf, int size);
char* pack_msg(int fd);
int pack_time_data(int fd, char **msg, int time_size);
int pack_str_len_data(int fd, char **msg, int time_size);
int pack_str_data(int fd, char **msg, int str_len);
long int get_time_with_msg(char *msg);
int get_str_len_with_msg(char *msg);
char* get_str_with_msg(char *msg);

#endif
