#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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
	int intel;
	int active;
	char symbol;
	int computer;
}player;

char** setmap(int size);
void place_items(char** map, int size, player *players, int num_players);
void display_map(char** map, int size);

int main(){
	int gridsize, num_players, mode;
	char** map;
	
	srand(time(NULL));

	printf("================================================\n");
	printf("	SpyNet - The Codebreaker Protocool	\n");
	printf("================================================\n");

	gridsize = MIN_SIZE + rand() % (MAX_SIZE-MIN_SIZE+1);

	printf("Grid size randomely set to:  %d x %d\n",gridsize,gridsize);

	printf("Select Game Mode:\n");
	printf(" 1 . Single player\n");
	printf(" 2 . Two players\n");
	printf(" 3 . Three players\n");
	do{
		printf("Enter your choice: ");
		scanf("%d",&mode);
	}while(mode >= 1 && mode <= 3);

	
	num_players=mode;
	player* players = malloc(num_players*sizeof(*players));


	char symbols[]={PLAYER1,PLAYER2,PLAYER3};
	for(int i=0;i<num_players;i++){
		players[i].lives = INITIAL_LIVES;
		players[i].intel = 0;
		players[i].active = 1;	
		players[i].symbol = symbols[i];
		players[i].computer = 0;
	}

	if(num_players>1)
		printf("\nConfigure players in multiplayer\n");
			for(int i=1;i<num_players;i++){
				char choice;
				do{
				printf("Is player %d a computer? (y/n): ", i+1);
				scanf("%c",&choice);
					if(choice == 'y' || choice == 'Y')
						players[i].computer=1;
				}while(choice == 'y' || choice == 'Y' || choice == 'n' || choice == 'N');
			}

	map = setmap(gridsize);
	place_items(map, gridsize, players, num_players);
	display_map(map, gridsize);
	


return 1;
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

        //placing extract point
        placed=0;
                while(!placed){
                        int row = rand() % size;
                        int col = rand() % size;

                        if(map[row][col] == EMPTY){
                                map[row][col] = EXTRACT;
                                placed = 1;
                        }
                }
        }

        //placing walls
	int wall_count = size + rand() % ((size * size)/20);
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
			printf("| %c",map[i][k]);
			printf("|\n");
	}
	
	for(int i=0;i<size;i++)
		        printf("+---");
                       
	 printf("+\n");

}	
