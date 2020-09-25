#define _XOPEN_SOURCE

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <curses.h>
#include <menu.h>

#define CTRLD 	4

char *choices[] = {
						"Програмувати GBIC      ",
						"Консоль COM-порта CISCO",
						"Відкрити термінал      ",
						"Вимкнути               ",
						(char *)NULL,
				  };

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

int main()
{
	ITEM **my_items;
	int c;
	MENU *my_menu;
	WINDOW *my_menu_win;
	int n_choices, i;
	setlocale(LC_ALL,"");

	/* Initialize curses */
	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	init_pair(3, COLOR_CYAN, COLOR_BLACK);

	/* Create items */
	n_choices = sizeof(choices) / sizeof(choices[0]);
	my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));

	for(i = 0; i < n_choices; ++i)
	{
		my_items[i] = new_item(choices[i], "");
	}

	/* Crate menu */
	my_menu = new_menu((ITEM **)my_items);

	set_menu_fore(my_menu, COLOR_PAIR(1));
	set_menu_back(my_menu, A_BOLD | COLOR_PAIR(2));

	int window_width = 27;

	/* Create the window to be associated with the menu */
	my_menu_win = newwin(8, window_width, 8, 27);
	keypad(my_menu_win, TRUE);

	/* Set main window and sub window */
	set_menu_win(my_menu, my_menu_win);
	set_menu_sub(my_menu, derwin(my_menu_win, 4, window_width-2, 3, 1));

	/* Set menu mark to the string " " */
	set_menu_mark(my_menu, " ");

	/* Print a border around the main window and print a title */
	box(my_menu_win, 0, 0);
	print_in_middle(my_menu_win, 1, 2, 33, "Оберіть дію", COLOR_PAIR(3));

	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, window_width-2);
	mvwaddch(my_menu_win, 2, window_width-1, ACS_RTEE);
	refresh();

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_menu_win);

	int choice = 0;

	while((c = wgetch(my_menu_win)) != 27)
	{

		switch(c)
		{
			case KEY_DOWN:
				choice++;
				menu_driver(my_menu, REQ_DOWN_ITEM);
			break;

			case KEY_UP:
				choice--;
				menu_driver(my_menu, REQ_UP_ITEM);
			break;

			case 10:

				switch( choice )
				{

					case 0:
						unpost_menu(my_menu);
						free_menu(my_menu);

						for(i = 0; i < n_choices; ++i)
						{
							free_item(my_items[i]);
						}

						endwin();

						system("~/gbic");

						return 0;
					break;

					case 1:
						unpost_menu(my_menu);
						free_menu(my_menu);

						for(i = 0; i < n_choices; ++i)
						{
							free_item(my_items[i]);
						}

						endwin();

						system("minicom");

						return 0;
					break;

					case 3:
						unpost_menu(my_menu);
						free_menu(my_menu);

						for(i = 0; i < n_choices; ++i)
						{
							free_item(my_items[i]);
						}

						endwin();

						system("poweroff");

						return 0;
					break;

					case 2:
						unpost_menu(my_menu);
						free_menu(my_menu);

						for(i = 0; i < n_choices; ++i)
						{
							free_item(my_items[i]);
						}

						endwin();

						return 0;
					break;
				}
			break;
		}

		wrefresh(my_menu_win);
	}	

	/* Unpost and free all the memory taken up */
	unpost_menu(my_menu);
	free_menu(my_menu);

	for(i = 0; i < n_choices; ++i)
	{
		free_item(my_items[i]);
	}

	endwin();
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{
	int length, x, y;
	float temp;

	if(win == NULL)
	{
		win = stdscr;
	}

	getyx(win, y, x);

	if(startx != 0)
	{
		x = startx;
	}

	if(starty != 0)
	{
		y = starty;
	}

	if(width == 0)
	{
		width = 80;
	}

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win, color | A_BOLD );
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color | A_BOLD );
	refresh();
}
