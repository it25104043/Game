#include "game.h"

char accept_input() {
	char move;
        do {
                printf("Move (W/A/S/D) or Q to quit: ");
                scanf(" %c", &move);
                while (getchar() != '\n');
                move = toupper(move);
        } while (!(move == 'W' || move == 'A' || move == 'S' || move == 'D' || move == 'Q'));
        return move;
}

char move_computer(char **map, int size, player *players, int current_player) {
        char directions[] = {'W', 'A', 'S', 'D'};
        int smart_chance = rand() % 100;

        //85% chance to try to find a valid move, 15% random move
        if (smart_chance < 85) {
                int start_direct = rand() % 4;

                for (int i = 0; i < 4; i++) {
                        char try_move = directions[(start_direct + i) % 4];
                        int new_row = players[current_player].row;
                        int new_col = players[current_player].col;

                        switch(try_move){
                                case 'W':
                                        new_row--;
                                        break;
                                case 'S':
                                        new_row++;
                                        break;
                                case 'A':
                                        new_col--;
                                        break;
                                case 'D':
                                        new_col++;
                                        break;
                        }

                        if (validate_move(map, size, new_row, new_col))
                                return try_move;
                }
        }
        return directions[rand() % 4];
}

