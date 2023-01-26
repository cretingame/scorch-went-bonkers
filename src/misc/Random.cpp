#include <cstdlib>
#include <ctime>
#include "Random.h"

/************************************************************/

void initRandom()
{
  std::srand(std::time(NULL));
}

/************************************************************/

float randomFloat(float min, float max)
{
  return min + ((float)std::rand() / (float)RAND_MAX) * (max - min);
}

/************************************************************/

long randomInt(long min, long max)
{
	return std::rand() % (max - min + 1) + min;
}
