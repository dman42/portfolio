//
// Created by cousi on 4/10/2018.
//

#include <stdlib.h>

#define SIZE_OF_ARRAY(x)  (sizeof(x) / sizeof((x)[0]))

#ifndef CAB403_COMMUNICATION_H
#define CAB403_COMMUNICATION_H

#define DATA_TYPE_LOGIN 1
#define DATA_TYPE_MENU 3

#define CHOICE_REVEAL_TILE 11
#define CHOICE_PLACE_FLAG 22
#define CHOICE_QUIT_GAME 33

#define MINE_HIT 66
#define MINE_FLAGGED 77
#define MINE_DOESNT_EXIST 88
#define GAME_WON 99

#define SHOW_LEADERBOARD 90

#define ERROR_TILE_ALREADY_REVEALED 44
#define ERROR_TILE_ALREADY_FLAGGED 55

#define ERROR_MSG_LENGTH 50

#define USERNAME_CHAR_LENGTH 10
#define PASSWORD_CHAR_LENGTH 10

#define LEADERBOARD_SIZE 10
#define SIZE_OF_MAP 300


void set_data_type(int n, char packet[][n], char *packet_type);

int str_is_equal(char* str_1, char * str_2);

void send_uint16(int socket, uint16_t *number);

void recv_uint16(int socket, uint16_t *number);

void send_char_array(int socket, char array[], size_t array_size);

void recv_char_array(int socket, char array[], size_t array_size);

#endif //CAB403_COMMUNICATION_H
