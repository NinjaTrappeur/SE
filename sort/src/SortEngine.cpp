#include <cstdio>
#include <unistd.h>
#include <iostream>
#include "SortEngine.h"


SortEngine::SortEngine(SortInterface* interface, int fdRead, int fdWrite):_interface(interface), _fdRead(fdRead), _fdWrite(fdWrite), _pid(getpid())
{
  _interface->setPid(_pid);
  _inputVector=_readQVectorFromPipe(fdRead);
  _interface->setInputVector(_inputVector);
  _initSig();
}

void SortEngine::sigUsrHandler(int signal)
{
  _interface->setOutputVector("SIGUSR1 reçu!");
}

void SortEngine::_initSig()
{
  _action.sa_handler = SortEngine::sigUsrHandler;
  sigemptyset(&_action.sa_mask);
  if(sigaction(SIGUSR1, &_action, NULL) == -1) 
    {
      std::cerr<<"Impossible de gérer sigusr1"<<std::endl;
      exit(-1);
    }
}

void SortEngine::splitVector(QVector<unsigned int>& splittedVector1, QVector<unsigned int>& splittedVector2)
{
  int middle = _inputVector.size()/2; // Le type int tronque les chiffres apres la virgule.
  splittedVector1= _inputVector.mid(0,middle-1);
  splittedVector2= _inputVector.mid(middle, _inputVector.size()-1);
}

pid_t SortEngine::callChild(int fdRead, int fdWrite)
{
  pid_t pid;
  switch (pid=fork())
    {
    case -1:
      std::cerr<<"Impossible de forker"<<std::endl;
      exit(-1);
    case 0:
      char fdCRead[100], fdCWrite[100] ;
      sprintf(fdCRead, "%d", fdRead);
      sprintf(fdCWrite, "%d", fdWrite);
      execlp("./Sort", "./Sort", fdCRead, fdCWrite, (char*)NULL);
    default:
      return pid;
    }
}

void SortEngine::_saveQVectorToPipe(int fd, QVector<unsigned int>& vector)
{
  unsigned int data;
  for(int i=0;i<vector.size();i++)
    {
      data=vector[i];
      if(write(fd, &data, sizeof(unsigned int)) < sizeof(unsigned int))
	{
	  std::cerr<<"Impossible d'écrire dans le pipe"<<std::endl;
	  exit(-1);
	}
    }
  ::close(fd);
}

QVector<unsigned int> SortEngine::_readQVectorFromPipe(int fd)
{
  QVector<unsigned int> vector;
  unsigned int data;
  while(read(fd,&data,sizeof(int)) !=0)
    vector.append(data);
  return vector;
}

