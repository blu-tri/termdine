#include "../include/termdine/log.h"

void prInfo(char* info)
{
	printf("%s%s", PREINFO, info);
}

void prWarn(char* warning)
{
	printf("%s%s", PREWARN, warning);
}

void prError(char* error)
{
	printf("%s%s", PREERROR, error);
}
