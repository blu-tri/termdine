#include <ncurses.h>

WINDOW* mainWin;

int main(void)
{
	int c;
	int i=0;
	int running = TRUE;

	initscr();
	nodelay(stdscr, TRUE);
	raw();
	keypad(stdscr, TRUE);
	noecho();
	
	curs_set(0);

	mainWin = newwin(9, 16, 0, 0);
	box(mainWin, 0, 0);


	while (running)
	{
		wrefresh(mainWin);
		refresh();
		c = wgetch(mainWin);
		
		/* input
		switch (c)
		{
			case 'c':
				mvprintw(0, 1, "test");
		}*/

		/* drawing */
		mvprintw(0, 1, "test");
		mvwprintw(mainWin, 1, 1, "%d", i);
		box(mainWin, 0, 0);
		
		if (c == 'q' || c == 27) 
			running = FALSE;
		i++;
	}

	endwin();

	return 0;
}
