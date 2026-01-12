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
#define INTELS 3

typedef struct{
	int row;
	int col;
	int lives;
	int intel;
	int active;
	char symbol;
	int is_computer;
}player;



int main(){
	int gridsize,num_players,mode;
	
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
	printf("Enter your choice: ");
	scanf("%d",&mode);

	if(mode<1||mode>3){
		printf("Invalid Input: \n");
		printf("exiting...");
		return 1;
	}
	
	num_players=mode;
	char symbols[]={PLAYER1,PLAYER2,PLAYER3}
	for(int i=0;i<num_players;i++)
		player	




return 1;
}
