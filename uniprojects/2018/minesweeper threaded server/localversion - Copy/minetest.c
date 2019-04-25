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
#include <time.h>
#include "communication.h"
#include "../../../../Users/davoi/AppData/Local/Packages/CanonicalGroupLimited.UbuntuonWindows_79rhkp1fndgsc/LocalState/rootfs/usr/lib/gcc/x86_64-linux-gnu/7/include/stdbool.h"

#define AUTHENTICATION_FILENAME "Authentication.txt"

#define BACKLOG 10     /* how many pending connections queue will hold */

#define RANDOM_NUMBER_SEED 42
#define FILE_BUF_SIZE 500

#define NUM_TILES_X 9
#define NUM_TILES_Y 9
#define NUM_MINES 10

typedef struct {
    int adjacent_mines;
    bool revealed;
    bool ismine;
    bool isflagged;
} Tile;

typedef struct{
    int num_of_mines;
    bool gameEnded;
    bool gameWon;
    Tile tiles[NUM_TILES_Y][NUM_TILES_X];
} Game;


void reveal_tile_directly(Game* game, int x, int y) {
    Tile* mine_tile = &game->tiles[x][y];
    mine_tile->revealed = true;
}

void flag_tile(Game* game, int x, int y){
    Tile* mine_tile = &game->tiles[x][y];
    mine_tile->isflagged = true;
    game->num_of_mines--;
    if (game->num_of_mines == 0){
        printf("All mines flagged successfully");
        game->gameEnded = true;
    }
}

bool tile_flagged(Game* game, int x, int y){
    Tile* mine_tile = &game->tiles[x][y];
    return mine_tile->isflagged;
}

bool tile_revealed(Game* game, int x, int y) {
    Tile* mine_tile = &game->tiles[x][y];
    return mine_tile->revealed;
}

bool tile_contains_mine(Game* game, int x, int y) {
    Tile* mine_tile = &game->tiles[x][y];
    return mine_tile->ismine;
}

int get_adjacent(Game* game, int x, int y) {
    Tile* mine_tile = &game->tiles[x][y];
    return mine_tile->adjacent_mines;
}

void print_minefield(Game* game){

    char letters[9] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I'};

    printf("\t1 2 3 4 5 6 7 8 9 \n");
    printf("\t----------------- \n");

    for (int i = 0; i < NUM_TILES_Y; i++){
        printf("%c | ", letters[i]);
        for (int j = 0; j < NUM_TILES_X; j++) {
            if (tile_flagged(game, i, j)){
                printf("+ ");
            }

            else if (tile_revealed(game, i, j)) {
                int adj = get_adjacent(game, i, j);
                if (tile_contains_mine(game, i, j)) {
                    printf("* ");
                } else if (adj > 0) {
                    printf("%d ", adj);
                } else {
                    printf("  ");
                }
            }
            else {
                  printf("X ");
            }

        }
        printf("\n");
    }
}

void place_mines(Game* game) {

    for (int i = 0; i < NUM_MINES; i++) {

        int x, y;

        do {
            x = rand() % NUM_TILES_X;
            y = rand() % NUM_TILES_Y;
        } while (tile_contains_mine(game, x, y));

        Tile* mine_tile = &game->tiles[x][y];
        mine_tile->ismine = true;
    }
}

void calc_adjacent(Game* game){

    for (int x = 0; x < NUM_TILES_X; x++){
        for (int y = 0; y < NUM_TILES_Y; y++){
            int offs_x = -1;
            int offs_y = -1;
            int counter = 0;

            do {
                if (x + offs_x >= 0 && y + offs_y >= 0 && x + offs_x < NUM_TILES_X && y + offs_y < NUM_TILES_Y
                && !(offs_x == 0 && offs_y == 0)) {
                    if (tile_contains_mine(game, x+offs_x, y+offs_y)) counter++;
                }

                if (offs_x == 1){
                    offs_x = -1;
                    offs_y++;
                }
                else {
                    offs_x++;
                }
            } while (offs_y < 2);

            Tile* mine_tile = &game->tiles[x][y];
            mine_tile->adjacent_mines = counter;
        }
    }
}

