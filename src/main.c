#define TERMDINEDEBUG /* turn on debug mode */

#include <stdio.h>
#include "../include/termdine/jsonLoader.h"
#include "../include/termdine/log.h"

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
