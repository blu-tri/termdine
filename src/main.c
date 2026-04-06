#define TERMDINEDEBUG /* turn on debug mode */

#include <stdio.h>
#include "../include/termdine/jsonLoader.h"
#include "../include/termdine/log.h"

/* main function (crazy right?) */
int main(void)
{
	Directory expansions = loadDirectory("expansions");

	for (int i=0;i<expansions.childrenAmount;i++)
	{
		printf("%s\n", expansions.childrenNames[i]);
	}
	

	return 0;
}
