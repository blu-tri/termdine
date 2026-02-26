#define _GNU_SOURCE
#include <ncurses.h>
#include <time.h>

WINDOW* mainWin;

int main(void)
{
	int c,i,j=0;
	int running = TRUE;

	initscr();
	raw();
	noecho();
	
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	
	curs_set(0);

	mainWin = newwin(9, 16, 0, 0);
	box(mainWin, 0, 0);


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
		mvprintw(0, 1, "test");
		mvwprintw(mainWin, 1, 1, "%d", i);
		box(mainWin, 0, 0);

		struct timespec time = {0, 750000};
		nanosleep(&time, NULL);

		i++;

	}

	endwin();

	return 0;
}
