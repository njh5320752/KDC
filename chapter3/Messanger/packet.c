#include "packet.h"
#include "message.h"

char* make_req_all_msg_packet(short op_code) {
    char *req_all_msg_packet;
    req_all_msg_packet = make_packet_space(OP_CODE_MEMORY_SIZE);

    if (!req_all_msg_packet) {
        printf("Failed to make req all msg packet memory space\n");
    }
    req_all_msg_packet = write_op_code_to_packet(req_all_msg_packet, op_code);

    return req_all_msg_packet;
}

char* make_res_all_msg_packet_with_fd(int fd, short op_code) {

}

char* make_snd_msg_and_rcv_msg+packet_with_fd(int fd, short op_code) {

}

char* make_req_last_msg_packet_with_fd(int fd, short op_code) {

}

short get_op_code_with_fd(int fd) {
    short op_code;
    int n_byte;
    n_byte = read(fd, &op_code, OP_CODE_MEMORY_SIZE);
    if (n_byte < OP_CODE_MEMORY_SIZE) {
        printf("Failed to read op_code:%02x\n", op_code);
    }
    return op_code;
}

char* make_packet_space(int size) {
    char *packet_space;
    packet_space = (char*) malloc(size);

    if (!packet_space) {
        printf("Failed to make space\n");
        return NULL;
    }

    return packet_space;
}

int write_op_code_to_packet(char *packet, short op_code) {
    packet = memcpy(packet, &op_code, OP_CODE_MEMORY_SIZE);
    if (!packet) {
        printf("Fail to write op_code\n");
        return 0;
    }
    return 1;
}
