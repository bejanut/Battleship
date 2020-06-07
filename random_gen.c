#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct
{
	int x;
	int y;
}mount_point;

typedef struct
{
	int lenght;
	int orientation;
	mount_point mp;
	char destroyed;
}ship;

void initialize_ships(ship ship[]){
	int i, j, nr = 0;

	srand (time(NULL));
	for(i = 0; i < 4; i++)
		for(j = 0; j <= i; j++)
		{
			ship[nr].destroyed = 0;
			ship[nr].lenght = 4-i;
			ship[nr].orientation = rand() % 2;
			nr ++;
		}
}

int test(ship ship, int **board){
	int i, ok = 1;

	if(ship.orientation)

		for(i = 0; i < ship.lenght; i++)
		{
			if(board[ship.mp.y + i][ship.mp.x] != 0)
			{
				ok = 0; 
				break;
			}
		}
	else
		for(i = 0; i < ship.lenght; i++)
		{
			if(board[ship.mp.y][ship.mp.x + i] != 0)
			{
				ok = 0; 
				break;
			}
		}

	return ok;
}

int **start_board(int **board, ship *ship){
	int i, j, **aux;
	aux =(int **)malloc(10 * sizeof(int *));
	for(i = 0; i < 10; i++){
		ship[i].mp.x -= 1;
		ship[i].mp.y -= 1;
	}
	free(board[0]);
	free(board[11]);
		for(i = 1; i <= 10; i++)
		{
			aux[i-1] =(int *)malloc(10 * sizeof(int));
			for(j = 1; j <= 10; j++)
				if( board[i][j] == 2)
					aux[i-1][j-1] = 0;
				else
					aux[i-1][j-1] = board[i][j];
			free(board[i]);
		}
	free(board);
	return aux;
}

void create_board(int **board, ship *ship){
	int i, k;
	srand(time(NULL));
	for(k = 0; k < 10; k++)
	{
		if(ship[k].orientation)
			{
				do{
					ship[k].mp.y = rand() % (11-ship[k].lenght) + 1;
					ship[k].mp.x = rand() % 10 + 1;
				}while(test(ship[k], board) != 1);

				for(i = 0; i < ship[k].lenght; i++ )
					{
						board[ship[k].mp.y+i][ship[k].mp.x] = 1;
						board[ship[k].mp.y+i][ship[k].mp.x - 1] = 2;
						board[ship[k].mp.y+i][ship[k].mp.x + 1] = 2;
					}

				for(i = -1; i <= 1; i++ )
				{
					if(board[ship[k].mp.y - 1][ship[k].mp.x + i] != 1)
						board[ship[k].mp.y - 1][ship[k].mp.x + i] = 2;
					if(board[ship[k].mp.y + ship[k].lenght][ship[k].mp.x + i] != 1)
						board[ship[k].mp.y + ship[k].lenght][ship[k].mp.x + i] = 2;
				}
			}
		else
			{
				do{
					ship[k].mp.y = rand() % 10 + 1;
					ship[k].mp.x = rand() % (11-ship[k].lenght) + 1;
				}while(test(ship[k], board) != 1);

				for(i = 0; i < ship[k].lenght; i++ )
				{
					board[ship[k].mp.y][ship[k].mp.x + i] = 1;
					board[ship[k].mp.y +1][ship[k].mp.x + i] = 2;
					board[ship[k].mp.y -1][ship[k].mp.x + i] = 2;
				}

					for(i = -1; i <= 1; i++ )
				{
					if(board[ship[k].mp.y + i][ship[k].mp.x - 1] != 1)
						board[ship[k].mp.y + i][ship[k].mp.x - 1] = 2;
					if(board[ship[k].mp.y + i][ship[k].mp.x + ship[k].lenght] != 1)
						board[ship[k].mp.y + i][ship[k].mp.x + ship[k].lenght] = 2;
				}
			}
	}
}
char int_to_char(int value){
	if(value == 0 || value == 2)
				return ' ';
			else
				if(value == 1 || value == 3)
					return '#';
}

int main(){
	ship computer_ship[10];
	int **computer_board,i;

	computer_board = (int **)malloc(12 * sizeof(int *));
	for(i = 0; i < 12; i++)
		computer_board[i] = (int *)calloc(12, sizeof(int));

	initialize_ships(computer_ship);
	create_board(computer_board, computer_ship);
	computer_board = start_board(computer_board, computer_ship);
	for(i = 0; i < 10; i++){
		for(int j = 0; j < 10; j++ )
			printf("|%c",int_to_char(computer_board[i][j]));
		printf("\n");
	}
}