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

typedef struct{
	int row;
	int col;
	int lives;
	int intels;
	int active;
	char symbol;
	int computer;
}player;

char** setmap(int size);
void place_items(char** map, int size, player *players, int num_players);
void display_map(char** map, int size);
int count_active_players(player* players,int num_players);
void display_player_stats(player* players,int num_players);

int main(){
	int gridsize, num_players, mode;
	char** map;
	char symbols[]={PLAYER1,PLAYER2,PLAYER3};
	
	srand(time(NULL));

	printf("================================================\n");
	printf("	SpyNet - The Codebreaker Protocool	\n");
	printf("================================================\n");

	gridsize = MIN_SIZE + rand() % (MAX_SIZE-MIN_SIZE+1);

	printf("\nGrid size randomely set to:  %d x %d\n\n",gridsize,gridsize);

	printf("Select Game Mode:\n");
	printf(" 1 . Single player\n");
	printf(" 2 . Two players\n");
	printf(" 3 . Three players\n");
	do{
		printf("\nEnter your choice: ");
		scanf(" %d",&mode);
		while(getchar() != '\n');
	}while(mode < 1 || mode > 3);

	
	num_players=mode;
	player* players = malloc(num_players*sizeof(*players));


	for(int i=0;i<num_players;i++){
		players[i].lives = INITIAL_LIVES;
		players[i].intels = 0;
		players[i].active = 1;	
		players[i].symbol = symbols[i];
		players[i].computer = 0;
	}

	if(num_players>1){
		printf("\nConfigure players in multiplayer mode\n");
			for(int i=1;i<num_players;i++){
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

	map = setmap(gridsize);
	place_items(map, gridsize, players, num_players);
	
	//main game loop
	int current_player = 0;
	int game_running = 1;
	
	while(game_running){

		if(!players[current_player].active){
			current_player = (current_player +1) % num_players;
			continue;
		}

	
	int active_count = count_active_players(players,num_players);
	if (active_count == 1 && num_players>1){
		for(int i=0;i<num_players;i++){
			if(players[i].active)
				printf("Player %d [%c] WON!!!", i+1,players[i].symbol);
		}
		break;
	}
	
	if(active_count == 0){
		printf("\nAll players were eliminated.\n");
		printf("==============GAME OVER==============");
	break;
	}

	display_map(map,gridsize);
	display_player_stats(players,num_players);

	
	//player movements
	int move_count;
	char move;
	int new_row = players[current_player].row;
	int new_col = players[current_player].col;

	if(players[current_player].computer){//not finish
		printf("\n>>>Computer player %d [%c] is thinking.............",current_player +1 ,players[current_player].symbol);
		//computer_move(map,gridsize,&players[current_player]);
		//log_file(map,gridsize,players,num_players,move_count);
	}
	else{
		printf("\n>>> Player %d [%c] - Lives: %d | Intel: %d/%d\n",current_player+1,players[current_player].symbol,players[current_player].lives,players[current_player].intels,INTEL_COUNT);

	do{		
		printf("Move (W/A/S/D) or Q to quit: ");
		scanf("%c",&move);
		while(getchar() !='\n');
		move = toupper(move);
	}while(!(move == 'W' || move =='A' || move =='S' || move =='D' || move =='Q'));


		if(move == 'Q'){
			printf("Player %d quit the game\n",current_player+1);
			players[current_player].active=0;
			map[players[current_player].row][players[current_player].col] = EMPTY;
			current_player = (current_player + 1) % num_players;
			continue;
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
		
	if(validate_move(map,gridsize,new_row,new_col)){
		//clear old position
		map[players[current_player].row][players[current_player].col] = EMPTY;
		//update player stat
		update_player_stat(map,gridsize,&players[current_player],new_row,new_col);
		//Extract point check
		if(map[new_row][new_col] == EXTRACT)
			continue;

		//update position
		players[current_player].row = new_row;
		players[current_player].col = new_col;
		map[new_row][new_row] = players[current_player].symbol;

		move_count++;
		//log_file();
		
		//win conditions
		if(players[current_player].intels >= INTEL_COUNT && players[current_player].active){
			display_map(map,gridsize,players,num_players);
			printf("\n*** MISSION ACCOMPLISHED ***\n");
			printf("player %d [%c] WINS!\n",current_player+1,players[current_player].symbol);
			printf("Intel Collected: %d\n",players[current_player].intels);
			printf("Lives remaning: %d\n",players[current_player].lives);
			game_running = 0;
		}

		if(players[current_player].lives <=0){
			printf("Player %d eliminated (No lives remaining).\n",current_player +1);
			players[current_player].active = 0;
			map[players[current_player].row][players[current_player].col] = EMPTY;
		}
	} else {


		}






	}



	






	}

return 0;
}


char** setmap(int size) {
	char** map = malloc(size * sizeof(*map));
	for(int i = 0; i<size;i++){
		map[i]=malloc(size*sizeof(char));
			for(int j = 0; j<size;j++)
				map[i][j]=EMPTY;
	}
return map;
}

void place_items(char** map, int size, player *players, int num_players){
	int placed;

	//placing players
	for(int i=0;i<num_players;i++){
	placed=0;
		while(!placed){
			int row = rand() % size;
			int col = rand() % size;
		
			if(map[row][col] == EMPTY){
				map[row][col] = players[i].symbol;
				players[i].row = row;
				players[i].row = row;
				placed = 1;
			}
		}	
	}
        //placing intels
        for(int i=0;i<INTEL_COUNT;i++){
        placed=0;
                while(!placed){
                        int row = rand() % size;
                        int col = rand() % size;
                
                        if(map[row][col] == EMPTY){
                                map[row][col] = INTEL;
                                placed = 1;
                        }
                }
        }

        //placing lives
        for(int i=0;i<LIFE_COUNT;i++){
        placed=0;
                while(!placed){
                        int row = rand() % size;
                        int col = rand() % size;

                        if(map[row][col] == EMPTY){
                                map[row][col] = LIFE;
                                placed = 1;
                        }
                }
        }

        //placing the extract point
        placed=0;
                while(!placed){
                        int row = rand() % size;
                        int col = rand() % size;

                        if(map[row][col] == EMPTY){
                                map[row][col] = EXTRACT;
                                placed = 1;
                        }
                }
        

        //placing walls
	int wall_count = size + rand() % ((size * size)/5);
	for(int i=0;i<wall_count;i++){
        placed=0;
                while(!placed){
                        int row = rand() % size;
                        int col = rand() % size;

                        if(map[row][col] == EMPTY){
                                map[row][col] = WALL;
                                placed = 1;
                        }
                }
        }
}

void display_map(char** map, int size){
	printf("\n");

	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++)
			printf("+---");
			
		printf("+\n");

		for(int k=0;k<size;k++)
			printf("| %c ",map[i][k]);
			printf("|\n");
	}
	
	for(int i=0;i<size;i++)
		        printf("+---");
                       
	 printf("+\n");

}	


int count_active_players(player* players,int num_players){
	int count = 0;
	for(int i=0;i<num_players;i++)
		if(players[i].active)
			count++;


	return count;
}


void display_player_stats(player* players,int num_players){
	printf("\n----------------Player Status----------------\n");
	for(int i=0;i<num_players;i++){
		if(players[i].active)
			printf("Player %d [%c]: Lives = %d | Intels = %d/%d |%s\n",i+1,players[i].symbol,players[i].lives,players[i].intels,INTEL_COUNT,players[i].computer?"Computer":"Human");

		else
			printf("Player %d [%c]: LOST",i+1,players[i].symbol);
		
	}
	printf("---------------------------------------------\n");

}

void computer_move(map,gridsize,&players[current_player]);
void log_file(char** map,int size,char** players,int num_players,int move_count);

int validate_move(char** map, int size, int new_row, int new_col){
	//check boundaries
	if(new_row<0 || new_row >=size || new_col<0 || new_row >=size)
		return 0;

	//check walls
	if(map[new_row][new_col] == WALL)
		return 0;

	//check for other players
	if(map[new_row][new_col] == PLAYER1 || map[new_row][new_col] == PLAYER2 || map[new_row][new_col] == PLAYER3)
		return 0;
	//else
	return 1;
}
void update_player_stat(char** map, int size, player* player, int new_row, int new_col){
	char cell = map[new_row][new_col];

	if(cell == INTEL){
		player->intels++;
		printf("INTEL collected! Total: %d/%d\n",player->intels,INTEL_COUNT);
	} else if(cell == LIFE){
		player->lives++;
                printf("LIFE gained! Total Lives: %d\n",player->lives)
	} else if(cell == EXTRACT){
		if(player->intels < INTEL_COUNT){
			printf("Extraction failed! Need all %d intel. Mission aborted.\n ", INTEL_COUNT);
			player->active = 0;
		}
	}
}



