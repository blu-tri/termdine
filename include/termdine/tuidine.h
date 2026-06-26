#ifndef TUIDINE
#define TUIDINE

#include <math.h>
#include <ncurses.h>
#include <string.h>

/* settings */
#define MAXTEXTSIZE 100

#define MAXBUTTONS 50
#define MAXTOGGLES 100

#define WIDTH 32
#define HEIGHT 9

#define MAXTITLESIZE WIDTH-2
#define MAXNAMESIZE WIDTH-8 // 2 for sides, 2 for selector thing, 4 for toggle thing

/* type definitions */
typedef enum action
{
	QUIT,
	GOTOMENU,
	CHANGETEXT,
	CHANGECURRENTTEXT,
	TOGGLE,
	NOTHING,
} Action;

typedef struct button
{
	char name[MAXNAMESIZE];
	Action action;
	char actionInput[MAXTEXTSIZE+3];
} Button;

typedef struct menu
{
	char title[MAXTITLESIZE];
	char text[MAXTEXTSIZE];
	int buttonAmount;
	Button buttons[MAXBUTTONS];
} Menu;

typedef struct app
{
	int running;
	int selectedMenu;
	int selectedButton;
	Menu* menus;
	char toggles[MAXTOGGLES][MAXTEXTSIZE];
	int togglesAmount;
} App;

/* function definitions */
Menu   createMenu(char* title, int buttonAmount, Button* buttons);
Menu   createMenuWithText(char* title, int buttonAmount, Button* buttons, char* text);
Button createButton(char* name, Action action, char* actionInput);
void   doButtonAction(App* app, Button button);
App    initApp(void);
void   changeText(Menu* menu, char* newText, int amount);
void   addButton(Menu* menu, Button button, int index);
void   removeButton(Menu* menu, int index);
void   doToggle(App* app, char* actionInput);
int    toggled(App app, char* toggle);
void   processInput(App* app, int c);
void   drawScreen(App app, WINDOW* mainWin, WINDOW* textWin);
void   doAppLogic(App* app);
void   initNcurses(void);

#endif
