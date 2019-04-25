#define _GNU_SOURCE

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>
//#include <semaphore.h>
#include <time.h>
#include "communication.h"

#define AUTHENTICATION_FILENAME "Authentication.txt"

#define BACKLOG 10     /* how many pending connections queue will hold */

#define RANDOM_NUMBER_SEED 42
#define FILE_BUF_SIZE 500

#define PLAYER_NAME_LENGTH 20
#define NUM_OF_PLAYERS 10
#define NUM_TILES_X 9
#define NUM_TILES_Y 9
#define NUM_MINES 10
#define NUM_HANDLER_THREADS 10

pthread_mutexattr_t request_mutex_attr;
pthread_mutex_t request_mutex;
pthread_mutex_t crit_mutex;
pthread_mutex_t reader_mutex;
pthread_mutex_t rand_mutex;
//sem_t empty;
//sem_t full;

pthread_cond_t got_request = PTHREAD_COND_INITIALIZER;
int num_requests = 0;
int read_count = 0;
int server_socket;

/* format of a single request. */
struct request {
    int socket;             /* number of the request                  */
    struct request *next;   /* pointer to next request, NULL if none. */
};

struct request *requests = NULL;     /* head of linked list of requests. */
struct request *last_request = NULL; /* pointer to last request.         */

typedef struct {
    int adjacent_mines;
    bool revealed;
    bool ismine;
    bool isflagged;
} Tile;

typedef struct {
    uint16_t num_of_mines;
    uint16_t time;
    bool gameEnded;
    bool gameWon;
    Tile tiles[NUM_TILES_Y][NUM_TILES_X];
} Game;

struct Player {
    char *name;
    int gamesWon;
    int gamesPlayed;
    int lastTime;
};

struct LeaderboardEntry {
    char **name;
    int *gamesWon;
    int *gamesPlayed;
    int bestTime;
};

struct Player *allPlayers[10] = {0};
struct LeaderboardEntry *leaderboard[LEADERBOARD_SIZE];

void concatenate(char *str1, char *str2) {
    strcat(str1, str2);
}

char *itoa(int val) {

    int base = 10;

    static char buf[32] = {0};

    int i = 30;

    for (; val && i; --i, val /= base)

        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i + 1];

}

void reveal_tile_directly(Game *game, int x, int y) {
    game->tiles[x][y].revealed = true;
}

void flag_tile(Game *game, int x, int y) {
    game->tiles[x][y].isflagged = true;
    game->num_of_mines--;
}

bool tile_flagged(Game *game, int x, int y) {
    return game->tiles[x][y].isflagged;
}

bool tile_revealed(Game *game, int x, int y) {
    return game->tiles[x][y].revealed;
}

bool tile_contains_mine(Game *game, int x, int y) {
    return game->tiles[x][y].ismine;
}

int get_adjacent(Game *game, int x, int y) {
    return game->tiles[x][y].adjacent_mines;
}

void minefield(Game *game, char map_string[]) {

    char *letters[9] = {"A", "B", "C", "D", "E", "F", "G", "H", "I"};

    strcpy(map_string, "\t1 2 3 4 5 6 7 8 9 \n");
    concatenate(map_string, "\t----------------- \n");

    for (int i = 0; i < NUM_TILES_Y; i++) {
        concatenate(map_string, letters[i]);
        concatenate(map_string, " | ");

        for (int j = 0; j < NUM_TILES_X; j++) {
            if (tile_flagged(game, i, j)) {
                concatenate(map_string, "+ ");
            } else if (tile_revealed(game, i, j)) {
                int adj = get_adjacent(game, i, j);
                if (tile_contains_mine(game, i, j)) {
                    concatenate(map_string, "* ");
                } else if (adj > 0) {
                    concatenate(map_string, itoa(adj));
                    concatenate(map_string, " ");
                } else {
                    concatenate(map_string, "  ");
                }
            } else {
                concatenate(map_string, "X ");
            }

        }
        concatenate(map_string, "\n");
    }
}

void place_mines(Game *game) {

    for (int i = 0; i < NUM_MINES; i++) {

        int x, y;

        do {
            pthread_mutex_lock(&rand_mutex);
            x = rand() % NUM_TILES_X;
            y = rand() % NUM_TILES_Y;
            pthread_mutex_unlock(&rand_mutex);

        } while (tile_contains_mine(game, x, y));

        game->tiles[x][y].ismine = true;
    }
}

