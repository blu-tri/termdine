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
	Button B_quit         = createButton("quit",     QUIT,      "");
	Button B_gotoSettings = createButton("settings", GOTOMENU, "1");
	Button B_gotoTitle    = createButton("back",     GOTOMENU, "0");


	Button TitleButtons[2];
	TitleButtons[0] = B_gotoSettings;
	TitleButtons[1] = B_quit;

	Button settingsButtons[1];
	settingsButtons[0] = B_gotoTitle;

	Menu M_title = createMenuWithText("Termdiee", 2, TitleButtons, "RunTermdine(\"Deluxe\")");
	Menu M_settings = createMenu("Settings", 1, settingsButtons);

	int menuAmount = 2;
	Menu menus[menuAmount];

	menus[0] = M_title;
	menus[1] = M_settings;

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

		// drawing //
		drawScreen(termdine, mainWin, textWin);
	}

	endwin();

	return 0;
}
