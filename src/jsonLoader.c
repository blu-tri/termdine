#define TERMDINEDEBUG /* turn on debug mode */

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <cjson/cJSON.h>
#include <stdlib.h>
#include "../include/termdine/log.h"

#define MAXNAMESIZE   24 /*  */
#define MAXDESCSIZE  256 /*  */
#define MAXFISHAMOUNT 20 /*  */

/* type definitions */
typedef struct fish 
{
	char name[MAXNAMESIZE];
	int minSize;
	int avgSize;
	int maxSize;
	char location[MAXNAMESIZE];
	char description[MAXDESCSIZE];
} Fish;

typedef struct location
{
	char name[MAXNAMESIZE];
	char fish[MAXFISHAMOUNT][MAXDESCSIZE];
	int fishAmount;
	char description[MAXDESCSIZE];
} Location;

/* function definitions */
int hasStringValue(cJSON* object);
int hasIntValue(cJSON* object);
char* getStringValue(cJSON* object, char* ifNone);
int getIntValue(cJSON* object, int ifNone);
char* convertToFilename(char* name);
Fish loadFish(char* fishName);
Location loadLocation(char* locationName);


/* main function (crazy right?) */
int main(int argc, char* argv[])
{
	/* checking if program got input */
	if (argc < 2)
	{
		prError("Not enough arguments! You should run this command as jsonLoader <locationname>.\n");
		return 1;
	}
	Location locationPlace = loadLocation(argv[1]);

	printf("location: %s\n%s\nfish:\n", locationPlace.name, locationPlace.description);
	for (int i=0;i<locationPlace.fishAmount;i++)
	{
		Fish fish = loadFish(locationPlace.fish[i]);

		printf("%s\nsize:\n  min: %d\n  avg: %d\n  max: %d\n%s\n", fish.name, fish.minSize, fish.avgSize, fish.maxSize, fish.description);
	}

	return 0;
}

Fish loadFish(char* fishName)
{
	Fish fish;

	prInfo("loading fish: ");
	printf("%s\n", fishName);

	char path[100];

	sprintf(path, "content/fish/%s.json", convertToFilename(fishName));

	FILE* jsonFile = fopen(path, "r");

	/* checking if file exists */
	if (jsonFile == NULL) 
	{
		prError("Couldn't open file does the following file actually exist?\n");
		prError(path);
		printf("\n");
		exit(1);
	}

	/* reading and closing file */
	char jsonFileContent[8192];
	fread(jsonFileContent, 1, sizeof(jsonFileContent), jsonFile);
	fclose(jsonFile);
 
	/* check if can parse into json */
	cJSON* json = cJSON_Parse(jsonFileContent);
	if (json == NULL)
	{
		const char* error = cJSON_GetErrorPtr();
		if (error != NULL)
		{
			printf("Couldn't parse file into json\n%s\n", error);
		}
		cJSON_Delete(json);
		exit(1);
	}

	/* get json values */
	cJSON* name        = cJSON_GetObjectItemCaseSensitive(json,        "name");
	cJSON* location    = cJSON_GetObjectItemCaseSensitive(json,    "location");
	cJSON* description = cJSON_GetObjectItemCaseSensitive(json, "description");

	cJSON* size    = cJSON_GetObjectItemCaseSensitive(json, "size");
	cJSON* minsize = cJSON_GetObjectItemCaseSensitive(size,  "min");
	cJSON* avgsize = cJSON_GetObjectItemCaseSensitive(size,  "avg");
	cJSON* maxsize = cJSON_GetObjectItemCaseSensitive(size,  "max");

	char* nameValue        = getStringValue(name,        "???");
	char* locationValue    = getStringValue(location,    "???");
	char* descriptionValue = getStringValue(description, "???");
	
	/* NOTE: i have no clue if this is actually safe to do, probably not */
	memcpy(fish.name,        nameValue,         24);
	memcpy(fish.location,    locationValue,     48);
	memcpy(fish.description, descriptionValue, 256);


	fish.minSize = getIntValue(minsize, 0);
	fish.avgSize = getIntValue(avgsize, 0);
	fish.maxSize = getIntValue(maxsize, 0);

	cJSON_Delete(json);

	return fish;
}

Location loadLocation(char* locationName)
{
	Location location;

	prInfo("loading location: ");
	printf("%s\n", locationName);

	char path[100];

	sprintf(path, "content/locations/%s.json", convertToFilename(locationName));

	FILE* jsonFile = fopen(path, "r");

	/* checking if file exists */
	if (jsonFile == NULL) 
	{
		prError("Couldn't open file does the following file actually exist?\n");
		prError(path);
		printf("\n");
		exit(1);
	}

	/* reading and closing file */
	char jsonFileContent[8192];
	fread(jsonFileContent, 1, sizeof(jsonFileContent), jsonFile);
	fclose(jsonFile);
 
	/* check if can parse into json */
	cJSON* json = cJSON_Parse(jsonFileContent);
	if (json == NULL)
	{
		const char* error = cJSON_GetErrorPtr();
		if (error != NULL)
		{
			printf("Couldn't parse file into json\n%s\n", error);
		}
		cJSON_Delete(json);
		exit(1);
	}

	/* get json values */
	cJSON* name        = cJSON_GetObjectItemCaseSensitive(json,        "name");
	cJSON* description = cJSON_GetObjectItemCaseSensitive(json, "description");
	cJSON* fish        = cJSON_GetObjectItemCaseSensitive(json,        "fish");

	char* nameValue        = getStringValue(name,        "???");
	char* descriptionValue = getStringValue(description, "???");
	
	/* NOTE: i have no clue if this is actually safe to do, probably not */
	memcpy(location.name,        nameValue,         24);
	memcpy(location.description, descriptionValue, 256);

	if (!cJSON_IsArray(fish))
	{
		prError("location does not have a list of fish!\n");
		exit(1);
	}
	location.fishAmount = cJSON_GetArraySize(fish);

	for (int i=0;i<location.fishAmount;i++)
	{
		char* fishNameValue = getStringValue(cJSON_GetArrayItem(fish, i), "???");
		memcpy(location.fish[i], fishNameValue, 24);
	}

	cJSON_Delete(json);

	return location;
}

/* function implementations */
int hasStringValue(cJSON* object)
{
	return (cJSON_IsString(object) && object->valuestring != NULL);	
}

int hasIntValue(cJSON* object)
{
	return (cJSON_IsNumber(object));	
}

char* getStringValue(cJSON* object, char* ifNone)
{
	return (hasStringValue(object) ? object->valuestring : ifNone);
}

int getIntValue(cJSON* object, int ifNone)
{
	return (hasIntValue(object) ? object->valueint : ifNone);
}

char* convertToFilename(char* name)
{
	char* filename = name;

	for (int i=0;filename[i]!='\0';i++)
	{
		filename[i] = tolower(filename[i]);
		if (filename[i]==' ')
			filename[i] = '_';
	}

	return filename;
}
