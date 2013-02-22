#include <cstdio>
#include <unistd.h>
#include <iostream>
#include "SortEngine.h"

int SortEngine::_child1ResultFd=0;
int SortEngine::_child2ResultFd=0;
int SortEngine::_returnFd=0;

QVector<unsigned int> SortEngine::_inputVector = QVector<unsigned int>();
QVector<unsigned int> SortEngine::_son1Vector = QVector<unsigned int>(); 
QVector<unsigned int> SortEngine::_son2Vector = QVector<unsigned int>(); 

int SortEngine::count=0;

SortEngine::SortEngine():_pid(getpid())
{
  sortInterface->setPid(_pid);
  sortInterface->setInputVector(SortEngine::_inputVector);
  _initSig();
}

void SortEngine::sigUsrHandler(int signal)
{
  std::cout<<"size= "<< _inputVector.size()<<std::endl;
  if(_inputVector.size()==1)
    {
      _saveQVectorToPipe(SortEngine::_returnFd, _inputVector);
      QApplication::quit();
    }
  else
    {
      switch(SortEngine::count)
	{
	case 0:
	  startChildren();
	  count++;
	  wait();
	  wait();
	  break;

	case 1:
	  _readSonsResults();
	  _printSonsResults();
	  count++;
	  break;
	}
    }
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

void SortEngine::_splitVector(QVector<unsigned int> _inputVector, QVector<unsigned int>& splittedVector1, QVector<unsigned int>& splittedVector2)
{
  int middle = _inputVector.size()/2; // Le type int tronque les chiffres apres la virgule.
  splittedVector1= _inputVector.mid(0,middle);
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
      char fdCRead[100], fdCWrite[100];
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

void SortEngine::_createPipe(int fd[])
{
  if(pipe(fd) == -1)
    std::cerr<<"Impossible de créer un pipe!"<<std::endl;
}

void SortEngine::startChildren()
{
  QVector<unsigned int> childOneVector, childTwoVector;
  int fdPipeChild1[2], fdPipeChild2[2], fdChild1Result[2], fdChild2Result[2];

  _createPipe(fdPipeChild1);
  _createPipe(fdPipeChild2);
  _createPipe(fdChild1Result);
  _createPipe(fdChild2Result);
  _splitVector(_inputVector, childOneVector, childTwoVector);
  _child1ResultFd=fdChild1Result[0];
  _child2ResultFd=fdChild2Result[0];
  _saveQVectorToPipe(fdPipeChild1[1], childOneVector);
  _saveQVectorToPipe(fdPipeChild2[1], childTwoVector);
  callChild(fdPipeChild1[0],fdChild1Result[1]);
  callChild(fdPipeChild2[0],fdChild2Result[1]);
}

void SortEngine::_readSonsResults()
{
  _son1Vector=_readQVectorFromPipe(_child1ResultFd);
  _son2Vector=_readQVectorFromPipe(_child2ResultFd);
}

void SortEngine::_printSonsResults()
{
  sortInterface->setRightSonVector(_son1Vector);
  sortInterface->setLeftSonVector(_son2Vector);
}
