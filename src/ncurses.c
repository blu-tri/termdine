#include <ncurses.h>
#include <string.h>

#define MAXTITLESIZE 14
#define MAXNAMESIZE  10

#define MAXMENUS 5

#define WIDTH 16
#define HEIGHT 9

/* type definitions */
typedef enum action
{
	QUIT,
	GOTOMENU,
	NOTHING,
} Action;

typedef struct button 
{
	char name[MAXNAMESIZE];
	Action action;
	int actionInput;
} Button;

typedef struct menu 
{
	char title[MAXTITLESIZE];
	int buttonAmount;
	Button* buttons;
} Menu;

typedef struct app
{
	int running;
	int selectedMenu;
	int selectedButton;
} App;

/* function definitions */
Menu createMenu(char* title, int buttonAmount, Button* buttons);
Button createButton(char* name, Action action, int actionInput);
void doButtonAction(App* app, Button button);
App initApp(void);

WINDOW* mainWin;

int main(void)
{
	/* ncurses init */
	int c = 0;
	App termdine = initApp();

	initscr();
	raw();
	noecho();
	
	keypad(stdscr, TRUE);
	timeout(10);
	
	curs_set(0);

	mainWin = newwin(9, 16, 0, 0);

	/* menu init */
	Button testButton = createButton("test", NOTHING, 0);
	Button quitButton = createButton("quit", QUIT, 0);


	Button mainMenuButtons[3];
	mainMenuButtons[0] = testButton;
	mainMenuButtons[1] = testButton;
	mainMenuButtons[2] = quitButton;

	Menu mainMenu = createMenu("Termdine", 3, mainMenuButtons);

	Menu menus[1];

	menus[0] = mainMenu;

	while (termdine.running)
	{
		wrefresh(mainWin);
		refresh();
		c = getch();
		
		/* input */
		if (c != ERR) {
			switch (c)
			{
				case KEY_UP: termdine.selectedButton -= 1; break;
				case KEY_DOWN: termdine.selectedButton += 1; break;
				case 'w': termdine.selectedButton -= 1; break;
				case 's': termdine.selectedButton += 1; break;
				case 'e': doButtonAction(&termdine, menus[termdine.selectedMenu].buttons[termdine.selectedButton]); break; 
				case KEY_ENTER: doButtonAction(&termdine, menus[termdine.selectedMenu].buttons[termdine.selectedButton]); break; 
			}

			if (c == 'q' || c == 27) 
				termdine.running = FALSE;
		}

		/* logic */
		while (termdine.selectedButton<0||termdine.selectedButton>menus[termdine.selectedMenu].buttonAmount-1)
		{
			if (termdine.selectedButton<0)
				termdine.selectedButton += menus[termdine.selectedMenu].buttonAmount;
			if (termdine.selectedButton>menus[termdine.selectedMenu].buttonAmount-1)
				termdine.selectedButton -= menus[termdine.selectedMenu].buttonAmount;
		}

		/* drawing */
		mvprintw(0, 1, "%s", menus[termdine.selectedMenu].title);
		for (int i=0;i<menus[termdine.selectedMenu].buttonAmount;i++)
		{
			mvprintw(i+1, 1, "%s%s", (termdine.selectedButton==i ? "> " : ""), menus[termdine.selectedMenu].buttons[i].name);
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

Button createButton(char* name, Action action, int actionInput)
{
	Button button;

	button.action = action;
	button.actionInput = actionInput;

	memcpy(button.name, name, MAXNAMESIZE);

	return button;
}

void doButtonAction(App* app, Button button)
{
	switch(button.action)
	{
		case QUIT: app->running = FALSE; break;
		case GOTOMENU: app->selectedMenu = button.actionInput; break;
		case NOTHING: break;
	}
}

App initApp(void)
{
	App app;

	app.running = TRUE;
	app.selectedMenu = 0;
	app.selectedButton = 0;

	return app;
}