void calc_adjacent(Game *game) {

    for (int x = 0; x < NUM_TILES_X; x++) {
        for (int y = 0; y < NUM_TILES_Y; y++) {
            int offs_x = -1;
            int offs_y = -1;
            int counter = 0;

            do {
                if (x + offs_x >= 0 && y + offs_y >= 0 && x + offs_x < NUM_TILES_X && y + offs_y < NUM_TILES_Y
                    && !(offs_x == 0 && offs_y == 0)) {
                    if (tile_contains_mine(game, x + offs_x, y + offs_y)) counter++;
                }

                if (offs_x == 1) {
                    offs_x = -1;
                    offs_y++;
                } else {
                    offs_x++;
                }
            } while (offs_y < 2);

            game->tiles[x][y].adjacent_mines = counter;
        }
    }
}

void process_minefield(Game *game, int x, int y) {

    int adj = get_adjacent(game, x, y);

    if (tile_contains_mine(game, x, y)) {
        for (int i = 0; i < NUM_TILES_Y; i++) {
            for (int j = 0; j < NUM_TILES_X; j++) {
                if (tile_contains_mine(game, i, j)) {
                    reveal_tile_directly(game, i, j);
                }
            }
        }
    } else if (adj > 0) {
        reveal_tile_directly(game, x, y);
    } else {
        reveal_tile_directly(game, x, y);
        int offs_x = -1;
        int offs_y = -1;

        do {
            if (x + offs_x >= 0 && y + offs_y >= 0 && x + offs_x < NUM_TILES_X && y + offs_y < NUM_TILES_Y
                && !(offs_x == 0 && offs_y == 0)) {
                if (!(tile_revealed(game, x + offs_x, y + offs_y))) {
                    process_minefield(game, x + offs_x, y + offs_y);
                }
            }

            if (offs_x == 1) {
                offs_x = -1;
                offs_y++;
            } else {
                offs_x++;
            }
        } while (offs_y < 2);
    }
}

void signalHandler() {
    close(server_socket);
    printf("\n\nExiting...\n\n");
    exit(1);
}

int isValidAccount(char *username, char *password) {
    FILE *fp;
    char buf[FILE_BUF_SIZE];
    char *token, *line;
    char *line_username, *line_password;
    char *saveptr1, *saveptr2;
    char *filename = AUTHENTICATION_FILENAME;
    int line_index;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Could not find authentication file");
        return -1;
    }

    fread(buf, FILE_BUF_SIZE, 1, fp);
    fclose(fp);

    for (line_index = 1, line = buf;; line_index++, line = NULL) {
        token = strtok_r(line, "\n", &saveptr1);

        if (token == NULL)
            break;

        else if (line_index == 1) {
            continue;
        }

        line_username = strtok_r(token, " \t", &saveptr2);
        line_password = strtok_r(NULL, " \t", &saveptr2);

        if (strcmp(line_username, username) == 0 && strcmp(line_password, password) == 0)
            return 1;
    }

    return 0;
}

int compare_leaderboard_entrys(struct LeaderboardEntry *entry1, struct LeaderboardEntry *entry2) {

    if (entry1->bestTime == entry2->bestTime) {

        if (*entry1->gamesWon == *entry2->gamesWon) {

            return strcmp(*entry1->name, *entry2->name) < 0;
        }

        return *entry1->gamesWon > *entry2->gamesWon;
    }

    return entry1->bestTime > entry2->bestTime;
}

void order_leaderboard() {

    int i, j;
    struct LeaderboardEntry *key;
    for (i = 1; i < LEADERBOARD_SIZE; i++) {
        if (leaderboard[i] == NULL) {
            break;
        }

        key = leaderboard[i];
        j = i - 1;

        /* Move elements of arr[0..i-1], that are
           greater than key, to one position ahead
           of their current position */
        while (j >= 0 && compare_leaderboard_entrys(leaderboard[j], key)) {

            leaderboard[j + 1] = leaderboard[j];
            j = j - 1;

        }
        leaderboard[j + 1] = key;
    }
}

