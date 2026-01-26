#include "game.h"

int remain_intel = INTEL_COUNT;

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

        //Game mode selection
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
		int winner = automatic_win(active_count, num_players, players);

                if (winner != 0) {
                        display_map(map, gridsize);
                        display_player_stats(players, num_players);

                        printf("\n*** Only one player remains! ***\n");
                        printf("Player %d [%c] WINS automatically!\n", winner, players[winner-1].symbol);
                        break;
                }

                if (active_count == 0) {
                        display_map(map, gridsize);
                        display_player_stats(players, num_players);

                        printf("\n      No players remain\n");
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

                        move_count++;
                        log_file(map, gridsize, players, num_players, move_count, current_player);
                }
                current_player = (current_player + 1) % num_players;
        }

        //Freeing allocated memories
        free_map(map, gridsize);
        free(players);

        printf("\nGame log saved to 'game_log.txt'\n");
        return 0;
}                                                           
