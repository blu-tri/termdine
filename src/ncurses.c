#include <ncurses.h>
#include <string.h>

#define MAXTITLESIZE 14
#define MAXNAMESIZE  10
#define WIDTH 16
#define HEIGHT 9

/* type definitions */
typedef struct button 
{
	char name[MAXNAMESIZE];
} Button;

typedef struct menu 
{
	char title[MAXTITLESIZE];
	int buttonAmount;
	Button* buttons;
} Menu;

/* function definitions */
Menu createMenu(char* title, int buttonAmount, Button* buttons);
Button createButton(char* name);

WINDOW* mainWin;

int main(void)
{
	/* ncurses init */
	int c = 0;
	int selectedMenu   = 0;
	int selectedButton = 0;
	int running = TRUE;

	initscr();
	raw();
	noecho();
	
	keypad(stdscr, TRUE);
	timeout(10);
	
	curs_set(0);

	mainWin = newwin(9, 16, 0, 0);

	/* menu init */
	Button testButton = createButton("test");

	Button mainMenuButtons[3];
	mainMenuButtons[0] = testButton;
	mainMenuButtons[1] = testButton;
	mainMenuButtons[2] = testButton;

	Menu mainMenu = createMenu("Termdine", 3, mainMenuButtons);

	Menu menus[1];

	menus[0] = mainMenu;

	while (running)
	{
		wrefresh(mainWin);
		refresh();
		c = getch();
		
		/* input */
		if (c != ERR) {
			switch (c)
			{
				case KEY_UP: selectedButton -= 1; break;
				case KEY_DOWN: selectedButton += 1; break;
				case 'w': selectedButton -= 1; break;
				case 's': selectedButton += 1; break;
			}

			if (c == 'q' || c == 27) 
				running = FALSE;
		}

		/* logic */
		while (selectedButton<0||selectedButton>menus[selectedMenu].buttonAmount-1)
		{
			if (selectedButton<0)
				selectedButton += menus[selectedMenu].buttonAmount;
			if (selectedButton>menus[selectedMenu].buttonAmount-1)
				selectedButton -= menus[selectedMenu].buttonAmount;
		}

		/* drawing */
		mvprintw(0, 1, "%s", menus[selectedMenu].title);
		for (int i=0;i<menus[selectedMenu].buttonAmount;i++)
		{
			mvprintw(i+1, 1, "%s%s", (selectedButton==i ? "> " : ""), menus[selectedMenu].buttons[selectedButton].name);
		}
		box(mainWin, 0, 0);
	}

	endwin();

	return 0;
}

Menu createMenu(char* title, int buttonAmount, Button* buttons)
{
	Menu menu;

	menu.buttonAmount = buttonAmount;
	menu.buttons = buttons;

	memcpy(menu.title, title, MAXTITLESIZE);

	return menu;
}

Button createButton(char* name)
{
	Button button;

	memcpy(button.name, name, MAXNAMESIZE);

	return button;
}
