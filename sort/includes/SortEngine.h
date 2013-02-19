#ifndef SORTENGINE
#define SORTENGINE
#include <csignal>
#include "SortInterface.h"

class SortEngine
{
private:
  struct sigaction _action;
  QVector<unsigned int> _inputVector; 
  int _fdRead;
  int _fdWrite;
  int _pid;
  void _saveQVectorToPipe(int fd, QVector<unsigned int>& vector);
  QVector<unsigned int> _readQVectorFromPipe(int fd);
  void _initSig();
  void _waitForSigUsr();
  SortInterface* _interface;

public:
  SortEngine(SortInterface* interface, int fdRead, int fdWrite);
  void splitVector(QVector<unsigned int>& _splittedVector1, QVector<unsigned int>& _splittedVector2);
  pid_t callChild(int fdRead, int fdWrite);
  static void sigUsrHandler(int signal);

};

#endif