#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define PLAYER1 '@'
#define PLAYER2 '&'
#define PLAYER3 '$'
#define WALL '#'
#define INTEL 'I'
#define LIFE 'L'
#define EXTRACT 'X'
#define EMPTY '.'

#define MIN_SIZE 5
#define MAX_SIZE 15
#define INTEL_COUNT 3
#define INITIAL_LIVES 3
#define LIFE_COUNT 3

extern int remain_intel;

typedef struct {
        int row;
        int col;
        int lives;
        int intels;
        int active;
        char symbol;
        int computer;
} player;

//Map functions
char **initialize_map(int size);
void place_items(char **map, int size, player *players, int num_players);
void display_map(char **map, int size);
void free_map(char **map, int size);

//Player functions
int count_active_players(player *players, int num_players);
void display_player_stats(player *players, int num_players);
void update_player_state(char **map, int size, player *player, int new_row, int new_col, int num_players);

//Movement functions
char accept_input();
char move_computer(char **map, int size, player *players, int current_player);

//Game logic functions
int validate_move(char **map, int size, int new_row, int new_col);
int check_win_conditions(player *players, char **map, int current_player, int num_players);
int automatic_win(int active_count, int num_players, player *players);

//Instruction & logging functions
void display_instructions();
void display_rules(int mode);
void log_file(char **map, int size, player *players, int num_players, int move_count, int current_player);

#endif
