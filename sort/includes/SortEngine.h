#ifndef SORTENGINE
#define SORTENGINE
#include <csignal>
#include <QSocketNotifier>
#include <QObject>
#include "SortInterface.h"

class SortEngine : public QObject
{
Q_OBJECT

private:
  QVector<unsigned int> _inputVector; 
  SortInterface* _interface;
  int _fdRead;
  int _fdWrite;
  int _pid;
  void _saveQVectorToPipe(int fd, QVector<unsigned int>& vector);
  QVector<unsigned int> _readQVectorFromPipe(int fd);
  int _child1ResultFd;
  int _child2ResultFd;
  QVector<unsigned int> _son1Vector; 
  QVector<unsigned int> _son2Vector;
  void _createPipe(int fd[]);
  void _readSonsResults();
  void _printSonsResults();
  static int _sigusrFd[2];
  QSocketNotifier* _snUsr;
  int _count;

public:
  SortEngine(SortInterface* interface, int fdRead, int fdWrite);
  void splitVector(QVector<unsigned int>& _splittedVector1, QVector<unsigned int>& _splittedVector2);
  pid_t callChild(int fdRead, int fdWrite);
  void startChildren();
  void stepForward();
  static void usrSignalHandler(int unused);

  public slots:
  void handleSigUsr();
};

#endif
