#include "game.h"

char **initialize_map(int size) {
        char **map = malloc(size * sizeof(*map));
        for (int i = 0; i < size; i++) {
                map[i] = malloc(size * sizeof(char));
                for (int j = 0; j < size; j++)
                        map[i][j] = EMPTY;
        }
        return map;
}

void free_map(char **map, int size) {
        for (int i = 0; i < size; i++) {
                free(map[i]);
        }
        free(map);
}

void place_items(char **map, int size, player *players, int num_players) {
        int placed;
        int row, col;

        //Place players
        for (int i = 0; i < num_players; i++) {
                placed = 0;
                while (!placed) {
                        row = rand() % size;
                        col = rand() % size;
                        if (map[row][col] == EMPTY) {
                                map[row][col] = players[i].symbol;
                                players[i].row = row;
                                players[i].col = col;
                                placed = 1;
                        }
                }
	}

        //Place Intels
        for (int i = 0; i < INTEL_COUNT; i++) {
                placed = 0;
                while (!placed) {
                        row = rand() % size;
                        col = rand() % size;
                        if (map[row][col] == EMPTY) {
                                map[row][col] = INTEL;
                                placed = 1;
                        }
                }
        }

        //Place lives
        for (int i = 0; i < LIFE_COUNT; i++) {
                placed = 0;
                while (!placed) {
                        row = rand() % size;
                        col = rand() % size;
                        if (map[row][col] == EMPTY) {
                                map[row][col] = LIFE;
                                placed = 1;
                        }
                }
        }

        //Place extract point
        placed = 0;
        while (!placed) {
                row = rand() % size;
                col = rand() % size;
                if (map[row][col] == EMPTY) {
                        map[row][col] = EXTRACT;
                        placed = 1;
                }
        }

        //Place walls
        int wall_count = (size * size) * 0.2; //20% of the grid 
        for (int i = 0; i < wall_count; i++) {
                placed = 0;
                while (!placed) {
                        row = rand() % size;
                        col = rand() % size;
                        if (map[row][col] == EMPTY) {
                                map[row][col] = WALL;
                                placed = 1;
                        }
                }
        }
}

void display_map(char **map, int size) {
        printf("\n");
        for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++)
                        printf("+---");

                printf("+\n");
                for (int k = 0; k < size; k++)
                        printf("| %c ", map[i][k]);

                printf("|\n");
        }

        for (int i = 0; i < size; i++)
                printf("+---");

        printf("+\n");
}

