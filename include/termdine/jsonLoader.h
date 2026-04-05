#ifndef TERMDINEJSONLOADER
#define TERMDINEJSONLOADER


#include <cjson/cJSON.h>
#include <dirent.h>

#define MAXNAMESIZE   24 /*  */ 
#define MAXDESCSIZE  256 /*  */
#define MAXFISHAMOUNT 20 /*  */
#define MAXEXPANSIONS 30 /*  */

/* type definitions */
typedef struct fish 
{
	char name[MAXNAMESIZE];
	int minSize;
	int avgSize;
	int maxSize;
	char description[MAXDESCSIZE];
} Fish;

typedef struct location
{
	char name[MAXNAMESIZE];
	char fish[MAXFISHAMOUNT][MAXDESCSIZE];
	int fishAmount;
	char description[MAXDESCSIZE];
} Location;

typedef struct directory
{
	char childrenNames[MAXEXPANSIONS][MAXDESCSIZE];
	int childrenAmount;
} Directory;

/* function definitions */
int hasStringValue(cJSON* object);
int hasIntValue(cJSON* object);
char* getStringValue(cJSON* object, char* ifNone);
int getIntValue(cJSON* object, int ifNone);
char* convertToFilename(char* name);
Fish loadFish(char* fishName);
Location loadLocation(char* locationName);
Directory loadDirectory(char* path);

#endif
