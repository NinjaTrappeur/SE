#include <QVector>

class Random
{
public:
  Random();
  unsigned int generateRandomNumber(unsigned int max);
  QVector<unsigned int> generateRandomVector(unsigned int size, unsigned int max);
};
