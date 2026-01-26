#include "game.h"

int count_active_players(player *players, int num_players) {
        int count = 0;
        for (int i = 0; i < num_players; i++)
                if (players[i].active)
                        count++;
        return count;
}

void display_player_stats(player *players, int num_players) {
        printf("\n----------------Player Status----------------\n");
        for (int i = 0; i < num_players; i++) {
                if (players[i].active)
                        printf("Player %d [%c]: Lives = %d | Intels = %d/%d | %s\n", i + 1, players[i].symbol, players[i].lives, players[i].intels, INTEL_COUNT, players[i].computer ? "Computer" : "Human");
                else
                        printf("Player %d [%c]: LOST\n", i + 1, players[i].symbol);
        }
        printf("---------------------------------------------\n");
}

void update_player_state(char **map, int size, player *player, int new_row, int new_col, int num_players) {
        char cell = map[new_row][new_col];

        if (cell == INTEL) {
                player->intels++;
                printf("INTEL collected! Total: %d/%d\n", player->intels, INTEL_COUNT);
                remain_intel--;

        } else if (cell == LIFE) {
                player->lives++;
                printf("LIFE gained! Total Lives: %d\n", player->lives);

        } else if (cell == EXTRACT) {
                int win_threshold = (INTEL_COUNT + num_players - 1) / num_players; //Minimum intel count needed to win

                if ((remain_intel != 0) || (remain_intel == 0 && player->intels < win_threshold)) {
                        printf("\n[!] Extraction failed! You don't have enough intel. Mission aborted.\n");
                        player->active = 0;
                }
        }
}