void add_leaderboard_entry(struct LeaderboardEntry *entry, struct Player *player) {

    entry->name = &player->name;
    entry->gamesPlayed = &player->gamesPlayed;
    entry->gamesWon = &player->gamesWon;
    entry->bestTime = player->lastTime;
}

bool is_new_highscore(struct Player *currentPlayer) {
    for (int i = 0; i < LEADERBOARD_SIZE; i++) {
        if (leaderboard[i] != NULL) {
            if (currentPlayer->lastTime < leaderboard[i]->bestTime) {
                return true;
            }
        } else {
            return true;
        }
    }

    return false;
}

void update_leaderboard(struct Player *currentPlayer) {
    int i = 0;
    int noNull = 1;
    for (; i < LEADERBOARD_SIZE; i++) {
        if (leaderboard[i] == NULL) {
            leaderboard[i] = malloc(sizeof(struct LeaderboardEntry));
            add_leaderboard_entry(leaderboard[i], currentPlayer);
            noNull = 0;
            break;
        }
    };

    if (noNull) {
        add_leaderboard_entry(leaderboard[i], currentPlayer);
    }

    order_leaderboard();
}

void show_leaderboard(int new_fd) {

    char stringLeaderboard[1000] = {0};
    strcpy(stringLeaderboard, "");

    if (leaderboard[0] == NULL) {
        strcpy(stringLeaderboard, "No information is available at the moment, try again later.");
    } else {
        for (int i = 0; i < LEADERBOARD_SIZE; i++) {
            if (leaderboard[i] != NULL) {
                concatenate(stringLeaderboard, *leaderboard[i]->name);
                concatenate(stringLeaderboard, "\t");
                concatenate(stringLeaderboard, itoa(leaderboard[i]->bestTime));
                concatenate(stringLeaderboard, " seconds\t");
                concatenate(stringLeaderboard, itoa(*leaderboard[i]->gamesWon));
                concatenate(stringLeaderboard, " games won, ");
                concatenate(stringLeaderboard, itoa(*leaderboard[i]->gamesPlayed));
                concatenate(stringLeaderboard, " games played\t\n");
            }
        }
    }
    send_char_array(new_fd, stringLeaderboard, sizeof(stringLeaderboard));
}

void process_endgame(int new_fd, Game *game, struct Player *currentPlayer) {
    currentPlayer->gamesPlayed++;

    if (game->gameWon) {
        currentPlayer->gamesWon++;

        currentPlayer->lastTime = game->time;

        pthread_mutex_lock(&crit_mutex);

        if (is_new_highscore(currentPlayer)) {
            update_leaderboard(currentPlayer);
        }

        pthread_mutex_unlock(&crit_mutex);
    }
}

Game run_game(int new_fd) {
    Game game = {
            .gameEnded = false,
            .gameWon = false,
            .time = 0,
            .num_of_mines = NUM_MINES,
            .tiles = {{{0}}},
    };

    time_t startTime = time(NULL);

    place_mines(&game);
    calc_adjacent(&game);

    while (!game.gameEnded) {
        char map[SIZE_OF_MAP] = {0};

        send_uint16(new_fd, &game.num_of_mines);

        minefield(&game, map);
        send_char_array(new_fd, map, sizeof(map));

        uint16_t choice;
        recv_uint16(new_fd, &choice);

        uint16_t x, y;
        uint16_t response;

        switch (choice) {

            case CHOICE_REVEAL_TILE:
                recv_uint16(new_fd, &y);
                recv_uint16(new_fd, &x);

                if (game.tiles[x][y].revealed) {
                    response = ERROR_TILE_ALREADY_REVEALED;

                } else {
                    process_minefield(&game, x, y);
                    response = 0;

                    if (game.tiles[x][y].ismine) {
                        response = MINE_HIT;
                        game.gameWon = false;
                        game.gameEnded = true;
                    }
                }

                break;

            case CHOICE_PLACE_FLAG:
                recv_uint16(new_fd, &y);
                recv_uint16(new_fd, &x);

                if (tile_flagged(&game, x, y)) {
                    response = ERROR_TILE_ALREADY_FLAGGED;

                } else if (game.tiles[x][y].ismine) {
                    response = MINE_FLAGGED;
                    flag_tile(&game, x, y);

                    if (game.num_of_mines == 0) {
                        response = GAME_WON;
                        game.gameEnded = true;
                        game.gameWon = true;
                    }

                } else {
                    response = MINE_DOESNT_EXIST;
                }

                break;

            case CHOICE_QUIT_GAME:
                game.gameWon = false;
                game.gameEnded = true;
                return game;

            default:
                break;
        }

        send_uint16(new_fd, &response);

        if (response == MINE_HIT || response == GAME_WON) {
            minefield(&game, map);
            send_uint16(new_fd, &game.num_of_mines);
            send_char_array(new_fd, map, sizeof(map));
        }

        if (response == GAME_WON) {
            time_t endTime = time(NULL);
            game.time = (int) endTime - startTime;
            send_uint16(new_fd, &game.time);
        }
    }

    return game;
}

