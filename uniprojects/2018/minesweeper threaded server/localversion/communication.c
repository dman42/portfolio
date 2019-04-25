//
// Created by cousi on 17/10/2018.
//

#include <memory.h>
#include <sys/socket.h>
#include <stdint.h>

#include "communication.h"

void set_data_type(int n, char packet[][n], char *packet_type) {
    strncpy(packet[0], packet_type, sizeof(&packet_type));
}

int str_is_equal(char *str_1, char *str_2) {
    return strcmp(str_1, str_2) == 0;
}

void send_uint16(int socket, uint16_t *number) {

    size_t total_bytes_sent = 0;
    while (total_bytes_sent != sizeof(uint16_t)) {
        ssize_t bytes_sent = send(socket, number + total_bytes_sent, sizeof(uint16_t) - total_bytes_sent, 0);
        total_bytes_sent += bytes_sent;
    }

}

void recv_uint16(int socket, uint16_t *number) {

    ssize_t total_bytes_recv = 0;

    while (total_bytes_recv != sizeof(uint16_t)) {

        ssize_t bytes_recv = recv(socket, number, sizeof(uint16_t), 0);
        total_bytes_recv += bytes_recv;
    }
}

void send_char_array(int socket, char array[], size_t array_size) {

    size_t total_bytes_sent = 0;
    while (total_bytes_sent != array_size) {
        ssize_t bytes_sent = send(socket, array + total_bytes_sent, array_size - total_bytes_sent, 0);
        total_bytes_sent += bytes_sent;
    }

}

void recv_char_array(int socket, char array[], size_t array_size) {

    ssize_t total_bytes_recv = 0;

    while (total_bytes_recv != array_size) {
        ssize_t bytes_recv = recv(socket, array, array_size, 0);
        total_bytes_recv += bytes_recv;
    }
}