void reveal_tile(Game* game, int x, int y){

    int adj = get_adjacent(game, x, y);
    if (tile_contains_mine(game, x, y)) {
        for (int i = 0; i < NUM_TILES_Y; i++) {
            for (int j = 0; j < NUM_TILES_X; j++) {
                if (tile_contains_mine(game, i, j)){
                    reveal_tile_directly(game, i, j);
                }
            }
        }

        print_minefield(game);
        printf("\n You hit a mine!!\n");
        game->gameWon = false;
        game->gameEnded = true;
    }
    else if (adj > 0) {
        reveal_tile_directly(game, x, y);
    }
    else {
        reveal_tile_directly(game, x, y);
        int offs_x = -1;
        int offs_y = -1;

        do {
            if (x + offs_x >= 0 && y + offs_y >= 0 && x + offs_x < NUM_TILES_X && y + offs_y < NUM_TILES_Y
            && !(offs_x == 0 && offs_y == 0)) {
                if (!(tile_revealed(game, x + offs_x, y + offs_y))) {
                    printf("Tile will be revealed!: %d, %d\n", x + offs_x, y + offs_y);
                    reveal_tile(game, x + offs_x, y + offs_y);
                }
            }

            if (offs_x == 1){
                offs_x = -1;
                offs_y++;
            }
            else {
                offs_x++;
            }
        } while (offs_y < 2);
    }
}

int main() {

    time_t beginTime;
    time_t endTime;
    beginTime = time(NULL);

    srand(RANDOM_NUMBER_SEED);
    Game *game = (Game *)malloc(sizeof(Game));
    game->gameEnded = false;
    game->gameWon = true;
    game->num_of_mines = NUM_MINES;

    place_mines(game);
    calc_adjacent(game);

    while (game->gameEnded == false) {
        int x, y;
        printf("\nRemaining mines: %d\n\n\n", game->num_of_mines);
        print_minefield(game);
        printf("\nChoose an option:\n");
        printf("<R> Reveal Tile\n");
        printf("<P> Place Flag\n");
        printf("<Q> Quit Game\n");
        printf("\nOption (R, P, Q):");

        char buf[500];
        fgets(buf, sizeof(buf), stdin);
        switch(buf[0]){

            case 'R':
                printf("Enter tile co-ordinates:");
                char tileReveal[500];
                fgets(tileReveal, sizeof(tileReveal), stdin);
                x = tileReveal[0] - 65;
                y = tileReveal[1] - 49;
                if (!(x >= 0 && x <= NUM_TILES_X && y >= 0 && y <= NUM_TILES_Y)){
                    printf("Invalid entry, please try again.\n");
                }

                else if(tile_revealed(game, x, y)){
                    printf("Tile already revealed, please try again.\n");
                }

                else {
                    reveal_tile(game, x, y);
                    break;
                }

            case 'P':
                printf("Enter tile co-ordinates:");
                char tileFlag[500];
                fgets(tileFlag, sizeof(tileFlag), stdin);
                x = tileFlag[0] - 65;
                y = tileFlag[1] - 49;
                if (x >= 0 && x <= NUM_TILES_X && y >= 0 && y <= NUM_TILES_Y){
                    if (tile_flagged(game, x, y)){
                        printf("\nCannot flag an already flagged tile.\n");
                    }
                    else if(!(tile_contains_mine(game, x, y))){
                        printf("\nWARNING: Mine not under tile.\n");
                    }
                    else {
                        flag_tile(game, x, y);
                        break;
                    }
                }
                else {
                    printf("\nInvalid entry, please try again.\n");
                }

            case 'Q':
                printf("Exiting game...");
                game->gameWon = false;
                game->gameEnded = true;
                break;

            default:
                printf("Invalid option. Must be R, P, or Q. Reprinting minefield...");
        }
    }

    if (game->gameWon){
        endTime = time(NULL);
        print_minefield(game);
        printf ("\nCongratulations, you completed the game in %d seconds!", endTime - beginTime);
    }

    else {
        printf("\nYou lose.");
    }

}