int create_socket_server(int port) {
    int sockfd;
    struct sockaddr_in my_addr;    /* my and connector's address information */

    /* generate the socket */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    //	Clear Structure
    memset(&my_addr, 0, sizeof(struct sockaddr_in));

    /* generate the end point */
    my_addr.sin_family = AF_INET;         /* host byte order */
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY; /* All available interfaces */

    /* bind the socket to the end point */
    if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr_in)) == -1) {
        perror("bind");
        exit(1);
    }

    /* start listnening */
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    printf("server starts listnening ...\n");

    return sockfd;
}

int accept_connection(int sockfd) {
    int new_fd;
    socklen_t peer_addr_size;
    struct sockaddr_in peer_addr;

    peer_addr_size = sizeof(struct sockaddr_in);
    if ((new_fd = accept(sockfd, (struct sockaddr *) &peer_addr, &peer_addr_size)) == -1) {
        perror("acceptance error");
    }

    printf("server: got connection from %s\n", inet_ntoa(peer_addr.sin_addr));

    return new_fd;
}

int login(int socket, char *playerName) {
    uint16_t data_type;
    char username[USERNAME_CHAR_LENGTH];
    char password[PASSWORD_CHAR_LENGTH];

    while (1) {
        recv_uint16(socket, &data_type);

        if (data_type == DATA_TYPE_LOGIN) {

            recv_char_array(socket, username, sizeof(username));
            recv_char_array(socket, password, sizeof(password));

            printf("Username received: %s\n", username);
            printf("Password received: %s\n", password);

            uint16_t valid_account = (uint16_t) isValidAccount(username, password);

            if (valid_account) {
                strncpy(playerName, username, sizeof(username));
            }

            send_uint16(socket, &valid_account);

            if (valid_account) {
                return valid_account;
            }
        }
    }

    return -1;
}

struct Player *setup_player(char *playerName) {
    struct Player *currentPlayer = (struct Player *) malloc(sizeof(struct Player));
    currentPlayer->name = malloc(sizeof(char) * PLAYER_NAME_LENGTH);
    bool playerExists = false;

    for (int i = 0; i < NUM_OF_PLAYERS; i++) {
        if (allPlayers[i] != NULL) {
            if (str_is_equal(playerName, allPlayers[i]->name)) {
                playerExists = true;
                currentPlayer = allPlayers[i];
            }
        }
    }

    if (!playerExists) {
        strncpy(currentPlayer->name, playerName, sizeof(&playerName));
        currentPlayer->lastTime = 0;
        currentPlayer->gamesPlayed = 0;
        currentPlayer->gamesWon = 0;

        for (int i = 0; i < NUM_OF_PLAYERS; i++) {
            if (allPlayers[i] != NULL) {
                allPlayers[i] = currentPlayer;
            }
        }
    }

    return currentPlayer;
}

void init_menu(int new_fd) {
    struct Player *currentPlayer;
    char player_name[PLAYER_NAME_LENGTH];
    int valid_login = login(new_fd, player_name);

    if (valid_login) {
        currentPlayer = setup_player(player_name);
    }

    bool active = true;
    while (active) {

        uint16_t data_type;
        recv_uint16(new_fd, &data_type);

        if (data_type == DATA_TYPE_MENU) {

            uint16_t menu_option;
            recv_uint16(new_fd, &menu_option);

            switch (menu_option) {
                case 1: {
                    Game played_game = run_game(new_fd);
                    process_endgame(new_fd, &played_game, currentPlayer);
                    break;
                }
                case 2:
                    pthread_mutex_lock(&reader_mutex);

                    read_count++;
                    if (read_count == 1)
                        pthread_mutex_lock(&crit_mutex);

                    pthread_mutex_unlock(&reader_mutex);

                    show_leaderboard(new_fd);

                    pthread_mutex_lock(&reader_mutex);
                    read_count--;
                    if (read_count == 0)
                        pthread_mutex_unlock(&crit_mutex);
                    pthread_mutex_unlock(&reader_mutex);


                    break;
                case 3:
                    active = false;
                    break;
                default:
                    break;
            }
        }
    }

    close(new_fd);
}

