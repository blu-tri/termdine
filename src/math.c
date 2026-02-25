#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * #include <math.h>
 *
int triangleRandom(int min, int avg, int max)  
{
	* copy and pasted from https://stackoverflow.com/questions/3510475/generate-random-numbers-according-to-distributions/3510549#3510549 *
	double U = (double) rand() / (double) RAND_MAX;
  int F = (avg - min) / (max - min);
	if (U <= F)
		return min + sqrt(U * (max - min) * (avg - min));
	else
		return max - sqrt((1 - U) * (max - min) * (max - avg));
}*/

int main(void)
{
	srand(time(NULL));

	printf("%d\n", rand() % 50 + 1);
}
