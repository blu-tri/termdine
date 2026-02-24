#include <stdio.h>
#include <cjson/cJSON.h>

/* function definitions */
int hasStringValue(cJSON* object);
int hasIntValue(cJSON* object);

/* main function (crazy right?) */
int main(int argc, char* argv[])
{
	/* checking if program got inputted a possible file */
	if (argc < 2)
	{
		printf("Not enough arguments! You should run this command as jsonLoader <filename>.\n");
		return 1;
	}

	FILE* jsonFile = fopen(argv[1], "r");

	/* checking if file exists */
	if (jsonFile == NULL) 
	{
		printf("Couldn't open file %s, does this file actually exist?\n", argv[1]);
		return 1;
	}

	/* reading and closing file */
	char jsonFileContent[4096];
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
		return 1;
	}

	/* print json values */
	cJSON* name = cJSON_GetObjectItemCaseSensitive(json, "name");
	if (hasStringValue(name))
	{
		printf("fish name: %s\n", name->valuestring);
	}

	cJSON_Delete(json);
	return 0;
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
