#ifndef __STDLIB__
#define __STDLIB__

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

typedef struct
{
	int pos_x, pos_y;
	char name[12];
}choice;

typedef struct{
char name[15];
int score;
}board_data;

/*Initializing positions for the strings printed in the menu*/
void positioning(WINDOW *menu, choice *option);

/*Function used for printing the menu options and highlighting the use choice*/
void initialize_menu(WINDOW *menu, choice *option, int selection, int Resume);

/*Function responsible for the movement through the menu*/
int selection(WINDOW *menu, choice *option, int selection, int Resume);

/*Transforming an integer to a specific char, to be printed on the screen*/
char int_to_char(int value);

/*A combinations of the previous menu functions, which creates the whole menu
and returns the user selection*/
int menu_function(WINDOW *menu, choice *option, int choice, int Resume);

/*Function used to print a gameboard, at a specific mountpoint*/
void print_board(int **board, WINDOW *game, mount_point start);

/*Initializing all the atributes a vector of 10 ships*/
void initialize_ships(ship ship[]);

/*Checking if a mountpoit for a ship is a valid one*/
int test(ship ship, int **board);

/*Function in witch ships are put in a random combination on the board,
acording to the rules*/
void create_board(int **board, ship *ship);

/*Function that shrinks the initial 12X12 board used for positioning the ships
to a 10X10 board and also removes auxiliary values for the board*/
int **start_board(int **board, ship *ship);

/*Function that allows the user to move on the board and returns the positions
chosen with the ENTER key. This function also records if the user wants to 
acces the menu or to use a special function.*/
mount_point game_movement(WINDOW *game, mount_point start_board, int **board,
	mount_point hitpoint, WINDOW *menu, choice *option, int *emergency_exit,
	int *destroy_in_advance, int *randomize_ships);

/*The value translated from the hitpoint is incremented by 2*/
void hit(int **original_board, int **game_board, mount_point hitpoint);

/*Updates a specific point specified by a hitpoit. The function will
highlight the point dependind on the value of highlight*/
void update_point(int **board, WINDOW *game, mount_point start,
	mount_point hitpoint, int highlight);

/*Checks if a new ship is destroyed and updates is. It also return a special
value used to print how many ships were destroeyd and it also signals the end
of the game*/
int ship_destroyed(WINDOW *game, ship *ship, int **surface_board,
	mount_point surface_board_mnt, int **internal_board, int *end_game);

/*Function used for the choices of the computer, alowing him to make inteligent
moves*/
mount_point computer_choice(WINDOW *game, char *ship_detected, char *old_move,
	char *direction, char *next_move, char *first_hit, char *nr, int **board,
	char *options);

/*A value is searched in a vector, if found the postion + 1 is returned,
this was used to leave 0 as a return in case of not found*/
int search_value(char *vector, char searched_value, char vector_size);

/*Function that sets mountpoints for the 2 boards*/
void set_mountpoints(WINDOW *win, mount_point *first, mount_point *second);

/*The board is translated from the file into a matrix of integer values*/
void read_matrix(int **board, ship *ship, char *file);

/*Function that takes the player's ships and puts them in a new random
position*/
void randomize_ships_func(int **board, ship *ship);

/*Checks is a value is within a specific interval*/
int interval(int start, int end, int value);

/*Returns a random mountpoint translated from a vector*/
mount_point random_choice(char *options, char *nr);

/*Reads and prints the name of a player*/
char *enter_name(WINDOW *game);

/*Reads the leaderboard and allows the user to reset it*/
void read_leaderboard(WINDOW *win);

/*Checks if the given score is greater than the scores already in the
leaderboard and adds it in there if it is*/
void modify_leaderboard(char *name, int score);

/*Resets the leaderboard*/
void reset_leaderboard();

/*Prints the player's score and name, how many ships were destroyed and what
other options the player has*/
void print_statistics(WINDOW *game, int score, int ships_down, int *old_ships,
	char *player_name);

/*Frees teh dinamicaly alocated memory*/
void free_memory(char *name, int **board_1, int **board_2, int **board_3,
	char *locations, ship *ship_1, ship *ship_2);

#endif
