#ifndef SORTENGINE
#define SORTENGINE
#include <csignal>
#include "SortInterface.h"

class SortEngine
{
private:
  struct sigaction _action;
  QVector<unsigned int> _inputVector; 
  SortInterface* _interface;
  int _fdRead;
  int _fdWrite;
  int _pid;
  void _saveQVectorToPipe(int fd, QVector<unsigned int>& vector);
  QVector<unsigned int> _readQVectorFromPipe(int fd);
  void _initSig();
  void _waitForSigUsr();

public:
  SortEngine(SortInterface* interface, int fdRead, int fdWrite);
  void splitVector(QVector<unsigned int>& _splittedVector1, QVector<unsigned int>& _splittedVector2);
  pid_t callChild(int fdRead, int fdWrite);
  void process();
  static void sigUsrHandler(int n);
  static bool userInterupt;

};

#endif
