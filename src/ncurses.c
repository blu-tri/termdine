#include <ncurses.h>

WINDOW* mainWin;

int main(void)
{
	int c=0;
	int i=0;
	int j=0;
	int running = TRUE;

	initscr();
	raw();
	noecho();
	
	keypad(stdscr, TRUE);
	timeout(10);
	
	curs_set(0);

	mainWin = newwin(9, 16, 0, 0);

	while (running)
	{
		wrefresh(mainWin);
		refresh();
		c = getch();
		
		/* input */
		if (c != ERR) {
			switch (c)
			{
				case 'a': j+=1; break; 
			}

			if (c == 'q' || c == 27) 
				running = FALSE;
		}

		/* drawing */
		mvwprintw(mainWin, 1, 1, "%d", i);
		mvwprintw(mainWin, 2, 1, "%d", j);


		mvprintw(0, 1, "test");
		box(mainWin, 0, 0);

		i++;

	}

	endwin();

	return 0;
}
