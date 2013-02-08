#include "Random.h"
#include <ctime> 

Random::Random()
{
  
}

unsigned int Random::generateRandomNumber()
{
  srand((unsigned)time(NULL));
  return (rand()%10)+1;
}

QVector<unsigned int> Random::generateRandomVector(unsigned int size)
{
  QVector<unsigned int> vector;
  unsigned int i;
  for(i=0;i<size;++i)
    vector<< generateRandomNumber();
  return vector;
}
