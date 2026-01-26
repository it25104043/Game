#include "game.h"

int validate_move(char **map, int size, int new_row, int new_col) {
	if (new_row < 0 || new_row >= size || new_col < 0 || new_col >= size) //Checks out of bounds
		return 0;

	if (map[new_row][new_col] == WALL) //Checks walls
		return 0;

	if (map[new_row][new_col] == PLAYER1 || map[new_row][new_col] == PLAYER2 || map[new_row][new_col] == PLAYER3) //Checks collision with another player
		return 0;

	return 1;
}

int check_win_conditions(player *players, char **map, int current_player, int num_players) {
	if (map[players[current_player].row][players[current_player].col] == EXTRACT && remain_intel == 0 && players[current_player].active) {
		int primary_target = (num_players == 1) ? INTEL_COUNT : (INTEL_COUNT / num_players + 1); //Minimum intel count to win
		int tie_target = INTEL_COUNT / num_players;

		if (players[current_player].intels >= primary_target)
			return 1;

		if (INTEL_COUNT % num_players == 0 && players[current_player].intels >= tie_target)
			return 1;
	}
	return 0;
}

int automatic_win(int active_count, int num_players, player *players){
	if (active_count == 1 && num_players > 1)
		for (int i = 0; i < num_players; i++){
			if (players[i].active)
				return i+1;
		}
	return 0;
}
