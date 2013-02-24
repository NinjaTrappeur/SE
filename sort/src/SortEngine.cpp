#include <cstdio>
#include <unistd.h>
#include <iostream>
#include "SortEngine.h"
#include <sys/socket.h>

int SortEngine::_sigusrFd[2];

static int setup_unix_signal_handlers()
{
  struct sigaction usr;
  
  usr.sa_handler = SortEngine::usrSignalHandler;
  sigemptyset(&usr.sa_mask);
  usr.sa_flags = 0;
  usr.sa_flags |= SA_RESTART;
  
  if (sigaction(SIGUSR1, &usr, 0) == -1)
    {
      std::cerr<<"Impossible de gérer sigusr1"<<std::endl;
      exit(-1);
    }
  
  return 0;
}

void SortEngine::usrSignalHandler(int unused)
{
  char a = 1;
  int r = ::write(_sigusrFd[0], &a, sizeof(a));
  if(r<0)
    std::cerr<<"Write error"<<std::endl;
}

void SortEngine::handleSigUsr()
{
  _snUsr->setEnabled(false);
  char tmp;
  int r=::read(_sigusrFd[1], &tmp, sizeof(tmp));
  if(r<0)
    std::cerr<<"Write error"<<std::endl;  
  stepForward();
  _snUsr->setEnabled(true);
}


SortEngine::SortEngine(SortInterface* interface, int fdRead, int fdWrite):_interface(interface), _fdRead(fdRead), _fdWrite(fdWrite), _pid(getpid()), _count(0)
{
  setup_unix_signal_handlers();
  _interface->setPid(_pid);
  _inputVector=_readQVectorFromPipe(fdRead);
  _interface->setInputVector(_inputVector);
  
  if (::socketpair(AF_UNIX, SOCK_STREAM, 0, _sigusrFd))
    qFatal("Couldn't create USR socketpair");
  
  _snUsr = new QSocketNotifier(_sigusrFd[1], QSocketNotifier::Read, this);
  connect(_snUsr, SIGNAL(activated(int)), this, SLOT(handleSigUsr()));
  
}

void SortEngine::splitVector(QVector<unsigned int>& splittedVector1, QVector<unsigned int>& splittedVector2)
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
  splitVector(childOneVector, childTwoVector);
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
  _interface->setRightSonVector(_son1Vector);
  _interface->setLeftSonVector(_son2Vector);
}

void SortEngine::stepForward()
{
  std::cout<<"count= "<< _count<<std::endl;
  if(_inputVector.size()==1)
    {
      _saveQVectorToPipe(_returnFd, _inputVector);
      QApplication::quit();
    }
  else
    {
      switch(_count)
	{
	case 0:
	  startChildren();
	  _count++;
	  wait();
	  wait();
	  break;

	case 1:
	  std::cout<<"Case 1 exécuté dans pid "<<getpid() <<std::endl;
	  _readSonsResults();
	  _printSonsResults();
	  _count++;
	  break;
	}
    }
}
