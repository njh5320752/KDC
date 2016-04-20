#ifndef __MESSAGE_H__
#define __MESSAGE_H__

typedef struct _Message Message;

Message* new_message(long int time, int strlen, char *str);
long int get_time_with_fd(int fd);
int get_strlen_with_fd(int fd);
char* get_str_with_fd(int fd, char *str, int strlen);
long int get_time_with_msg(Message* message);
int get_strlen_with_msg(Message *message);
char* get_str_with_msg(Message *message);

#endif
