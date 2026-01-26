#include "game.h"

void display_instructions() {
	printf("====================================================\n");
	printf("          SPYNET: THE CODEBREAKER PROTOCOL          \n");
	printf("====================================================\n\n");
	printf("OBJECTIVE:\n");
	printf("  Navigate the grid to collect 3 Intel (I) items and reach the Extraction Point (X) to win!\n\n");
	printf("CONTROLS:\n");
	printf("  W - Move Up         S - Move Down  \n");
	printf("  A - Move Left       D - Move Right \n");
	printf("  Q - Quit Game\n\n");
	printf("SYMBOLS:\n");
	printf("  @/&/$ : Players 1, 2, and 3 \n");
	printf("  I     : Intel \n");
	printf("  L     : Extra Lives \n");
	printf("  #     : Wall \n");
	printf("  X     : Extraction Point\n\n");
	printf("PENALTIES:\n");
	printf("  - Moving into Walls (#) or Out-of-Bounds costs 1 Life (L).\n");
	printf("  - If Lives reach 0, you are eliminated.\n\n");
}

void display_rules(int mode) {
printf("\n------------------------------------ EXTRACTION RULES ------------------------------------\n");

	if (mode == 1) {
		printf("SINGLE PLAYER:\n");
		printf("- You MUST collect all 3 Intel items before reaching (X).\n");
		printf("- Reaching (X) early results in immediate elimination.\n");
	} else {
		printf("MULTI-PLAYER:\n");
		printf("- If no Intel is left on the grid:\n");
		printf("  * The player with the MOST Intel who reaches (X) wins.\n");
		printf("  * If Intel counts are tied, the first person to reach (X) wins.\n");
		printf("- If you reach (X) and do NOT have the most Intel, you are eliminated!\n");
	}

	printf("------------------------------------------------------------------------------------------\n\n");
}

void log_file(char **map, int size, player *players, int num_players, int move_count, int current_player) {
	FILE *file = fopen("game_log.txt", "a");

	if (file == NULL) {
		printf("Warning: Could not open log file.\n");
		return;
	}

	time_t play;
	time(&play);

	fprintf(file,"\ntime_stamp: %s", ctime(&play));
	fprintf(file, "=== MOVE %d by player %d ===\n", move_count, current_player+1);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++)
			fprintf(file, "%c ", map[i][j]);

		fprintf(file, "\n");
	}

	fprintf(file, "\nPlayer Status:\n");
	for (int i = 0; i < num_players; i++) {
		fprintf(file, "Player %d (%c): Lives=%d, Intel=%d, Active=%d, Pos=(%d,%d)\n", i + 1, players[i].symbol, players[i].lives, players[i].intels, players[i].active, players[i].row, players[i].col);
}

	//Check for automatic win (only one player remaining)
	int active_count = count_active_players(players, num_players);
	int winner = automatic_win(active_count, num_players, players);
       
	if (winner != 0) {
		fprintf(file,"---------------------------------------------------------\n");
		fprintf(file, "Player %d [%c] WINS automatically!\n", winner, players[winner-1].symbol);
        	fprintf(file,"---------------------------------------------------------\n");

	 }

        //Check if current player won
	if (check_win_conditions(players, map, current_player, num_players)) {
		fprintf(file,"---------------------------------------------------------\n");
                fprintf(file, "Player %d [%c] WINS!\n", current_player + 1, players[current_player].symbol);
		fprintf(file,"---------------------------------------------------------\n");

	}

	fclose(file);
}


