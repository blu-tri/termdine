#include <math.h>
#include <ncurses.h>
#include <string.h>

#define MAXTEXTSIZE 100

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
	CHANGETEXT,
	CHANGECURRENTTEXT,
	NOTHING,
} Action;

typedef struct button 
{
	char name[MAXNAMESIZE];
	Action action;
	char actionInput[MAXTEXTSIZE+2];
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
	Menu* menus;
} App;

/* function definitions */
Menu createMenu(char* title, int buttonAmount, Button* buttons);
Menu createMenuWithText(char* title, int buttonAmount, Button* buttons, char* text);
Button createButton(char* name, Action action, char* actionInput);
void doButtonAction(App* app, Button button);
App initApp(void);
void changeText(Menu* menu, char* newText, int amount);
void addButton(Menu* menu, Button button, int index);
void removeButton(Menu* menu, int index);

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
	timeout(15);
	
	curs_set(0);

	mainWin = newwin(HEIGHT, WIDTH, 0, 0);
	textWin = newwin(HEIGHT-2, WIDTH-2, 1, 1);

	/* menu init */
	Button doNothingButton = createButton("do nothing", NOTHING, "");

	Button fishButton = createButton("fish", GOTOMENU, "1");
	Button textButton = createButton("go to text", GOTOMENU, "3");
	Button settingsButton = createButton("settings", GOTOMENU, "2");
	Button backButton = createButton("back", GOTOMENU, "0");
	Button fishBackButton = createButton("back", GOTOMENU, "1");
	Button sayHiButton = createButton("hello", CHANGECURRENTTEXT, "2Hi");
		Button setToHelloButton = createButton("set text to Hello", CHANGETEXT, "35Hello");
	Button quitButton = createButton("quit", QUIT, "");


	Button mainMenuButtons[3];
	mainMenuButtons[0] = fishButton;
	mainMenuButtons[1] = settingsButton;
	mainMenuButtons[2] = quitButton;

	Button fishButtons[2];
	fishButtons[0] = textButton;
	fishButtons[1] = backButton;

	Button settingsButtons[3];
	settingsButtons[0] = doNothingButton;
	settingsButtons[1] = setToHelloButton;
	settingsButtons[2] = backButton;

	Button textButtons[2];
	textButtons[0] = sayHiButton;
	textButtons[1] = fishBackButton;

	Menu mainMenu = createMenu("Termdine", 3, mainMenuButtons);
	Menu settingsMenu = createMenu("Settings", 3, settingsButtons);
	Menu fishMenu = createMenu("Fish", 2, fishButtons);
	Menu textMenu = createMenuWithText("Text", 2, textButtons, "this is a test to see how well this works");

	Menu menus[4];

	menus[0] = mainMenu;
	menus[1] = fishMenu;
	menus[2] = settingsMenu;
	menus[3] = textMenu;

	termdine.menus = menus;

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
				case 'e': doButtonAction(&termdine, termdine.menus[termdine.selectedMenu].buttons[termdine.selectedButton]); break; 
				case 10: doButtonAction(&termdine, termdine.menus[termdine.selectedMenu].buttons[termdine.selectedButton]); break; 
			}

			if (c == 'q' || c == 27) 
				termdine.running = FALSE;
		}

		/* logic */
		while (termdine.selectedButton<0||termdine.selectedButton>termdine.menus[termdine.selectedMenu].buttonAmount-1)
		{
			if (termdine.selectedButton<0)
				termdine.selectedButton += termdine.menus[termdine.selectedMenu].buttonAmount;
			if (termdine.selectedButton>termdine.menus[termdine.selectedMenu].buttonAmount-1)
				termdine.selectedButton -= termdine.menus[termdine.selectedMenu].buttonAmount;
		}

		/* drawing */
		for (int i=0;i<termdine.menus[termdine.selectedMenu].buttonAmount;i++)
		{
			
			mvprintw(i+ceil((double)strlen(termdine.menus[termdine.selectedMenu].text)/MAXTITLESIZE)+3, 1, "%s%s", (termdine.selectedButton==i ? "> " : ""), termdine.menus[termdine.selectedMenu].buttons[i].name);

		}

		mvprintw(0, 1, "%s", termdine.menus[termdine.selectedMenu].title);
		mvwprintw(textWin, 0, 0, "%s", termdine.menus[termdine.selectedMenu].text);
		
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

Button createButton(char* name, Action action, char* actionInput)
{
	Button button;

	button.action = action;
	
	memcpy(button.actionInput, actionInput, MAXTEXTSIZE+2);
	memcpy(button.name, name, MAXNAMESIZE);

	return button;
}

void doButtonAction(App* app, Button button)
{
	switch(button.action)
	{
		case QUIT: app->running = FALSE; break;
		case GOTOMENU: app->selectedButton = 0; app->selectedMenu = button.actionInput[0]-'0'; break;
		case CHANGETEXT: changeText(&app->menus[button.actionInput[0]-'0'], button.actionInput+2, button.actionInput[1]-'0'+1); break;
		case CHANGECURRENTTEXT: changeText(&app->menus[app->selectedMenu], button.actionInput+1, button.actionInput[1]-'0'+1); break;
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

void changeText(Menu* menu, char* newText, int amount)
{
	memcpy(menu->text, newText, (amount>MAXTEXTSIZE?MAXTEXTSIZE:amount));
}

void addButton(Menu* menu, Button button, int index);
void removeButton(Menu* menu, int index);