void add_connection(int client_socket, pthread_mutex_t *p_mutex, pthread_cond_t *p_cond_var) {
    struct request *a_request;      /* pointer to newly added request.     */

    /* create structure with new request */
    a_request = (struct request *) malloc(sizeof(struct request));
    if (!a_request) { /* malloc failed?? */
        fprintf(stderr, "add_connection: out of memory\n");
        exit(1);
    }
    a_request->socket = client_socket;
    a_request->next = NULL;

    /* lock the mutex, to assure exclusive access to the list */
    pthread_mutex_lock(p_mutex);

    /* add new request to the end of the list, updating list */
    /* pointers as required */
    if (num_requests == 0) { /* special case - list is empty */
        requests = a_request;
        last_request = a_request;
    } else {
        last_request->next = a_request;
        last_request = a_request;
    }

    /* increase total number of pending requests by one. */
    num_requests++;

    /* unlock mutex */
    pthread_mutex_unlock(p_mutex);

    /* signal the condition variable - there's a new request to handle */
    pthread_cond_signal(p_cond_var);
}

struct request *get_connection(pthread_mutex_t *p_mutex) {
    struct request *a_request;      /* pointer to request.                 */

    /* lock the mutex, to assure exclusive access to the list */
    pthread_mutex_lock(p_mutex);

    if (num_requests > 0) {
        a_request = requests;
        requests = a_request->next;
        if (requests == NULL) { /* this was the last request on the list */
            last_request = NULL;
        }
        /* decrease the total number of pending requests */
        num_requests--;
    } else { /* requests list is empty */
        a_request = NULL;
    }

    /* unlock mutex */
    pthread_mutex_unlock(p_mutex);

    /* return the request to the caller. */
    return a_request;
}

void *handle_connections_loop() {
    struct request *a_request;      /* pointer to a request.               */

    /* do forever.... */
    while (1) {

        if (num_requests > 0) { /* a request is pending */
            a_request = get_connection(&request_mutex);
            if (a_request) { /* got a request - handle it and free it */
                /* unlock mutex - so other threads would be able to handle */
                init_menu(a_request->socket);
                free(a_request);
            }
        } else {
            /* wait for a request to arrive. note the mutex will be */
            /* unlocked here, thus allowing other threads access to */
            /* requests list.                                       */

            pthread_cond_wait(&got_request, &request_mutex);
            /* and after we return from pthread_cond_wait, the mutex  */
            /* is locked again, so we don't need to lock it ourselves */

        }
    }
}

int main(int argc, char *argv[]) {
    int port;

    setbuf(stdout, 0);
    srand(RANDOM_NUMBER_SEED);
    signal(SIGINT, signalHandler);

    // Threading variables
    pthread_t p_threads[NUM_HANDLER_THREADS];   /* thread's structures   */
    pthread_mutexattr_init(&request_mutex_attr);
    pthread_mutexattr_settype(&request_mutex_attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&request_mutex, &request_mutex_attr);
    pthread_mutex_init(&crit_mutex, &request_mutex_attr);
    pthread_mutex_init(&reader_mutex, &request_mutex_attr);
    pthread_mutex_init(&rand_mutex, &request_mutex_attr);

    /* Get port number for server to listen on */
    if (argc != 2) {
        fprintf(stderr, "defaulting to port: 12345\n");
        port = 12345;
    } else {
        port = atoi(argv[1]);
    }

    server_socket = create_socket_server(port);

    /* create the request-handling threads */
    for (int i = 0; i < NUM_HANDLER_THREADS; i++) {
        pthread_create(&p_threads[i], NULL, handle_connections_loop, NULL);
    }

    while (1) {
        int client_socket = accept_connection(server_socket);
        add_connection(client_socket, &request_mutex, &got_request);
    }
}
