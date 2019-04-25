#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include "communication.h"

#define BUF_SIZE 500 /* max number of bytes we can get at once */

#define NUM_TILES_X 9
#define NUM_TILES_Y 9
#define NUM_MINES 10


char *itoa(int val) {

    int base = 10;

    static char buf[32] = {0};

    int i = 30;

    for (; val && i; --i, val /= base)
        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i + 1];

}

void disp_gamestate(uint16_t mines, char *map) {
    printf("\nRemaining mines: %d\n\n", mines);
    printf("%s\n", map);
}

int process_move(int sockfd) {
    char input[30];
    uint16_t y, x;

    while (1) {
        printf("Enter tile coordinates: ");
        fgets(input, sizeof(input), stdin);

        x = toupper(input[0]) - 65;
        y = input[1] - 49;

        if (x < 0 || x >= NUM_TILES_X || y < 0 || y >= NUM_TILES_Y || strlen(input) > 3) {
            printf("Invalid entry, please try again.\n");
        } else {
            break;
        }
    }

    send_uint16(sockfd, &y);
    send_uint16(sockfd, &x);

    uint16_t response = 0;
    recv_uint16(sockfd, &response);

    printf("\n");
    switch (response) {
        case MINE_FLAGGED:
            printf("You found a mine!!\n");
            break;
        case MINE_DOESNT_EXIST:
            printf("WARNING: Mine not under tile. Tile not flagged.\n");
            break;
        case ERROR_TILE_ALREADY_FLAGGED:
            printf("Cannot flag an already flagged tile.\n");
            break;
        case ERROR_TILE_ALREADY_REVEALED:
            printf("Tile already revealed, please try again.\n");
            break;
        default:
            break;
    }

    return response;
}

void game(int sockfd) {

    bool game_in_progress = true;
    uint16_t mines = 0;
    char map[SIZE_OF_MAP];
    int response = -1;

    while (game_in_progress) {
        recv_uint16(sockfd, &mines);
        recv_char_array(sockfd, map, sizeof(map));
        disp_gamestate(mines, map);

        if (mines < 0) {
            game_in_progress = false;
        }

        printf("\nChoose an option:\n");
        printf("<R> Reveal Tile\n");
        printf("<P> Place Flag\n");
        printf("<Q> Quit Game\n");

        char input[30];
        char choice = 0;
        int invalid_input;

        do {
            printf("\nOption (R, P, Q): ");

            invalid_input = 0;
            fgets(input, sizeof(input), stdin);

            if (strlen(input) == 2) {
                choice = toupper(input[0]);

                switch (choice) {

                    case 'R': {
                        uint16_t choice_type = CHOICE_REVEAL_TILE;
                        send_uint16(sockfd, &choice_type);
                        response = process_move(sockfd);
                        break;
                    }

                    case 'P': {
                        uint16_t choice_type = CHOICE_PLACE_FLAG;
                        send_uint16(sockfd, &choice_type);
                        response = process_move(sockfd);
                        break;
                    }

                    case 'Q': {
                        uint16_t choice_type = CHOICE_QUIT_GAME;
                        send_uint16(sockfd, &choice_type);
                        printf("\nExiting game...");
                        game_in_progress = false;
                        break;
                    }

                    default: {
                        printf("\nInvalid option.\n");
                        invalid_input = 1;
                        break;
                    }
                }
            }
        } while (invalid_input);

        if (response == GAME_WON || response == MINE_HIT) {
            recv_uint16(sockfd, &mines);
            recv_char_array(sockfd, map, sizeof(map));
            disp_gamestate(mines, map);
            game_in_progress = false;
        }

        switch (response) {
            case GAME_WON: {
                uint16_t totalTime = 0;
                recv_uint16(sockfd, &totalTime);
                printf("Congratulations! You have located all the mines.\nYou won in %d seconds \n", totalTime);
                break;
            }
            case MINE_HIT: {
                printf("You hit a mine! Game Over!\n");
                break;
            }

            default:
                break;
        }
    }
}

void draw_line() {
    for (int i = 0; i < 30; i++) {
        printf("=");
    }

    printf("\n\n");
}

void show_leaderboard(int sockfd) {
    char leaderboard[1000] = {0};
    draw_line();
    recv_char_array(sockfd, leaderboard, sizeof(leaderboard));
    printf("%s\n\n", leaderboard);
    draw_line();
}

int login(int sockfd) {

    printf("\nWelcome to the online Minesweeper gaming system\n\n");
    printf("Please log in to your account\n");

    while(1) {
        uint16_t data_type = DATA_TYPE_LOGIN;
        send_uint16(sockfd, &data_type);

        char username[USERNAME_CHAR_LENGTH];
        char password[PASSWORD_CHAR_LENGTH];

        printf("Username: ");
        fgets(username, USERNAME_CHAR_LENGTH, stdin);
        printf("Password: ");
        fgets(password, PASSWORD_CHAR_LENGTH, stdin);

        // Remove \n from collected input
        username[strcspn(username, "\n")] = '\0';
        password[strcspn(password, "\n")] = '\0';

        send_char_array(sockfd, username, sizeof(username));
        send_char_array(sockfd, password, sizeof(password));

        uint16_t response;
        recv_uint16(sockfd, &response);

        if (response) {
            printf("\nLogin Successful!\n");
            return 1;
        } else {
            printf("\nUsername or password incorrect, try again\n");
        }
    }
}

int menu(int sockfd) {
    uint16_t menuOption = 0;

    bool active = true;
    while (active) {

        printf("\nPlease enter a selection:\n\n");
        printf("<1> Play Minesweeper\n");
        printf("<2> Show Leaderboard\n");
        printf("<3> Exit\n");

        do {
            printf("Selection option (1-3): ");

            char *ptr;

            char input = getchar();
            while (getchar() != '\n');
            menuOption = (uint16_t) strtol(&input, &ptr, 10);

            if (menuOption > 3 || menuOption < 1) {
                printf("\n Invalid option, please try again\n");
            }
        } while (menuOption > 3 || menuOption < 1);

        uint16_t menu_request = DATA_TYPE_MENU;
        send_uint16(sockfd, &menu_request);
        send_uint16(sockfd, &menuOption);

        switch (menuOption) {
            case 1:
                game(sockfd);
                break;
            case 2:
                show_leaderboard(sockfd);
                break;
            case 3:
                printf("Exiting...\n");
                active = false;
                break;
            default:
                break;
        }

    }

    return 0;
}

int main(int argc, char *argv[]) {
    int sockfd;
    struct hostent *he;
    struct sockaddr_in server_addr; /* connector's address information */

    setbuf(stdout, 0);

    if (argc != 3) {
        fprintf(stderr, "usage: client_hostname port_number\n");
        exit(1);
    }

    if ((he = gethostbyname(argv[1])) == NULL) {  /* get the host info */
        perror("gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    //	Clear Structure
    memset(&server_addr, 0, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;      /* host byte order */
    server_addr.sin_port = htons(atoi(argv[2]));    /* short, network byte order */
    server_addr.sin_addr = *((struct in_addr *) he->h_addr_list[0]);

    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) == -1) {
        perror("connect");
        exit(1);
    }

    int valid = login(sockfd);

    if (valid) {
        menu(sockfd);
    }

    close(sockfd);

    return 0;
}
