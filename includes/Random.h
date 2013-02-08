#include <QVector>

class Random
{
public:
  Random();
  unsigned int generateRandomNumber();
  QVector<unsigned int> generateRandomVector(unsigned int size);
};
