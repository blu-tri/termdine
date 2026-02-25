#ifndef TERMDINELOGGER
#define TERMDINELOGGER

#include <stdio.h>
#define  PREINFO "\033[32m[INFO] \033[0m"
#define  PREWARN "\033[33m[WARN] \033[0m"
#define PREERROR "\033[31m[ERROR] \033[0m"

void prInfo(char* info);
void prWarn(char* warning);
void prError(char* error);


#endif

