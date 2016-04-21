#ifndef __PACKET_H__
#define __PACKET_H__

#define OP_CODE_MEMORY_SIZE      2
#define MSG_NUM_MEMORY_SIZE      4
#define TIME_MEMORY_SIZE         8
#define STR_LENGTH_MEMORY_SIZE   4

#define REQ_ALL_MSG             0x01
#define RES_ALL_MSG             0x02
#define SND_MSG                 0x03
#define RCV_MSG                 0x04
#define REQ_LAST_MSG_FR_FS      0x05
#define RES_LAST_MSG_FR_FS      0x06
#define REQ_LAST_MSG_FR_LS      0x07
#define RES_LAST_MSG_FR_LS      0x08

void print_data(void *buf, int size);
void print_packet(char *buf, int size);
short get_op_code_with_fd(int fd);
char* make_packet_space(int size);
int write_op_code_to_packet(char *packet, short op_code);
int write_msg_num_to_packet(char *packet, int msg_num);
int write_time_to_packet(char *packet, long int time);
int write_strlen_to_packet(char *packet, int strlen);
int write_str_to_packet(char *packet, char *str, int strlen);

#endif
