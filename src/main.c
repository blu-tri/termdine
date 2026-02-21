#include <ncurses.h>

WINDOW* mainWin;

int main()
{
	int c;
	int running = TRUE;


	initscr();
	raw();
	keypad(mainWin, TRUE);
	noecho();
	nodelay(mainWin, TRUE);
	curs_set(0);

	mainWin = newwin(9, 16, 0, 0);
	box(mainWin, 0, 0);


	while (running == TRUE)
	{
		c = wgetch(mainWin);

		if (c == 'q' || c == 27) 
			running = FALSE;
	}

	wrefresh(mainWin);
	endwin();
}
