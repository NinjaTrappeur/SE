#include <cstdio>
#include <unistd.h>
#include <iostream>
#include "SortEngine.h"
#include <sys/socket.h>
#include <sys/wait.h>


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

pid_t SortEngine::callChild(int fdRead[], int fdWrite[])
{
  pid_t pid;
  switch (pid=fork())
    {
    case -1:
      std::cerr<<"Impossible de forker"<<std::endl;
      exit(-1);
    case 0:
      char fdCRead[100], fdCWrite[100] ;
      ::close(fdRead[1]);
      ::close(fdWrite[0]);
      sprintf(fdCRead, "%d", fdRead[0]);
      sprintf(fdCWrite, "%d", fdWrite[1]);
      execlp("./Sort", "./Sort", fdCRead, fdCWrite, (char*)NULL);
    default:
      ::close(fdRead[0]);
      ::close(fdWrite[1]);
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
  ::close(fd);
}

void SortEngine::_createPipe(int fd[])
{
  if(pipe(fd) == -1)
    std::cerr<<"Impossible de créer un pipe!"<<std::endl;
}

void SortEngine::startChildren()
{
  //Splitting vector
  QVector<unsigned int> childOneVector, childTwoVector;
  splitVector(childOneVector, childTwoVector);
  
  //Calling first child
  _createPipe(_fdReadSon1);
  _createPipe(_fdWriteSon1);
  _saveQVectorToPipe(_fdReadSon1[1], childOneVector);
  callChild(_fdReadSon1,_fdWriteSon1);

  //Calling second child
  _createPipe(_fdReadSon2);
  _createPipe(_fdWriteSon2);
  _saveQVectorToPipe(_fdReadSon2[1], childTwoVector);
  callChild(_fdReadSon2,_fdWriteSon2);
}

void SortEngine::_readSonsResults()
{
  _son1Vector=_readQVectorFromPipe(_fdWriteSon1[0]);
  _son2Vector=_readQVectorFromPipe(_fdWriteSon2[0]);
}

void SortEngine::_printSonsResults()
{
  _interface->setRightSonVector(_son1Vector);
  _interface->setLeftSonVector(_son2Vector);
}

void SortEngine::stepForward()
{
  int status;
  if(_inputVector.size()==1)
    {
      _outputVector=_inputVector;
      _saveQVectorToPipe(_fdWrite, _outputVector);
      _interface->setOutputVector(_outputVector);
      QApplication::quit();
    }
  else
    {
      switch(_count)
	{
	case 0:
	  startChildren();
	  wait(&status);
	  wait(&status);
	  _count++;
	  break;

	case 1:
	  _readSonsResults();
	  _printSonsResults();
	  _count++;
	  break;

	case 2:
	  _sortSonsResults();
	  _interface->setOutputVector(_outputVector);
	  _saveQVectorToPipe(_fdWrite, _outputVector);
	  _count++;
	  break;
	  
	case 3:
	  QApplication::quit();
	  break;
	}
    }
}


void SortEngine::_sortSonsResults()
{
  _outputVector.clear();

  //On entrelace les vecteurs
  
  while(_son1Vector.size()>0 && _son2Vector.size()>0)
    {
      if(_son1Vector.front() <  _son2Vector.front())
	{
	  _outputVector.push_back(_son1Vector.front());
	  _son1Vector.erase(_son1Vector.begin());
	}
      else if (_son1Vector.front() == _son2Vector.front())
	{
	  _outputVector.push_back(_son1Vector.front());
	  _outputVector.push_back(_son2Vector.front());
	  
	  _son1Vector.erase(_son1Vector.begin());
	  _son2Vector.erase(_son2Vector.begin());
	}
      else
	{
	  _outputVector.push_back(_son2Vector.front());
	  
	  _son2Vector.erase(_son2Vector.begin());
	}
    }
  
  //On vide le vecteur restant

  while(_son1Vector.size()>0)
    {
      _outputVector.push_back(_son1Vector.front());
      _son1Vector.erase(_son1Vector.begin());
      
    }
  
  
  while(_son2Vector.size()>0)
    {
      _outputVector.push_back(_son2Vector.front());
      _son2Vector.erase(_son2Vector.begin());
    }
}
