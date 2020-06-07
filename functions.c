#include "functions.h"

void positioning(WINDOW *menu, choice *option){
	int max_y, max_x;

	getmaxyx(menu, max_y, max_x);
	box(menu, 0, 0);

		option[0].pos_x = (max_x - strlen("Resume Game")) /2;
		option[0].pos_y = 3;
		strcpy(option[0].name, "Resume Game");

		option[1].pos_x = (max_x - strlen("New Game")) /2;
		option[1].pos_y = 6;
		strcpy(option[1].name, "New Game");

		option[2].pos_x = (max_x - strlen("Leaderboard")) /2;
		option[2].pos_y = 9;
		strcpy(option[2].name, "Leaderboard");

		option[3].pos_x = (max_x - strlen("Quit")) /2;
		option[3].pos_y = 12;
		strcpy(option[3].name, "Quit");

		
}

void initialize_menu(WINDOW *menu, choice *option, int selection, int Resume){
	int i, max_y, max_x;
	if(Resume == 0){
		wattron(menu, A_DIM);
		mvwprintw(menu, option[0].pos_y, option[0].pos_x, option[0].name);
		wattroff(menu, A_DIM);
		for(i = 1; i < 4; i++)
		{
			if(selection == i){
				wattron(menu, A_BLINK | A_UNDERLINE);
				mvwprintw(menu, option[i].pos_y, option[i].pos_x,
					option[i].name);
				wattroff(menu, A_BLINK | A_UNDERLINE);
			}
			else
				mvwprintw(menu, option[i].pos_y, option[i].pos_x,
					option[i].name);
		}
	}
	else
	{
		for(i = 0; i < 4; i++)
		{
			if(selection == i){
				wattron(menu, A_BLINK | A_UNDERLINE);
				mvwprintw(menu, option[i].pos_y, option[i].pos_x,
					option[i].name);
				wattroff(menu, A_BLINK | A_UNDERLINE);
			}
			else
				mvwprintw(menu, option[i].pos_y, option[i].pos_x,
					option[i].name);
		}
	}
	getmaxyx(stdscr, max_y, max_x);
	mvprintw((max_y - 4), (max_x - strlen("Use the arrow keys for navigation"))
		/ 2, "Use the arrow keys for navigation");
	mvprintw((max_y - 3), (max_x - strlen("Use ENTER for selection")) / 2,
		"Use ENTER for selection");
	attron( A_BOLD | COLOR_PAIR(7));
	mvprintw(0, (max_x - 47) / 2,
	 " _           _   _   _           _     _       ");
	mvprintw(1, (max_x - 47) / 2,
	 "| |         | | | | | |         | |   (_)      ");
	mvprintw(2, (max_x - 47) / 2,
	 "| |__   __ _| |_| |_| | ___  ___| |__  _ _ __  ");
	mvprintw(3, (max_x - 47) / 2,
	 "| '_ \\ / _` | __| __| |/ _ \\/ __| '_ \\| | '_ \\ ");
	mvprintw(4, (max_x - 47) / 2,
	 "| |_) | (_| | |_| |_| |  __/\\__ \\ | | | | |_) |");
	mvprintw(5, (max_x - 47) / 2,
	 "|_.__/ \\__,_|\\__|\\__|_|\\___||___/_| |_|_| .__/ ");
	mvprintw(6, (max_x - 47) / 2,
	 "                                        |_|    ");
	attroff( A_BOLD | COLOR_PAIR(7));
	refresh();
	wrefresh(menu);
}

int selection(WINDOW *menu, choice *option, int selection, int Resume){
	int key;
	noecho();
	cbreak();
	keypad(menu, TRUE);
	while(1){
		key = wgetch(menu);
		switch (key){
			case KEY_DOWN:{
				if(selection == 3)
					if(Resume == 0)
						selection = 1;
					else
						selection = 0;
				else
					selection += 1;
				break;
			}
			case KEY_UP:{
				if(selection == 1)
					if(Resume == 0)
						selection = 3;
					else
						selection = 0;
				else
					if(selection == 0)
						selection = 3;
					else 
						selection -= 1;
				break;
			}
			case 10:{
				return selection;
			}
		}
		initialize_menu(menu, option, selection, Resume);
	}
}

int menu_function(WINDOW *menu, choice *option, int choice, int Resume){
	positioning(menu, option);
	initialize_menu(menu, option, choice, Resume);
	return selection(menu, option, choice, Resume);
}

