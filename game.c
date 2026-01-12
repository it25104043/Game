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
#define LIFE_COUNT 2
#define INITIAL_LIVES 3

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
	int gridsize,numplayers,mode;
	
	srand(time(NULL));

	printf("================================================\n");
	printf("	SpyNet - The Codebreaker Protocool	\n");
	printf("================================================\n");

	gridsize=MIN_SIZE +rand()%(MAX_SIZE-)




return 1;
}
