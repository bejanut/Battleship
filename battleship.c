#include "functions.h"


#define M_HEIGHT 16
#define M_WIDTH 30

int start_new_game(WINDOW *game, WINDOW *menu, choice *option, 
	char *player_file);

int main(int argc, char **argv)
{	
	WINDOW *menu, *game;
	choice *option;
	int max_x, max_y, test_for_new_game = 0, random_file;
	if(argc == 1){
		printf("[Eroare]:Nu a fost precizat niciun fisier pentru harta\n");
		exit (1);
	}
/*initialization part, where we make sure everything will be propery displayed*/
	initscr();
	clear();
	noecho();
	cbreak();
	start_color();
	curs_set(0);
/*Initializind all the color pairs that will be used throughout the game*/
	init_pair(1, COLOR_BLACK, COLOR_BLUE);
	init_pair(2, COLOR_YELLOW, COLOR_BLUE);
	init_pair(3, COLOR_YELLOW, COLOR_BLUE);
	init_pair(4, COLOR_RED, COLOR_BLUE);
	init_pair(5, COLOR_RED, COLOR_BLUE);
	init_pair(6, COLOR_GREEN, COLOR_BLACK);
	init_pair(7, COLOR_RED, COLOR_BLACK);

	option =(choice *)malloc(4 * sizeof(choice));
/*Seting the positions for the windows in witch the game and the menu will
be displayed*/
	getmaxyx(stdscr, max_y, max_x);

	menu = newwin(M_HEIGHT, M_WIDTH, (max_y - M_HEIGHT) / 2,
		(max_x - M_WIDTH) / 2);
	game = newwin(max_y, max_x, 0, 0);
/*Making sure we can use special keys*/
	keypad(menu, TRUE);
	keypad(game, TRUE);
	srand(time(NULL));
	/*chosing a random map from the list of maps provided by the player*/
	menu_label:random_file = random() % (argc - 1) + 1; 
	/*setting a label because after the leaderboard is shown
the program goes back to the menu if the new game option was selected from
an ongoing game (test_for_new_game = 1) the menu wil now be displayed*/
	if(test_for_new_game == 1)
		test_for_new_game = start_new_game(game, menu, option,
			argv[random_file]);
	else
		switch (menu_function(menu, option, 1, 0))
		{
			case 0:
			{
				break;
			}
			case 1:
			{
				test_for_new_game = start_new_game(game, menu, option,
					argv[random_file]);
				break;
			}
			case 2:
			{
				wclear(menu);
				wclear(game);
				read_leaderboard(game);
				goto menu_label;
				break;
			}
			case 3:
			{
				endwin();
				exit(0); 
			}
		}
	/*In order for the game to be played multiple times it is tested if the
start_new game function returned a true value and the program goes back to
the menu*/
	if(test_for_new_game)
		goto menu_label;

	endwin();
	free(option);
	
	return 0;
}
/*the function responsible for the game itself*/
int start_new_game(WINDOW *game, WINDOW *menu, choice *option, 
	char *player_file) {
/*declaration part*/
	int **computer_board, **first_board, **player_board, i, j,
	check, emergency_exit = 0, end_game = 0,  destroy_in_advance = 0,
	randomize_ships = 0, x, y, score = 100, ships_down = 0, old_ships = 0;

	char *free_locations, turn = 1, *positions, positions_size = 0,
	free_locations_size = 99, win = 0, key, *name, ship_detected_2 = 0,
	old_move_2 = 0, direction_2 = 0, next_move_2 = 1, first_hit_2;

	mount_point first_board_mnt, second_board_mnt, hitpoint_1, hitpoint_2;
	ship *computer_ship, *player_ship;
/*making sure nothing is desplayed on the screen*/
	wclear(game);
/*setting the mountpoints for the 2 boards*/
	set_mountpoints(game, &first_board_mnt, &second_board_mnt);
/*alocating memory*/
	computer_ship = (ship *)malloc(10 * sizeof(ship));

	player_ship = (ship *)malloc(10 * sizeof(ship));

	computer_board = (int **)malloc(12 * sizeof(int *));
	for(i = 0; i < 12; i++)
		computer_board[i] = (int *)calloc(12, sizeof(int));

	first_board = (int **)malloc(10 * sizeof(int *));
	for(i = 0; i < 10; i++)
		first_board[i] = (int *)calloc(10, sizeof(int));

	player_board = (int **)malloc(10 * sizeof(int *));
	for(i = 0; i < 10; i++)
		player_board[i] = (int *)calloc(10, sizeof(int));

	free_locations = (char *)malloc(100 * sizeof(int));
/*creating a vector with all posible locations on a board*/
	for(i = 0; i < 100; i++){
		free_locations[i] = i;
	}

	read_matrix(player_board, player_ship, player_file);
	box(game, 0, 0);
	name = enter_name(game);
	initialize_ships(computer_ship);
	create_board(computer_board, computer_ship);
	computer_board = start_board(computer_board, computer_ship);

	hitpoint_1.x = 0;
	hitpoint_1.y = 0;
	hitpoint_2 = hitpoint_1;

	print_board(first_board, game, first_board_mnt);
	print_board(player_board, game, second_board_mnt);
	update_point(first_board, game, first_board_mnt, hitpoint_1, 1);
/*players take turns depending on the variable turn, until the game ends*/
	while(end_game != 1){
		if(turn){
			do{
				print_statistics(game, score, ships_down, &old_ships, name);
				/*if destory in advance is not activated the player is free to
				move on the board and choose his point to hit*/
				if(!destroy_in_advance){
					hitpoint_1 = game_movement(game, first_board_mnt, 
						first_board, hitpoint_1, menu, option, &emergency_exit,
					 	&destroy_in_advance, &randomize_ships);
				}

					if(destroy_in_advance == 11){
						positions_size = 0;
						positions = (char *)calloc(100, sizeof(char));
						for(i = 0; i < 10; i++)
							for(j = 0; j < 10; j++)
								if(first_board[i][j] == 0){
									positions[positions_size] = i * 10 + j;
									positions_size += 1;
								}
						if(positions_size)
							positions = realloc(positions, positions_size * 
								sizeof(char));
						destroy_in_advance = 10;
					}
				if(destroy_in_advance > 0 && destroy_in_advance <= 10){
					update_point(first_board, game, first_board_mnt,
						hitpoint_1, 0);
					if(positions_size)
						hitpoint_1 = random_choice(positions, &positions_size);
					destroy_in_advance -= 1;

					if(destroy_in_advance == 0){
						free(positions);
						positions_size = 0;
					}
				}
	/*the menu is being displayed without exiting the function in order to
	resume the game more easily*/
	menu_label:if(emergency_exit == 1)
				{
					wclear(game);
					wrefresh(game);
					switch (menu_function(menu, option, 1, 1))
					{
						case 0:
						{
							wclear(menu);
							wclear(game);
							print_board(first_board, game, first_board_mnt);
							print_board(player_board, game, second_board_mnt);
							print_statistics(game, score, ships_down, 
								&old_ships, name);
							emergency_exit = 0;
							break;
						}
						case 1:
						{
							free_memory(name, player_board, computer_board,
								first_board, free_locations, player_ship,
								computer_ship);
							return 1;
						}
						case 2:
						{
							wclear(menu);
							wclear(game);
							read_leaderboard(game);
							goto menu_label;
							break;
						}
						case 3:
						{
							free_memory(name, player_board, computer_board,
								first_board, free_locations, player_ship,
								computer_ship);
							endwin();
							exit(0); 
						}
					}
				}
				/*tests if randomize ships was activated*/
				if(randomize_ships == 1){
					randomize_ships_func(player_board, player_ship);
					print_board(player_board, game, second_board_mnt);
					free_locations_size = 0;
					for(i = 0; i < 10; i++)
							for(j = 0; j < 10; j++){
								free_locations[free_locations_size] = 
								i * 10 + j;
								free_locations_size += 1;
							}
					free_locations_size -= 1;
					randomize_ships = -1;
				}
				/*The score is being updated depending on the hit*/
				check = computer_board[hitpoint_1.y][hitpoint_1.x];
				if(check == 1)
					score += 5;
				else
					if(check == 0)
						score -= 1;

				hit(computer_board, first_board, hitpoint_1);
				ships_down = ship_destroyed(game, computer_ship, first_board, 
					first_board_mnt, computer_board, &end_game);
				update_point(first_board, game, first_board_mnt, hitpoint_1, 1);

				if(end_game == 1){
					win = 1;
					break;
				}
				
			}while(check != 0);
			turn = 0;
		}
		else{
			do{
				/*if destory in advance is activated the computer turns are
				instant*/
				if(!destroy_in_advance)
					sleep(1);
				update_point(player_board, game, second_board_mnt, 
					hitpoint_2, 0);
			/*an emergency exit in case there are no more positions to hit*/
				if (free_locations_size == -1){
					break;
				}
				hitpoint_2 = computer_choice(game, &ship_detected_2,
					&old_move_2, &direction_2, &next_move_2, &first_hit_2,
					&free_locations_size, player_board, free_locations);

				hit(player_board, player_board, hitpoint_2);
				update_point(player_board, game, second_board_mnt,
					hitpoint_2, 1);

				ship_destroyed(game, player_ship, player_board,
					second_board_mnt, player_board, &end_game);

			}while(player_board [hitpoint_2.y][hitpoint_2.x] >= 3);
			turn = 1;

		}
	}
	/*if the game has finished before destory in advance was finised,
	the memory from the player's random moves is freed*/
	if(destroy_in_advance != 0)
		free(positions);

	wclear(game);
	getmaxyx(game,y,x);
/*endgame screen printed depending on the value of win*/
	if(win){
		wattron(game, A_BOLD | COLOR_PAIR(6));
		mvwprintw(game ,y / 2 - 3, (x - 72) / 2,
	"   _____                            _         _       _   _             ");
		mvwprintw(game ,y / 2 - 2, (x - 72) / 2,
"  / ____|                          | |       | |     | | (_)            ");
		mvwprintw(game ,y / 2 - 1, (x - 72) / 2,
" | |     ___  _ __   __ _ _ __ __ _| |_ _   _| | __ _| |_ _  ___  _ __  ");
		mvwprintw(game ,y / 2, (x - 72) / 2,
" | |    / _ \\| '_ \\ / _` | '__/ _` | __| | | | |/ _` | __| |/ _ \\| '_ \\ ");
		mvwprintw(game ,y / 2 + 1, (x - 72) / 2,
" | |___| (_) | | | | (_| | | | (_| | |_| |_| | | (_| | |_| | (_) | | | |");
		mvwprintw(game ,y / 2 + 2, (x - 72) / 2,
"  \\_____\\___/|_| |_|\\__, |_|  \\__,_|\\__|\\__,_|_|\\__,_|\\__|_|\\___/|_| |_|");
		mvwprintw(game ,y / 2 + 3, (x - 72) / 2,
"                     __/ |                                              ");
		mvwprintw(game ,y / 2 + 4, (x - 72) / 2,
"                    |___/                                               ");
			mvwprintw(game, y / 2 + 6, (x - strlen("Your Score is:   ") ) / 2,
				"Your Score is: %d",score);
		wattroff(game, A_BOLD);

		wattron(game, A_BLINK);
			mvwprintw(game, y / 2 + 8, (x - strlen("Press ENTER to continue"))
			/ 2, "Press ENTER to continue");
		wattroff(game, A_BLINK | COLOR_PAIR(6));
		modify_leaderboard(name, score);
	}
	else{
		wattron(game, A_BOLD | COLOR_PAIR(7));
		mvwprintw(game ,y / 2 - 3, (x - 52) / 2,
			"   _____                                            ");
		mvwprintw(game ,y / 2 - 2, (x - 52) / 2,
			"  / ____|                                           ");
		mvwprintw(game ,y / 2 - 1, (x - 52) / 2,
			" | |  __  __ _ _ __ ___   ___    _____   _____ _ __ ");
		mvwprintw(game ,y / 2, (x - 52) / 2,
			" | | |_ |/ _` | '_ ` _ \\ / _ \\  / _ \\ \\ / / _ \\ '__|");
		mvwprintw(game ,y / 2 + 1, (x - 52) / 2,
			" | |__| | (_| | | | | | |  __/ | (_) \\ V /  __/ |   ");
		mvwprintw(game ,y / 2 + 2, (x - 52) / 2,
			"  \\_____|\\__,_|_| |_| |_|\\___|  \\___/ \\_/ \\___|_|   ");
		wattroff(game, A_BOLD | COLOR_PAIR(7));

		wattron(game, A_BLINK | COLOR_PAIR(6));
			mvwprintw(game, y / 2 + 5, (x - strlen("Press ENTER to continue"))
				/ 2, "Press ENTER to continue");
		wattroff(game, A_BLINK | COLOR_PAIR(6));
	}

	wrefresh(game);

	do{
		key = wgetch(game);
	}while(key != 10);

	wclear(game);
	wrefresh(game);
	free_memory(name, player_board, computer_board, first_board,
		free_locations, player_ship, computer_ship);
	return 2;
}


