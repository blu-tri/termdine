#define TERMDINEDEBUG /* turn on debug mode */

#include "../include/termdine/tuidine.h"

/* main function (crazy right?) */
int main(void)
{

	WINDOW* mainWin;
	WINDOW* textWin;
	// ncurses init //
	int c = 0;
	App termdine = initApp();
	
	initNcurses();

	mainWin = newwin(HEIGHT, WIDTH, 0, 0);
	textWin = newwin(HEIGHT-2, WIDTH-2, 1, 1);


  // menu init //
	Button doNothingButton = createButton("do nothing", NOTHING, "");

	Button fishButton = createButton("fish", GOTOMENU, "1");
	Button textButton = createButton("go to text", GOTOMENU, "3");
	Button settingsButton = createButton("settings", GOTOMENU, "2");
	Button backButton = createButton("back", GOTOMENU, "0");
	Button fishBackButton = createButton("back", GOTOMENU, "1");
	Button sayHiButton = createButton("hello", CHANGECURRENTTEXT, "2Hi");
	Button setToHelloButton = createButton("set text to Hello", CHANGETEXT, "305Hello");
	Button quitButton = createButton("quit", QUIT, "");
	Button testToggleButton = createButton("toggle main menu text", TOGGLE, "toggle");


	Button mainMenuButtons[3];
	mainMenuButtons[0] = fishButton;
	mainMenuButtons[1] = settingsButton;
	mainMenuButtons[2] = quitButton;

	Button fishButtons[2];
	fishButtons[0] = textButton;
	fishButtons[1] = backButton;

	Button settingsButtons[4];
	settingsButtons[0] = doNothingButton;
	settingsButtons[1] = setToHelloButton;
	settingsButtons[2] = testToggleButton;
	settingsButtons[3] = backButton;

	Button textButtons[2];
	textButtons[0] = sayHiButton;
	textButtons[1] = fishBackButton;

	Menu mainMenu = createMenu("Termdine", 3, mainMenuButtons);
	Menu settingsMenu = createMenu("Settings", 4, settingsButtons);
	Menu fishMenu = createMenu("Fish", 2, fishButtons);
	Menu textMenu = createMenuWithText("Text", 2, textButtons, "this is a test to see how well this works");

	int menuAmount = 4;
	Menu menus[menuAmount];

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
		
		// input //
		processInput(&termdine, c);	

		// logic //
		doAppLogic(&termdine);

		if (toggled(termdine, "toggle"))
		{
			changeText(&termdine.menus[0], "The fishing game", 17);
		}
		else
		{
			changeText(&termdine.menus[0], "", 1);
		}

		// drawing //
		drawScreen(termdine, mainWin, textWin);
	}

	endwin();

	return 0;
}