void print_board(int **board, WINDOW *game, mount_point start){
	int i, j, x, y;
	y = start.y;
	box(game, 0, 0);
	for(i = 0; i < 10; i++){
		x = start.x;
		for(j = 0; j < 9; j++){
			wattron(game, COLOR_PAIR(board[i][j] + 1));
			mvwprintw(game, y, x, "%c",int_to_char(board[i][j]));
			mvwprintw(game, y, x + 1, "%c",int_to_char(board[i][j]));
			mvwprintw(game, y + 1, x, "%c",int_to_char(board[i][j]));
			mvwprintw(game, y + 1, x + 1, "%c",int_to_char(board[i][j]));
			wattroff(game, COLOR_PAIR(board[i][j] + 1));

			wattron(game, COLOR_PAIR(1));
			mvwprintw(game, y, x + 2, "|");
			mvwprintw(game, y + 1, x + 2, "|");
			wattroff(game, COLOR_PAIR(1));
			x += 3;
		}
		wattron(game, COLOR_PAIR(board[i][j] + 1));
		mvwprintw(game, y, x, "%c",int_to_char(board[i][j]));
		mvwprintw(game, y, x + 1, "%c",int_to_char(board[i][j]));
		mvwprintw(game, y + 1, x, "%c",int_to_char(board[i][j]));
		mvwprintw(game, y + 1, x + 1, "%c",int_to_char(board[i][j]));
		wattroff(game, COLOR_PAIR(board[i][j] + 1));
		y += 2;
	}
	wrefresh(game);
}

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

void create_board(int **board, ship *ship){
	int i, k;
	srand(time(NULL));
	for(k = 0; k < 10; k++)
	{
		if(ship[k].orientation)
			{
				/*the ship receives a random mountpoint until it finds a
				valid position*/
				do{
					ship[k].mp.y = rand() % (11-ship[k].lenght) + 1;
					ship[k].mp.x = rand() % 10 + 1;
				}while(test(ship[k], board) != 1);
				/*around the ship it is created a border of values of 2, to
				make sure the ships are not touching one another*/
				for(i = 0; i < ship[k].lenght; i++ )
					{
						board[ship[k].mp.y+i][ship[k].mp.x] = 1;
						board[ship[k].mp.y+i][ship[k].mp.x - 1] = 2;
						board[ship[k].mp.y+i][ship[k].mp.x + 1] = 2;
					}

				for(i = -1; i <= 1; i++ ){
					if(board[ship[k].mp.y - 1][ship[k].mp.x + i] != 1)
						board[ship[k].mp.y - 1][ship[k].mp.x + i] = 2;
					if(board[ship[k].mp.y + ship[k].lenght][ship[k].mp.x + i]
						!= 1)
					board[ship[k].mp.y + ship[k].lenght][ship[k].mp.x + i] = 2;
				}
			}
		else
			{
				do{
					ship[k].mp.y = rand() % 10 + 1;
					ship[k].mp.x = rand() % (11-ship[k].lenght) + 1;
				}while(test(ship[k], board) != 1);

				for(i = 0; i < ship[k].lenght; i++ ){
					board[ship[k].mp.y][ship[k].mp.x + i] = 1;
					board[ship[k].mp.y +1][ship[k].mp.x + i] = 2;
					board[ship[k].mp.y -1][ship[k].mp.x + i] = 2;
				}

					for(i = -1; i <= 1; i++ ){
						if(board[ship[k].mp.y + i][ship[k].mp.x - 1] != 1)
							board[ship[k].mp.y + i][ship[k].mp.x - 1] = 2;
						if(board[ship[k].mp.y + i][ship[k].mp.x + 
							ship[k].lenght] != 1)

							board[ship[k].mp.y + i][ship[k].mp.x + 
								ship[k].lenght] = 2;
					}
			}
	}
}

