#ifndef TERMDINELOGGER
#define TERMDINELOGGER

#include <stdio.h>
#define  PREINFO "\033[38,2,0,255,0m[INFO] \033[0m"
#define  PREWARN "\033[38,2,255,255,0m[WARN] \033[0m"
#define PREERROR "\033[38,2,255,0,0m[ERROR] \033[0m"

void prInfo(char* info)
{
	#ifdef TERMDINEDEBUG
	printf("%s%s", PREINFO, info);
	#endif
}

void prWarn(char* warning)
{
	printf("%s%s", PREWARN, warning);
}

void prError(char* error)
{
	printf("%s%s", PREERROR, error);
}

#endif

