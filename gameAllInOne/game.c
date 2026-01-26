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

int remain_intel = INTEL_COUNT;

typedef struct{
	int row;
	int col;
	int lives;
	int intels;
	int active;
	char symbol;
	int computer;
}player;

char **initialize_map(int size);
void place_items(char **map, int size, player *players, int num_players);
void display_map(char **map, int size);
int count_active_players(player *players, int num_players);
void display_player_stats(player *players, int num_players);
int check_win_conditions(player *players, char **map, int current_player, int num_players);
void update_player_state(char **map, int size, player *player, int new_row, int new_col, int num_players);
int validate_move(char **map, int size, int new_row, int new_col);
char accept_input();
char move_computer(char **map, int size, player *players, int current_player);
void log_file(char **map, int size, player *players, int numPlayers, int moveCount, int current_player);
void free_map(char **map, int size);
void display_instructions();
void display_rules(int mode);

int main(){
	int gridsize, num_players, mode;
	char **map;
	char symbols[] = {PLAYER1, PLAYER2, PLAYER3};
	
	srand(time(NULL));


	display_instructions();

	//Input grid size
	do {
		printf("Enter grid size (%d-%d) or 0 for random: ", MIN_SIZE, MAX_SIZE);
		scanf(" %d", &gridsize); 
		while (getchar() != '\n');
	} while (gridsize != 0 && (gridsize < MIN_SIZE || gridsize > MAX_SIZE));

	
	if (gridsize == 0) {
		gridsize = MIN_SIZE + rand() % (MAX_SIZE - MIN_SIZE + 1);
		printf("Grid size randomly set to:  %d x %d\n\n", gridsize, gridsize);
	}

	// Game mode selection
	printf("Select Game Mode:\n");
	printf(" 1 . Single player\n");
	printf(" 2 . Two players\n");
	printf(" 3 . Three players\n");

	do {
		printf("\nEnter your choice: ");
		scanf(" %d", &mode); 
		while (getchar() != '\n');
	} while (mode < 1 || mode > 3);

	display_rules(mode);

	
	num_players = mode;
	player *players = malloc(num_players * sizeof(*players));

	//Initialize players
	for(int i=0;i<num_players;i++){
		players[i].lives = INITIAL_LIVES;
		players[i].intels = 0;
		players[i].active = 1;	
		players[i].symbol = symbols[i];
		players[i].computer = 0;
	}

	if(num_players > 1){
		printf("\nConfigure players in multiplayer mode\n");
		for(int i = 1; i < num_players ; i++){
			char choice;
			do{
				printf("Is player %d a computer? (y/n): ", i+1);
				scanf(" %c",&choice);
				while(getchar() != '\n');
					if(choice == 'y' || choice == 'Y')
						players[i].computer=1;
			}while(choice != 'y' &&  choice != 'Y' && choice != 'n' && choice != 'N');
		}
	}

	//Game initialization
	map = initialize_map(gridsize);
	place_items(map, gridsize, players, num_players);
	
	//Main game loop
	int current_player = 0;
	int game_running = 1;
	int move_count = 0;	

	log_file(map, gridsize, players, num_players, move_count, current_player);

	while (game_running) {
		int active_count = count_active_players(players, num_players);

		if (active_count == 1 && num_players > 1) {
			display_map(map, gridsize);
                	display_player_stats(players, num_players);
			
			printf("\n*** Only one player remains! ***\n");
				for (int i = 0; i < num_players; i++) {
					if (players[i].active)
						printf("Player %d [%c] WINS automatically!\n", i + 1, players[i].symbol);
				}
				break;
		}

		if (active_count == 0) {
               		display_map(map, gridsize);
                	display_player_stats(players, num_players);

			printf("\n	No players remain\n");
			printf("\n===========GAME OVER===========\n");
			break;
		}

		//Skip turn if player isnt active
		if (!players[current_player].active) {
			current_player = (current_player + 1) % num_players;
			continue;
		}

		display_map(map, gridsize);
		display_player_stats(players, num_players);

		//Input movements
		char move;
		int new_row = players[current_player].row;
		int new_col = players[current_player].col;

		//Input movements from computer
		if (players[current_player].computer) {
			printf("\n>>> Computer Player %d [%c] is thinking...\n", current_player + 1, players[current_player].symbol);
			move = move_computer(map, gridsize, players, current_player);
		//Input movements from user
		} else {
			printf("\n>>> Player %d [%c] - Lives: %d | Intel: %d/%d\n", current_player + 1, players[current_player].symbol, players[current_player].lives, players[current_player].intels, INTEL_COUNT);
			move = accept_input();

			if (move == 'Q') {
				printf("Player %d quit the game\n", current_player + 1);
				players[current_player].active = 0;
				map[players[current_player].row][players[current_player].col] = EMPTY;
				current_player = (current_player + 1) % num_players;
				continue;
			}
		}

		switch(move){
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
		
		//If move is a valid move 		
		if (validate_move(map, gridsize, new_row, new_col)) {
			
			//Update map
			map[players[current_player].row][players[current_player].col] = EMPTY;
			//Update player state
			update_player_state(map, gridsize, &players[current_player], new_row, new_col, num_players);

			players[current_player].row = new_row;
			players[current_player].col = new_col;

			if (map[new_row][new_col] != EXTRACT)
				map[new_row][new_col] = players[current_player].symbol;

			move_count++;
			log_file(map, gridsize, players, num_players, move_count, current_player);

			//Checks if the  player is a winner
			if (check_win_conditions(players, map, current_player, num_players)) {
				display_map(map, gridsize);
				printf("\n*** MISSION ACCOMPLISHED ***\n");
				printf("player %d [%c] WINS!\n", current_player + 1, players[current_player].symbol);
				printf("Intel Collected: %d\n", players[current_player].intels);
				printf("Lives remaning: %d\n", players[current_player].lives);
				game_running = 0;
}
		//If move isn't valid
		} else {

			players[current_player].lives--;
			printf("\nINVALID MOVE! Life penalty applied. Lives remaining: %d\n", players[current_player].lives);

				if (players[current_player].lives == 0) {
					printf("Player %d eliminated (no lives remaining).\n", current_player + 1);
					players[current_player].active = 0;
					map[players[current_player].row][players[current_player].col] = EMPTY;
				}
			}
		current_player = (current_player + 1) % num_players;
	}

	//Freeing allocated memories
	free_map(map, gridsize);
	free(players);

	printf("\nGame log saved to 'game_log.txt'\n");
	return 0;
}

char **initialize_map(int size) {
	char **map = malloc(size * sizeof(*map));
	for (int i = 0; i < size; i++) {
		map[i] = malloc(size * sizeof(char));
		for (int j = 0; j < size; j++)
			map[i][j] = EMPTY;
	}
	return map;
}

//Free memory map
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

int validate_move(char **map, int size, int new_row, int new_col) {
	if (new_row < 0 || new_row >= size || new_col < 0 || new_col >= size) //Checks out of bounds
		return 0;

	if (map[new_row][new_col] == WALL) //Checks walls
		return 0;

	if (map[new_row][new_col] == PLAYER1 || map[new_row][new_col] == PLAYER2 || map[new_row][new_col] == PLAYER3) //Checks collision with another player
		return 0;

	return 1;
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

void log_file(char **map, int size, player *players, int numPlayers, int moveCount,int current_player) {
	FILE *file = fopen("game_log.txt", "a");

	if (file == NULL) {
		printf("Warning: Could not open log file.\n");
	return;
	}
	
	time_t play;
	time(&play);

	fprintf(file,"\ntime_stamp: %s", ctime(&play));
	fprintf(file, "=== MOVE %d by player %d ===\n", moveCount, current_player+1);
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) 
				fprintf(file, "%c ", map[i][j]);
			
				fprintf(file, "\n");
		}		

	fprintf(file, "\nPlayer Status:\n");
		for (int i = 0; i < numPlayers; i++) {
			fprintf(file, "Player %d (%c): Lives=%d, Intel=%d, Active=%d, Pos=(%d,%d)\n", i + 1, players[i].symbol, players[i].lives, players[i].intels, players[i].active, players[i].row, players[i].col);
		}		
	fclose(file);
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

