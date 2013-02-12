#include "Random.h"
#include <ctime> 

Random::Random()
{
    srand((unsigned)time(NULL));
}

unsigned int Random::generateRandomNumber(unsigned int max)
{
  return (rand()%max)+1;
}

QVector<unsigned int> Random::generateRandomVector(unsigned int size, unsigned int max)
{
  QVector<unsigned int> vector;
  unsigned int i;
  for(i=0;i<size;++i)
    vector<< generateRandomNumber(max);
  return vector;
}
