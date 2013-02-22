#ifndef SORTENGINE
#define SORTENGINE
#include <csignal>
#include "SortInterface.h"

extern SortInterface* sortInterface;

class SortEngine
{
private:
  struct sigaction _action;
  int _pid;
  void _initSig();
  void _waitForSigUsr();

public:
  SortEngine();
  static int _child1ResultFd;
  static int _child2ResultFd;
  static int _returnFd;
  static QVector<unsigned int> _inputVector; 
  static QVector<unsigned int> _son1Vector; 
  static QVector<unsigned int> _son2Vector; 
  static int count;
  static pid_t callChild(int fdRead, int fdWrite);
  static void sigUsrHandler(int signal);
  static void startChildren();
  static void _createPipe(int fd[]);
  static QVector<unsigned int> _readQVectorFromPipe(int fd);
  static void _splitVector(QVector<unsigned int> _inputVector, QVector<unsigned int>& _splittedVector1, QVector<unsigned int>& _splittedVector2);
  static void _saveQVectorToPipe(int fd, QVector<unsigned int>& vector);
  static void _readSonsResults();
  static void _printSonsResults();
};
#endif
