#include <ncurses.h>
#include <string.h>

#define MAXTEXTSIZE  40

#define MAXMENUS 5

#define WIDTH 32
#define HEIGHT 9

#define MAXTITLESIZE WIDTH-2
#define MAXNAMESIZE  WIDTH-4

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
	char text[MAXTEXTSIZE];
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
Menu createMenuWithText(char* title, int buttonAmount, Button* buttons, char* text);
Button createButton(char* name, Action action, int actionInput);
void doButtonAction(App* app, Button button);
App initApp(void);

WINDOW* mainWin;
WINDOW* textWin;

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

	mainWin = newwin(HEIGHT, WIDTH, 0, 0);
	textWin = newwin(HEIGHT-2, WIDTH-2, 1, 1);

	/* menu init */
	Button doNothingButton = createButton("do nothing", NOTHING, 0);

	Button fishButton = createButton("fish", GOTOMENU, 1);
	Button textButton = createButton("go to text", GOTOMENU, 3);
	Button settingsButton = createButton("settings", GOTOMENU, 2);
	Button backButton = createButton("back", GOTOMENU, 0);
	Button fishBackButton = createButton("back", GOTOMENU, 1);

	Button quitButton = createButton("quit", QUIT, 0);


	Button mainMenuButtons[3];
	mainMenuButtons[0] = fishButton;
	mainMenuButtons[1] = settingsButton;
	mainMenuButtons[2] = quitButton;

	Button fishButtons[2];
	fishButtons[0] = textButton;
	fishButtons[1] = backButton;

	Button settingsButtons[2];
	settingsButtons[0] = doNothingButton;
	settingsButtons[1] = backButton;

	Button textButtons[1];
	textButtons[0] = fishBackButton;

	Menu mainMenu = createMenu("Termdine", 3, mainMenuButtons);
	Menu settingsMenu = createMenu("Settings", 2, settingsButtons);
	Menu fishMenu = createMenu("Fish", 2, fishButtons);
	Menu textMenu = createMenuWithText("Text", 1, textButtons, "this is a test to see how well this works");

	Menu menus[4];

	menus[0] = mainMenu;
	menus[1] = fishMenu;
	menus[2] = settingsMenu;
	menus[3] = textMenu;

	while (termdine.running)
	{
		wrefresh(mainWin);
		wrefresh(textWin);
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
				case 10: doButtonAction(&termdine, menus[termdine.selectedMenu].buttons[termdine.selectedButton]); break; 
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
		mvwaddstr(textWin, 0, 0, menus[termdine.selectedMenu].text);
		
		for (int i=0;i<menus[termdine.selectedMenu].buttonAmount;i++)
		{
			
			mvprintw(i+(strlen(menus[termdine.selectedMenu].text)/14)+1, 1, "%s%s", (termdine.selectedButton==i ? "> " : ""), menus[termdine.selectedMenu].buttons[i].name);

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
	memcpy(menu.text, "", 1);

	return menu;
}

Menu createMenuWithText(char* title, int buttonAmount, Button* buttons, char* text)
{
	Menu menu;

	menu.buttonAmount = buttonAmount;
	menu.buttons = buttons;

	memcpy(menu.title, title, MAXTITLESIZE);
	memcpy(menu.text, text, MAXTEXTSIZE);

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
