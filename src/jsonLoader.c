#define TERMDINEDEBUG /* turn on debug mode */

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <cjson/cJSON.h>
#include <stdlib.h>
#include "../include/termdine/log.h"

/* type definitions */
typedef struct fish 
{
	char name[24];
	int minSize;
	int avgSize;
	int maxSize;
	char location[48];
	char description[256];
} Fish;

/* function definitions */
int hasStringValue(cJSON* object);
int hasIntValue(cJSON* object);
char* getStringValue(cJSON* object, char* ifNone);
int getIntValue(cJSON* object, int ifNone);
char* convertToFilename(char* name);
Fish loadFish(char* fishName);

/* main function (crazy right?) */
int main(int argc, char* argv[])
{
	/* checking if program got inputted a possible file */
	if (argc < 2)
	{
		prError("Not enough arguments! You should run this command as jsonLoader <fishname>.\n");
		return 1;
	}

	Fish dashil = loadFish("dashil");

	printf("%s\n  size:\n    min: %d\n    avg: %d\n    max: %d\n  location: %s,\n%s\n", dashil.name, dashil.minSize, dashil.avgSize, dashil.maxSize, dashil.location, dashil.description);

	return 0;
}

Fish loadFish(char* fishName)
{
	Fish fish;

	prInfo("loading fish: ");
	printf("%s\n", convertToFilename(fishName));

	char* path = "content/fish/dashil.json";

	FILE* jsonFile = fopen(path, "r");

	/* checking if file exists */
	if (jsonFile == NULL) 
	{
		prError("Couldn't open file does the following file actually exist?\n");
		prError(path);
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
	memcpy(fish.name,        nameValue,         sizeof(char)*24);
	memcpy(fish.location,    locationValue,     sizeof(char)*48);
	memcpy(fish.description, descriptionValue, sizeof(char)*256);


	fish.minSize = getIntValue(minsize, 0);
	fish.avgSize = getIntValue(avgsize, 0);
	fish.maxSize = getIntValue(maxsize, 0);

	cJSON_Delete(json);

	return fish;
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