int **start_board(int **board, ship *ship){
	int i, j, **aux;
	aux =(int **)malloc(10 * sizeof(int *));
	if(aux == NULL){
		endwin();
		printf("Nu s-a putut aloca memorie");
		exit(1);
	}
	for(i = 0; i < 10; i++){
		ship[i].mp.x -= 1;
		ship[i].mp.y -= 1;
	}
	free(board[0]);
	free(board[11]);
		for(i = 1; i <= 10; i++)
		{
			aux[i-1] =(int *)malloc(10 * sizeof(int));
			if(aux[i-1] == NULL){
				endwin();
				printf("Nu s-a putut aloca memorie");
				exit(1);
			}
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

mount_point game_movement(WINDOW *game, mount_point start_board, int **board,
	mount_point hitpoint, WINDOW *menu, choice *option, int *emergency_exit,
	int *destroy_in_advance, int *randomize_ships){

	int key;

	keypad(game, TRUE);
	while(1){

		key = wgetch(game);

		if(key == 'q' || key =='Q'){
			*emergency_exit = 1;
			return hitpoint;
		}
		if(key == 'd' || key =='D'){
			*destroy_in_advance = 11;
			return hitpoint;
		}
		if((key == 'r' || key =='R') && *randomize_ships != -1){
			*randomize_ships = 1;
			return hitpoint;
		}
			switch(key)
			{	case KEY_UP:{
					wattron(game, 
						COLOR_PAIR(board[hitpoint.y][hitpoint.x] + 1) );
					mvwprintw(game, start_board.y + 2 * hitpoint.y,
						(start_board.x + hitpoint.x * 3), "%c",
						int_to_char(board[hitpoint.y][hitpoint.x]));
					mvwprintw(game, start_board.y + 2 * hitpoint.y,
						(start_board.x + hitpoint.x * 3 + 1), "%c",
						int_to_char(board[hitpoint.y][hitpoint.x]));
					mvwprintw(game, start_board.y + 2 * hitpoint.y + 1,
						(start_board.x + hitpoint.x * 3), "%c",
						int_to_char(board[hitpoint.y][hitpoint.x]));
					mvwprintw(game, start_board.y + 2 * hitpoint.y + 1,
						(start_board.x + hitpoint.x * 3 + 1), "%c",
						int_to_char(board[hitpoint.y][hitpoint.x]));
					wattroff(game,
						COLOR_PAIR(board[hitpoint.y][hitpoint.x] + 1) );
					if(hitpoint.y == 0)
						hitpoint.y = 9;
					else
						hitpoint.y -= 1;
					break;
				}
				case KEY_DOWN:{
					wattron(game,
						COLOR_PAIR(board[hitpoint.y][hitpoint.x] + 1));
					mvwprintw(game, start_board.y + 2 * hitpoint.y,
						(start_board.x + hitpoint.x * 3), "%c", 
						int_to_char(board[hitpoint.y][hitpoint.x]));
					mvwprintw(game, start_board.y + 2 * hitpoint.y,
						(start_board.x + hitpoint.x * 3 + 1), "%c",
						int_to_char(board[hitpoint.y][hitpoint.x]));
					mvwprintw(game, start_board.y + 2 * hitpoint.y + 1,
						(start_board.x + hitpoint.x * 3), "%c",
						int_to_char(board[hitpoint.y][hitpoint.x]));
					mvwprintw(game, start_board.y + 2 * hitpoint.y + 1,
						(start_board.x + hitpoint.x * 3 + 1), "%c",
						int_to_char(board[hitpoint.y][hitpoint.x]));
					wattroff(game,
						COLOR_PAIR(board[hitpoint.y][hitpoint.x] + 1));
					if(hitpoint.y == 9)
						hitpoint.y = 0;
					else 
						hitpoint.y += 1;
					break;
				}
				case KEY_LEFT:{
					wattron(game, 
						COLOR_PAIR(board[hitpoint.y][hitpoint.x] +1));
					mvwprintw(game, start_board.y + 2 * hitpoint.y,
						(start_board.x + hitpoint.x * 3), "%c",
						int_to_char(board[hitpoint.y][hitpoint.x]));
					mvwprintw(game, start_board.y + 2 * hitpoint.y,
						(start_board.x + hitpoint.x * 3 + 1), "%c",
						int_to_char(board[hitpoint.y][hitpoint.x]));
					mvwprintw(game, start_board.y + 2 * hitpoint.y + 1,
						(start_board.x + hitpoint.x * 3), "%c",
						int_to_char(board[hitpoint.y][hitpoint.x]));
					mvwprintw(game, start_board.y + 2 * hitpoint.y + 1,
						(start_board.x + hitpoint.x * 3 + 1), "%c",
						int_to_char(board[hitpoint.y][hitpoint.x]));
					wattroff(game, 
						COLOR_PAIR(board[hitpoint.y][hitpoint.x] +1));
					if(hitpoint.x == 0)
						hitpoint.x = 9;
					else 
						hitpoint.x -= 1;
					break;
				}
				case KEY_RIGHT:{
					wattron(game, 
						COLOR_PAIR(board[hitpoint.y][hitpoint.x] +1));
					mvwprintw(game, start_board.y + 2 * hitpoint.y,
						(start_board.x + hitpoint.x * 3), "%c",
						int_to_char(board[hitpoint.y][hitpoint.x]));
					mvwprintw(game, start_board.y + 2 * hitpoint.y,
						(start_board.x + hitpoint.x * 3 + 1), "%c",
						int_to_char(board[hitpoint.y][hitpoint.x]));
					mvwprintw(game, start_board.y + 2 * hitpoint.y + 1,
						(start_board.x + hitpoint.x * 3), "%c",
						int_to_char(board[hitpoint.y][hitpoint.x]));
					mvwprintw(game, start_board.y + 2 * hitpoint.y + 1,
						(start_board.x + hitpoint.x * 3 + 1), "%c",
						int_to_char(board[hitpoint.y][hitpoint.x]));
					wattroff(game, 
						COLOR_PAIR(board[hitpoint.y][hitpoint.x] +1));
					if(hitpoint.x == 9)
						hitpoint.x = 0;
					else 
						hitpoint.x += 1;
					break;
				}
				case 10:{
				 	return hitpoint;
				}
			}
			update_point(board, game, start_board, hitpoint, 1);
	}
}

char int_to_char(int value){
	if(value == 0 || value == 2)
				return '~';
			else
				if(value == 1 || value == 3)
					return '#';
				else
					if(value == 4)
						return 'X';
					else
						return '-';
}

void hit(int **original_board, int **game_board, mount_point hitpoint){
	int x, y;
	x = hitpoint.x;
	y = hitpoint.y;
	if(original_board[y][x] < 2){
		original_board[y][x] += 2;
		game_board[y][x] = original_board[y][x];
	}
}

void update_point(int **board, WINDOW *game, mount_point start,
mount_point hitpoint, int highlight){

int x = hitpoint.x, y = hitpoint.y;
if(highlight){
	wattron(game,  A_REVERSE | A_BLINK | COLOR_PAIR(board[y][x] + 1));
	mvwprintw(game, 2 * y + start.y, 3 * x + start.x, "%c",
		int_to_char(board[y][x]));
	mvwprintw(game, 2 * y + start.y, 3 * x + start.x +1, "%c",
		int_to_char(board[y][x]));
	mvwprintw(game, 2 * y + start.y + 1, 3 * x + start.x, "%c",
		int_to_char(board[y][x]));
	mvwprintw(game, 2 * y + start.y + 1, 3 * x + start.x + 1, "%c",
		int_to_char(board[y][x]));
	wattroff(game,  A_REVERSE | A_BLINK | COLOR_PAIR(board[y][x] + 1));
}
else{
	wattron(game,  COLOR_PAIR(board[y][x] + 1));
	mvwprintw(game, 2 * y + start.y, 3 * x + start.x, "%c",
		int_to_char(board[y][x]));
	mvwprintw(game, 2 * y + start.y, 3 * x + start.x +1, "%c",
		int_to_char(board[y][x]));
	mvwprintw(game, 2 * y + start.y + 1, 3 * x + start.x, "%c",
		int_to_char(board[y][x]));
	mvwprintw(game, 2 * y + start.y + 1, 3 * x + start.x + 1, "%c",
		int_to_char(board[y][x]));
	wattroff(game, COLOR_PAIR(board[y][x] + 1));
}
wrefresh(game);
}

int ship_destroyed(WINDOW *game, ship *ship, int **surface_board, 
	mount_point surface_board_mnt, int **internal_board, int *end_game){

int i, j, ok, ships_down = 0;
mount_point update;
/*checking if any new whip has been destroyed*/
for(i = 0; i < 10; i++)
			if(ship[i].destroyed == 0){
				ok = 1;
				if(ship[i].orientation)
					{for(j = 0; j < ship[i].lenght; j++)
						if(surface_board[ship[i].mp.y + j][ship[i].mp.x] != 3)
						{
							ok = 0;
						}
					}
				else
					{for(j = 0; j < ship[i].lenght; j++)
						if(surface_board[ship[i].mp.y][ship[i].mp.x + j] != 3)
						{
							ok = 0;
						}
					}
				if (ok)
					ship[i].destroyed = 1;
				
				/*If any ship has been destroyed, its aspect is changed*/
				if(ok){
					if(ship[i].orientation)
						for(j = 0; j < ship[i].lenght; j++){
							internal_board[ship[i].mp.y + j][ship[i].mp.x] = 4;
							update.x = ship[i].mp.x;
							update.y = ship[i].mp.y + j;
							surface_board[update.y][update.x] = 4;
							update_point(surface_board, game, surface_board_mnt
								, update, 0);
						}
							
					else
						for(j = 0; j < ship[i].lenght; j++){
							internal_board[ship[i].mp.y][ship[i].mp.x + j] = 4;
							update.y = ship[i].mp.y;
							update.x = ship[i].mp.x + j;
							surface_board[update.y][update.x] = 4;
							update_point(surface_board, game, surface_board_mnt
								, update, 0);
						}
					}
				
			}
/*if all the ships of a player are destroyed the game is going to end*/
*end_game = 1;

/*the varaible ships_down is being updated, to print how many ships 
were destroyed*/
for(i = 0; i < 10; i++)
		if(ship[i].destroyed == 0)
			*end_game = 0;
		else
			if(ship[i].lenght == 4)
				ships_down += 1000;
			else
				if(ship[i].lenght == 3)
					ships_down += 100;
				else
					if(ship[i].lenght == 2)
						ships_down += 10;
					else
						ships_down += 1;
	wrefresh(game);
	return ships_down;
}

mount_point computer_choice(WINDOW *game, char *ship_detected, char *old_move,
	char *direction, char *next_move, char *first_hit, char *nr, int **board,
	char *free_locations){

char change_direction, random_choice, x, y;
mount_point choice;
srand(time(NULL));
	/*if the ships has ben destroyed the values are reset*/
	if(board[*old_move / 10][*old_move % 10] == 4){
		*ship_detected = 0;
		*direction = 0;
		*next_move = 1;
	}
	/*if a ship has not been found computer choses radom boxes*/
	if (*ship_detected != 1){
		random_choice = rand() % (*nr + 1);
		y = free_locations[random_choice] / 10;
		x = free_locations[random_choice] % 10;
		if (board[y][x] == 1)
			*ship_detected = 1;
		*first_hit = *old_move = free_locations[random_choice];	
	}
/*if a directions has been set the computer keeps hiting in that direction*/
	else{
		if(*direction){
			switch (*direction){
				case 1:{
					change_direction = 1;
					if(interval(0, 8, *old_move % 10))
						if(search_value(free_locations, *old_move + 1, *nr) ){
							change_direction = 0;
							*old_move += 1;

							if (board[*old_move / 10][*old_move % 10] != 1)
								*direction = 3;
						}
					if(change_direction){
						*direction = 3;
						*old_move = *first_hit;
						if(interval(1, 9, *old_move % 10)){
							if(search_value(free_locations, 
								*old_move - 1, *nr)){
								*old_move -= 1;
							}
						}
					}
					break;
				}
				case 2:{
					change_direction = 1;
					if(interval(0, 8, *old_move / 10))
						if(search_value(free_locations, *old_move + 10, *nr)){
							change_direction = 0;
							*old_move += 10;
							if (board[*old_move / 10][*old_move % 10] != 1)
								*direction = 4;
						}
					if(change_direction){
						*direction = 4;
						*old_move = *first_hit;
						if(interval(1, 9, *old_move / 10)){
							if(search_value(free_locations, 
								*old_move - 10, *nr)){
								*old_move -= 10;
							}
						}
					}
					break;
				}
				case 3:{
					if(*old_move > *first_hit)
						*old_move = *first_hit;
					if(interval(1, 9, *old_move % 10)){
						if(search_value(free_locations, *old_move - 1, *nr)){
							*old_move -= 1;
						}
					}
					break;
				}case 4:{
					if(*old_move + 9 > *first_hit)
						*old_move = *first_hit;
					if(interval(1, 9, *old_move / 10)){
						if(search_value(free_locations, *old_move - 10, *nr)){
							*old_move -= 10;
						}
					}
					break;
				}
			}
		}
	/*depending on the values near the first hit, a direction is being set*/
		else{
			switch (*next_move){
				case 1:{
					if(interval(0, 8, *first_hit % 10))
						if(search_value(free_locations, *first_hit + 1, *nr)){
							*old_move = *first_hit + 1;
							if (board[*old_move /10][*old_move % 10] == 1){
								*direction = 1;
							}
							else
								*next_move = 2;
							break;
						}
				}
				case 2:{
					if(interval(0, 8, *first_hit / 10))
						if(search_value(free_locations, *first_hit + 10, *nr)){
							*old_move = *first_hit + 10;
							if (board[*old_move /10][*old_move % 10] == 1){
								*direction = 2;
							}
							else
								*next_move = 3;
							break;
						}
				}
				case 3:{
					if(interval(1, 9, *first_hit % 10))
						if(search_value(free_locations, *first_hit - 1, *nr)){
							*old_move = *first_hit - 1;

							if (board[*old_move /10][*old_move % 10] == 1){
								*direction = 3;
							}
							else
								*direction = 4;
							break;
						}
				}
				case 4:{
					if(interval(1, 9, *first_hit / 10))
						if(search_value(free_locations, *first_hit - 10, *nr)){
							*old_move = *first_hit - 10;
							*direction = 4;
						}
				}

			}
		}
	}
	random_choice = search_value(free_locations, *old_move, *nr) - 1;
	free_locations[random_choice] = free_locations [*nr];
	*nr -= 1;
	choice.y = *old_move / 10;
	choice.x = *old_move % 10;
	return choice;
}

int search_value(char *vector, char searched_value, char vector_size){
	int i;
	for(i = 0; i <= vector_size; i++)
		if(searched_value == vector[i])
			return i + 1;
	return 0;
}

void set_mountpoints(WINDOW *win, mount_point *first, mount_point *second){
	int max_y, max_x;
	getmaxyx(win, max_y, max_x);
	second->y = first->y = 5;
	first->x = (max_x - 20) / 4;
	second->x = max_x - first->x - 20;
}

void read_matrix(int **board, ship *ship, char *file){

	int i, j, k, y = 0, x = 0, nr = 0, **board_copy;
	FILE *f;
	char c;

	f = fopen(file, "r");
	if(f == NULL){
		endwin();
		printf("[Eroare]:Fisierul %s nu poate fi deschis\n",file);
		exit(1);
	}
	for(i = 0; i < 10; i++){
		for(j = 0; j < 10; j++){
			fscanf(f, "|%c", &c);
			switch(c){
				case ' ':{
					board[i][j] = 0;
					break;}

				case '#':{
					board[i][j] = 1;
					break;}
			}
		}
		fscanf(f, "\n");
	}
	fclose(f);

	board_copy = (int **)malloc(11 * sizeof(int *));
	if(board_copy == NULL){
		endwin();
		printf("Nu s-a putut aloca memorie");
		exit(1);
	}
	for(i = 0; i < 10; i++){
		board_copy[i] = (int *)calloc(11, sizeof(int));
		if(board_copy[i] == NULL){
			endwin();
			printf("Nu s-a putut aloca memorie");
			exit(1);
		}
		for(j = 0; j < 10; j++)
			board_copy[i][j] = board[i][j];
	}
	board_copy[10] = (int *)calloc(11, sizeof(int));
	if(board_copy[10] == NULL){
		endwin();
		printf("Nu s-a putut aloca memorie");
		exit(1);
	}
		for(i = 0; i < 10; i++){
			for(j = 0; j < 10; j++)
			{
				if( board_copy[i][j] == 1){
					y = i;
					x = j;
					ship[nr].mp.y = y;
					ship[nr].mp.x = x;
					ship[nr].lenght = 1;
					ship[nr].destroyed = 0;

					if(board_copy[y][x + 1] == 1){

						ship[nr].orientation = 0;
						while(board_copy[y][x + ship[nr].lenght] == 1)
							ship[nr].lenght += 1;

						for(k = 0; k < ship[nr].lenght; k++)
							board_copy[y][x + k] = 0;
					}
					else{

						ship[nr].orientation = 1;
						while(board_copy[y + ship[nr].lenght][x] == 1)
							ship[nr].lenght += 1;

						for(k = 0; k < ship[nr].lenght; k++)
							board_copy[y + k][x] = 0;
					}

					nr ++;
				}
			}
	}
	for(i = 0; i < 11; i++)
		free(board_copy[i]);
	free(board_copy);
}

void randomize_ships_func(int **board, ship *ships){
	int i, j, y, x;
	for(i = 0; i < 10; i++)
		for(j = 0; j < 10; j++)
			if(board[i][j] != 4)
				board[i][j] = 0;

	for (i = 0; i < 10; i++)
			if(ships[i].orientation){
				y = ships[i].mp.y;
				x = ships[i].mp.x;

				for(j = 0; j < ships[i].lenght; j++ )
					{
						board[y +j][x] = 0;
					}
			}
			else{
				y = ships[i].mp.y;
				x = ships[i].mp.x;
				
				for(j = 0; j < ships[i].lenght; j++ )
				{
					board[y][x + j] = 0;
					
				}
			}
	for (i = 0; i < 10; i++)
		if(ships[i].destroyed == 0)
			if(ships[i].orientation)
			{
				do{
					ships[i].mp.y = rand() % (11-ships[i].lenght);
					ships[i].mp.x = rand() % 10;
				}while(test(ships[i], board) != 1);

				y = ships[i].mp.y;
				x = ships[i].mp.x;

				for(j = 0; j < ships[i].lenght; j++ )
					{
						board[y + j][x] = 1;
						if(interval(0, 9, x - 1))
							board[y + j][x - 1] = 2;
						if(interval(0, 9, x + 1))
							board[y + j][x + 1] = 2;
					}

				for(j = -1; j <= 1; j++ )
				{
					if(interval(0, 9, y - 1) && interval(0, 9, x + j))
						board[y - 1][x + j] = 2;
					if(interval(0, 9, y + ships[i].lenght) &&
						interval(0, 9, x + j))
						board[y + ships[i].lenght][x + j] = 2;
				}
			}
		else
			{
				do{
					ships[i].mp.x = rand() % (11-ships[i].lenght);
					ships[i].mp.y = rand() % 10;
				}while(test(ships[i], board) != 1);

				y = ships[i].mp.y;
				x = ships[i].mp.x;
				
				for(j = 0; j < ships[i].lenght; j++ )
				{
					board[y][x + j] = 1;
					if(interval(0, 9, y + 1))
						board[y +1][x + j] = 2;
					if(interval(0, 9, y - 1))
						board[y -1][x + j] = 2;
				}

					for(j = -1; j <= 1; j++ )
				{
					if(interval(0, 9, x -1) && interval(0, 9, y + j))
						board[y + j][x - 1] = 2;
					if(interval(0, 9, x + ships[i].lenght) &&
						interval(0, 9, y + j))
						board[y + j][x + ships[i].lenght] = 2;
				}
			}
	for(i = 0; i < 10; i++)
			for(j = 0; j < 10; j++)

				if( board[i][j] == 2)
					board[i][j] = 0;
}
int interval(int start, int end, int value){
	if(value >= start && value <= end)
		return 1;
	else
		return 0;
}

mount_point random_choice(char *free_locations, char *nr){
	srand(time(NULL));
	int random_choice;
	mount_point choice;

	random_choice = rand() % (*nr);
	choice.x = free_locations[random_choice] % 10;
	choice.y = free_locations[random_choice] / 10;
	free_locations[random_choice] = free_locations [*nr - 1];
	*nr -= 1;
return choice;
}

char *enter_name(WINDOW *game){
	char *name;
	int x, y, print_position, i;

	name =(char *)calloc(15, sizeof(char));
	if(name == NULL){
		endwin();
		printf("Nu s-a putut aloca memorie");
		exit(1);
	}
	getmaxyx(game, y, x);
	mvwprintw(game, y / 2, (x - strlen("Enter Your Name:")) / 2,
		"Enter Your Name:");
	mvwprintw(game, y / 2 + 2, (x - strlen("Press ENTER to continue")) / 2,
		"Press ENTER to continue");
	i = 0;
	print_position = (x - strlen("Enter Your Name:")) / 2 + 
		strlen("Enter Your Name:");
	do{
		name[i] = wgetch(game);
		mvwprintw(game, y / 2 + 4, (x -strlen("Invalid Character!")) / 2,
			"                     ");
		if(!interval('A','z',name[i]))
			mvwprintw(game, y / 2 + 4, (x -strlen("Invalid Character!")) / 2,
				"Invalid Character!");
		else{
				i += 1;
				mvwprintw(game, y / 2, print_position, "%c",name[i - 1]);
				print_position += 1;
		}
	if(name[0] == 10)
		name[i] = 11;
	wrefresh(game);
	}while(name[i] != 10 && i < 14);

	name[i] = 0;
	wrefresh(game);
	wclear(game);
	return name;
}

void read_leaderboard(WINDOW *win){
	FILE *leaderboard;
	int x, y;
	char key;

	leaderboard = fopen("leaderboard.txt","rb");
	if(leaderboard == NULL){
		void reset_leaderboard();
		leaderboard = fopen("leaderboard.txt","rb");
	}
	board_data *players;
	players = (board_data *)malloc(5 * sizeof(board_data));
	if(players == NULL){
		endwin();
		printf("Nu s-a putut aloca memorie");
		exit(1);
	}
	fread(players, sizeof(board_data), 5, leaderboard);
	fclose(leaderboard);
	getmaxyx(win, y, x);

	wattron(win, A_BOLD | A_REVERSE);
	mvwprintw(win, 3, (x - strlen("Leaderboard")) / 2, "Leaderboard");
	wattroff(win, A_REVERSE);

	for(int i = 0; i < 5; i++){
		mvwprintw(win, y / 2 - (2 - i) * 3, (x - (strlen(" Score: ") +
			strlen(players[i].name) + 3)) / 2, "%s Score: %d\n",
		players[i].name, players[i].score);
	}
	wattroff(win, A_BOLD);
	free(players);

	wattron(win, A_BLINK);
	mvwprintw(win, y - 2, (x - strlen("Press any key to go back")) / 2,
		"Press any key to go back");
	wattroff(win, A_BLINK);
	mvwprintw(win, y - 4, (x - strlen("Press R to reset the leaderboard")) / 2,
	"Press R to reset the leaderboard");

	wrefresh(win);
	key = wgetch(win);
	if(key == 'R' || key == 'r')
		reset_leaderboard();
	wclear(win);
	wrefresh(win);
}

void modify_leaderboard(char *player_name, int points){
	FILE *leaderboard;
	board_data *players, aux;
	int i;

	leaderboard = fopen("leaderboard.txt","rb");
	if(leaderboard == NULL){
		void reset_leaderboard();
		leaderboard = fopen("leaderboard.txt","rb");
	}

	players = (board_data *)malloc(6 * sizeof(board_data) );
	if(players == NULL){
		endwin();
		printf("Nu s-a putut aloca memorie");
		exit(1);
	}
	fread(players, sizeof(board_data), 5, leaderboard);
	fclose(leaderboard);
	strcpy(players[5].name, player_name);
	players[5].score = points;
	
	i = 5;
	while(players[i].score > players[i - 1].score){
		aux = players[i];
		players[i] = players [i -1];
		players[i -1] = aux;
		i -= 1;
		if (i == 0)
			break;
	}

	leaderboard = fopen("leaderboard.txt","wb");
	fwrite(players, sizeof(board_data), 5 ,leaderboard);
	fclose(leaderboard);
	free(players);
}
void reset_leaderboard(){
	FILE *leaderboard;
	board_data *players;
	int i;

	players = (board_data *)malloc(5 * sizeof(board_data) );
	if(players == NULL){
		endwin();
		printf("Nu s-a putut aloca memorie");
		exit(1);
	}

	for(i = 0; i < 5; i++){
		sprintf(players[i].name,"player %d",i);
		players[i].score = 0;
	}
	leaderboard = fopen("leaderboard.txt","wb");
	fwrite(players, sizeof(board_data), 5 ,leaderboard);
	fclose(leaderboard);
	free(players);
}

void print_statistics(WINDOW *game, int score, int ships_down, int *old_ships,
	char *player_name){
	int x, y;
	getmaxyx(game, y, x);
	mvwprintw(game, y - 4, 1, "Use key R to randomise your ships");
	mvwprintw(game, y - 3, 1, "Use key D to destroy 10 ships in advance");
	mvwprintw(game, y - 2, 1, "Use key Q to acces the menu");
	mvwprintw(game, 4, (x - strlen("Ship destroyed") + 5) / 2,
		"              ");
	mvwprintw(game, 2, (x - strlen("Score:") - 1 - strlen(player_name) ) / 2,
		"                              ");
	mvwprintw(game, 2, (x - strlen("Score:") - 1 - strlen(player_name) ) / 2,
		"%s's Score:%d", player_name, score);
	mvwprintw(game, 30, (x - strlen("Ship #### down 1 of 1")) / 2,
		"Ship #### down %d of 1", ships_down / 1000);
	mvwprintw(game, 31, (x - strlen("Ship #### down 1 of 1")) / 2,
		"Ship ###  down %d of 2", (ships_down / 100) % 10);
	mvwprintw(game, 32, (x - strlen("Ship #### down 1 of 1")) / 2,
		"Ship  ##  down %d of 3", (ships_down / 10) % 10);
	mvwprintw(game, 33, (x - strlen("Ship #### down 1 of 1")) / 2,
		"Ship   #  down %d of 4", ships_down % 10);
	if(*old_ships != ships_down){
		mvwprintw(game, 4, (x - strlen("Ship destroyed") + 5) / 2, 
			"Ship destroyed");
		*old_ships = ships_down;
	}
	wrefresh(game);
}

void free_memory(char *name, int **board_1, int **board_2, int **board_3,
	char *locations, ship *ship_1, ship *ship_2){
	int i;
	free(name);
	for(i = 0; i < 10; i++){
		free(board_1[i]);
		free(board_2[i]);
		free(board_3[i]);
	}
	free(board_1);
	free(board_2);
	free(board_3);
	free(ship_1);
	free(ship_2);
	free(locations);
}