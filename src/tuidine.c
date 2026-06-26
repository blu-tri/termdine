/*
 *
 * This is a cool and a very pointless comment
 * idk why i added it
 * 
*/

#include <math.h>
#include <ncurses.h>
#include <string.h>
#include "../include/termdine/tuidine.h"


Menu createMenu(char* title, int buttonAmount, Button* buttons)
{
	Menu menu;

	menu.buttonAmount = buttonAmount;
	for (int i=0;i<buttonAmount;i++)
	{
		menu.buttons[i] = buttons[i];
	}

	memcpy(menu.title, title, MAXTITLESIZE);
	memcpy(menu.text, "", 1);

	return menu;
}

Menu createMenuWithText(char* title, int buttonAmount, Button* buttons, char* text)
{
	Menu menu;

	menu.buttonAmount = buttonAmount;
	for (int i=0;i<buttonAmount;i++)
	{
		menu.buttons[i] = buttons[i];
	}

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
		case CHANGETEXT: changeText(&app->menus[button.actionInput[0]-'0'], button.actionInput+3, (button.actionInput[1]-'0')*10+button.actionInput[2]-'0'+1); break;
		case CHANGECURRENTTEXT: changeText(&app->menus[app->selectedMenu], button.actionInput+1, button.actionInput[1]-'0'+1); break;
		case NOTHING: break;
		case TOGGLE: doToggle(app, button.actionInput);
	}
}

App initApp(void)
{
	App app;

	app.running = TRUE;
	app.selectedMenu = 0;
	app.selectedButton = 0;
	app.togglesAmount = 1;
	memcpy(app.toggles[0], "running", MAXTEXTSIZE);

	return app;
}

void changeText(Menu* menu, char* newText, int amount)
{
	memcpy(menu->text, newText, (amount>MAXTEXTSIZE?MAXTEXTSIZE:amount));
}

void addButton(Menu* menu, Button button, int index)
{
	if (menu->buttonAmount >= MAXBUTTONS)
	{
		return;
	}

	menu->buttonAmount += 1;

	for (int i=menu->buttonAmount;i>index-1;i--)
	{
		menu->buttons[i+1] = menu->buttons[i];
	}

	menu->buttons[index] = button;
}

void removeButton(Menu* menu, int index)
{
	if (menu->buttonAmount <= 1)
	{
		return;
	}

	for (int i=index+1;i<menu->buttonAmount;i++)
	{
		menu->buttons[i-1] = menu->buttons[i];
	}

	menu->buttonAmount -= 1;
}

void doToggle(App* app, char* actionInput)
{
	for (int i=0;i<app->togglesAmount;i++)
	{
		if (strcmp(actionInput, app->toggles[i])==0)
		{
				for (int j=i+1;j<app->togglesAmount;j++)
				{
					memcpy(app->toggles[j-1], app->toggles[j], MAXTEXTSIZE);
				}

				app->togglesAmount -= 1;
				return;
		}
	}
	
	memcpy(app->toggles[app->togglesAmount], actionInput, MAXTEXTSIZE);
	app->togglesAmount += 1;
	return;
}

int toggled(App app, char* toggle)
{
	for (int i=0;i<app.togglesAmount;i++)
	{
		if (strcmp(app.toggles[i], toggle)==0)
		{
			return 1;
		}
	}

	return 0;
}

void processInput(App* app, int c)
{
	if (c != ERR) {
			switch (c)
			{
				case KEY_UP: app->selectedButton -= 1; break;
				case KEY_DOWN: app->selectedButton += 1; break;
				case KEY_RIGHT: doButtonAction(app, app->menus[app->selectedMenu].buttons[app->selectedButton]); break; 
				case 'w': app->selectedButton -= 1; break;
				case 's': app->selectedButton += 1; break;
				case 'e': doButtonAction(app, app->menus[app->selectedMenu].buttons[app->selectedButton]); break; 
				case  10: doButtonAction(app, app->menus[app->selectedMenu].buttons[app->selectedButton]); break;
			}

			if (c == 'q' || c == 27) 
				app->running = FALSE;
		}
}

void drawScreen(App app, WINDOW* mainWin, WINDOW* textWin)
{
		for (int i=0;i<app.menus[app.selectedMenu].buttonAmount;i++)
		{
			

			if (app.menus[app.selectedMenu].buttons[i].action != TOGGLE)
			{
				mvprintw(i+ceil((double)strlen(app.menus[app.selectedMenu].text)/MAXTITLESIZE)+3, 1, "%s%s", (app.selectedButton==i ? "> " : ""), app.menus[app.selectedMenu].buttons[i].name);
			}
			else if (app.menus[app.selectedMenu].buttons[i].action == TOGGLE)
			{
				mvprintw(i+ceil((double)strlen(app.menus[app.selectedMenu].text)/MAXTITLESIZE)+3, 1, "%s[%c] %s", (app.selectedButton==i ? "> " : ""), ((toggled(app, app.menus[app.selectedMenu].buttons[i].actionInput)) ? '*' : ' '), app.menus[app.selectedMenu].buttons[i].name);
			}
		}

		mvprintw(0, 1, "%s", app.menus[app.selectedMenu].title);
		mvwprintw(textWin, 0, 0, "%s", app.menus[app.selectedMenu].text);
		
		box(mainWin, 0, 0);
}

void doAppLogic(App* app)
{

		while (app->selectedButton<0||app->selectedButton>app->menus[app->selectedMenu].buttonAmount-1)
		{
			if (app->selectedButton<0)
				app->selectedButton += app->menus[app->selectedMenu].buttonAmount;
			if (app->selectedButton>app->menus[app->selectedMenu].buttonAmount-1)
				app->selectedButton -= app->menus[app->selectedMenu].buttonAmount;
		}
}

void initNcurses(void)
{
	initscr();
	raw();
	noecho();
	
	keypad(stdscr, TRUE);
	timeout(15);
	
	curs_set(0);

}

